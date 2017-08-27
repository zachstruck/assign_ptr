#include "assign_ptr.hpp"

#include "Catch\single_include\catch.hpp"

#include <cstddef>
#include <memory>
#include <new>

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

    SECTION("auto_ptr")
    {
        std::auto_ptr<int> p;
        foo_ref_ptr(zpp::assign_ptr(p));

        REQUIRE(p.get() != nullptr);
        CHECK(*p == 42);
    }

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

    SECTION("auto_ptr")
    {
        std::auto_ptr<int> p;
        foo_ptr_ptr(zpp::assign_ptr(p));

        REQUIRE(p.get() != nullptr);
        CHECK(*p == 42);
    }

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

        for (std::size_t i = 0; i < len; ++i)
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

    SECTION("shared_ptr")
    {
        // TODO  C++17
        // std::shared_ptr does not yet support arrays
        /*
        std::shared_ptr<int[]> p;
        std::size_t len = 0;
        foo_ref_array(zpp::assign_ptr(p), len);

        REQUIRE(p != nullptr);
        REQUIRE(len == 3);
        CHECK(p[0] == 0);
        CHECK(p[1] == 1);
        CHECK(p[2] == 2);
        */
    }
}

// Pointer to pointer

namespace
{
    void foo_ptr_array(int** p, std::size_t& len)
    {
        len = 3;

        *p = new int[len];

        for (std::size_t i = 0; i < len; ++i)
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

    SECTION("shared_ptr")
    {
        // TODO  C++17
        /*
        std::shared_ptr<int[]> p;
        std::size_t len = 0;
        foo_ptr_array(zpp::assign_ptr(p), len);

        REQUIRE(p != nullptr);
        REQUIRE(len == 3);
        CHECK(p[0] == 0);
        CHECK(p[1] == 1);
        CHECK(p[2] == 2);
        */
    }
}
