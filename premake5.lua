-- Copyright (c) 2012 Martin Ridgers
-- License: http://opensource.org/licenses/MIT

local to = ".build/"..(_ACTION or "nullaction")

--------------------------------------------------------------------------------
local function postbuild_copy(src, cfg)
    src = path.getabsolute(src)
    src = path.translate(src)

    local dest = to.."/bin/"..cfg
    dest = path.getabsolute(dest)
    dest = path.translate(dest)
    postbuildcommands("copy /y \""..src.."\" \""..dest.."\" 1>nul 2>nul")
end

--------------------------------------------------------------------------------
local function setup_cfg(cfg)
    configuration(cfg)
        targetdir(to.."/bin/"..cfg)
        objdir(to.."/obj/")

    configuration({cfg, "x32"})
        targetsuffix("_x86")

    configuration({cfg, "x64"})
        targetsuffix("_x64")
end

--------------------------------------------------------------------------------

workspace("vsreadline")
    configurations({"Debug", "Release"})
    platforms({"x32", "x64"})
    location(to)

    characterset("MBCS")
    flags("NoManifest")
    flags("NoManifest")
    staticruntime("on")
    rtti("off")
    symbols("on")
    exceptionhandling("off")
    defines("HAVE_CONFIG_H")
    defines("HANDLE_MULTIBYTE")

    setup_cfg("Release")
    setup_cfg("Debug")

    configuration("Debug")
        optimize("off")
        defines("DEBUG")
        defines("_DEBUG")

    configuration("Release")
        optimize("full")
        defines("NDEBUG")

    configuration("Debug or Release")
        defines("CLINK_BUILD_ROOT=\""..path.getabsolute(to).."\"")

    configuration("vs*")
        defines("_HAS_EXCEPTIONS=0")
        defines("_CRT_SECURE_NO_WARNINGS")
        defines("_CRT_NONSTDC_NO_WARNINGS")

    configuration("gmake")
        defines("__MSVCRT_VERSION__=0x0601")
        defines("WINVER=0x0502")

project("clink_core")
    flags("fatalwarnings")
    language("c++")
    kind("staticlib")
    includedirs("clink/core/include/core")
    files("clink/core/src/**")
    files("clink/core/include/**")

    includedirs("clink/core/src")
    configuration("vs*")
        pchheader("pch.h")
        pchsource("clink/core/src/pch.cpp")

project("clink_terminal") -- provides impl for readline/readline/termcap.h and hostenv.cpp and wcwidth.cpp; could use PDCurses instead?
    flags("fatalwarnings")
    language("c++")
    kind("staticlib")
    links("clink_core")
    includedirs("clink/terminal/include/terminal")
    includedirs("clink/core/include")
    files("clink/terminal/src/**")
    files("clink/terminal/include/**")

    includedirs("clink/terminal/src")
    configuration("vs*")
        exceptionhandling("on")     -- for std::wregex
        pchheader("pch.h")
        pchsource("clink/terminal/src/pch.cpp")

project("readline")
    language("c")
    flags("fatalwarnings")
    kind("sharedlib")
    links("clink_terminal")
    defines("BUILD_READLINE")
    includedirs("readline")
    includedirs("readline/compat")
    files("readline/readline/*.c")
    files("readline/readline/*.h")
    files("readline/compat/*.c")
    files("readline/compat/*.h")

    -- these are included in keymap.c, so exclude to not duplicate symbols
    excludes("readline/readline/emacs_keymap.c")
    excludes("readline/readline/vi_keymap.c")

    excludes("readline/readline/support/wcwidth.c") -- better impl provided in clink_terminal
    -- todo: release this as gpl
    -- todo: it looks like keyboard handling is still broken somehow, ctrl+keypress and tab and completion don't work
