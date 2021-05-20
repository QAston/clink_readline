// Copyright (c) 2016 Martin Ridgers
// License: http://opensource.org/licenses/MIT

#include "pch.h"
#include "match_pipeline.h"
#include "line_state.h"
#include "match_generator.h"
#include "match_pipeline.h"
#include "matches_impl.h"

#include <core/array.h>
#include <core/path.h>
#include <core/str_compare.h>
#include <core/settings.h>
#include <terminal/ecma48_iter.h>
#include <readline/readline.h>

#include <algorithm>
#include <assert.h>

//------------------------------------------------------------------------------
static setting_enum g_sort_dirs(
    "match.sort_dirs",
    "Where to sort matching directories",
    "Matching directories can go before files, with files, or after files.",
    "before,with,after",
    1);



//------------------------------------------------------------------------------
extern "C" {
extern int rl_complete_with_tilde_expansion;
extern int _rl_completion_case_fold;
extern int _rl_locale_sort;
int compare_string(const char* s1, const char* s2, int casefold);
};

static bool s_nosort = false;



//------------------------------------------------------------------------------
static unsigned int normal_selector(
    const char* needle,
    match_info* infos,
    int count)
{
    int select_count = 0;
    for (int i = 0; i < count; ++i)
    {
        const char* name = infos[i].match;
        int j = str_compare(needle, name);
        infos[i].select = (j < 0 || !needle[j]);
        ++select_count;
    }

    return select_count;
}

//------------------------------------------------------------------------------
static bool is_dir_match(const wstr_base& match, match_type type)
{
    if (is_match_type(type, match_type::dir))
        return true;
    if (!is_match_type(type, match_type::none))
        return false;
    if (match.empty())
        return false;
    return path::is_separator(match.c_str()[match.length() - 1]);
}

//------------------------------------------------------------------------------
inline bool sort_worker(wstr_base& l, match_type l_type,
                        wstr_base& r, match_type r_type,
                        int order)
{
    bool l_dir = is_dir_match(l, l_type);
    bool r_dir = is_dir_match(r, r_type);

    if (order != 1 && l_dir != r_dir)
        return (order == 0) ? l_dir : r_dir;

    if (l_dir)
        path::maybe_strip_last_separator(l);
    if (r_dir)
        path::maybe_strip_last_separator(r);

    DWORD flags = SORT_DIGITSASNUMBERS|NORM_LINGUISTIC_CASING;
    if (true/*casefold*/)
        flags |= LINGUISTIC_IGNORECASE;
    int cmp = CompareStringW(LOCALE_USER_DEFAULT, flags,
                            l.c_str(), l.length(),
                            r.c_str(), r.length());
    cmp -= CSTR_EQUAL;
    if (cmp) return (cmp < 0);

    unsigned char t1 = ((unsigned char)l_type) & MATCH_TYPE_MASK;
    unsigned char t2 = ((unsigned char)r_type) & MATCH_TYPE_MASK;

    cmp = int(t1 == MATCH_TYPE_DIR) - int(t2 == MATCH_TYPE_DIR);
    if (cmp) return (cmp < 0);

    cmp = int(t1 == MATCH_TYPE_ALIAS) - int(t2 == MATCH_TYPE_ALIAS);
    if (cmp) return (cmp < 0);

    cmp = int(t1 == MATCH_TYPE_WORD) - int(t2 == MATCH_TYPE_WORD);
    if (cmp) return (cmp < 0);

    cmp = int(t1 == MATCH_TYPE_ARG) - int(t2 == MATCH_TYPE_ARG);
    if (cmp) return (cmp < 0);

    cmp = int(t1 == MATCH_TYPE_FILE) - int(t2 == MATCH_TYPE_FILE);
    if (cmp) return (cmp < 0);

    return (cmp < 0);
}

//------------------------------------------------------------------------------
//#define SORT_MATCH_PIPELINE
#ifdef SORT_MATCH_PIPELINE // Unused.
static void alpha_sorter(match_info* infos, int count)
{
    int order = g_sort_dirs.get();
    wstr<> ltmp;
    wstr<> rtmp;

    auto predicate = [&] (const match_info& lhs, const match_info& rhs) {
        ltmp.clear();
        rtmp.clear();
        to_utf16(ltmp, lhs.match);
        to_utf16(rtmp, rhs.match);
        return sort_worker(ltmp, lhs.type, rtmp, rhs.type, order);
    };

    std::sort(infos, infos + count, predicate);
}
#endif

//------------------------------------------------------------------------------
static int _cdecl qsort_match_compare(const void* pv1, const void* pv2)
{
    const char** s1 = (const char**)pv1;
    const char** s2 = (const char**)pv2;
    return compare_string(*s1, *s2, 1 /*casefold*/);
}

//------------------------------------------------------------------------------
void sort_match_list(char** matches, int len)
{
    if (s_nosort || len <= 0)
        return;

    if (!rl_completion_matches_include_type)
    {
        qsort(matches, len, sizeof(matches[0]), qsort_match_compare);
        return;
    }

    int order = g_sort_dirs.get();
    wstr<> ltmp;
    wstr<> rtmp;

    auto predicate = [&] (const char* l, const char* r) {
        match_type l_type = match_type(*(l++));
        match_type r_type = match_type(*(r++));

        ltmp.clear();
        rtmp.clear();
        to_utf16(ltmp, l);
        to_utf16(rtmp, r);

        return sort_worker(ltmp, l_type, rtmp, r_type, order);
    };

    std::sort(matches, matches + len, predicate);
}



//------------------------------------------------------------------------------
match_pipeline::match_pipeline(matches_impl& matches)
: m_matches(matches)
{
}

//------------------------------------------------------------------------------
void match_pipeline::reset() const
{
    m_matches.reset();
    s_nosort = false;
}

//------------------------------------------------------------------------------
void match_pipeline::set_nosort(bool nosort)
{
    s_nosort = nosort;
}

//------------------------------------------------------------------------------
void match_pipeline::generate(
    const line_state& state,
    const array<match_generator*>& generators) const
{
    m_matches.set_word_break_position(state.get_end_word_offset());

    match_builder builder(m_matches);
    for (auto* generator : generators)
        if (generator->generate(state, builder))
            break;

#ifdef DEBUG
    if (dbg_get_env_int("DEBUG_PIPELINE"))
    {
        printf("GENERATE, %u matches, file_comp %u %s --%s",
               m_matches.get_match_count(),
               m_matches.is_filename_completion_desired().get(),
               m_matches.is_filename_completion_desired().is_explicit() ? "(exp)" : "(imp)",
               m_matches.get_match_count() ? "" : " <none>");

        int i = 0;
        for (matches_iter iter = m_matches.get_iter(); i < 21 && iter.next(); i++)
        {
            if (i == 20)
                printf(" ...");
            else
                printf(" %s", iter.get_match());
        }
        printf("\n");
    }
#endif
}

//------------------------------------------------------------------------------
void match_pipeline::select(const char* needle) const
{
    int count = m_matches.get_info_count();
    unsigned int selected_count = 0;

    char* expanded = nullptr;
    if (rl_complete_with_tilde_expansion)
    {
        expanded = tilde_expand(needle);
        if (expanded)
            needle = expanded;
    }

#ifdef DEBUG
    str<32> debug_needle(needle); // needle goes out of scope before DEBUG_PIPELINE.
#endif

    if (count)
        selected_count = normal_selector(needle, m_matches.get_infos(), count);

    m_matches.coalesce(selected_count);

    free(expanded);

#ifdef DEBUG
    if (dbg_get_env_int("DEBUG_PIPELINE"))
    {
        printf("COALESCED, file_comp %u %s -- needle '%s' selected %u matches\n",
               m_matches.is_filename_completion_desired().get(),
               m_matches.is_filename_completion_desired().is_explicit() ? "(exp)" : "(imp)",
               debug_needle.c_str(),
               m_matches.get_match_count());
    }
#endif
}

//------------------------------------------------------------------------------
void match_pipeline::sort() const
{
    // There is no point in sorting here.  Readline re-sorts whatever we do here
    // anyway.  Unless sorting is turned off in Readline, in which case maybe we
    // shouldn't be sorting here anyway.
#ifdef SORT_MATCH_PIPELINE
    int count = m_matches.get_match_count();
    if (!count)
        return;

    alpha_sorter(m_matches.get_infos(), count);
#endif
}
