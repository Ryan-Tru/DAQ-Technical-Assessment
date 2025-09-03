
FROM ubuntu:22.04 AS dev
ARG DEBIAN_FRONTEND=noninteractive

WORKDIR /app

RUN apt-get update && \
    apt-get install -y python3-pip \
    libboost-all-dev libxml2-dev \
    wget autoconf libtool git make g++ valgrind \
    cmake can-utils 

COPY . .

RUN find . -name "CMakeCache.txt" -delete

RUN cd build/ && rm CMakeCache.txt || true

RUN cd build/ && cmake -DCMAKE_BUILD_TYPE=Release ..

RUN cd build/ && make -j    

ENTRYPOINT ["./build/solution/answer"]
# ENTRYPOINT ["bash"]

