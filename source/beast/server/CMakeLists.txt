set(TARGET_NAME beast_server)

add_executable(${TARGET_NAME} main.cpp)

target_include_directories(${TARGET_NAME} PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(${TARGET_NAME} ${Boost_SYSTEM_LIBRARY} -lpthread)
