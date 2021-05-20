// Copyright (c) 2020 Christopher Antos, Martin Ridgers
// License: http://opensource.org/licenses/MIT

#pragma once

#include "editor_module.h"
#include "input_dispatcher.h"
#include "pager.h"

//------------------------------------------------------------------------------
class pager_impl
    : public editor_module
    , public pager
{
public:
    enum pager_amount { unlimited, line, half_page, page, first_page };
                    pager_impl(input_dispatcher& dispatcher);
    virtual void    start_pager(printer& printer) override;
    virtual bool    on_print_lines(printer& printer, int lines) override;

private:
    virtual void    bind_input(binder& binder) override;
    virtual void    on_begin_line(const context& context) override;
    virtual void    on_end_line() override;
    virtual void    on_input(const input& input, result& result, const context& context) override;
    virtual void    on_terminal_resize(int columns, int rows, const context& context) override;
    void            set_limit(printer& printer, pager_amount amount);
    int             m_max = 0;
    int             m_pager_bind_group = -1;
    input_dispatcher& m_dispatcher;
};
