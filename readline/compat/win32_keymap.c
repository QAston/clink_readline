/*
    // Bind extended keys so editing follows Windows' conventions.
    static const char* emacs_key_binds[][2] = {
        { "\\e[1;5D",       "backward-word" },           // ctrl-left
        { "\\e[1;5C",       "forward-word" },            // ctrl-right
        { "\\e[F",          "end-of-line" },             // end
        { "\\e[H",          "beginning-of-line" },       // home
        { "\\e[3~",         "delete-char" },             // del
        { "\\e[1;5F",       "kill-line" },               // ctrl-end
        { "\\e[1;5H",       "backward-kill-line" },      // ctrl-home
        { "\\e[5~",         "history-search-backward" }, // pgup
        { "\\e[6~",         "history-search-forward" },  // pgdn
        { "\\e[3;5~",       "kill-word" },               // ctrl-del
        { "\\d",            "backward-kill-word" },      // ctrl-backspace
        { "\\e[2~",         "overwrite-mode" },          // ins
        { bindableEsc,      "clink-reset-line" },        // esc
        { "\\C-c",          "clink-ctrl-c" },            // ctrl-c
        { "\\C-v",          "clink-paste" },             // ctrl-v
        { "\\C-z",          "undo" },                    // ctrl-z
        { "\\C-x\\C-r",     "clink-reload" },            // ctrl-x,ctrl-r
        {}
    };

    static const char* general_key_binds[][2] = {
        { "\\M-a",          "clink-insert-dot-dot" },    // alt-a
        { "\\M-c",          "clink-copy-cwd" },          // alt-c
        { "\\M-h",          "clink-show-help" },         // alt-h
        { "\\M-H",          "clink-show-help-raw" },     // alt-H
        { "\\M-\\C-c",      "clink-copy-line" },         // alt-ctrl-c
        { "\\M-\\C-d",      "remove-history" },          // alt-ctrl-d
        { "\\M-\\C-e",      "clink-expand-env-var" },    // alt-ctrl-e
        { "\\M-\\C-f",      "clink-expand-doskey-alias" }, // alt-ctrl-f
        { "\\M-\\C-k",      "add-history" },             // alt-ctrl-k
        { "\\M-\\C-n",      "clink-old-menu-complete-numbers"},// alt-ctrl-n
        { "\\e[27;8;78~",   "clink-popup-complete-numbers"},// alt-ctrl-N
        { "\\M-\\C-w",      "clink-copy-word" },         // alt-ctrl-w
        { "\\e[5;5~",       "clink-up-directory" },      // ctrl-pgup
        { "\\e\\eOS",       "clink-exit" },              // alt-f4
        { "\\e[1;3H",       "clink-scroll-top" },        // alt-home
        { "\\e[1;3F",       "clink-scroll-bottom" },     // alt-end
        { "\\e[5;3~",       "clink-scroll-page-up" },    // alt-pgup
        { "\\e[6;3~",       "clink-scroll-page-down" },  // alt-pgdn
        { "\\e[1;3A",       "clink-scroll-line-up" },    // alt-up
        { "\\e[1;3B",       "clink-scroll-line-down" },  // alt-down
        {}
    };

    static const char* vi_insertion_key_binds[][2] = {
        { "\\M-\\C-i",      "tab-insert" },              // alt-ctrl-i
        { "\\M-\\C-j",      "emacs-editing-mode" },      // alt-ctrl-j
        { "\\M-\\C-k",      "kill-line" },               // alt-ctrl-k
        { "\\M-\\C-m",      "emacs-editing-mode" },      // alt-ctrl-m
        { bindableEsc,      "vi-movement-mode" },        // esc
        { "\\C-_",          "vi-undo-mode" },            // ctrl--
        { "\\M-0",          "vi-arg-digit" },            // alt-0
        { "\\M-1",          "vi-arg-digit" },            // alt-1
        { "\\M-2",          "vi-arg-digit" },            // alt-2
        { "\\M-3",          "vi-arg-digit" },            // alt-3
        { "\\M-4",          "vi-arg-digit" },            // alt-4
        { "\\M-5",          "vi-arg-digit" },            // alt-5
        { "\\M-6",          "vi-arg-digit" },            // alt-6
        { "\\M-7",          "vi-arg-digit" },            // alt-7
        { "\\M-8",          "vi-arg-digit" },            // alt-8
        { "\\M-9",          "vi-arg-digit" },            // alt-9
        { "\\M-[",          "arrow-key-prefix" },        // arrow key prefix
        { "\\d",            "backward-kill-word" },      // ctrl-backspace
        {}
    };

    static const char* vi_movement_key_binds[][2] = {
        { "\\M-\\C-j",      "emacs-editing-mode" },      // alt-ctrl-j
        { "\\M-\\C-m",      "emacs-editing-mode" },      // alt-ctrl-m
        {}
    };

    rl_unbind_key_in_map(' ', emacs_meta_keymap);
    bind_keyseq_list(general_key_binds, emacs_standard_keymap);
    bind_keyseq_list(emacs_key_binds, emacs_standard_keymap);

    rl_unbind_key_in_map(27, vi_insertion_keymap);
    bind_keyseq_list(general_key_binds, vi_insertion_keymap);
    bind_keyseq_list(general_key_binds, vi_movement_keymap);
    bind_keyseq_list(vi_insertion_key_binds, vi_insertion_keymap);
    bind_keyseq_list(vi_movement_key_binds, vi_movement_keymap);
*/