// Copyright (c) 2020 Christopher Antos
// License: http://opensource.org/licenses/MIT

#include "pch.h"

#include <core/match_wild.h>

//------------------------------------------------------------------------------
TEST_CASE("path::match_wild()")
{
    SECTION("Basic")
    {
        REQUIRE(!path::match_wild("a*/ghi", "abc/def/ghi"));

        REQUIRE(path::match_wild("*foo*", "food"));
        REQUIRE(path::match_wild("*foo*", "qfood"));
        REQUIRE(path::match_wild("*foo*", "qfoo"));
        REQUIRE(path::match_wild("*foo*bar", "foobar"));
        REQUIRE(path::match_wild("*foo*bar", "foodbar"));
        REQUIRE(path::match_wild("*foo*bar", "foodbar"));
        REQUIRE(!path::match_wild("*foo*bar", "foodbard"));
        REQUIRE(path::match_wild("*foo*bar", "build.foobar"));
        REQUIRE(path::match_wild("*foo*bar", "build.foo123bar"));
        REQUIRE(!path::match_wild("*foo*bar", "build.foo123bard"));
        REQUIRE(!path::match_wild("*foo*bar", "build.fo123bar"));
        REQUIRE(path::match_wild("build*.log", "build.foo.bar.log"));
        REQUIRE(!path::match_wild("build*.log", "wmbuild.foo.bar.log"));
        REQUIRE(path::match_wild("wmbuild*.log", "wmbuild.foo.bar.log"));

        REQUIRE(path::match_wild("*r*p", "error.cpp"));
    }

    SECTION("Period")
    {
        REQUIRE(path::match_wild("bu*", "build"));
        REQUIRE(path::match_wild("bu*", ".build"));
        REQUIRE(path::match_wild("bu*", "..build"));

        REQUIRE(!path::match_wild(".bu*", "build"));
        REQUIRE(path::match_wild(".bu*", ".build"));
        REQUIRE(!path::match_wild(".bu*", "..build"));

        REQUIRE(!path::match_wild("abc/bu*", "build"));
        REQUIRE(path::match_wild("abc/bu*", "abc/build"));
        REQUIRE(!path::match_wild("abc/bu*", ".build"));
        REQUIRE(path::match_wild("abc/bu*", "abc/.build"));
    }

    SECTION("Components")
    {
        REQUIRE(path::match_wild("abc/def/ghi", "abc/def/ghi"));
        REQUIRE(path::match_wild("a*/def/ghi", "abc/def/ghi"));
        REQUIRE(path::match_wild("a*/d?f/*i", "abc/def/ghi"));

        REQUIRE(!path::match_wild("abc/def", "abc/def/build"));
        REQUIRE(!path::match_wild("abc/def/?i*", "abc/def/build"));
        REQUIRE(path::match_wild("abc/def/??i*", "abc/def/build"));
    }

    SECTION("Slashes")
    {
        REQUIRE(path::match_wild("abc/def/ghi", "abc\\def\\ghi"));

        REQUIRE(!path::match_wild("a*/ghi", "abc/def/ghi"));
        REQUIRE(path::match_wild("a*/*/ghi", "abc/def/ghi"));
        REQUIRE(!path::match_wild("abc/*", "abc/def/ghi"));
        REQUIRE(path::match_wild("abc/*", "abc/def"));

        REQUIRE(!path::match_wild("abc/def", "abc/def/build"));
        REQUIRE(!path::match_wild("abc/def/?i*", "abc/def/build"));
        REQUIRE(path::match_wild("abc/def/??i*", "abc/def/build"));
    }

    SECTION("End star")
    {
        REQUIRE(!path::match_wild("ori*", "origin/master", false));
        REQUIRE(path::match_wild("ori*", "origin/master", true));
    }
}
