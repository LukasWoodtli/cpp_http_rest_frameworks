# Comparison cpprest and boost::beast


## cpprest

- Open source project by Microsoft
- Supports all major platforms (Linux, Windows, macOS)
- Includes a task library (PPLX) based on boost:asio
- Includes a JSON library
- Easy to use
- boost::asio is hidden
- Supports REST directly
- works with continuation model (tasks)


## beast

- Based on boost::asio
- Part of boost (since version 1.66)
- Needs some boilerplate code
- Understanding of boost::asio needed
- will probably be more adopted by the industry (boost)
- works with completion handlers (boost::asio)


## Comparison

- From [boost::beast (HTTP Comparison to Other Libraries)](https://www.boost.org/doc/libs/1_66_0/libs/beast/doc/html/beast/design_choices/http_comparison_to_other_librari.html#beast.design_choices.http_comparison_to_other_librari.c_rest_sdk_cpprestsdk)
    - The only customization [point] is in the `concurrency::streams::istream and concurrency::streams::ostream` reference
    - Setting the [HTTP] body: `vector` or a `concurrency::streams::istream`. No user defined types are possible.
