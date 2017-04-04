file (GLOB COMMON_SRC *.cpp)
set (SOURCE ${COMMON_SRC})

add_compile_options(-std=c++14)
add_subdirectory (ontology)
add_executable(${PROJECT} ${SOURCE})
target_link_libraries(dispatcher smartcare)

target_link_libraries(dispatcher ${SMARTSLOG_LIBRARIES})
target_link_libraries(dispatcher ${CKPI_LIBRARIES})
target_link_libraries(dispatcher ${CMAKE_THREAD_LIBS_INIT} ${Boost_LIBRARIES})
target_link_libraries(dispatcher ${LibOSRM_LIBRARIES} ${LibOSRM_DEPENDENT_LIBRARIES})

set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} ${LibOSRM_CXXFLAGS} -fprofile-arcs -ftest-coverage")
target_link_libraries(dispatcher gcov)
