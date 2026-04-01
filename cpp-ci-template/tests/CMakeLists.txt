include(FetchContent)

# ─── Fetch Google Test ───────────────────────────────────────────
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.14.0
)
# Prevent overriding the parent project's compiler/linker settings (Windows)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# ─── Test Executable ─────────────────────────────────────────────
add_executable(unit_tests
    test_ConfigParser.cpp
    test_ConfigManager.cpp
)

target_link_libraries(unit_tests
    PRIVATE
        config_lib
        GTest::gtest_main
)

target_include_directories(unit_tests
    PRIVATE ${CMAKE_SOURCE_DIR}/src
)

# ─── Register with CTest ─────────────────────────────────────────
include(GoogleTest)
gtest_discover_tests(unit_tests)
