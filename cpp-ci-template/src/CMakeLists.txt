# ─── Library: config_lib (reusable core) ────────────────────────
add_library(config_lib
    ConfigManager.cpp
    ConfigParser.cpp
    Environment.cpp
)

target_include_directories(config_lib
    PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE ${CMAKE_SOURCE_DIR}/config
)

# ─── Executable ─────────────────────────────────────────────────
add_executable(app_config_manager
    main.cpp
)

target_link_libraries(app_config_manager
    PRIVATE config_lib
)
