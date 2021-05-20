// Copyright (c) 2020 Christopher Antos
// License: http://opensource.org/licenses/MIT

#pragma once

#include <core/base.h> // for #define ARCHITECTURE

//------------------------------------------------------------------------------
#if defined(CLINK_FINAL)
    #define CLINK_USE_EMBEDDED_SCRIPTS
#else
    //#define CLINK_USE_EMBEDDED_SCRIPTS
#endif
