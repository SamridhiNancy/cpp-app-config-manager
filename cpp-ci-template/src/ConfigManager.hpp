#pragma once

#include <string>
#include <unordered_map>
#include <stdexcept>
#include <optional>

/**
 * ConfigManager - Application Configuration Management
 *
 * Loads key-value config from files and environment variables.
 * Environment variables take precedence over file values (12-factor app pattern).
 *
 * Usage:
 *   ConfigManager cfg;
 *   cfg.loadFromFile("config/app.conf");
 *   cfg.loadFromEnvironment();
 *   std::string host = cfg.get("DB_HOST");
 */
class ConfigManager {
public:
    ConfigManager() = default;
    ~ConfigManager() = default;

    // Non-copyable, movable
    ConfigManager(const ConfigManager&) = delete;
    ConfigManager& operator=(const ConfigManager&) = delete;
    ConfigManager(ConfigManager&&) = default;
    ConfigManager& operator=(ConfigManager&&) = default;

    /// Load config from a .conf / .env file (KEY=VALUE format)
    void loadFromFile(const std::string& filepath);

    /// Override/extend with process environment variables
    void loadFromEnvironment();

    /// Get a required value (throws if missing)
    std::string get(const std::string& key) const;

    /// Get an optional value
    std::optional<std::string> getOptional(const std::string& key) const;

    /// Get with a default fallback
    std::string getOrDefault(const std::string& key, const std::string& defaultValue) const;

    /// Check if a key exists
    bool has(const std::string& key) const;

    /// Set a value at runtime
    void set(const std::string& key, const std::string& value);

    /// Print all config (redacts values containing "SECRET", "PASSWORD", "TOKEN")
    void dump() const;

    /// Number of loaded keys
    std::size_t size() const { return store_.size(); }

private:
    std::unordered_map<std::string, std::string> store_;

    static std::string trim(const std::string& s);
    static bool isSensitiveKey(const std::string& key);
};
