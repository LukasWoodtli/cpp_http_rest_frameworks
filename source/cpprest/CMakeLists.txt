
find_package(OpenSSL REQUIRED)
find_package(Boost 1.53.0 REQUIRED thread system program_options)

add_subdirectory(client)
add_subdirectory(server)

