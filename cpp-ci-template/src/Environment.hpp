#pragma once
#include <string>
#include <unordered_map>
#include <optional>

/**
 * Environment - Wrapper around process environment variables
 */
class Environment {
public:
    /// Get all environment variables as a map
    static std::unordered_map<std::string, std::string> getAll();

    /// Get a single environment variable
    static std::optional<std::string> get(const std::string& key);
};
