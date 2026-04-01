#include "ConfigManager.hpp"
#include "ConfigParser.hpp"
#include "Environment.hpp"

#include <iostream>
#include <algorithm>
#include <cctype>

// ─── Public Methods ──────────────────────────────────────────────

void ConfigManager::loadFromFile(const std::string& filepath) {
    auto parsed = ConfigParser::parseFile(filepath);
    for (auto& [key, value] : parsed) {
        store_[key] = std::move(value);
    }
    std::cout << "[ConfigManager] Loaded " << parsed.size()
              << " keys from: " << filepath << "\n";
}

void ConfigManager::loadFromEnvironment() {
    auto envVars = Environment::getAll();
    std::size_t count = 0;
    for (auto& [key, value] : envVars) {
        if (store_.count(key)) {         // only override known keys
            store_[key] = std::move(value);
            ++count;
        }
    }
    std::cout << "[ConfigManager] Environment overrode " << count << " key(s)\n";
}

std::string ConfigManager::get(const std::string& key) const {
    auto it = store_.find(key);
    if (it == store_.end()) {
        throw std::runtime_error("[ConfigManager] Missing required key: " + key);
    }
    return it->second;
}

std::optional<std::string> ConfigManager::getOptional(const std::string& key) const {
    auto it = store_.find(key);
    if (it == store_.end()) return std::nullopt;
    return it->second;
}

std::string ConfigManager::getOrDefault(const std::string& key,
                                        const std::string& defaultValue) const {
    return getOptional(key).value_or(defaultValue);
}

bool ConfigManager::has(const std::string& key) const {
    return store_.count(key) > 0;
}

void ConfigManager::set(const std::string& key, const std::string& value) {
    store_[key] = value;
}

void ConfigManager::dump() const {
    std::cout << "\n── Config Dump (" << store_.size() << " keys) ──\n";
    for (const auto& [key, value] : store_) {
        if (isSensitiveKey(key)) {
            std::cout << "  " << key << " = [REDACTED]\n";
        } else {
            std::cout << "  " << key << " = " << value << "\n";
        }
    }
    std::cout << "────────────────────────────────\n";
}

// ─── Private Helpers ─────────────────────────────────────────────

std::string ConfigManager::trim(const std::string& s) {
    auto start = s.begin();
    while (start != s.end() && std::isspace(*start)) ++start;
    auto end = s.end();
    while (end != start && std::isspace(*(end - 1))) --end;
    return {start, end};
}

bool ConfigManager::isSensitiveKey(const std::string& key) {
    static const std::vector<std::string> sensitiveWords = {
        "SECRET", "PASSWORD", "TOKEN", "KEY", "PASS"
    };
    std::string upper = key;
    std::transform(upper.begin(), upper.end(), upper.begin(), ::toupper);
    for (const auto& word : sensitiveWords) {
        if (upper.find(word) != std::string::npos) return true;
    }
    return false;
}
