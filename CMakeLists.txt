cmake_minimum_required(VERSION 3.14) # Minimum for FetchContent
project(RalphArcade VERSION 1.0.0 LANGUAGES CXX)

if(POLICY CMP0135)
  cmake_policy(SET CMP0135 NEW)
endif()

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
option(ENABLE_COVERAGE "Enable Code Coverage (Gcc/Clang only)" OFF)

# --- Code Coverage ---
if(ENABLE_COVERAGE)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -fprofile-arcs -ftest-coverage")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
        message(STATUS "Code Coverage habilitado")
    else()
        message(WARNING "Coverage não suportado para este compilador.")
    endif()
endif()

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

# --- Fetch backward-cpp ---
if(NOT EMSCRIPTEN)
    FetchContent_Declare(
        backward
        GIT_REPOSITORY https://github.com/bombela/backward-cpp.git
        GIT_TAG        master
    )
    FetchContent_MakeAvailable(backward)
endif()

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
find_program(CPPCHECK_EXE NAMES "cppcheck")
if(CPPCHECK_EXE AND NOT EMSCRIPTEN)
    add_custom_target(cppcheck
        COMMAND ${CPPCHECK_EXE} 
        --enable=warning,performance,portability,style 
        --inconclusive 
        --force 
        --inline-suppr
        --suppress=missingIncludeSystem
        -I ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        COMMENT "Rodando análise estática profunda com Cppcheck..."
    )
endif()

find_program(CLANG_TIDY_EXE NAMES "clang-tidy")
if(CLANG_TIDY_EXE AND NOT EMSCRIPTEN)
    set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")
endif()

find_program(CLANG_FORMAT_EXE NAMES "clang-format")
if(CLANG_FORMAT_EXE)
    file(GLOB_RECURSE ALL_CXX_SOURCE_FILES src/*.cpp include/*.h tests/*.cpp)
    add_custom_target(format COMMAND ${CLANG_FORMAT_EXE} -i -style=file ${ALL_CXX_SOURCE_FILES})
endif()

# --- Documentation (Doxygen) ---
find_package(Doxygen)
if (DOXYGEN_FOUND)
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

    add_custom_target(docs
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Gerando documentação com Doxygen..."
        VERBATIM)
endif()

# --- Asset Validation & Packing ---
find_package(Python3 REQUIRED)
if(Python3_FOUND)
    # Target de validação
    add_custom_target(validate-assets
        COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/scripts/validate_assets.py
        COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/scripts/validate_translations.py
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Validando assets e arquivos de tradução..."
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

# --- Core Library (RalphArcadeCore) ---
file(GLOB_RECURSE CORE_SOURCES "src/*.cpp")
list(FILTER CORE_SOURCES EXCLUDE REGEX "src/main.cpp")
add_library(RalphArcadeCore STATIC ${CORE_SOURCES})
target_include_directories(RalphArcadeCore PUBLIC include)
target_link_libraries(RalphArcadeCore PUBLIC raylib spdlog::spdlog)

if(NOT EMSCRIPTEN)
    target_link_libraries(RalphArcadeCore PUBLIC backward)
    target_compile_definitions(RalphArcadeCore PUBLIC BACKWARD_HAS_DW=1)
endif()

if(TRACY_ENABLE AND NOT EMSCRIPTEN)
    target_link_libraries(RalphArcadeCore PUBLIC TracyClient)
    target_compile_definitions(RalphArcadeCore PUBLIC TRACY_ENABLE)
endif()

# --- Main Executable ---
if(APPLE AND NOT EMSCRIPTEN)
    # Suporte a ícone nativo no Mac (.icns)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/resources/ui/icon.icns")
        set(MACOSX_ICON "${CMAKE_CURRENT_SOURCE_DIR}/resources/ui/icon.icns")
        set_source_files_properties(${MACOSX_ICON} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
        add_executable(${PROJECT_NAME} MACOSX_BUNDLE src/main.cpp ${MACOSX_ICON})
    else()
        add_executable(${PROJECT_NAME} MACOSX_BUNDLE src/main.cpp)
    endif()

    set_target_properties(${PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE_GUI_IDENTIFIER ${PROJECT_BUNDLE_ID}
        MACOSX_BUNDLE_BUNDLE_NAME ${PROJECT_NAME}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION}
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_COPYRIGHT "Copyright (c) 2026 ${PROJECT_AUTHOR}"
        MACOSX_BUNDLE_ICON_FILE "icon.icns"
    )
elseif(WIN32)
    # Suporte a ícone nativo no Windows (.rc/.ico)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/resources/ui/icon.rc")
        add_executable(${PROJECT_NAME} WIN32 src/main.cpp "${CMAKE_CURRENT_SOURCE_DIR}/resources/ui/icon.rc")
    else()
        add_executable(${PROJECT_NAME} WIN32 src/main.cpp)
    endif()
else()
    add_executable(${PROJECT_NAME} src/main.cpp)
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE RalphArcadeCore)
add_dependencies(${PROJECT_NAME} pack-assets)

if(APPLE AND NOT EMSCRIPTEN)
    target_link_libraries(${PROJECT_NAME} PRIVATE "-framework IOKit" "-framework Cocoa" "-framework OpenGL")
endif()

# --- Unit Tests ---
if(NOT EMSCRIPTEN)
    file(GLOB_RECURSE TEST_SOURCES "tests/*.cpp")
    if(TEST_SOURCES)
        add_executable(unit_tests ${TEST_SOURCES})
        target_link_libraries(unit_tests PRIVATE RalphArcadeCore GTest::gtest_main)
        if(APPLE)
            target_link_libraries(unit_tests PRIVATE "-framework IOKit" "-framework Cocoa" "-framework OpenGL")
        endif()
        add_test(NAME AllTests COMMAND unit_tests)
    endif()
endif()

# --- Visual Regression Tests ---
if(NOT EMSCRIPTEN AND Python3_FOUND)
    add_test(NAME VisualRegression
        COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_CURRENT_BINARY_DIR} 
        ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_SOURCE_DIR}/scripts/run_visual_test.cmake
    )
endif()

# --- Performance Benchmark Test ---
if(NOT EMSCRIPTEN AND Python3_FOUND)
    set(BENCH_EXE "./RalphArcade")
    if(APPLE)
        set(BENCH_EXE "./RalphArcade.app/Contents/MacOS/RalphArcade")
    elseif(WIN32)
        set(BENCH_EXE "./RalphArcade.exe")
    endif()

    add_test(NAME PerformanceBenchmark
        COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/scripts/verify_performance.py ${BENCH_EXE} 55.0 300
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
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
    install(TARGETS ${PROJECT_NAME} RalphArcadeCore
        BUNDLE DESTINATION .
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
    )
    
    if(APPLE)
        install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/resources.pak DESTINATION ${PROJECT_NAME}.app/Contents/Resources)
        install(FILES config.txt DESTINATION ${PROJECT_NAME}.app/Contents/Resources)
    else()
        install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/resources.pak DESTINATION .)
        install(FILES config.txt DESTINATION .)
    endif()
    
    set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
    set(CPACK_PACKAGE_VENDOR "${PROJECT_AUTHOR}")
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PROJECT_DESCRIPTION}")
    set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
    set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
    set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")

    if(APPLE)
        set(CPACK_GENERATOR "DragNDrop")
        set(CPACK_DMG_VOLUME_NAME "${PROJECT_NAME}")
        set(CPACK_DMG_FORMAT "UDZO")
    elseif(WIN32)
        set(CPACK_GENERATOR "NSIS;ZIP")
        set(CPACK_NSIS_DISPLAY_NAME "${PROJECT_NAME}")
        set(CPACK_NSIS_PACKAGE_NAME "${PROJECT_NAME}")
        set(CPACK_NSIS_MODIFY_PATH ON)
        set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)
        if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/resources/ui/icon.ico")
            set(CPACK_NSIS_MUI_ICON "${CMAKE_CURRENT_SOURCE_DIR}/resources/ui/icon.ico")
            set(CPACK_NSIS_MUI_UNIICON "${CMAKE_CURRENT_SOURCE_DIR}/resources/ui/icon.ico")
        endif()
    else()
        set(CPACK_GENERATOR "DEB;TGZ")
        set(CPACK_DEBIAN_PACKAGE_MAINTAINER "${PROJECT_AUTHOR}")
    endif()
    
    include(CPack)
endif()
