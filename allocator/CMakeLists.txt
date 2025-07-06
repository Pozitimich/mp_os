cmake_minimum_required(VERSION 3.21)
project(mp_os_allctr
        VERSION 1.0
        DESCRIPTION "allocators interface & implementations project"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(allocator)