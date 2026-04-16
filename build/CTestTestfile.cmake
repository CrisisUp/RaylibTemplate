# CMake generated Testfile for 
# Source directory: /Users/cristiano/Desktop/teste/RaylibTemplate
# Build directory: /Users/cristiano/Desktop/teste/RaylibTemplate/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(AllTests "/Users/cristiano/Desktop/teste/RaylibTemplate/build/unit_tests")
set_tests_properties(AllTests PROPERTIES  _BACKTRACE_TRIPLES "/Users/cristiano/Desktop/teste/RaylibTemplate/CMakeLists.txt;146;add_test;/Users/cristiano/Desktop/teste/RaylibTemplate/CMakeLists.txt;0;")
subdirs("_deps/spdlog-build")
subdirs("_deps/raylib-build")
subdirs("_deps/tracy-build")
subdirs("_deps/googletest-build")
