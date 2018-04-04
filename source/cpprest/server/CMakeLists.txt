set(TARGET_NAME cpprest_server)

add_executable(${TARGET_NAME} main.cpp)

# Bad practice to use absolute pathes, but ok for this example
target_include_directories(${TARGET_NAME} PRIVATE "/usr/include/cpprest/" PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(${TARGET_NAME} "/usr/lib64/libcpprest.so" ${Boost_SYSTEM_LIBRARY} ${OPENSSL_LIBRARIES} -lpthread)
