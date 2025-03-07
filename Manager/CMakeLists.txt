# Manager/CMakeLists.txt
set(MANAGER_SOURCES
    ConfigManager.cpp
    WatchdogManager.cpp
    StreamManager.cpp
)

set(MANAGER_HEADERS
    ConfigManager.h
    WatchdogManager.h
    StreamManager.h
)

add_library(Manager ${MANAGER_SOURCES} ${MANAGER_HEADERS})

target_link_libraries(Manager nlohmann_json::nlohmann_json)
