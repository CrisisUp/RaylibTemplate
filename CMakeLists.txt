cmake_minimum_required(VERSION 3.14) # Minimum for FetchContent
project(RalphArcade VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
enable_testing()

# --- Options ---
option(TRACY_ENABLE "Enable Tracy profiler" ON)

# --- Emscripten / Web Support ---
if(EMSCRIPTEN)
    set(PLATFORM "PLATFORM_WEB" CACHE STRING "Raylib platform")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s ASYNCIFY -s USE_GLFW=3 --preload-file resources --preload-file config.txt")
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
endif()

# --- Fetch spdlog ---
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG        v1.13.0
)
FetchContent_MakeAvailable(spdlog)

# --- Fetch Raylib ---
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
  raylib
  GIT_REPOSITORY https://github.com/raysan5/raylib.git
  GIT_TAG        5.0
)
FetchContent_MakeAvailable(raylib)

# --- Fetch Tracy Profiler ---
if(TRACY_ENABLE AND NOT EMSCRIPTEN)
    FetchContent_Declare(
        tracy
        GIT_REPOSITORY https://github.com/wolfpld/tracy.git
        GIT_TAG        v0.10
    )
    FetchContent_MakeAvailable(tracy)
endif()

# --- Fetch GoogleTest ---
if(NOT EMSCRIPTEN)
    FetchContent_Declare(
      googletest
      URL https://github.com/google/googletest/archive/refs/heads/main.zip
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
endif()

# --- Static Analysis & Formatting ---
find_program(CLANG_TIDY_EXE NAMES "clang-tidy")
if(CLANG_TIDY_EXE AND NOT EMSCRIPTEN)
    set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")
endif()

find_program(CLANG_FORMAT_EXE NAMES "clang-format")
if(CLANG_FORMAT_EXE)
    file(GLOB_RECURSE ALL_CXX_SOURCE_FILES src/*.cpp include/*.h tests/*.cpp)
    add_custom_target(format COMMAND ${CLANG_FORMAT_EXE} -i -style=file ${ALL_CXX_SOURCE_FILES})
endif()

# --- Asset Validation ---
find_package(Python3 REQUIRED)
if(Python3_FOUND)
    add_custom_target(validate-assets
        COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/scripts/validate_assets.py
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
endif()

# --- Include Directories ---
include_directories(include)

# --- Source Files ---
file(GLOB_RECURSE SOURCES "src/*.cpp")
list(FILTER SOURCES EXCLUDE REGEX "src/main.cpp")

# --- Executable ---
add_executable(${PROJECT_NAME} ${SOURCES} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE raylib spdlog::spdlog)

if(TRACY_ENABLE AND NOT EMSCRIPTEN)
    target_link_libraries(${PROJECT_NAME} PRIVATE TracyClient)
    target_compile_definitions(${PROJECT_NAME} PRIVATE TRACY_ENABLE)
endif()

if(APPLE AND NOT EMSCRIPTEN)
    target_link_libraries(${PROJECT_NAME} PRIVATE "-framework IOKit" "-framework Cocoa" "-framework OpenGL")
endif()

# --- Unit Tests ---
if(NOT EMSCRIPTEN)
    file(GLOB_RECURSE TEST_SOURCES "tests/*.cpp")
    if(TEST_SOURCES)
        add_executable(unit_tests ${TEST_SOURCES} ${SOURCES})
        target_link_libraries(unit_tests PRIVATE GTest::gtest_main raylib spdlog::spdlog)
        if(APPLE)
            target_link_libraries(unit_tests PRIVATE "-framework IOKit" "-framework Cocoa" "-framework OpenGL")
        endif()
        add_test(NAME AllTests COMMAND unit_tests)
    endif()
endif()

# --- Resource Handling ---
if(NOT MSVC AND NOT EMSCRIPTEN)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources
        COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/config.txt $<TARGET_FILE_DIR:${PROJECT_NAME}>/config.txt
    )
endif()

# --- Installation & CPack ---
if(NOT EMSCRIPTEN)
    install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION . BUNDLE DESTINATION .)
    install(DIRECTORY resources DESTINATION .)
    install(FILES config.txt DESTINATION .)
    
    set(CPACK_PACKAGE_NAME "RalphArcade")
    set(CPACK_PACKAGE_VERSION "1.0.0")
    if(APPLE)
        set(CPACK_GENERATOR "DragNDrop")
    elseif(WIN32)
        set(CPACK_GENERATOR "ZIP;NSIS")
    else()
        set(CPACK_GENERATOR "TGZ;DEB")
    endif()
    include(CPack)
endif()
