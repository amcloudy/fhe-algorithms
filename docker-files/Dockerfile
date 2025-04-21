FROM --platform=linux/arm64 ubuntu:20.04

ENV DEBIAN_FRONTEND=noninteractive
ENV CC=/usr/bin/gcc-10
ENV CXX=/usr/bin/g++-10
ENV NO_THREADS=4
ENV CMAKE_VERSION=3.22.0
ENV LD_LIBRARY_PATH=/usr/local/lib:${LD_LIBRARY_PATH}
ENV VCPKG_FORCE_SYSTEM_BINARIES=1

# Update and install basic system dependencies
RUN apt-get update && \
    apt-get install -y --no-install-recommends \
        software-properties-common \
        openjdk-17-jdk \
        openjdk-17-jre \
        wget \
        git \
        curl \
        build-essential \
        gcc-10 \
        g++-10 \
        zip \
        unzip \
        tar \
        pkg-config \
        autoconf \
        clang-10 \
        libomp5 \
        libomp-dev \
        doxygen \
        graphviz \
        libboost-all-dev=1.71.0.0ubuntu2 \
        ninja-build \
        libssl-dev \
        zlib1g-dev \
        ca-certificates && \
    apt-add-repository ppa:ubuntu-toolchain-r/test && \
    apt-get update && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

# Install CMake from source
RUN wget https://github.com/Kitware/CMake/releases/download/v${CMAKE_VERSION}/cmake-${CMAKE_VERSION}.tar.gz && \
    tar -xzvf cmake-${CMAKE_VERSION}.tar.gz && \
    cd cmake-${CMAKE_VERSION} && \
    ./bootstrap && make -j$(nproc) && make install && \
    cd .. && rm -rf cmake-${CMAKE_VERSION}*

# Install Catch2 (for tests, optional)
RUN git clone https://github.com/catchorg/Catch2.git && \
    cd Catch2 && \
    cmake -B build -S . -DBUILD_TESTING=OFF && \
    cmake --build build/ --target install && \
    cd .. && rm -rf Catch2

# Clone and build OpenFHE
RUN git clone https://github.com/openfheorg/openfhe-development.git && \
    cd openfhe-development && \
    git checkout v0.9.1 && \
    git submodule update --init --recursive && \
    mkdir build && cd build && \
    cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local && \
    make -j${NO_THREADS} && \
    make install && \
    cd ../.. && rm -rf openfhe-development

# Install yaml-cpp using vcpkg
RUN git clone https://github.com/microsoft/vcpkg.git /opt/vcpkg && \
    /opt/vcpkg/bootstrap-vcpkg.sh && \
    /opt/vcpkg/vcpkg install yaml-cpp && \
    /opt/vcpkg/vcpkg integrate install

# Set workdir to the mounted project
WORKDIR /fhe-algorithms

CMD ["bash"]
