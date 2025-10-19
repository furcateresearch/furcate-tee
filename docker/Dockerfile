FROM ubuntu:22.04

# Install Open Enclave dependencies
RUN apt-get update && apt-get install -y \
    wget \
    gnupg \
    software-properties-common \
    build-essential \
    cmake \
    git \
    && rm -rf /var/lib/apt/lists/*

# Install Open Enclave SDK
RUN echo 'deb [arch=amd64] https://download.01.org/intel-sgx/sgx_repo/ubuntu jammy main' > /etc/apt/sources.list.d/intel-sgx.list \
    && wget -qO - https://download.01.org/intel-sgx/sgx_repo/ubuntu/intel-sgx-deb.key | apt-key add -

RUN apt-get update && apt-get install -y \
    open-enclave \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /furcate-tee

COPY . .

RUN mkdir -p build && cd build && cmake .. && make

CMD ["./build/host/furcate_tee_host", "./build/enclave/furcate_enclave.signed"]
