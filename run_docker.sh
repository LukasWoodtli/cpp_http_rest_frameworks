#!/bin/bash

docker build -t lukaswoodtli/cpp_http_rest_frameworks .

docker run -it --rm --privileged=true -v /etc/passwd:/etc/passwd:ro -v /etc/group:/etc/group:ro -u $UID:$(id -g) -v $(pwd):/home/$USER/cpp_http_rest_frameworks lukaswoodtli/cpp_http_rest_frameworks
