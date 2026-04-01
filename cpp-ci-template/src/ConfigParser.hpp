#pragma once
#include <string>
#include <unordered_map>

/**
 * ConfigParser - Parses KEY=VALUE config files
 *
 * Supports:
 *   - Comments starting with '#'
 *   - Quoted values: KEY="hello world"
 *   - Whitespace trimming
 *   - Empty line skipping
 */
class ConfigParser {
public:
    using ConfigMap = std::unordered_map<std::string, std::string>;

    /// Parse a file, returns key-value map
    static ConfigMap parseFile(const std::string& filepath);

    /// Parse a single line, returns {key, value} or empty if invalid
    static std::pair<std::string, std::string> parseLine(const std::string& line);

private:
    static std::string trim(const std::string& s);
    static std::string stripQuotes(const std::string& s);
};
