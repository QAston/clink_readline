/* Readline.h -- the names of functions callable from within readline. */

/* Copyright (C) 1987-2020 Free Software Foundation, Inc.

   This file is part of the GNU Readline Library (Readline), a library
   for reading lines of text with interactive input and history editing.      

   Readline is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   Readline is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Readline.  If not, see <http://www.gnu.org/licenses/>.
*/

#if !defined (_READLINE_H_)
#define _READLINE_H_

#ifdef __cplusplus
READLINE_API "C" {
#endif

#if defined (READLINE_LIBRARY)
#  include "rlstdc.h"
#  include "rltypedefs.h"
#  include "keymaps.h"
#  include "tilde.h"
#else
#  include <readline/rlstdc.h>
#  include <readline/rltypedefs.h>
#  include <readline/keymaps.h>
#  include <readline/tilde.h>
#endif

/* Hex-encoded Readline version number. */
#define RL_READLINE_VERSION	0x0801		/* Readline 8.0 */
#define RL_VERSION_MAJOR	8
#define RL_VERSION_MINOR	1

/* begin_clink_change */
#define VER_AS_STR(x)    #x
#define MAKE_LIBRARY_VERSION(maj, min)    VER_AS_STR(maj) ## "." ## VER_AS_STR(min)
#define RL_LIBRARY_VERSION    MAKE_LIBRARY_VERSION(RL_VERSION_MAJOR, RL_VERSION_MINOR)
/* end_clink_change */


/* Readline data structures. */

/* Maintaining the state of undo.  We remember individual deletes and inserts
   on a chain of things to do. */

/* The actions that undo knows how to undo.  Notice that UNDO_DELETE means
   to insert some text, and UNDO_INSERT means to delete some text.   I.e.,
   the code tells undo what to undo, not how to undo it. */
enum undo_code { UNDO_DELETE, UNDO_INSERT, UNDO_BEGIN, UNDO_END };

/* What an element of THE_UNDO_LIST looks like. */
typedef struct undo_list {
  struct undo_list *next;
  int start, end;		/* Where the change took place. */
  char *text;			/* The text to insert, if undoing a delete. */
  enum undo_code what;		/* Delete, Insert, Begin, End. */
} UNDO_LIST;

/* The current undo list for RL_LINE_BUFFER. */
READLINE_API UNDO_LIST *rl_undo_list;

/* The data structure for mapping textual names to code addresses. */
typedef struct _funmap {
  const char *name;
  rl_command_func_t *function;
} FUNMAP;

READLINE_API FUNMAP **funmap;

/* **************************************************************** */
/*								    */
/*	     Functions available to bind to key sequences	    */
/*								    */
/* **************************************************************** */

/* Bindable commands for numeric arguments. */
READLINE_API int rl_digit_argument PARAMS((int, int));
READLINE_API int rl_universal_argument PARAMS((int, int));

/* Bindable commands for moving the cursor. */
READLINE_API int rl_forward_byte PARAMS((int, int));
READLINE_API int rl_forward_char PARAMS((int, int));
READLINE_API int rl_forward PARAMS((int, int));
READLINE_API int rl_backward_byte PARAMS((int, int));
READLINE_API int rl_backward_char PARAMS((int, int));
READLINE_API int rl_backward PARAMS((int, int));
READLINE_API int rl_beg_of_line PARAMS((int, int));
READLINE_API int rl_end_of_line PARAMS((int, int));
READLINE_API int rl_forward_word PARAMS((int, int));
READLINE_API int rl_backward_word PARAMS((int, int));
READLINE_API int rl_refresh_line PARAMS((int, int));
READLINE_API int rl_clear_screen PARAMS((int, int));
READLINE_API int rl_clear_display PARAMS((int, int));
READLINE_API int rl_skip_csi_sequence PARAMS((int, int));
READLINE_API int rl_arrow_keys PARAMS((int, int));

READLINE_API int rl_previous_screen_line PARAMS((int, int));
READLINE_API int rl_next_screen_line PARAMS((int, int));

/* Bindable commands for inserting and deleting text. */
READLINE_API int rl_insert PARAMS((int, int));
READLINE_API int rl_quoted_insert PARAMS((int, int));
READLINE_API int rl_tab_insert PARAMS((int, int));
READLINE_API int rl_newline PARAMS((int, int));
READLINE_API int rl_do_lowercase_version PARAMS((int, int));
READLINE_API int rl_rubout PARAMS((int, int));
READLINE_API int rl_delete PARAMS((int, int));
READLINE_API int rl_rubout_or_delete PARAMS((int, int));
READLINE_API int rl_delete_horizontal_space PARAMS((int, int));
READLINE_API int rl_delete_or_show_completions PARAMS((int, int));
READLINE_API int rl_insert_comment PARAMS((int, int));

/* Bindable commands for changing case. */
READLINE_API int rl_upcase_word PARAMS((int, int));
READLINE_API int rl_downcase_word PARAMS((int, int));
READLINE_API int rl_capitalize_word PARAMS((int, int));

/* Bindable commands for transposing characters and words. */
READLINE_API int rl_transpose_words PARAMS((int, int));
READLINE_API int rl_transpose_chars PARAMS((int, int));

/* Bindable commands for searching within a line. */
READLINE_API int rl_char_search PARAMS((int, int));
READLINE_API int rl_backward_char_search PARAMS((int, int));

/* Bindable commands for readline's interface to the command history. */
/* begin_clink_change */
READLINE_API int rl_add_history PARAMS((int, int));
/* end_clink_change */
READLINE_API int rl_beginning_of_history PARAMS((int, int));
READLINE_API int rl_end_of_history PARAMS((int, int));
READLINE_API int rl_get_next_history PARAMS((int, int));
READLINE_API int rl_get_previous_history PARAMS((int, int));
READLINE_API int rl_operate_and_get_next PARAMS((int, int));
/* begin_clink_change */
READLINE_API int rl_remove_history PARAMS((int, int));
/* end_clink_change */

/* Bindable commands for managing the mark and region. */
READLINE_API int rl_set_mark PARAMS((int, int));
READLINE_API int rl_exchange_point_and_mark PARAMS((int, int));

/* Bindable commands to set the editing mode (emacs or vi). */
READLINE_API int rl_vi_editing_mode PARAMS((int, int));
READLINE_API int rl_emacs_editing_mode PARAMS((int, int));

/* Bindable commands to change the insert mode (insert or overwrite) */
READLINE_API int rl_overwrite_mode PARAMS((int, int));

/* Bindable commands for managing key bindings. */
READLINE_API int rl_re_read_init_file PARAMS((int, int));
READLINE_API int rl_dump_functions PARAMS((int, int));
READLINE_API int rl_dump_macros PARAMS((int, int));
READLINE_API int rl_dump_variables PARAMS((int, int));

/* Bindable commands for word completion. */
READLINE_API int rl_complete PARAMS((int, int));
READLINE_API int rl_possible_completions PARAMS((int, int));
READLINE_API int rl_insert_completions PARAMS((int, int));
READLINE_API int rl_old_menu_complete PARAMS((int, int));
/* begin_clink_change */
READLINE_API int rl_backward_old_menu_complete PARAMS((int, int));
/* end_clink_change */
READLINE_API int rl_menu_complete PARAMS((int, int));
READLINE_API int rl_backward_menu_complete PARAMS((int, int));
/* begin_clink_change */
READLINE_API char** rl_get_completions PARAMS((int, int*, char**, int*, int*, int*, char*));
READLINE_API void rl_insert_match PARAMS((char*, char*, int, int, char));
/* end_clink_change */

/* Bindable commands for killing and yanking text, and managing the kill ring. */
READLINE_API int rl_kill_word PARAMS((int, int));
READLINE_API int rl_backward_kill_word PARAMS((int, int));
READLINE_API int rl_kill_line PARAMS((int, int));
READLINE_API int rl_backward_kill_line PARAMS((int, int));
READLINE_API int rl_kill_full_line PARAMS((int, int));
READLINE_API int rl_unix_word_rubout PARAMS((int, int));
READLINE_API int rl_unix_filename_rubout PARAMS((int, int));
READLINE_API int rl_unix_line_discard PARAMS((int, int));
READLINE_API int rl_copy_region_to_kill PARAMS((int, int));
READLINE_API int rl_kill_region PARAMS((int, int));
READLINE_API int rl_copy_forward_word PARAMS((int, int));
READLINE_API int rl_copy_backward_word PARAMS((int, int));
READLINE_API int rl_yank PARAMS((int, int));
READLINE_API int rl_yank_pop PARAMS((int, int));
READLINE_API int rl_yank_nth_arg PARAMS((int, int));
READLINE_API int rl_yank_last_arg PARAMS((int, int));
READLINE_API int rl_bracketed_paste_begin PARAMS((int, int));
/* Not available unless _WIN32 is defined. */
#if defined (_WIN32)
READLINE_API int rl_paste_from_clipboard PARAMS((int, int));
#endif

/* Bindable commands for incremental searching. */
READLINE_API int rl_reverse_search_history PARAMS((int, int));
READLINE_API int rl_forward_search_history PARAMS((int, int));

/* Bindable keyboard macro commands. */
READLINE_API int rl_start_kbd_macro PARAMS((int, int));
READLINE_API int rl_end_kbd_macro PARAMS((int, int));
READLINE_API int rl_call_last_kbd_macro PARAMS((int, int));
READLINE_API int rl_print_last_kbd_macro PARAMS((int, int));

/* Bindable undo commands. */
READLINE_API int rl_revert_line PARAMS((int, int));
READLINE_API int rl_undo_command PARAMS((int, int));

/* Bindable tilde expansion commands. */
READLINE_API int rl_tilde_expand PARAMS((int, int));

/* Bindable terminal control commands. */
READLINE_API int rl_restart_output PARAMS((int, int));
READLINE_API int rl_stop_output PARAMS((int, int));

/* Miscellaneous bindable commands. */
READLINE_API int rl_abort PARAMS((int, int));
READLINE_API int rl_tty_status PARAMS((int, int));

/* Bindable commands for incremental and non-incremental history searching. */
READLINE_API int rl_history_search_forward PARAMS((int, int));
READLINE_API int rl_history_search_backward PARAMS((int, int));
READLINE_API int rl_history_substr_search_forward PARAMS((int, int));
READLINE_API int rl_history_substr_search_backward PARAMS((int, int));
READLINE_API int rl_noninc_forward_search PARAMS((int, int));
READLINE_API int rl_noninc_reverse_search PARAMS((int, int));
READLINE_API int rl_noninc_forward_search_again PARAMS((int, int));
READLINE_API int rl_noninc_reverse_search_again PARAMS((int, int));

/* Bindable command used when inserting a matching close character. */
READLINE_API int rl_insert_close PARAMS((int, int));

/* Not available unless READLINE_CALLBACKS is defined. */
READLINE_API void rl_callback_handler_install PARAMS((const char *, rl_vcpfunc_t *));
READLINE_API void rl_callback_read_char PARAMS((void));
READLINE_API void rl_callback_handler_remove PARAMS((void));
READLINE_API void rl_callback_sigcleanup PARAMS((void));
/* begin_clink_change */
READLINE_API int rl_is_insert_next_callback_pending PARAMS((void));
/* end_clink_change */

/* Things for vi mode. Not available unless readline is compiled -DVI_MODE. */
/* VI-mode bindable commands. */
READLINE_API int rl_vi_redo PARAMS((int, int));
READLINE_API int rl_vi_undo PARAMS((int, int));
READLINE_API int rl_vi_yank_arg PARAMS((int, int));
READLINE_API int rl_vi_fetch_history PARAMS((int, int));
READLINE_API int rl_vi_search_again PARAMS((int, int));
READLINE_API int rl_vi_search PARAMS((int, int));
READLINE_API int rl_vi_complete PARAMS((int, int));
READLINE_API int rl_vi_tilde_expand PARAMS((int, int));
READLINE_API int rl_vi_prev_word PARAMS((int, int));
READLINE_API int rl_vi_next_word PARAMS((int, int));
READLINE_API int rl_vi_end_word PARAMS((int, int));
READLINE_API int rl_vi_insert_beg PARAMS((int, int));
READLINE_API int rl_vi_append_mode PARAMS((int, int));
READLINE_API int rl_vi_append_eol PARAMS((int, int));
READLINE_API int rl_vi_eof_maybe PARAMS((int, int));
READLINE_API int rl_vi_insertion_mode PARAMS((int, int));
READLINE_API int rl_vi_insert_mode PARAMS((int, int));
READLINE_API int rl_vi_movement_mode PARAMS((int, int));
READLINE_API int rl_vi_arg_digit PARAMS((int, int));
READLINE_API int rl_vi_change_case PARAMS((int, int));
READLINE_API int rl_vi_put PARAMS((int, int));
READLINE_API int rl_vi_column PARAMS((int, int));
READLINE_API int rl_vi_delete_to PARAMS((int, int));
READLINE_API int rl_vi_change_to PARAMS((int, int));
READLINE_API int rl_vi_yank_to PARAMS((int, int));
READLINE_API int rl_vi_yank_pop PARAMS((int, int));
READLINE_API int rl_vi_rubout PARAMS((int, int));
READLINE_API int rl_vi_delete PARAMS((int, int));
READLINE_API int rl_vi_back_to_indent PARAMS((int, int));
READLINE_API int rl_vi_unix_word_rubout PARAMS((int, int));
READLINE_API int rl_vi_first_print PARAMS((int, int));
READLINE_API int rl_vi_char_search PARAMS((int, int));
READLINE_API int rl_vi_match PARAMS((int, int));
READLINE_API int rl_vi_change_char PARAMS((int, int));
READLINE_API int rl_vi_subst PARAMS((int, int));
READLINE_API int rl_vi_overstrike PARAMS((int, int));
READLINE_API int rl_vi_overstrike_delete PARAMS((int, int));
READLINE_API int rl_vi_replace PARAMS((int, int));
READLINE_API int rl_vi_set_mark PARAMS((int, int));
READLINE_API int rl_vi_goto_mark PARAMS((int, int));

/* VI-mode utility functions. */
READLINE_API int rl_vi_check PARAMS((void));
READLINE_API int rl_vi_domove PARAMS((int, int *));
READLINE_API int rl_vi_bracktype PARAMS((int));

READLINE_API void rl_vi_start_inserting PARAMS((int, int, int));

/* VI-mode pseudo-bindable commands, used as utility functions. */
READLINE_API int rl_vi_fWord PARAMS((int, int));
READLINE_API int rl_vi_bWord PARAMS((int, int));
READLINE_API int rl_vi_eWord PARAMS((int, int));
READLINE_API int rl_vi_fword PARAMS((int, int));
READLINE_API int rl_vi_bword PARAMS((int, int));
READLINE_API int rl_vi_eword PARAMS((int, int));

/* **************************************************************** */
/*								    */
/*			Well Published Functions		    */
/*								    */
/* **************************************************************** */

/* Readline functions. */
/* Read a line of input.  Prompt with PROMPT.  A NULL PROMPT means none. */
READLINE_API char *readline PARAMS((const char *));

READLINE_API int rl_set_prompt PARAMS((const char *));
READLINE_API int rl_expand_prompt PARAMS((char *));

READLINE_API int rl_initialize PARAMS((void));

/* Undocumented; unused by readline */
READLINE_API int rl_discard_argument PARAMS((void));

/* Utility functions to bind keys to readline commands. */
READLINE_API int rl_add_defun PARAMS((const char *, rl_command_func_t *, int));
READLINE_API int rl_bind_key PARAMS((int, rl_command_func_t *));
READLINE_API int rl_bind_key_in_map PARAMS((int, rl_command_func_t *, Keymap));
READLINE_API int rl_unbind_key PARAMS((int));
READLINE_API int rl_unbind_key_in_map PARAMS((int, Keymap));
READLINE_API int rl_bind_key_if_unbound PARAMS((int, rl_command_func_t *));
READLINE_API int rl_bind_key_if_unbound_in_map PARAMS((int, rl_command_func_t *, Keymap));
READLINE_API int rl_unbind_function_in_map PARAMS((rl_command_func_t *, Keymap));
READLINE_API int rl_unbind_command_in_map PARAMS((const char *, Keymap));
READLINE_API int rl_bind_keyseq PARAMS((const char *, rl_command_func_t *));
READLINE_API int rl_bind_keyseq_in_map PARAMS((const char *, rl_command_func_t *, Keymap));
READLINE_API int rl_bind_keyseq_if_unbound PARAMS((const char *, rl_command_func_t *));
READLINE_API int rl_bind_keyseq_if_unbound_in_map PARAMS((const char *, rl_command_func_t *, Keymap));
READLINE_API int rl_generic_bind PARAMS((int, const char *, char *, Keymap));

READLINE_API char *rl_variable_value PARAMS((const char *));
READLINE_API int rl_variable_bind PARAMS((const char *, const char *));

/* Backwards compatibility, use rl_bind_keyseq_in_map instead. */
READLINE_API int rl_set_key PARAMS((const char *, rl_command_func_t *, Keymap));

/* Backwards compatibility, use rl_generic_bind instead. */
READLINE_API int rl_macro_bind PARAMS((const char *, const char *, Keymap));

/* Undocumented in the texinfo manual; not really useful to programs. */
READLINE_API int rl_translate_keyseq PARAMS((const char *, char *, int *));
READLINE_API char *rl_untranslate_keyseq PARAMS((int));

READLINE_API rl_command_func_t *rl_named_function PARAMS((const char *));
READLINE_API rl_command_func_t *rl_function_of_keyseq PARAMS((const char *, Keymap, int *));
READLINE_API rl_command_func_t *rl_function_of_keyseq_len PARAMS((const char *, size_t, Keymap, int *));

READLINE_API void rl_list_funmap_names PARAMS((void));
READLINE_API char **rl_invoking_keyseqs_in_map PARAMS((rl_command_func_t *, Keymap));
READLINE_API char **rl_invoking_keyseqs PARAMS((rl_command_func_t *));
 
READLINE_API void rl_function_dumper PARAMS((int));
READLINE_API void rl_macro_dumper PARAMS((int));
READLINE_API void rl_variable_dumper PARAMS((int));

READLINE_API int rl_read_init_file PARAMS((const char *));
READLINE_API int rl_parse_and_bind PARAMS((char *));

/* Functions for manipulating keymaps. */
READLINE_API Keymap rl_make_bare_keymap PARAMS((void));
READLINE_API int rl_empty_keymap PARAMS((Keymap));
READLINE_API Keymap rl_copy_keymap PARAMS((Keymap));
READLINE_API Keymap rl_make_keymap PARAMS((void));
READLINE_API void rl_discard_keymap PARAMS((Keymap));
READLINE_API void rl_free_keymap PARAMS((Keymap));

READLINE_API Keymap rl_get_keymap_by_name PARAMS((const char *));
READLINE_API char *rl_get_keymap_name PARAMS((Keymap));
READLINE_API void rl_set_keymap PARAMS((Keymap));
READLINE_API Keymap rl_get_keymap PARAMS((void));

READLINE_API int rl_set_keymap_name PARAMS((const char *, Keymap));

/* Undocumented; used internally only. */
READLINE_API void rl_set_keymap_from_edit_mode PARAMS((void));
READLINE_API char *rl_get_keymap_name_from_edit_mode PARAMS((void));

/* Functions for manipulating the funmap, which maps command names to functions. */
READLINE_API int rl_add_funmap_entry PARAMS((const char *, rl_command_func_t *));
READLINE_API const char **rl_funmap_names PARAMS((void));
/* Undocumented, only used internally -- there is only one funmap, and this
   function may be called only once. */
READLINE_API void rl_initialize_funmap PARAMS((void));

/* Utility functions for managing keyboard macros. */
READLINE_API void rl_push_macro_input PARAMS((char *));

/* Functions for undoing, from undo.c */
READLINE_API void rl_add_undo PARAMS((enum undo_code, int, int, char *));
READLINE_API void rl_free_undo_list PARAMS((void));
READLINE_API int rl_do_undo PARAMS((void));
READLINE_API int rl_begin_undo_group PARAMS((void));
READLINE_API int rl_end_undo_group PARAMS((void));
READLINE_API int rl_modifying PARAMS((int, int));

/* Functions for redisplay. */
READLINE_API void rl_redisplay PARAMS((void));
READLINE_API int rl_on_new_line PARAMS((void));
READLINE_API int rl_on_new_line_with_prompt PARAMS((void));
READLINE_API int rl_forced_update_display PARAMS((void));
READLINE_API int rl_clear_visible_line PARAMS((void));
READLINE_API int rl_clear_message PARAMS((void));
READLINE_API int rl_reset_line_state PARAMS((void));
READLINE_API int rl_crlf PARAMS((void));

/* Functions to manage the mark and region, especially the notion of an
   active mark and an active region. */
READLINE_API void rl_keep_mark_active PARAMS((void));

READLINE_API void rl_activate_mark PARAMS((void));
READLINE_API void rl_deactivate_mark PARAMS((void));
READLINE_API int rl_mark_active_p PARAMS((void));

#if defined (USE_VARARGS) && defined (PREFER_STDARG)
READLINE_API int rl_message (const char *, ...)  __attribute__((__format__ (printf, 1, 2)));
#else
READLINE_API int rl_message ();
#endif

READLINE_API int rl_show_char PARAMS((int));

/* Undocumented in texinfo manual. */
READLINE_API int rl_character_len PARAMS((int, int));
READLINE_API void rl_redraw_prompt_last_line PARAMS((void));

/* Save and restore internal prompt redisplay information. */
READLINE_API void rl_save_prompt PARAMS((void));
READLINE_API void rl_restore_prompt PARAMS((void));

/* Modifying text. */
READLINE_API void rl_replace_line PARAMS((const char *, int));
READLINE_API int rl_insert_text PARAMS((const char *));
READLINE_API int rl_delete_text PARAMS((int, int));
READLINE_API int rl_kill_text PARAMS((int, int));
READLINE_API char *rl_copy_text PARAMS((int, int));

/* Terminal and tty mode management. */
READLINE_API void rl_prep_terminal PARAMS((int));
READLINE_API void rl_deprep_terminal PARAMS((void));
READLINE_API void rl_tty_set_default_bindings PARAMS((Keymap));
READLINE_API void rl_tty_unset_default_bindings PARAMS((Keymap));

READLINE_API int rl_tty_set_echoing PARAMS((int));
READLINE_API int rl_reset_terminal PARAMS((const char *));
READLINE_API void rl_resize_terminal PARAMS((void));
READLINE_API void rl_set_screen_size PARAMS((int, int));
READLINE_API void rl_get_screen_size PARAMS((int *, int *));
READLINE_API void rl_reset_screen_size PARAMS((void));

/* begin_clink_change */
READLINE_API rl_voidfunc_t *_rl_visual_bell_func;
/* end_clink_change */

READLINE_API char *rl_get_termcap PARAMS((const char *));

/* Functions for character input. */
READLINE_API int rl_stuff_char PARAMS((int));
READLINE_API int rl_execute_next PARAMS((int));
READLINE_API int rl_clear_pending_input PARAMS((void));
READLINE_API int rl_read_key PARAMS((void));
READLINE_API int rl_getc PARAMS((FILE *));
READLINE_API int rl_set_keyboard_input_timeout PARAMS((int));

/* `Public' utility functions . */
READLINE_API void rl_extend_line_buffer PARAMS((int));
READLINE_API int rl_ding PARAMS((void));
READLINE_API int rl_alphabetic PARAMS((int));
READLINE_API void rl_free PARAMS((void *));

/* Readline signal handling, from signals.c */
READLINE_API int rl_set_signals PARAMS((void));
READLINE_API int rl_clear_signals PARAMS((void));
READLINE_API void rl_cleanup_after_signal PARAMS((void));
READLINE_API void rl_reset_after_signal PARAMS((void));
READLINE_API void rl_free_line_state PARAMS((void));

READLINE_API int rl_pending_signal PARAMS((void));
READLINE_API void rl_check_signals PARAMS((void));

READLINE_API void rl_echo_signal_char PARAMS((int)); 

READLINE_API int rl_set_paren_blink_timeout PARAMS((int));

/* History management functions. */

READLINE_API void rl_clear_history PARAMS((void));

/* begin_clink_change */
/* Returns true if rl_operate_and_get_next has set a saved history offset,
   so that the host knows not to rearrange history entries yet. */
READLINE_API int rl_has_saved_history PARAMS((void));
/* end_clink_change */

/* Undocumented. */
READLINE_API int rl_maybe_save_line PARAMS((void));
READLINE_API int rl_maybe_unsave_line PARAMS((void));
READLINE_API int rl_maybe_replace_line PARAMS((void));

/* Completion functions. */
READLINE_API int rl_complete_internal PARAMS((int));
READLINE_API void rl_display_match_list PARAMS((char **, int, int));

READLINE_API char **rl_completion_matches PARAMS((const char *, rl_compentry_func_t *));
READLINE_API char *rl_username_completion_function PARAMS((const char *, int));
READLINE_API char *rl_filename_completion_function PARAMS((const char *, int));

READLINE_API int rl_completion_mode PARAMS((rl_command_func_t *));

/* begin_clink_change */
/* Path separator. */
READLINE_API int rl_is_path_separator PARAMS((char c));
/* Returns the last path separator character in string, or NULL if not found.
   (It's like strrchr, but supports multiple path separator characters.) */
READLINE_API char *rl_last_path_separator PARAMS((const char *string));
/* end_clink_change */

#if 0
/* Backwards compatibility (compat.c).  These will go away sometime. */
READLINE_API void free_undo_list PARAMS((void));
READLINE_API int maybe_save_line PARAMS((void));
READLINE_API int maybe_unsave_line PARAMS((void));
READLINE_API int maybe_replace_line PARAMS((void));

READLINE_API int ding PARAMS((void));
READLINE_API int alphabetic PARAMS((int));
READLINE_API int crlf PARAMS((void));

READLINE_API char **completion_matches PARAMS((char *, rl_compentry_func_t *));
READLINE_API char *username_completion_function PARAMS((const char *, int));
READLINE_API char *filename_completion_function PARAMS((const char *, int));
#endif

/* **************************************************************** */
/*								    */
/*			Well Published Variables		    */
/*								    */
/* **************************************************************** */

/* The version of this incarnation of the readline library. */
READLINE_API const char *rl_library_version;		/* e.g., "4.2" */
READLINE_API int rl_readline_version;			/* e.g., 0x0402 */

/* True if this is real GNU readline. */
READLINE_API int rl_gnu_readline_p;

/* Flags word encapsulating the current readline state. */
READLINE_API unsigned long rl_readline_state;

/* Says which editing mode readline is currently using.  1 means emacs mode;
   0 means vi mode. */
READLINE_API int rl_editing_mode;

/* Insert or overwrite mode for emacs mode.  1 means insert mode; 0 means
   overwrite mode.  Reset to insert mode on each input line. */
READLINE_API int rl_insert_mode;

/* The name of the calling program.  You should initialize this to
   whatever was in argv[0].  It is used when parsing conditionals. */
READLINE_API const char *rl_readline_name;

/* The prompt readline uses.  This is set from the argument to
   readline (), and should not be assigned to directly. */
READLINE_API char *rl_prompt;

/* The prompt string that is actually displayed by rl_redisplay.  Public so
   applications can more easily supply their own redisplay functions. */
READLINE_API char *rl_display_prompt;

/* The line buffer that is in use. */
READLINE_API char *rl_line_buffer;

/* The location of point, and end. */
READLINE_API int rl_point;
READLINE_API int rl_end;

/* The mark, or saved cursor position. */
READLINE_API int rl_mark;

/* Flag to indicate that readline has finished with the current input
   line and should return it. */
READLINE_API int rl_done;

/* If set to a character value, that will be the next keystroke read. */
READLINE_API int rl_pending_input;

/* Non-zero if we called this function from _rl_dispatch().  It's present
   so functions can find out whether they were called from a key binding
   or directly from an application. */
READLINE_API int rl_dispatching;

/* Non-zero if the user typed a numeric argument before executing the
   current function. */
READLINE_API int rl_explicit_arg;

/* The current value of the numeric argument specified by the user. */
READLINE_API int rl_numeric_arg;

/* The address of the last command function Readline executed. */
READLINE_API rl_command_func_t *rl_last_func;

/* The name of the terminal to use. */
READLINE_API const char *rl_terminal_name;

/* The input and output streams. */
READLINE_API FILE *rl_instream;
READLINE_API FILE *rl_outstream;

/* If non-zero, Readline gives values of LINES and COLUMNS from the environment
   greater precedence than values fetched from the kernel when computing the
   screen dimensions. */
READLINE_API int rl_prefer_env_winsize;

/* If non-zero, then this is the address of a function to call just
   before readline_internal () prints the first prompt. */
READLINE_API rl_hook_func_t *rl_startup_hook;

/* If non-zero, this is the address of a function to call just before
   readline_internal_setup () returns and readline_internal starts
   reading input characters. */
READLINE_API rl_hook_func_t *rl_pre_input_hook;
      
/* The address of a function to call periodically while Readline is
   awaiting character input, or NULL, for no event handling. */
READLINE_API rl_hook_func_t *rl_event_hook;

/* The address of a function to call if a read is interrupted by a signal. */
READLINE_API rl_hook_func_t *rl_signal_event_hook;

/* The address of a function to call if Readline needs to know whether or not
   there is data available from the current input source. */
READLINE_API rl_hook_func_t *rl_input_available_hook;

/* begin_clink_change */
/* Called when rl_add_history adds a history line. */
READLINE_API rl_history_hook_func_t *rl_add_history_hook;
/* Called when rl_remove_history removes a history line. */
READLINE_API rl_history_hook_func_t *rl_remove_history_hook;
/* Conditional support for backslash as a path separator.
   This is used by rl_is_path_separator.*/
READLINE_API int rl_backslash_path_sep;
/* Preferred path separator, used by filename completion. */
READLINE_API char rl_preferred_path_separator;
/* Function to call for post-processing of _rl_find_completion_word. */
READLINE_API rl_adjcmpwrd_func_t *rl_adjust_completion_word;
/* Function to call for post-processing of lcd. */
READLINE_API rl_postprocess_lcd_func_t *rl_postprocess_lcd_func;
/* Completion functions can set this to signal that the first char of each
   match is bit flags about the match type. */
#define MATCH_TYPE_NONE			1
#define MATCH_TYPE_WORD			2
#define MATCH_TYPE_ARG			3
#define MATCH_TYPE_COMMAND		4
#define MATCH_TYPE_ALIAS		5
#define MATCH_TYPE_FILE			6
#define MATCH_TYPE_DIR			7
#define MATCH_TYPE_LINK			8
#define MATCH_TYPE_MASK			0x0f
#define MATCH_TYPE_HIDDEN		0x40
#define MATCH_TYPE_READONLY		0x80
#define IS_MATCH_TYPE_NONE(x)		(((x) & MATCH_TYPE_MASK) == MATCH_TYPE_NONE)
#define IS_MATCH_TYPE_WORD(x)		(((x) & MATCH_TYPE_MASK) == MATCH_TYPE_WORD)
#define IS_MATCH_TYPE_ARG(x)		(((x) & MATCH_TYPE_MASK) == MATCH_TYPE_ARG)
#define IS_MATCH_TYPE_COMMAND(x)	(((x) & MATCH_TYPE_MASK) == MATCH_TYPE_COMMAND)
#define IS_MATCH_TYPE_ALIAS(x)		(((x) & MATCH_TYPE_MASK) == MATCH_TYPE_ALIAS)
#define IS_MATCH_TYPE_FILE(x)		(((x) & MATCH_TYPE_MASK) == MATCH_TYPE_FILE)
#define IS_MATCH_TYPE_DIR(x)		(((x) & MATCH_TYPE_MASK) == MATCH_TYPE_DIR)
#define IS_MATCH_TYPE_LINK(x)		(((x) & MATCH_TYPE_MASK) == MATCH_TYPE_LINK)
#define IS_MATCH_TYPE_HIDDEN(x)		(((x) & MATCH_TYPE_HIDDEN) == MATCH_TYPE_HIDDEN)
#define IS_MATCH_TYPE_READONLY(x)	(((x) & MATCH_TYPE_READONLY) == MATCH_TYPE_READONLY)
#define IS_MATCH_TYPE_PATHISH(x)	(((x) & MATCH_TYPE_MASK) >= MATCH_TYPE_FILE && \
					 ((x) & MATCH_TYPE_MASK) <= MATCH_TYPE_LINK)
READLINE_API int rl_completion_matches_include_type;
/* end_clink_change */

/* The address of the function to call to fetch a character from the current
   Readline input stream */
READLINE_API rl_getc_func_t *rl_getc_function;

READLINE_API rl_voidfunc_t *rl_redisplay_function;

/* begin_clink_change */
/* The address of a function to call before displaying the input line. */
READLINE_API rl_voidfunc_t *rl_before_display_function;
/* end_clink_change */

/* begin_clink_change */
READLINE_API const char *_rl_display_modmark_color;
READLINE_API const char *_rl_display_horizscroll_color;
READLINE_API const char *_rl_display_message_color;
READLINE_API char _rl_face_modmark;
READLINE_API char _rl_face_horizscroll;
READLINE_API rl_get_face_func_t *rl_get_face_func;
READLINE_API rl_puts_face_func_t *rl_puts_face_func;
/* end_clink_change */

READLINE_API rl_vintfunc_t *rl_prep_term_function;
READLINE_API rl_voidfunc_t *rl_deprep_term_function;

/* Dispatch variables. */
READLINE_API Keymap rl_executing_keymap;
READLINE_API Keymap rl_binding_keymap;

READLINE_API int rl_executing_key;
READLINE_API char *rl_executing_keyseq;
READLINE_API int rl_key_sequence_length;

/* begin_clink_change */
READLINE_API rl_macro_hook_func_t *rl_macro_hook_func;
READLINE_API rl_voidfunc_t *rl_last_func_hook_func;
/* end_clink_change */

/* Display variables. */
/* If non-zero, readline will erase the entire line, including any prompt,
   if the only thing typed on an otherwise-blank line is something bound to
   rl_newline. */
READLINE_API int rl_erase_empty_line;

/* If non-zero, the application has already printed the prompt (rl_prompt)
   before calling readline, so readline should not output it the first time
   redisplay is done. */
READLINE_API int rl_already_prompted;

/* A non-zero value means to read only this many characters rather than
   up to a character bound to accept-line. */
READLINE_API int rl_num_chars_to_read;

/* The text of a currently-executing keyboard macro. */
READLINE_API char *rl_executing_macro;

/* Variables to control readline signal handling. */
/* If non-zero, readline will install its own signal handlers for
   SIGINT, SIGTERM, SIGQUIT, SIGALRM, SIGTSTP, SIGTTIN, and SIGTTOU. */
READLINE_API int rl_catch_signals;

/* If non-zero, readline will install a signal handler for SIGWINCH
   that also attempts to call any calling application's SIGWINCH signal
   handler.  Note that the terminal is not cleaned up before the
   application's signal handler is called; use rl_cleanup_after_signal()
   to do that. */
READLINE_API int rl_catch_sigwinch;

/* If non-zero, the readline SIGWINCH handler will modify LINES and
   COLUMNS in the environment. */
READLINE_API int rl_change_environment;

/* Completion variables. */
/* Pointer to the generator function for completion_matches ().
   NULL means to use rl_filename_completion_function (), the default
   filename completer. */
READLINE_API rl_compentry_func_t *rl_completion_entry_function;

/* Optional generator for menu completion.  Default is
   rl_completion_entry_function (rl_filename_completion_function). */
READLINE_API rl_compentry_func_t *rl_menu_completion_entry_function;

/* If rl_ignore_some_completions_function is non-NULL it is the address
   of a function to call after all of the possible matches have been
   generated, but before the actual completion is done to the input line.
   The function is called with one argument; a NULL terminated array
   of (char *).  If your function removes any of the elements, they
   must be free()'ed. */
READLINE_API rl_compignore_func_t *rl_ignore_some_completions_function;

/* Pointer to alternative function to create matches.
   Function is called with TEXT, START, and END.
   START and END are indices in RL_LINE_BUFFER saying what the boundaries
   of TEXT are.
   If this function exists and returns NULL then call the value of
   rl_completion_entry_function to try to match, otherwise use the
   array of strings returned. */
READLINE_API rl_completion_func_t *rl_attempted_completion_function;

/* The basic list of characters that signal a break between words for the
   completer routine.  The initial contents of this variable is what
   breaks words in the shell, i.e. "n\"\\'`@$>". */
READLINE_API const char *rl_basic_word_break_characters;
READLINE_API const char *rl_basic_word_break_characters_without_backslash;

/* The list of characters that signal a break between words for
   rl_complete_internal.  The default list is the contents of
   rl_basic_word_break_characters.  */
READLINE_API /*const*/ char *rl_completer_word_break_characters;

/* Hook function to allow an application to set the completion word
   break characters before readline breaks up the line.  Allows
   position-dependent word break characters. */
READLINE_API rl_cpvfunc_t *rl_completion_word_break_hook;

/* List of characters which can be used to quote a substring of the line.
   Completion occurs on the entire substring, and within the substring   
   rl_completer_word_break_characters are treated as any other character,
   unless they also appear within this list. */
READLINE_API const char *rl_completer_quote_characters;

/* List of quote characters which cause a word break. */
READLINE_API const char *rl_basic_quote_characters;

/* List of characters that need to be quoted in filenames by the completer. */
READLINE_API const char *rl_filename_quote_characters;

/* List of characters that are word break characters, but should be left
   in TEXT when it is passed to the completion function.  The shell uses
   this to help determine what kind of completing to do. */
READLINE_API const char *rl_special_prefixes;

/* If non-zero, then this is the address of a function to call when
   completing on a directory name.  The function is called with
   the address of a string (the current directory name) as an arg.  It
   changes what is displayed when the possible completions are printed
   or inserted.  The directory completion hook should perform
   any necessary dequoting.  This function should return 1 if it modifies
   the directory name pointer passed as an argument.  If the directory
   completion hook returns 0, it should not modify the directory name
   pointer passed as an argument. */
READLINE_API rl_icppfunc_t *rl_directory_completion_hook;

/* If non-zero, this is the address of a function to call when completing
   a directory name.  This function takes the address of the directory name
   to be modified as an argument.  Unlike rl_directory_completion_hook, it
   only modifies the directory name used in opendir(2), not what is displayed
   when the possible completions are printed or inserted.  If set, it takes
   precedence over rl_directory_completion_hook.  The directory rewrite
   hook should perform any necessary dequoting.  This function has the same
   return value properties as the directory_completion_hook.

   I'm not happy with how this works yet, so it's undocumented.  I'm trying
   it in bash to see how well it goes. */
READLINE_API rl_icppfunc_t *rl_directory_rewrite_hook;

/* If non-zero, this is the address of a function for the completer to call
   before deciding which character to append to a completed name.  It should
   modify the directory name passed as an argument if appropriate, and return
   non-zero if it modifies the name.  This should not worry about dequoting
   the filename; that has already happened by the time it gets here. */
READLINE_API rl_icppfunc_t *rl_filename_stat_hook;

/* If non-zero, this is the address of a function to call when reading
   directory entries from the filesystem for completion and comparing
   them to the partial word to be completed.  The function should
   either return its first argument (if no conversion takes place) or
   newly-allocated memory.  This can, for instance, convert filenames
   between character sets for comparison against what's typed at the
   keyboard.  The returned value is what is added to the list of
   matches.  The second argument is the length of the filename to be
   converted. */
READLINE_API rl_dequote_func_t *rl_filename_rewrite_hook;

/* Backwards compatibility with previous versions of readline. */
#define rl_symbolic_link_hook rl_directory_completion_hook

/* If non-zero, then this is the address of a function to call when
   completing a word would normally display the list of possible matches.
   This function is called instead of actually doing the display.
   It takes three arguments: (char **matches, int num_matches, int max_length)
   where MATCHES is the array of strings that matched, NUM_MATCHES is the
   number of strings in that array, and MAX_LENGTH is the length of the
   longest string in that array. */
READLINE_API rl_compdisp_func_t *rl_completion_display_matches_hook;

/* begin_clink_change */
/* If non-zero, then this is the address of a function to call that completely
   replaces the display_matches behavior (rl_completion_display_matches_hook
   only replaces listing multiple matches).
   It takes one argument: (char** matches) where MATCHES is the array of
   strings that matched. */
READLINE_API rl_vcppfunc_t *rl_completion_display_matches_func;
/* If non-zero, then this is the address of a function to call to sort the
   list of matches.  It can accommodate any special sorting behavior the host
   may require, such as ignoring trailing path separators. */
READLINE_API rl_qsort_match_list_func_t *rl_qsort_match_list_func;
/* If non-zero, then this is the address of a function to call that determines
   whether a file extension is executable. */
READLINE_API rl_iccpfunc_t *rl_is_exec_func;
/* end_clink_change */

/* Non-zero means that the results of the matches are to be treated
   as filenames.  This is ALWAYS zero on entry, and can only be changed
   within a completion entry finder function. */
READLINE_API int rl_filename_completion_desired;

/* begin_clink_change */
/* Similar to rl_filename_completion_desired, but only affects display.
   This is ALWAYS zero on entry, and can only be changed within a
   completion entry finder function. */
READLINE_API int rl_filename_display_desired;
/* end_clink_change */

/* Non-zero means that the results of the matches are to be quoted using
   double quotes (or an application-specific quoting mechanism) if the
   filename contains any characters in rl_word_break_chars.  This is
   ALWAYS non-zero on entry, and can only be changed within a completion
   entry finder function. */
READLINE_API int rl_filename_quoting_desired;

/* Set to a function to quote a filename in an application-specific fashion.
   Called with the text to quote, the type of match found (single or multiple)
   and a pointer to the quoting character to be used, which the function can
   reset if desired. */
READLINE_API rl_quote_func_t *rl_filename_quoting_function;

/* Function to call to remove quoting characters from a filename.  Called
   before completion is attempted, so the embedded quotes do not interfere
   with matching names in the file system. */
READLINE_API rl_dequote_func_t *rl_filename_dequoting_function;

/* Function to call to decide whether or not a word break character is
   quoted.  If a character is quoted, it does not break words for the
   completer. */
READLINE_API rl_linebuf_func_t *rl_char_is_quoted_p;

/* Non-zero means to suppress normal filename completion after the
   user-specified completion function has been called. */
READLINE_API int rl_attempted_completion_over;

/* Set to a character describing the type of completion being attempted by
   rl_complete_internal; available for use by application completion
   functions. */
READLINE_API int rl_completion_type;

/* Set to the last key used to invoke one of the completion functions */
READLINE_API int rl_completion_invoking_key;

/* Up to this many items will be displayed in response to a
   possible-completions call.  After that, we ask the user if she
   is sure she wants to see them all.  The default value is 100. */
READLINE_API int rl_completion_query_items;

/* begin_clink_change */
/* When set, overrides rl_completion_query_items for `complete`
   based on the screen height and number of columns of matches. */
READLINE_API int rl_completion_auto_query_items;
/* end_clink_change */

/* Character appended to completed words when at the end of the line.  The
   default is a space.  Nothing is added if this is '\0'. */
READLINE_API int rl_completion_append_character;

/* If set to non-zero by an application completion function,
   rl_completion_append_character will not be appended. */
READLINE_API int rl_completion_suppress_append;

/* Set to any quote character readline thinks it finds before any application
   completion function is called. */
READLINE_API int rl_completion_quote_character;

/* Set to a non-zero value if readline found quoting anywhere in the word to
   be completed; set before any application completion function is called. */
READLINE_API int rl_completion_found_quote;

/* If non-zero, the completion functions don't append any closing quote.
   This is set to 0 by rl_complete_internal and may be changed by an
   application-specific completion function. */
READLINE_API int rl_completion_suppress_quote;

/* If non-zero, readline will sort the completion matches.  On by default. */
READLINE_API int rl_sort_completion_matches;

/* If non-zero, a slash will be appended to completed filenames that are
   symbolic links to directory names, subject to the value of the
   mark-directories variable (which is user-settable).  This exists so
   that application completion functions can override the user's preference
   (set via the mark-symlinked-directories variable) if appropriate.
   It's set to the value of _rl_complete_mark_symlink_dirs in
   rl_complete_internal before any application-specific completion
   function is called, so without that function doing anything, the user's
   preferences are honored. */
READLINE_API int rl_completion_mark_symlink_dirs;

/* If non-zero, then disallow duplicates in the matches. */
READLINE_API int rl_ignore_completion_duplicates;

/* If this is non-zero, completion is (temporarily) inhibited, and the
   completion character will be inserted as any other. */
READLINE_API int rl_inhibit_completion;

/* begin_clink_change */
/* This is a terminal sequence for the pager color.  The CSI and "m" are
   automatically added when writing it to the terminal. */
READLINE_API const char* _rl_pager_color;
/* This is a terminal sequence for the hidden file color.  The CSI and "m" are
   automatically added when writing it to the terminal. */
READLINE_API const char* _rl_hidden_color;
/* This is a terminal sequence for the readonly file color.  The CSI and "m" are
   automatically added when writing it to the terminal. */
READLINE_API const char* _rl_readonly_color;
/* This is a terminal sequence for the shell command color.  The CSI and "m" are
   automatically added when writing it to the terminal. */
READLINE_API const char* _rl_command_color;
/* This is a terminal sequence for the alias color.  The CSI and "m" are
   automatically added when writing it to the terminal. */
READLINE_API const char* _rl_alias_color;
/* This callback allows an application to provide keyboard input to readline
   even in modal input situations like the pager. */
READLINE_API rl_read_key_hook_func_t *rl_read_key_hook;
/* end_clink_change */

/* Applications can set this to non-zero to have readline's signal handlers
   installed during the entire duration of reading a complete line, as in
   readline-6.2.  This should be used with care, because it can result in
   readline receiving signals and not handling them until it's called again
   via rl_callback_read_char, thereby stealing them from the application.
   By default, signal handlers are only active while readline is active. */   
READLINE_API int rl_persistent_signal_handlers;

/* Input error; can be returned by (*rl_getc_function) if readline is reading
   a top-level command (RL_ISSTATE (RL_STATE_READCMD)). */
#define READERR			(-2)

/* Definitions available for use by readline clients. */
#define RL_PROMPT_START_IGNORE	'\001'
#define RL_PROMPT_END_IGNORE	'\002'

/* Possible values for do_replace argument to rl_filename_quoting_function,
   called by rl_complete_internal. */
#define NO_MATCH        0
#define SINGLE_MATCH    1
#define MULT_MATCH      2

/* Possible state values for rl_readline_state */
#define RL_STATE_NONE		0x000000		/* no state; before first call */

#define RL_STATE_INITIALIZING	0x0000001	/* initializing */
#define RL_STATE_INITIALIZED	0x0000002	/* initialization done */
#define RL_STATE_TERMPREPPED	0x0000004	/* terminal is prepped */
#define RL_STATE_READCMD	0x0000008	/* reading a command key */
#define RL_STATE_METANEXT	0x0000010	/* reading input after ESC */
#define RL_STATE_DISPATCHING	0x0000020	/* dispatching to a command */
#define RL_STATE_MOREINPUT	0x0000040	/* reading more input in a command function */
#define RL_STATE_ISEARCH	0x0000080	/* doing incremental search */
#define RL_STATE_NSEARCH	0x0000100	/* doing non-inc search */
#define RL_STATE_SEARCH		0x0000200	/* doing a history search */
#define RL_STATE_NUMERICARG	0x0000400	/* reading numeric argument */
#define RL_STATE_MACROINPUT	0x0000800	/* getting input from a macro */
#define RL_STATE_MACRODEF	0x0001000	/* defining keyboard macro */
#define RL_STATE_OVERWRITE	0x0002000	/* overwrite mode */
#define RL_STATE_COMPLETING	0x0004000	/* doing completion */
#define RL_STATE_SIGHANDLER	0x0008000	/* in readline sighandler */
#define RL_STATE_UNDOING	0x0010000	/* doing an undo */
#define RL_STATE_INPUTPENDING	0x0020000	/* rl_execute_next called */
#define RL_STATE_TTYCSAVED	0x0040000	/* tty special chars saved */
#define RL_STATE_CALLBACK	0x0080000	/* using the callback interface */
#define RL_STATE_VIMOTION	0x0100000	/* reading vi motion arg */
#define RL_STATE_MULTIKEY	0x0200000	/* reading multiple-key command */
#define RL_STATE_VICMDONCE	0x0400000	/* entered vi command mode at least once */
#define RL_STATE_CHARSEARCH	0x0800000	/* vi mode char search */
#define RL_STATE_REDISPLAYING	0x1000000	/* updating terminal display */

#define RL_STATE_DONE		0x2000000	/* done; accepted line */

#define RL_SETSTATE(x)		(rl_readline_state |= (x))
#define RL_UNSETSTATE(x)	(rl_readline_state &= ~(x))
#define RL_ISSTATE(x)		(rl_readline_state & (x))

struct readline_state {
  /* line state */
  int point;
  int end;
  int mark;
  int buflen;
  char *buffer;
  UNDO_LIST *ul;
  char *prompt;

  /* global state */
  int rlstate;
  int done;
  Keymap kmap;

  /* input state */
  rl_command_func_t *lastfunc;
  int insmode;
  int edmode;
  char *kseq;
  int kseqlen;

  int pendingin;
  FILE *inf;
  FILE *outf;
  char *macro;

  /* signal state */
  int catchsigs;
  int catchsigwinch;

  /* search state */

  /* completion state */
  rl_compentry_func_t *entryfunc;
  rl_compentry_func_t *menuentryfunc;
  rl_compignore_func_t *ignorefunc;
  rl_completion_func_t *attemptfunc;
  char *wordbreakchars;

  /* options state */

  /* hook state */
  
  /* reserved for future expansion, so the struct size doesn't change */
  char reserved[64];
};

READLINE_API int rl_save_state PARAMS((struct readline_state *));
READLINE_API int rl_restore_state PARAMS((struct readline_state *));

#ifdef __cplusplus
}
#endif

#endif /* _READLINE_H_ */
