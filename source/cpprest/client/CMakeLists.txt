cmake_minimum_required(VERSION 3.10)
project(cpprest_client)

find_package(OpenSSL REQUIRED)
find_package(Boost 1.53.0 REQUIRED thread system program_options)

add_executable(cpprest_client main.cpp)


# bad practice, but ok for this example
target_include_directories(cpprest_client PRIVATE "/usr/include/cpprest/" PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(cpprest_client /usr/lib64/libcpprest.so ${Boost_SYSTEM_LIBRARY} ${OPENSSL_LIBRARIES} -lpthread)
