set(TARGET_NAME beast_client)

add_executable(${TARGET_NAME} main.cpp)

# Bad practice to use absolute pathes, but ok for this example
target_include_directories(${TARGET_NAME} PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(${TARGET_NAME} ${Boost_SYSTEM_LIBRARY} -lpthread)
