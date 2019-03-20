add_library(libassign_ptr INTERFACE)

target_include_directories(libassign_ptr
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(libassign_ptr
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/zpp/assign_ptr.hpp
)
add_custom_target(libassign_ptr-ide
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/zpp/assign_ptr.hpp
)
