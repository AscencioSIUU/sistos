FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    build-essential \
    gcc \
    gdb \
    valgrind \
    htop \
    procps \
    vim \
    nano \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /labs

CMD ["/bin/bash"]
