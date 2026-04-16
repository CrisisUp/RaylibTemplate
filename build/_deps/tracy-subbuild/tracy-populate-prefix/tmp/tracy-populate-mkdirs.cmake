# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file LICENSE.rst or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "/Users/cristiano/Desktop/teste/RaylibTemplate/build/_deps/tracy-src")
  file(MAKE_DIRECTORY "/Users/cristiano/Desktop/teste/RaylibTemplate/build/_deps/tracy-src")
endif()
file(MAKE_DIRECTORY
  "/Users/cristiano/Desktop/teste/RaylibTemplate/build/_deps/tracy-build"
  "/Users/cristiano/Desktop/teste/RaylibTemplate/build/_deps/tracy-subbuild/tracy-populate-prefix"
  "/Users/cristiano/Desktop/teste/RaylibTemplate/build/_deps/tracy-subbuild/tracy-populate-prefix/tmp"
  "/Users/cristiano/Desktop/teste/RaylibTemplate/build/_deps/tracy-subbuild/tracy-populate-prefix/src/tracy-populate-stamp"
  "/Users/cristiano/Desktop/teste/RaylibTemplate/build/_deps/tracy-subbuild/tracy-populate-prefix/src"
  "/Users/cristiano/Desktop/teste/RaylibTemplate/build/_deps/tracy-subbuild/tracy-populate-prefix/src/tracy-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/cristiano/Desktop/teste/RaylibTemplate/build/_deps/tracy-subbuild/tracy-populate-prefix/src/tracy-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/cristiano/Desktop/teste/RaylibTemplate/build/_deps/tracy-subbuild/tracy-populate-prefix/src/tracy-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
