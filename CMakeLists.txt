cmake_minimum_required(VERSION 3.10)
project(RalphArcade VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Find Raylib ---
# On Mac, we assume it's installed via Homebrew
if(APPLE)
    set(RAYLIB_INCLUDE_DIR "/opt/homebrew/include")
    set(RAYLIB_LIB_DIR "/opt/homebrew/lib")
    link_directories(${RAYLIB_LIB_DIR})
endif()

# --- Include Directories ---
include_directories(include)
include_directories(${RAYLIB_INCLUDE_DIR})

# --- Source Files ---
file(GLOB_RECURSE SOURCES "src/*.cpp")

# --- Executable ---
add_executable(${PROJECT_NAME} ${SOURCES})

# --- Linking ---
if(APPLE)
    target_link_libraries(${PROJECT_NAME} raylib "-framework IOKit" "-framework Cocoa" "-framework OpenGL")
else()
    # For Windows/Linux (Basic setup)
    target_link_libraries(${PROJECT_NAME} raylib)
endif()

# --- Resource Handling ---
if(NOT MSVC)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E create_symlink 
        ${CMAKE_SOURCE_DIR}/resources 
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources
        COMMAND ${CMAKE_COMMAND} -E create_symlink 
        ${CMAKE_SOURCE_DIR}/config.txt 
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/config.txt
    )
endif()

# --- Installation (Optional) ---
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
install(DIRECTORY resources DESTINATION bin)
