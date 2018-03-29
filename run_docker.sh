#!/bin/bash

docker build -t lukaswoodtli/cpp_http_rest_frameworks .

docker run -it --rm --privileged=true -v /etc/passwd:/etc/passwd:ro -v /etc/group:/etc/group:ro -u $UID:$(id -g) -v "$HOME":"$HOME" -v $(pwd):/source lukaswoodtli/cpp_http_rest_frameworks
