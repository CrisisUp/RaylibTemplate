# CMake generated Testfile for 
# Source directory: /Users/cristiano/Desktop/teste/RaylibTemplate
# Build directory: /Users/cristiano/Desktop/teste/RaylibTemplate/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(AllTests "/Users/cristiano/Desktop/teste/RaylibTemplate/build/unit_tests")
set_tests_properties(AllTests PROPERTIES  _BACKTRACE_TRIPLES "/Users/cristiano/Desktop/teste/RaylibTemplate/CMakeLists.txt;206;add_test;/Users/cristiano/Desktop/teste/RaylibTemplate/CMakeLists.txt;0;")
add_test(VisualRegression "/opt/homebrew/bin/cmake" "-E" "chdir" "/Users/cristiano/Desktop/teste/RaylibTemplate/build" "/opt/homebrew/bin/cmake" "-P" "/Users/cristiano/Desktop/teste/RaylibTemplate/scripts/run_visual_test.cmake")
set_tests_properties(VisualRegression PROPERTIES  _BACKTRACE_TRIPLES "/Users/cristiano/Desktop/teste/RaylibTemplate/CMakeLists.txt;212;add_test;/Users/cristiano/Desktop/teste/RaylibTemplate/CMakeLists.txt;0;")
add_test(PerformanceBenchmark "/opt/homebrew/Frameworks/Python.framework/Versions/3.14/bin/python3.14" "/Users/cristiano/Desktop/teste/RaylibTemplate/scripts/verify_performance.py" "./RalphArcade.app/Contents/MacOS/RalphArcade" "55.0" "300")
set_tests_properties(PerformanceBenchmark PROPERTIES  WORKING_DIRECTORY "/Users/cristiano/Desktop/teste/RaylibTemplate/build" _BACKTRACE_TRIPLES "/Users/cristiano/Desktop/teste/RaylibTemplate/CMakeLists.txt;227;add_test;/Users/cristiano/Desktop/teste/RaylibTemplate/CMakeLists.txt;0;")
subdirs("_deps/spdlog-build")
subdirs("_deps/backward-build")
subdirs("_deps/raylib-build")
subdirs("_deps/tracy-build")
subdirs("_deps/googletest-build")
