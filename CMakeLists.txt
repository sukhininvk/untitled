cmake_minimum_required(VERSION 3.16)
project(untitled)

add_subdirectory(vendor/json)
add_subdirectory(vendor/SDL)

add_executable(untitled WIN32
        include/appinfo.h
        include/application.h
        include/paths.h
        include/preferences.h
        include/renderer.h
        include/window.h

        src/application.cpp
        src/preferences.cpp
        src/renderer.cpp
        src/window.cpp

        src/main.cpp
)

target_compile_features(untitled PRIVATE cxx_std_20)
target_link_libraries(untitled PUBLIC SDL3::SDL3)
target_link_libraries(untitled PUBLIC nlohmann_json::nlohmann_json)
target_include_directories(untitled PUBLIC include)

add_custom_command(TARGET untitled POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_RUNTIME_DLLS:untitled>
        $<TARGET_FILE_DIR:untitled>
        COMMAND_EXPAND_LISTS
)
