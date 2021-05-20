// Copyright (c) 2016 Martin Ridgers
// License: http://opensource.org/licenses/MIT

#include "pch.h"
#include "ecma48_iter.h"

#include <core/base.h>
#include <core/str_tokeniser.h>

#include <assert.h>

//------------------------------------------------------------------------------
unsigned int cell_count(const char* in)
{
    unsigned int count = 0;

    ecma48_state state;
    ecma48_iter iter(in, state);
    while (const ecma48_code& code = iter.next())
    {
        if (code.get_type() != ecma48_code::type_chars)
            continue;

        str_iter inner_iter(code.get_pointer(), code.get_length());
        while (int c = inner_iter.next())
            count += clink_wcwidth(c);
    }

    return count;
}

//------------------------------------------------------------------------------
static bool in_range(int value, int left, int right)
{
    return (unsigned(right - value) <= unsigned(right - left));
}

//------------------------------------------------------------------------------
static void strip_code_terminator(const char*& ptr, int& len)
{
    if (len <= 0)
        return;

    if (ptr[len - 1] == 0x07)
        len--;
    else if (len > 1 && ptr[len - 1] == 0x5c && ptr[len - 2] == 0x1b)
        len -= 2;
}

//------------------------------------------------------------------------------
static void strip_code_quotes(const char*& ptr, int& len)
{
    if (len < 2)
        return;

    if (ptr[0] == '"' && ptr[len - 1] == '"')
    {
        ptr++;
        len -= 2;
    }
}



//------------------------------------------------------------------------------
enum ecma48_state_enum
{
    ecma48_state_unknown = 0,
    ecma48_state_char,
    ecma48_state_esc,
    ecma48_state_esc_st,
    ecma48_state_csi_p,
    ecma48_state_csi_f,
    ecma48_state_cmd_str,
    ecma48_state_char_str,
};

//------------------------------------------------------------------------------
void ecma48_state::reset()
{
    state = ecma48_state_unknown;
    count = 0;
}



//------------------------------------------------------------------------------
bool ecma48_code::decode_csi(csi_base& base, int* params, unsigned int max_params) const
{
    if (get_type() != type_c1 || get_code() != c1_csi)
        return false;

    /* CSI P ... P I .... I F */
    str_iter iter(get_pointer(), get_length());

    // Skip CSI
    if (iter.next() == 0x1b)
        iter.next();

    // Is the parameter string tagged as private/experimental?
    if (base.private_use = in_range(iter.peek(), 0x3c, 0x3f))
        iter.next();

    // Extract parameters.
    base.intermediate = 0;
    base.final = 0;
    int param = 0;
    unsigned int count = 0;
    bool trailing_param = false;
    while (int c = iter.next())
    {
        if (in_range(c, 0x30, 0x3b))
        {
            trailing_param = true;

            if (c == 0x3b)
            {
                if (count < max_params)
                    params[count++] = param;

                param = 0;
            }
            else if (c != 0x3a) // Blissfully gloss over ':' part of spec.
                param = (param * 10) + (c - 0x30);
        }
        else if (in_range(c, 0x20, 0x2f))
            base.intermediate = char(c);
        else if (!in_range(c, 0x3c, 0x3f))
            base.final = char(c);
    }

    if (trailing_param)
        if (count < max_params)
            params[count++] = param;

    base.param_count = char(count);
    return true;
}

//------------------------------------------------------------------------------
bool ecma48_code::decode_osc(osc& out) const
{
    if (get_type() != type_c1 || get_code() != c1_osc)
        return false;

    str_iter iter(get_pointer(), get_length());

    // Skip CSI OSC
    if (iter.next() == 0x1b)
        iter.next();

    // Extract command.
    out.visible = false;
    out.command = iter.next();
    out.subcommand = 0;
    switch (out.command)
    {
    case '0':
    case '1':
    case '2':
        if (iter.next() == ';')
        {
            // Strip the terminator and optional quotes.
            const char* ptr = iter.get_pointer();
            int len = iter.length();
            strip_code_terminator(ptr, len);
            strip_code_quotes(ptr, len);

            ecma48_state state;
            ecma48_iter inner_iter(ptr, state, len);
            while (const ecma48_code& code = inner_iter.next())
            {
                if (code.get_type() == ecma48_code::type_c1 &&
                    code.get_code() == ecma48_code::c1_osc)
                {
                    // For OSC codes, only include output text.
                    ecma48_code::osc osc;
                    if (code.decode_osc(osc))
                        out.param.concat(osc.output.c_str(), osc.output.length());
                }
                else
                {
                    // Otherwise include the text verbatim.
                    out.param.concat(code.get_pointer(), code.get_length());
                }
            }
        }
        break;

    case '9':
        out.subcommand = (iter.next() == ';') ? iter.next() : 0;
        switch (out.subcommand)
        {
        case '8': /* get envvar */
            out.visible = true;
            out.output.clear();
            if (iter.next() == ';')
            {
                const char* ptr = iter.get_pointer();
                while (true)
                {
                    const char* end = iter.get_pointer();
                    int c = iter.next();
                    if (!c || c == 0x07 || (c == 0x1b && iter.peek() == 0x5c))
                    {
                        int len = int(end - ptr);
                        strip_code_quotes(ptr, len);

                        wstr<> name;
                        wstr<> value;

                        to_utf16(name, str_iter(ptr, len));
                        DWORD needed = GetEnvironmentVariableW(name.c_str(), 0, 0);
                        value.reserve(needed);
                        needed = GetEnvironmentVariableW(name.c_str(), value.data(), value.size());

                        if (needed < value.size())
                            to_utf8(out.output, wstr_iter(value.c_str(), needed));
                        break;
                    }
                }
            }
            break;
        }
        break;
    }

    return true;
}

//------------------------------------------------------------------------------
bool ecma48_code::get_c1_str(str_base& out) const
{
    if (get_type() != type_c1 || get_code() == c1_csi)
        return false;

    str_iter iter(get_pointer(), get_length());

    // Skip announce
    if (iter.next() == 0x1b)
        iter.next();

    const char* start = iter.get_pointer();

    // Skip until terminator
    while (int c = iter.peek())
    {
        if (c == 0x9c || c == 0x1b)
            break;

        iter.next();
    }

    out.clear();
    out.concat(start, int(iter.get_pointer() - start));
    return true;
}



//------------------------------------------------------------------------------
ecma48_iter::ecma48_iter(const char* s, ecma48_state& state, int len)
: m_iter(s, len)
, m_code(state.code)
, m_state(state)
, m_nested_cmd_str(0)
{
}

//------------------------------------------------------------------------------
const ecma48_code& ecma48_iter::next()
{
    m_code.m_str = m_iter.get_pointer();

    const char* copy = m_iter.get_pointer();
    bool done = true;
    while (1)
    {
        int c = m_iter.peek();
        if (!c)
        {
            if (m_state.state != ecma48_state_char)
            {
                m_code.m_length = 0;
                return m_code;
            }

            break;
        }

        assert(m_nested_cmd_str == 0 || m_state.state == ecma48_state_cmd_str);

        switch (m_state.state)
        {
        case ecma48_state_char:     done = next_char(c);     break;
        case ecma48_state_char_str: done = next_char_str(c); break;
        case ecma48_state_cmd_str:  done = next_cmd_str(c);  break;
        case ecma48_state_csi_f:    done = next_csi_f(c);    break;
        case ecma48_state_csi_p:    done = next_csi_p(c);    break;
        case ecma48_state_esc:      done = next_esc(c);      break;
        case ecma48_state_esc_st:   done = next_esc_st(c);   break;
        case ecma48_state_unknown:  done = next_unknown(c);  break;
        }

        if (m_state.state != ecma48_state_char)
        {
            while (copy != m_iter.get_pointer())
            {
                m_state.buffer[m_state.count] = *copy++;
                m_state.count += (m_state.count < sizeof_array(m_state.buffer) - 1);
            }
        }

        if (done)
            break;
    }

    if (m_state.state != ecma48_state_char)
    {
        m_code.m_str = m_state.buffer;
        m_code.m_length = m_state.count;
    }
    else
        m_code.m_length = int(m_iter.get_pointer() - m_code.get_pointer());

    m_state.reset();

    assert(m_nested_cmd_str == 0);
    m_nested_cmd_str = 0;

    return m_code;
}

//------------------------------------------------------------------------------
bool ecma48_iter::next_c1()
{
    // Convert c1 code to its 7-bit version.
    m_code.m_code = (m_code.m_code & 0x1f) | 0x40;

    switch (m_code.get_code())
    {
        case 0x50: /* dcs */
        case 0x5d: /* osc */
        case 0x5e: /* pm  */
        case 0x5f: /* apc */
            m_state.state = ecma48_state_cmd_str;
            return false;

        case 0x5b: /* csi */
            m_state.state = ecma48_state_csi_p;
            return false;

        case 0x58: /* sos */
            m_state.state = ecma48_state_char_str;
            return false;
    }

    return true;
}

//------------------------------------------------------------------------------
bool ecma48_iter::next_char(int c)
{
    if (in_range(c, 0x00, 0x1f))
    {
        m_code.m_type = ecma48_code::type_chars;
        return true;
    }

    m_iter.next();
    return false;
}

//------------------------------------------------------------------------------
bool ecma48_iter::next_char_str(int c)
{
    m_iter.next();

    if (c == 0x1b)
    {
        m_state.state = ecma48_state_esc_st;
        return false;
    }

    return (c == 0x9c);
}

//------------------------------------------------------------------------------
bool ecma48_iter::next_cmd_str(int c)
{
    if (c == 0x1b)
    {
        m_iter.next();
        int d = m_iter.peek();
        if (d == 0x5d)
            m_nested_cmd_str++;
        else if (d == 0x5c && m_nested_cmd_str > 0)
            m_nested_cmd_str--;
        else
            m_state.state = ecma48_state_esc_st;
        return false;
    }
    else if (c == 0x9c || c == 0x07) // Xterm supports OSC terminated by BEL.
    {
        m_iter.next();
        if (c == 0x07 && m_nested_cmd_str > 0)
        {
            m_nested_cmd_str--;
            return false;
        }
        return true;
    }
    else if (in_range(c, 0x08, 0x0d) || in_range(c, 0x20, 0x7e))
    {
        m_iter.next();
        return false;
    }

    // Reset
    m_code.m_str = m_iter.get_pointer();
    m_code.m_length = 0;
    m_state.reset();
    return false;
}

//------------------------------------------------------------------------------
bool ecma48_iter::next_csi_f(int c)
{
    if (in_range(c, 0x20, 0x2f))
    {
        m_iter.next();
        return false;
    }
    else if (in_range(c, 0x40, 0x7e))
    {
        m_iter.next();
        return true;
    }

    // Reset
    m_code.m_str = m_iter.get_pointer();
    m_code.m_length = 0;
    m_state.reset();
    return false;
}

//------------------------------------------------------------------------------
bool ecma48_iter::next_csi_p(int c)
{
    if (in_range(c, 0x30, 0x3f))
    {
        m_iter.next();
        return false;
    }

    m_state.state = ecma48_state_csi_f;
    return next_csi_f(c);
}

//------------------------------------------------------------------------------
bool ecma48_iter::next_esc(int c)
{
    m_iter.next();

    if (in_range(c, 0x40, 0x5f))
    {
        m_code.m_type = ecma48_code::type_c1;
        m_code.m_code = c;
        return next_c1();
    }
    else if (in_range(c, 0x60, 0x7f))
    {
        m_code.m_type = ecma48_code::type_icf;
        m_code.m_code = c;
        return true;
    }

    m_state.state = ecma48_state_char;
    return false;
}

//------------------------------------------------------------------------------
bool ecma48_iter::next_esc_st(int c)
{
    if (c == 0x5c)
    {
        m_iter.next();
        assert(m_nested_cmd_str == 0);
        return true;
    }

    m_code.m_str = m_iter.get_pointer();
    m_code.m_length = 0;
    m_state.reset();
    m_nested_cmd_str = 0;
    return false;
}

//------------------------------------------------------------------------------
bool ecma48_iter::next_unknown(int c)
{
    m_iter.next();

    if (c == 0x1b)
    {
        m_state.state = ecma48_state_esc;
        return false;
    }
    else if (in_range(c, 0x00, 0x1f))
    {
        m_code.m_type = ecma48_code::type_c0;
        m_code.m_code = c;
        return true;
    }
    else if (in_range(c, 0x80, 0x9f))
    {
        m_code.m_type = ecma48_code::type_c1;
        m_code.m_code = c;
        return next_c1();
    }

    m_code.m_type = ecma48_code::type_chars;
    m_state.state = ecma48_state_char;
    return false;
}
