#include <zpp/assign_ptr.hpp>

#include <catch2/catch.hpp>

#include <cstddef>
#include <memory>
#include <new>

#if ZPP_TARGET_COMPILER(CLANG)
#  if __cplusplus >= 201703L
#    define ZPP_CXX03_PRIV_DEF_AUTO_PTR() 0
#  else
#    define ZPP_CXX03_PRIV_DEF_AUTO_PTR() 1
#  endif
#elif ZPP_TARGET_COMPILER(GCC)
#  if __cplusplus >= 201703L
#    define ZPP_CXX03_PRIV_DEF_AUTO_PTR() 0
#  else
#    define ZPP_CXX03_PRIV_DEF_AUTO_PTR() 1
#  endif
#elif ZPP_TARGET_COMPILER(MSVC)
#  if _MSC_VER >= 1910 && _MSVC_LANG >= 201703L
#    define ZPP_CXX03_PRIV_DEF_AUTO_PTR() 0
#  else
#    define ZPP_CXX03_PRIV_DEF_AUTO_PTR() 1
#  endif
#elif ZPP_TARGET_COMPILER(UNKNOWN)
#  define ZPP_CXX03_PRIV_DEF_AUTO_PTR() 0
#endif

#if ZPP_TARGET_COMPILER(CLANG)
#  define ZPP_CXX17_PRIV_DEF_SHARED_PTR_ARRAY() 0
#elif ZPP_TARGET_COMPILER(GCC)
#  if __GNUC__ >= 7 && __cplusplus >= 201703L
#    define ZPP_CXX17_PRIV_DEF_SHARED_PTR_ARRAY() 1
#  else
#    define ZPP_CXX17_PRIV_DEF_SHARED_PTR_ARRAY() 0
#  endif
#elif ZPP_TARGET_COMPILER(MSVC)
#  if _MSC_VER >= 1912 && _MSVC_LANG >= 201703L
#    define ZPP_CXX17_PRIV_DEF_SHARED_PTR_ARRAY() 1
#  else
#    define ZPP_CXX17_PRIV_DEF_SHARED_PTR_ARRAY() 0
#  endif
#elif ZPP_TARGET_COMPILER(UNKNOWN)
#  define ZPP_CXX17_PRIV_DEF_SHARED_PTR_ARRAY() 0
#endif

#if ZPP_TARGET_COMPILER(CLANG)
#  if (__clang_major__ >= 4 || __clang_major__ == 3 && __clang_minor__ >= 9) && __cplusplus >= 201703L
#    define ZPP_CXX17_PRIV_DEF_INIT_IF() 1
#  else
#    define ZPP_CXX17_PRIV_DEF_INIT_IF() 0
#  endif
#elif ZPP_TARGET_COMPILER(GCC)
#  if __GNUC__ >= 7 && __cplusplus >= 201703L
#    define ZPP_CXX17_PRIV_DEF_INIT_IF() 1
#  else
#    define ZPP_CXX17_PRIV_DEF_INIT_IF() 0
#  endif
#elif ZPP_TARGET_COMPILER(MSVC)
#  if _MSC_VER >= 1911 && _MSVC_LANG >= 201703L
#    define ZPP_CXX17_PRIV_DEF_INIT_IF() 1
#  else
#    define ZPP_CXX17_PRIV_DEF_INIT_IF() 0
#  endif
#elif ZPP_TARGET_COMPILER(UNKNOWN)
#  define ZPP_CXX17_PRIV_DEF_INIT_IF() 0
#endif

// Single item

// Reference to pointer

namespace
{
    void foo_ref_ptr(int*& p)
    {
        p = new int(42);
    }
}

TEST_CASE("Allocate single item through reference output parameter")
{
    SECTION("Raw pointer")
    {
        int* p = nullptr;
        foo_ref_ptr(p);

        REQUIRE(p != nullptr);
        CHECK(*p == 42);

        delete p;
    }

#if ZPP_CXX03(AUTO_PTR)
    SECTION("auto_ptr")
    {
        std::auto_ptr<int> p;
        foo_ref_ptr(zpp::assign_ptr(p));

        REQUIRE(p.get() != nullptr);
        CHECK(*p == 42);
    }
#endif

    SECTION("unique_ptr")
    {
        std::unique_ptr<int> p;
        foo_ref_ptr(zpp::assign_ptr(p));

        REQUIRE(p != nullptr);
        CHECK(*p == 42);
    }

    SECTION("shared_ptr")
    {
        std::shared_ptr<int> p;
        foo_ref_ptr(zpp::assign_ptr(p));

        REQUIRE(p != nullptr);
        CHECK(*p == 42);
    }
}

// Pointer to a pointer

namespace
{
    void foo_ptr_ptr(int** p)
    {
        *p = new int(42);
    }
}

TEST_CASE("Allocate single item through pointer output parameter")
{
    SECTION("Raw pointer")
    {
        int* p = nullptr;
        foo_ptr_ptr(&p);

        REQUIRE(p != nullptr);
        CHECK(*p == 42);

        delete p;
    }

#if ZPP_CXX03(AUTO_PTR)
    SECTION("auto_ptr")
    {
        std::auto_ptr<int> p;
        foo_ptr_ptr(zpp::assign_ptr(p));

        REQUIRE(p.get() != nullptr);
        CHECK(*p == 42);
    }
#endif

    SECTION("unique_ptr")
    {
        std::unique_ptr<int> p;
        foo_ptr_ptr(zpp::assign_ptr(p));

        REQUIRE(p != nullptr);
        CHECK(*p == 42);
    }

    SECTION("shared_ptr")
    {
        std::shared_ptr<int> p;
        foo_ptr_ptr(zpp::assign_ptr(p));

        REQUIRE(p != nullptr);
        CHECK(*p == 42);
    }
}

// Array

// Reference to pointer

namespace
{
    void foo_ref_array(int*& p, std::size_t& len)
    {
        len = 3;

        p = new int[len];

        for (int i = 0; i < static_cast<int>(len); ++i)
        {
            p[i] = i;
        }
    }
}

TEST_CASE("Allocate array through reference output parameter")
{
    SECTION("Raw pointer")
    {
        int* p = nullptr;
        std::size_t len = 0;
        foo_ref_array(p, len);

        REQUIRE(p != nullptr);
        REQUIRE(len == 3);
        CHECK(p[0] == 0);
        CHECK(p[1] == 1);
        CHECK(p[2] == 2);

        delete[] p;
    }

    SECTION("auto_ptr")
    {
        // std::auto_ptr does not support arrays
    }

    SECTION("unique_ptr")
    {
        std::unique_ptr<int[]> p;
        std::size_t len = 0;
        foo_ref_array(zpp::assign_ptr(p), len);

        REQUIRE(p != nullptr);
        REQUIRE(len == 3);
        CHECK(p[0] == 0);
        CHECK(p[1] == 1);
        CHECK(p[2] == 2);
    }

#if ZPP_CXX17(SHARED_PTR_ARRAY)
    SECTION("shared_ptr")
    {
        std::shared_ptr<int[]> p;
        std::size_t len = 0;
        foo_ref_array(zpp::assign_ptr(p), len);

        REQUIRE(p != nullptr);
        REQUIRE(len == 3);
        CHECK(p[0] == 0);
        CHECK(p[1] == 1);
        CHECK(p[2] == 2);
    }
#endif
}

// Pointer to pointer

namespace
{
    void foo_ptr_array(int** p, std::size_t& len)
    {
        len = 3;

        *p = new int[len];

        for (int i = 0; i < static_cast<int>(len); ++i)
        {
            (*p)[i] = i;
        }
    }
}

TEST_CASE("Allocate array through pointer output parameter")
{
    SECTION("Raw pointer")
    {
        int* p = nullptr;
        std::size_t len = 0;
        foo_ptr_array(&p, len);

        REQUIRE(p != nullptr);
        REQUIRE(len == 3);
        CHECK(p[0] == 0);
        CHECK(p[1] == 1);
        CHECK(p[2] == 2);

        delete[] p;
    }

    SECTION("auto_ptr")
    {
        // std::auto_ptr does not support arrays
    }

    SECTION("unique_ptr")
    {
        std::unique_ptr<int[]> p;
        std::size_t len = 0;
        foo_ptr_array(zpp::assign_ptr(p), len);

        REQUIRE(p != nullptr);
        REQUIRE(len == 3);
        CHECK(p[0] == 0);
        CHECK(p[1] == 1);
        CHECK(p[2] == 2);
    }

#if ZPP_CXX17(SHARED_PTR_ARRAY)
    SECTION("shared_ptr")
    {
        std::shared_ptr<int[]> p;
        std::size_t len = 0;
        foo_ptr_array(zpp::assign_ptr(p), len);

        REQUIRE(p != nullptr);
        REQUIRE(len == 3);
        CHECK(p[0] == 0);
        CHECK(p[1] == 1);
        CHECK(p[2] == 2);
    }
#endif
}

// Overloaded ambiguity

namespace
{
    void foo_ambig(int*& p)
    {
        p = new int(42);
    }

    void foo_ambig(int** p)
    {
        *p = new int(42);
    }
}

TEST_CASE("Ambiguous function cannot use user-defined conversion")
{
    SECTION("Raw pointer")
    {
        int* p = nullptr;

        SECTION("by ref")
        {
            foo_ambig(p);

            REQUIRE(p != nullptr);
            CHECK(*p == 42);

            delete p;
        }
        SECTION("by ptr")
        {
            foo_ambig(&p);

            REQUIRE(p != nullptr);
            CHECK(*p == 42);

            delete p;
        }
    }

#if ZPP_CXX03(AUTO_PTR)
    SECTION("auto_ptr")
    {
        std::auto_ptr<int> p;

        SECTION("by ref")
        {
            foo_ambig(zpp::assign_ptr(p).by_ref());

            REQUIRE(p.get() != nullptr);
            CHECK(*p == 42);
        }
        SECTION("by ptr")
        {
            foo_ambig(zpp::assign_ptr(p).by_ptr());

            REQUIRE(p.get() != nullptr);
            CHECK(*p == 42);
        }
    }
#endif

    SECTION("unique_ptr")
    {
        std::unique_ptr<int> p;

        SECTION("by ref")
        {
            foo_ambig(zpp::assign_ptr(p).by_ref());

            REQUIRE(p.get() != nullptr);
            CHECK(*p == 42);
        }
        SECTION("by ptr")
        {
            foo_ambig(zpp::assign_ptr(p).by_ptr());

            REQUIRE(p.get() != nullptr);
            CHECK(*p == 42);
        }
    }

    SECTION("shared_ptr")
    {
        std::shared_ptr<int> p;

        SECTION("by ref")
        {
            foo_ambig(zpp::assign_ptr(p).by_ref());

            REQUIRE(p.get() != nullptr);
            CHECK(*p == 42);
        }
        SECTION("by ptr")
        {
            foo_ambig(zpp::assign_ptr(p).by_ptr());

            REQUIRE(p.get() != nullptr);
            CHECK(*p == 42);
        };
    }
}

// Caveat usage

namespace
{
    bool foo_caveat(int*& p)
    {
        p = new int(42);

        return true;
    }
}

TEST_CASE("Caveat usage")
{
    std::unique_ptr<int> p;

    // FIXME
    SECTION("Unexpected branching")
    {
        if (foo_caveat(zpp::assign_ptr(p)) && p != nullptr)
        {
            // Expected branch, but unable to enter this branch
            // because the destructor of zpp::assign_ptr(...) will not
            // run until the end of the expression after the NULL pointer check
            CHECK(false);
        }
        else
        {
            // Unexpected branch
            CHECK(true);
        }
    }

    SECTION("Workaround branching")
    {
        if (foo_caveat(zpp::assign_ptr(p)))
        {
            if (p != nullptr)
            {
                CHECK(p != nullptr);
            }
            else
            {
                CHECK(p == nullptr);
            }
        }
        else
        {
            CHECK(p == nullptr);
        }
    }

#if ZPP_CXX17(INIT_IF)
    SECTION("C++17 if-statement initialization")
    {
        if (foo_caveat(zpp::assign_ptr(p)); p != nullptr)
        {
            CHECK(p != nullptr);
        }
        else
        {
            CHECK(p == nullptr);
        }
    }
#endif
}
