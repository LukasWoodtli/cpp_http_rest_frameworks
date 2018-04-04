FROM fedora:rawhide
MAINTAINER Lukas Woodtli <woodtli.lukas@gmail.com>

RUN dnf update -y; dnf install -y \
	boost-devel \
	cgdb \
	clang \
	cmake \
	cpprest-devel \
	gdb \
	java \
	make \
	openssl-devel \
	python


ENTRYPOINT ["/bin/bash", "-l"]
