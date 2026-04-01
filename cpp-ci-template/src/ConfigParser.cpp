#include "ConfigParser.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cctype>
#include <algorithm>

ConfigParser::ConfigMap ConfigParser::parseFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("[ConfigParser] Cannot open file: " + filepath);
    }

    ConfigMap result;
    std::string line;
    int lineNum = 0;

    while (std::getline(file, line)) {
        ++lineNum;
        auto [key, value] = parseLine(line);
        if (!key.empty()) {
            result[key] = value;
        }
    }

    return result;
}

std::pair<std::string, std::string> ConfigParser::parseLine(const std::string& line) {
    std::string trimmed = trim(line);

    // Skip empty lines and comments
    if (trimmed.empty() || trimmed[0] == '#') {
        return {"", ""};
    }

    auto eqPos = trimmed.find('=');
    if (eqPos == std::string::npos) {
        return {"", ""}; // not a valid KEY=VALUE line
    }

    std::string key   = trim(trimmed.substr(0, eqPos));
    std::string value = trim(trimmed.substr(eqPos + 1));
    value = stripQuotes(value);

    // Strip inline comment (value = something # comment)
    auto hashPos = value.find(" #");
    if (hashPos != std::string::npos) {
        value = trim(value.substr(0, hashPos));
    }

    if (key.empty()) return {"", ""};
    return {key, value};
}

// ─── Private ─────────────────────────────────────────────────────

std::string ConfigParser::trim(const std::string& s) {
    auto start = s.begin();
    while (start != s.end() && std::isspace(*start)) ++start;
    auto end = s.end();
    while (end != start && std::isspace(*(end - 1))) --end;
    return {start, end};
}

std::string ConfigParser::stripQuotes(const std::string& s) {
    if (s.size() >= 2 &&
        ((s.front() == '"' && s.back() == '"') ||
         (s.front() == '\'' && s.back() == '\''))) {
        return s.substr(1, s.size() - 2);
    }
    return s;
}
