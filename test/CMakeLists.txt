if(BUILD_TESTING)
    add_executable(tests
        assign_ptr_test.cpp
        main.cpp
    )

    find_package(Catch2 CONFIG REQUIRED)

    target_link_libraries(tests
        PRIVATE
            Catch2::Catch2
            libassign_ptr
    )

    include(CTest)
    include(Catch)
    catch_discover_tests(tests)
endif(BUILD_TESTING)
