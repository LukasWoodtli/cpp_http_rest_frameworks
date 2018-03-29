FROM fedora:rawhide
MAINTAINER Lukas Woodtli <woodtli.lukas@gmail.com>

RUN dnf update -y; dnf install -y \
	boost-devel \
	clang \
	cmake \
	cpprest-devel \
	java \
	make \
	python


ENTRYPOINT ["/bin/bash", "-l"]
