# 🔧 AppConfigManager

> A production-grade **Application Configuration Management System** built in **C++17**,
> featuring multi-environment config loading, 30+ unit tests, and a full **GitHub Actions CI/CD pipeline**.

[![CI/CD Pipeline](https://github.com/SamridhiNancy/cpp-app-config-manager/actions/workflows/ci.yml/badge.svg)](https://github.com/SamridhiNancy/cpp-app-config-manager/actions/workflows/ci.yml)
[![Language](https://img.shields.io/badge/language-C%2B%2B17-blue.svg)](https://isocpp.org/)
[![Build System](https://img.shields.io/badge/build-CMake-orange.svg)](https://cmake.org/)
[![Testing](https://img.shields.io/badge/tests-Google%20Test-green.svg)](https://github.com/google/googletest)
[![License](https://img.shields.io/badge/license-MIT-lightgrey.svg)](LICENSE)

---

## 📌 What Is This?

Every real-world application needs configuration — database hosts, ports, API keys, log levels.
Hardcoding these values is unsafe and inflexible. This project solves that problem by providing
a clean C++ system that:

- Loads settings from a **`.conf` file** (KEY=VALUE format)
- **Overrides** them with **OS environment variables** at runtime
- Supports **dev / staging / production** environments seamlessly
- **Redacts sensitive values** (passwords, tokens, secrets) in logs automatically

This follows the industry-standard **12-Factor App** configuration pattern used by
companies like Google, Netflix, and Amazon.

```
DB_HOST=localhost        ← loaded from config/app.conf
DB_HOST=prod-server.com  ← overridden by environment variable at runtime
```

---

## ✨ Features

| Feature | Details |
|---|---|
| 📁 File-based config | Parses `KEY=VALUE` `.conf` files with comments, quotes, whitespace trimming |
| 🌍 Environment override | OS environment variables take precedence over file values |
| 🔒 Secret redaction | Keys containing `SECRET`, `PASSWORD`, `TOKEN` are auto-redacted in logs |
| 🛡️ Safe access | `get()` throws on missing keys, `getOptional()` returns `std::nullopt` |
| 🔁 Runtime override | `set()` allows programmatic config changes at runtime |
| 🧪 30+ unit tests | Full Google Test suite covering edge cases, error paths, and happy paths |
| ⚙️ CMake build system | Debug / Release / Coverage build profiles |
| 🚀 CI/CD pipeline | GitHub Actions: matrix builds, static analysis, coverage, release packaging |

---

## 🗂️ Project Structure

```
cpp-app-config-manager/
│
├── 📄 CMakeLists.txt                   # Root CMake build configuration
├── 📄 README.md                        # You are here
├── 📄 .gitignore                       # Git ignore rules
├── 📄 .clang-format                    # Code formatting rules
│
├── 📁 config/
│   └── app.conf                        # Default config (development environment)
│
├── 📁 src/
│   ├── CMakeLists.txt
│   ├── main.cpp                        # Demo entry point
│   ├── ConfigManager.hpp / .cpp        # Core config manager class
│   ├── ConfigParser.hpp  / .cpp        # KEY=VALUE file parser
│   └── Environment.hpp  / .cpp        # OS environment variable reader
│
├── 📁 tests/
│   ├── CMakeLists.txt
│   ├── test_ConfigParser.cpp           # 15+ parser unit tests
│   └── test_ConfigManager.cpp         # 15+ manager unit tests
│
├── 📁 scripts/
│   └── build.sh                        # Local build helper script
│
└── 📁 .github/workflows/
    ├── ci.yml                          # Main CI/CD pipeline
    └── pr-checks.yml                   # Pull request validation checks
```

---

## 🚀 Quick Start

### Prerequisites
- CMake 3.16+
- GCC 9+ / Clang 10+ / MSVC 2019+ (C++17 support required)
- Git

### Build & Run

```bash
# Clone the repository
git clone https://github.com/SamridhiNancy/cpp-app-config-manager.git
cd cpp-app-config-manager

# Using the build helper (Linux/Mac)
chmod +x scripts/build.sh
./scripts/build.sh            # Release build + run all tests

# OR manually with CMake
cmake -B build -DCMAKE_BUILD_TYPE=Release -DENABLE_TESTING=ON
cmake --build build --parallel
cd build && ctest --output-on-failure
```

### Build Modes

```bash
./scripts/build.sh release    # Optimized release build + tests
./scripts/build.sh debug      # Debug build with symbols + tests
./scripts/build.sh coverage   # Coverage report (requires lcov)
./scripts/build.sh clean      # Remove build directory
```

### Run the App

```bash
# After building
./build/src/app_config_manager                        # uses config/app.conf
./build/src/app_config_manager config/app.conf        # explicit config path

# Override with environment variables
DB_HOST=prod-server.com APP_ENV=production ./build/src/app_config_manager
```

---

## 💻 Usage Example

```cpp
#include "ConfigManager.hpp"

int main() {
    ConfigManager cfg;

    // 1. Load base config from file
    cfg.loadFromFile("config/app.conf");

    // 2. Override with environment variables (12-factor pattern)
    cfg.loadFromEnvironment();

    // 3. Access values
    std::string host = cfg.get("DB_HOST");                         // throws if missing
    std::string env  = cfg.getOrDefault("APP_ENV", "development"); // safe fallback
    auto port        = cfg.getOptional("DB_PORT");                 // returns std::optional

    if (port.has_value()) {
        std::cout << "Port: " << *port << "\n";
    }

    // 4. Dump all config (sensitive keys are auto-redacted)
    cfg.dump();
    // Output:
    //   APP_ENV   = development
    //   DB_HOST   = localhost
    //   DB_PORT   = 5432
    //   DB_PASSWORD = [REDACTED]   ← auto-redacted!

    return 0;
}
```

---

## ⚙️ Config File Format

```ini
# config/app.conf
# Lines starting with # are comments

# Application settings
APP_ENV=development
APP_NAME=MyApplication
APP_VERSION=1.0.0

# Database
DB_HOST=localhost
DB_PORT=5432
DB_NAME=mydb
DB_PASSWORD=changeme        # override via environment variable in production

# Logging
LOG_LEVEL=DEBUG             # DEBUG | INFO | WARN | ERROR

# Feature flags
FEATURE_DARK_MODE=true
FEATURE_BETA_API=false
```

**Supported syntax:**
- `KEY=VALUE` — basic assignment
- `KEY="value with spaces"` — quoted values
- `KEY='single quoted'` — single quotes also supported
- `# comment` — full line comments
- `KEY=value # inline comment` — inline comments stripped automatically
- `KEY=` — empty values allowed

---

## 🧪 Running Tests

```bash
# Build and run all tests
cmake -B build -DENABLE_TESTING=ON
cmake --build build
cd build && ctest --output-on-failure

# Verbose output
ctest -V

# Run specific test
ctest -R ConfigParser
ctest -R ConfigManager
```

**Test coverage includes:**
- ✅ Simple KEY=VALUE parsing
- ✅ Whitespace trimming
- ✅ Comment skipping (full line + inline)
- ✅ Quote stripping (single and double)
- ✅ Missing file error handling
- ✅ Missing required key (throws)
- ✅ Optional key (returns nullopt)
- ✅ Default value fallback
- ✅ Runtime set/override
- ✅ Sensitive key redaction in dump()
- ✅ Size tracking after operations

---

## 🔄 CI/CD Pipeline

Every push to `main` or `develop` automatically triggers:

```
Push / PR
    │
    ├── 🔨 Build & Test (matrix)
    │       ├── Ubuntu 22.04  × Debug
    │       ├── Ubuntu 22.04  × Release
    │       ├── Windows       × Debug
    │       └── Windows       × Release   ✅ All must pass
    │
    ├── 🔍 Static Analysis
    │       ├── cppcheck  — bug detection
    │       └── clang-tidy — style & correctness   ✅ Zero warnings
    │
    ├── 📊 Code Coverage
    │       └── lcov report generated   📈 Coverage tracked
    │
    └── 📦 Package Release  (on git tag only)
            ├── Build optimized binary
            ├── Create .tar.gz artifact
            ├── Generate SHA256 checksum
            └── Attach to GitHub Release   🚀 Auto-deployed
```

### PR Checks
Every pull request also runs:
- **Commit message validation** (Conventional Commits format)
- **clang-format** style check
- **Quick build sanity** check

---

## 🛠️ Technologies Used

| Technology | Purpose |
|---|---|
| **C++17** | Core language — `std::optional`, smart pointers, RAII, STL |
| **CMake 3.16+** | Cross-platform build system |
| **Google Test** | Unit testing framework (fetched via CMake FetchContent) |
| **GitHub Actions** | CI/CD pipeline automation |
| **cppcheck** | Static analysis — bug and undefined behaviour detection |
| **clang-tidy** | Linting and code style enforcement |
| **clang-format** | Automated code formatting |
| **lcov** | Code coverage report generation |

---

## 📐 Design Decisions

**Why separate `ConfigParser` from `ConfigManager`?**
Single Responsibility Principle — the parser only parses, the manager only manages state.
This makes both independently testable and replaceable.

**Why do environment variables override file values?**
This is the [12-Factor App](https://12factor.net/config) standard. Config files ship with
the codebase (safe defaults), while secrets and environment-specific values are injected
at runtime via environment variables — never hardcoded.

**Why `std::optional` for `getOptional()`?**
Forces the caller to explicitly handle the "key not found" case, eliminating
silent bugs from unchecked null/empty returns.

---

## 👤 Author

**Samridhi Nancy**
- GitHub: [@SamridhiNancy](https://github.com/SamridhiNancy)

---

## 📄 License

This project is licensed under the **MIT License** — use it freely in personal or commercial projects.

---

*Built as part of a C++ / Backend / Systems engineering portfolio — demonstrating
CMake build systems, CI/CD pipelines, unit testing, and Software Configuration Management (SCM).*
