# AppConfigManager

> C++17 Application Configuration Manager with CMake, Google Test & GitHub Actions CI/CD

[![CI/CD Pipeline](https://github.com/YOUR_USERNAME/cpp-app-config-manager/actions/workflows/ci.yml/badge.svg)](https://github.com/YOUR_USERNAME/cpp-app-config-manager/actions/workflows/ci.yml)

## What It Does

Loads application configuration from `.conf` files and environment variables, with environment variables taking precedence (12-factor app pattern). Useful for managing settings across dev / staging / production environments.

```
DB_HOST=localhost        ← from config/app.conf
DB_HOST=prod-server.com  ← overridden by environment variable at runtime
```

## Project Structure

```
cpp-app-config-manager/
├── CMakeLists.txt              # Root CMake build config
├── config/
│   └── app.conf                # Default config (dev environment)
├── src/
│   ├── ConfigManager.hpp/.cpp  # Core config manager class
│   ├── ConfigParser.hpp/.cpp   # KEY=VALUE file parser
│   ├── Environment.hpp/.cpp    # OS environment variable reader
│   └── main.cpp                # Demo entry point
├── tests/
│   ├── test_ConfigParser.cpp   # 15+ unit tests for parser
│   └── test_ConfigManager.cpp  # 15+ unit tests for manager
├── scripts/
│   └── build.sh                # Local build helper
└── .github/workflows/
    ├── ci.yml                  # Main CI/CD pipeline
    └── pr-checks.yml           # Pull request validation
```

## Build & Run

**Prerequisites:** CMake 3.16+, GCC/Clang/MSVC with C++17 support

```bash
# Quick build (uses build.sh helper)
chmod +x scripts/build.sh
./scripts/build.sh            # Release build + tests
./scripts/build.sh debug      # Debug build + tests
./scripts/build.sh coverage   # Coverage report (needs lcov)
./scripts/build.sh clean      # Remove build directory

# Manual CMake
cmake -B build -DCMAKE_BUILD_TYPE=Release -DENABLE_TESTING=ON
cmake --build build --parallel
cd build && ctest --output-on-failure
```

## CI/CD Pipeline

Every push triggers:

| Job | What it does |
|-----|-------------|
| **Build & Test** | Compiles on Ubuntu + Windows, runs all unit tests |
| **Static Analysis** | `cppcheck` + `clang-tidy` code quality checks |
| **Code Coverage** | `lcov` coverage report generation |
| **Package Release** | Creates `.tar.gz` binary on GitHub release tag |

## Skills Demonstrated

- **C++17** — `std::optional`, smart pointers, RAII, STL containers
- **OOP Design** — Separated concerns: Parser / Manager / Environment
- **CMake** — Multi-target build, FetchContent for Google Test
- **Unit Testing** — Google Test, 30+ tests, edge cases
- **CI/CD & SCM** — GitHub Actions matrix builds, static analysis, coverage, release packaging

## Usage Example

```cpp
ConfigManager cfg;
cfg.loadFromFile("config/app.conf");   // load from file
cfg.loadFromEnvironment();             // env vars override file values

std::string host = cfg.get("DB_HOST");                        // throws if missing
std::string env  = cfg.getOrDefault("APP_ENV", "development"); // safe fallback
cfg.dump();                            // prints all config (redacts secrets)
```

## License

MIT
