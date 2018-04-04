
find_package(OpenSSL REQUIRED)
find_package(Boost 1.53.0 COMPONENTS system REQUIRED)

add_subdirectory(client)
add_subdirectory(server)

