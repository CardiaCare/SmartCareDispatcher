cmake_minimum_required(VERSION 3.2)
set (PROJECT dispatcher)
set( EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin )

find_package(PkgConfig)
# SmartSlog
pkg_search_module(SMARTSLOG REQUIRED smartslog)
pkg_search_module(CKPI REQUIRED ckpi)
include_directories(${SMARTSLOG_INCLUDE_DIRS})
include_directories(${CKPI_INCLUDE_DIRS})
link_directories(${SMARTSLOG_LIBRARY_DIRS})
link_directories(${CKPI_LIBRARY_DIRS})
# Boost
set(Boost_USE_STATIC_LIBS   ON)
set(Boost_USE_MULTITHREADED ON)
find_package( Boost 1.54 REQUIRED )
find_package(Boost 1.54 REQUIRED COMPONENTS thread date_time program_options system unit_test_framework)
include_directories( ${Boost_INCLUDE_DIR} )
link_directories( ${Boost_LIBRARY_DIRS})
# to source...
add_subdirectory (sources)
add_subdirectory(test)
enable_testing()