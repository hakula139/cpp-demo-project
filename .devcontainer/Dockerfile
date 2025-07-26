FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive
ENV LLVM_VERSION=20
ENV GCC_VERSION=14

# Install common dependencies
RUN apt-get update && apt-get install -y \
  build-essential \
  catch2 \
  cmake \
  cppcheck \
  ninja-build \
  git \
  htop \
  jq \
  tree \
  zsh

RUN chsh -s /bin/zsh

# Install Python
RUN apt-get update && apt-get install -y \
  python3 \
  python3-pip \
  python3-venv

# Install pre-commit and gersemi
RUN pip3 install --break-system-packages \
  pre-commit \
  gersemi

# Install GCC
RUN add-apt-repository ppa:ubuntu-toolchain-r/ppa -y \
  && apt-get update && apt-get install -y gcc-${GCC_VERSION} \
  && update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-${GCC_VERSION} ${GCC_VERSION}00 \
  && update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-${GCC_VERSION} ${GCC_VERSION}00

# Install LLVM
RUN wget https://apt.llvm.org/llvm.sh \
  && chmod +x ./llvm.sh \
  && ./llvm.sh ${LLVM_VERSION} \
  && rm ./llvm.sh

RUN apt-get install -y \
  clang-format-${LLVM_VERSION} \
  clang-tidy-${LLVM_VERSION} \
  && update-alternatives --install /usr/bin/clang clang /usr/bin/clang-${LLVM_VERSION} ${LLVM_VERSION}00 \
  && update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-${LLVM_VERSION} ${LLVM_VERSION}00 \
  && update-alternatives --install /usr/bin/clang-format clang-format /usr/bin/clang-format-${LLVM_VERSION} ${LLVM_VERSION}00 \
  && update-alternatives --install /usr/bin/clang-tidy clang-tidy /usr/bin/clang-tidy-${LLVM_VERSION} ${LLVM_VERSION}00 \
  && update-alternatives --install /usr/bin/lldb lldb /usr/bin/lldb-${LLVM_VERSION} ${LLVM_VERSION}00

ENV CC=clang
ENV CXX=clang++

# Set working directory to workspace
WORKDIR /workspaces/cpp-demo-project
