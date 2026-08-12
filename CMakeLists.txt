cmake_minimum_required(VERSION 3.16)
project(untitled)

add_subdirectory(vendor/SDL)

add_executable(untitled WIN32
        include/gameobjects/camera.h
        include/application.h

        src/gameobjects/camera.cpp
        src/application.cpp
        src/main.cpp
)

target_compile_features(untitled PRIVATE cxx_std_20)
target_link_libraries(untitled PUBLIC SDL3::SDL3)
target_include_directories(untitled PUBLIC include)

add_custom_command(TARGET untitled POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_RUNTIME_DLLS:untitled>
        $<TARGET_FILE_DIR:untitled>
        COMMAND_EXPAND_LISTS
)
