cmake_minimum_required(VERSION 3.16)

project(AppConfigManager
    VERSION 1.0.0
    DESCRIPTION "Application Configuration Management System"
    LANGUAGES CXX
)

# ─── C++ Standard ───────────────────────────────────────────────
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ─── Build Type Default ─────────────────────────────────────────
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# ─── Compiler Warnings ──────────────────────────────────────────
if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

# ─── Options ────────────────────────────────────────────────────
option(ENABLE_TESTING "Enable unit tests" ON)
option(ENABLE_COVERAGE "Enable code coverage (GCC/Clang only)" OFF)

# ─── Coverage Flags ─────────────────────────────────────────────
if(ENABLE_COVERAGE AND NOT MSVC)
    add_compile_options(--coverage -O0)
    add_link_options(--coverage)
endif()

# ─── Subdirectories ─────────────────────────────────────────────
add_subdirectory(src)

if(ENABLE_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()

# ─── Install Rules ──────────────────────────────────────────────
install(TARGETS app_config_manager
    RUNTIME DESTINATION bin
)

# ─── Version Info ───────────────────────────────────────────────
message(STATUS "Project: ${PROJECT_NAME} v${PROJECT_VERSION}")
