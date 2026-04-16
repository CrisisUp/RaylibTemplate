cmake_minimum_required(VERSION 3.14) # Minimum for FetchContent
project(RalphArcade VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
enable_testing()

# --- Project Metadata ---
set(PROJECT_AUTHOR "Ralph Dev Team")
set(PROJECT_DESCRIPTION "Um jogo de arcade frenético com Ralph!")
set(PROJECT_BUNDLE_ID "com.ralph.arcade")

# --- Options ---
option(TRACY_ENABLE "Enable Tracy profiler" ON)
option(USE_SANITIZERS "Enable ASan and UBSan (Debug only)" ON)

# --- Sanitizers Configuration ---
if(USE_SANITIZERS AND NOT MSVC AND NOT EMSCRIPTEN)
    set(SANITIZER_FLAGS "-fsanitize=address,undefined -fno-omit-frame-pointer")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${SANITIZER_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${SANITIZER_FLAGS}")
    message(STATUS "Sanitizers ativos: Address e Undefined Behavior")
endif()

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

# --- Asset Validation & Packing ---
find_package(Python3 REQUIRED)
if(Python3_FOUND)
    # Target de validação
    add_custom_target(validate-assets
        COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/scripts/validate_assets.py
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )

    # Comando para empacotar assets
    add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/resources.pak
        COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/scripts/pack_assets.py
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/scripts/pack_assets.py
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Empacotando assets em resources.pak..."
    )
    add_custom_target(pack-assets ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/resources.pak)
endif()

# --- Include Directories ---
include_directories(include)

# --- Source Files ---
file(GLOB_RECURSE SOURCES "src/*.cpp")
list(FILTER SOURCES EXCLUDE REGEX "src/main.cpp")

# --- Executable ---
if(APPLE AND NOT EMSCRIPTEN)
    add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${SOURCES} src/main.cpp)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE_GUI_IDENTIFIER ${PROJECT_BUNDLE_ID}
        MACOSX_BUNDLE_BUNDLE_NAME ${PROJECT_NAME}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION}
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_COPYRIGHT "Copyright (c) 2026 ${PROJECT_AUTHOR}"
    )
else()
    add_executable(${PROJECT_NAME} ${SOURCES} src/main.cpp)
endif()

# Garante que o pack-assets rode antes do executável
add_dependencies(${PROJECT_NAME} pack-assets)

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
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/resources.pak $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources.pak
    )
endif()

# --- Installation & CPack ---
if(NOT EMSCRIPTEN)
    if(APPLE)
        # Install as a .app Bundle
        install(TARGETS ${PROJECT_NAME} BUNDLE DESTINATION .)
        # No macOS Bundle, resources.pak vai para Contents/Resources
        install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/resources.pak DESTINATION ${PROJECT_NAME}.app/Contents/Resources)
        install(FILES config.txt DESTINATION ${PROJECT_NAME}.app/Contents/Resources)
    else()
        # Install as a normal executable
        install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
        install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/resources.pak DESTINATION share/${PROJECT_NAME})
        install(FILES config.txt DESTINATION share/${PROJECT_NAME})
    endif()
    
    set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
    set(CPACK_PACKAGE_VENDOR "${PROJECT_AUTHOR}")
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PROJECT_DESCRIPTION}")
    set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
    set(CPACK_PACKAGE_CONTACT "ralph-support@example.com")
    set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
    set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")

    if(APPLE)
        set(CPACK_GENERATOR "DragNDrop")
    elseif(WIN32)
        set(CPACK_GENERATOR "NSIS;ZIP")
        set(CPACK_NSIS_MODIFY_PATH ON)
        set(CPACK_NSIS_DISPLAY_NAME "Ralph Arcade Installer")
        set(CPACK_NSIS_PACKAGE_NAME "RalphArcade")
    else()
        set(CPACK_GENERATOR "DEB;TGZ")
        set(CPACK_DEBIAN_PACKAGE_MAINTAINER "${PROJECT_AUTHOR}")
        set(CPACK_DEBIAN_PACKAGE_SECTION "games")
    endif()
    
    include(CPack)
endif()
