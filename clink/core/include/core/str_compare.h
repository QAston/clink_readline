// Copyright (c) 2015 Martin Ridgers
// License: http://opensource.org/licenses/MIT

#pragma once

#include "base.h"
#include "path.h"
#include "str_iter.h"

#include <Windows.h>
#include <assert.h>

#include <map>

//------------------------------------------------------------------------------
class str_compare_scope
{
public:
    enum
    {
        exact,
        caseless,
        relaxed,    // case insensitive with -/_ considered equivalent.
        num_scope_values
    };

                str_compare_scope(int mode, bool fuzzy_accent=false);
                ~str_compare_scope();
    static int  current();
    static bool current_fuzzy_accents();

private:
    int         m_prev_mode;
    bool        m_prev_fuzzy_accents;
    threadlocal static int ts_mode;
    threadlocal static bool ts_fuzzy_accents;
};



//------------------------------------------------------------------------------
int normalize_accent(int c);

//------------------------------------------------------------------------------
// Returns how many characters match at the beginning of the strings, or -1 if
// the entire strings match.
template <class T, int MODE, bool fuzzy_accents>
int str_compare_impl(str_iter_impl<T>& lhs, str_iter_impl<T>& rhs)
{
    const T* start = lhs.get_pointer();

    while (1)
    {
        int c = lhs.peek();
        int d = rhs.peek();
        if (!c || !d)
            break;

        if (MODE > 0)
        {
            c = (c > 0xffff) ? c : int(uintptr_t(CharLowerW(LPWSTR(uintptr_t(c)))));
            d = (d > 0xffff) ? d : int(uintptr_t(CharLowerW(LPWSTR(uintptr_t(d)))));
        }

        if (MODE > 1)
        {
            c = (c == '-') ? '_' : c;
            d = (d == '-') ? '_' : d;
        }

        if (c == '\\') c = '/';
        if (d == '\\') d = '/';

        if (c != d)
        {
            if (!fuzzy_accents)
                break;
            c = normalize_accent(c);
            d = normalize_accent(d);
            if (c != d)
                break;
        }

        lhs.next();
        rhs.next();

        // Advance past path separators (consider "\\\\" and "\" equal).
        assert((c == '/') == (d == '/'));
        if (c == '/')
        {
            while (path::is_separator(lhs.peek()))
                lhs.next();
            while (path::is_separator(rhs.peek()))
                rhs.next();
        }
    }

    if (lhs.more() || rhs.more())
        return int(lhs.get_pointer() - start);

    return -1;
}

//------------------------------------------------------------------------------
template <class T>
int str_compare(str_iter_impl<T>& lhs, str_iter_impl<T>& rhs)
{
    bool fuzzy_accents = str_compare_scope::current_fuzzy_accents();
    switch (str_compare_scope::current())
    {
    case str_compare_scope::relaxed:
        if (fuzzy_accents)  return str_compare_impl<T, 2, true>(lhs, rhs);
        else                return str_compare_impl<T, 2, false>(lhs, rhs);
    case str_compare_scope::caseless:
        if (fuzzy_accents)  return str_compare_impl<T, 1, true>(lhs, rhs);
        else                return str_compare_impl<T, 1, false>(lhs, rhs);
    default:
        if (fuzzy_accents)  return str_compare_impl<T, 0, true>(lhs, rhs);
        else                return str_compare_impl<T, 0, false>(lhs, rhs);
    }
}

//------------------------------------------------------------------------------
template <class T>
int str_compare(const T* lhs, const T* rhs)
{
    str_iter_impl<T> lhs_iter(lhs);
    str_iter_impl<T> rhs_iter(rhs);
    return str_compare(lhs_iter, rhs_iter);
}

//------------------------------------------------------------------------------
template <class T>
int str_compare(const str_impl<T>& lhs, const str_impl<T>& rhs)
{
    str_iter_impl<T> lhs_iter(lhs);
    str_iter_impl<T> rhs_iter(rhs);
    return str_compare(lhs_iter, rhs_iter);
}
