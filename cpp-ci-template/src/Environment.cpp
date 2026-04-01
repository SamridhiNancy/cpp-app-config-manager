#include "Environment.hpp"

#include <cstdlib>
#include <cstring>

// POSIX
extern char** environ;

std::unordered_map<std::string, std::string> Environment::getAll() {
    std::unordered_map<std::string, std::string> result;

    if (!environ) return result;

    for (char** env = environ; *env != nullptr; ++env) {
        std::string entry(*env);
        auto eqPos = entry.find('=');
        if (eqPos != std::string::npos) {
            result[entry.substr(0, eqPos)] = entry.substr(eqPos + 1);
        }
    }
    return result;
}

std::optional<std::string> Environment::get(const std::string& key) {
    const char* val = std::getenv(key.c_str());
    if (!val) return std::nullopt;
    return std::string(val);
}
