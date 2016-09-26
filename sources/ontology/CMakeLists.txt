file (GLOB COMMON_SRC *.c)
set (SOURCE ${COMMON_SRC})

add_compile_options(-std=c99)
add_library(smartcare STATIC ${SOURCE})

target_link_libraries(smartcare ${SMARTSLOG_LIBRARIES})
target_link_libraries(smartcare ${CKPI_LIBRARIES})
