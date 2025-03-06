cmake_minimum_required(VERSION 3.15.4)
project(CPPUTILS LANGUAGES CXX)

option(CPPUTILS_TEST "Enable testing" OFF)

# Adhere to GNU conventions
include(GNUInstallDirs)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(fmt CONFIG)

file(GLOB CPPUTILS_PUBLIC_HEADERS include/cpputils/*.h)
file(GLOB CPPUTILS_SOURCES cpputils/*.cpp)
add_library(cpputils "")
target_sources(
  cpputils
  PRIVATE ${CPPUTILS_SOURCES}
  PUBLIC ${CPPUTILS_PUBLIC_HEADERS})
target_include_directories(cpputils PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                                           $<INSTALL_INTERFACE:include/>)
target_link_libraries(cpputils PRIVATE fmt::fmt)

if(CPPUTILS_TEST)
  find_package(GTest REQUIRED)
endif()

install(FILES "${CPPUTILS_PUBLIC_HEADERS}" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/cpputils")
install(
  TARGETS cpputils
  RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
  ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}")
