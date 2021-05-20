// Copyright (c) 2015 Martin Ridgers
// License: http://opensource.org/licenses/MIT

#pragma once

#include <core/str_iter.h>

#include <vector>

//------------------------------------------------------------------------------
struct word
{
    unsigned int        offset : 16;
    unsigned int        length : 16;
    bool                command_word : 1;
    bool                is_alias : 1;
    bool                quoted;
    unsigned char       delim;
};

//------------------------------------------------------------------------------
class line_state
{
public:
                        line_state(const char* line, unsigned int cursor, unsigned int command_offset, const std::vector<word>& words);
    const char*         get_line() const;
    unsigned int        get_cursor() const;
    unsigned int        get_command_offset() const;
    unsigned int        get_end_word_offset() const;
    const std::vector<word>& get_words() const;
    unsigned int        get_word_count() const;
    bool                get_word(unsigned int index, str_base& out) const;  // STRIPS quotes.
    str_iter            get_word(unsigned int index) const;                 // INCLUDES quotes.
    bool                get_end_word(str_base& out) const;                  // STRIPS quotes.
    str_iter            get_end_word() const;                               // INCLUDES quotes.

private:
    const std::vector<word>& m_words;
    const char*         m_line;
    unsigned int        m_cursor;
    unsigned int        m_command_offset;
};
