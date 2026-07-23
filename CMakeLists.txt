cmake_minimum_required(VERSION 3.21)
project(EntropyEngine VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(ENTROPY_BUILD_APP "build the raylib GUI app (needs GL)" ON)
option(ENTROPY_BUILD_TESTS "build the engine self-check tests" ON)

set(SOURCES src/main.cpp)

if(ENTROPY_BUILD_APP)
    include(FetchContent)
    FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG master
    )
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(raylib)

    add_executable(${PROJECT_NAME} ${SOURCES})
    target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)
    target_link_libraries(${PROJECT_NAME} PRIVATE raylib)
    if(MSVC)
        target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
    else()
        target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Werror -pedantic)
    endif()
endif()

if(ENTROPY_BUILD_TESTS)
    enable_testing()
    add_executable(engine_tests tests/test_engine.cpp)
    target_include_directories(engine_tests PRIVATE ${CMAKE_SOURCE_DIR}/include)
    if(MSVC)
        target_compile_options(engine_tests PRIVATE /W4 /WX)
    else()
        target_compile_options(engine_tests PRIVATE -Wall -Wextra -Werror -pedantic)
    endif()
    add_test(NAME engine_tests COMMAND engine_tests)
endif()
