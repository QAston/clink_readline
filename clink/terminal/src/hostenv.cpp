#include "pch.h"

#include <core/base.h>
#include <core/str.h>
#include <core/os.h>
#include <core/path.h>
#include <core/str_hash.h>


extern "C" const char* host_get_env(const char* name)
{
    static int rotate = 0;
    static str<> rotating_tmp[10];

    str<>& s = rotating_tmp[rotate];
    rotate = (rotate + 1) % sizeof_array(rotating_tmp);
    if (!os::get_env(name, s))
        return nullptr;
    return s.c_str();
}
