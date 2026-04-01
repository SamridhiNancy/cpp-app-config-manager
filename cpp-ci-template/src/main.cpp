#include "ConfigManager.hpp"

#include <iostream>
#include <cstdlib>

int main(int argc, char* argv[]) {
    std::cout << "=== Application Configuration Manager v1.0.0 ===\n\n";

    ConfigManager cfg;

    // 1. Load base config from file
    std::string configFile = (argc > 1) ? argv[1] : "config/app.conf";
    try {
        cfg.loadFromFile(configFile);
    } catch (const std::exception& e) {
        std::cerr << "[WARN] " << e.what() << " — using defaults only\n";
    }

    // 2. Override with environment variables (12-factor app pattern)
    cfg.loadFromEnvironment();

    // 3. Use config values
    std::string env    = cfg.getOrDefault("APP_ENV",  "development");
    std::string host   = cfg.getOrDefault("DB_HOST",  "localhost");
    std::string port   = cfg.getOrDefault("DB_PORT",  "5432");
    std::string logLvl = cfg.getOrDefault("LOG_LEVEL","INFO");

    std::cout << "\n[App] Starting in environment : " << env    << "\n";
    std::cout << "[App] Database host           : " << host   << "\n";
    std::cout << "[App] Database port           : " << port   << "\n";
    std::cout << "[App] Log level               : " << logLvl << "\n";

    // 4. Dump full config (sensitive keys are redacted)
    cfg.dump();

    return EXIT_SUCCESS;
}
