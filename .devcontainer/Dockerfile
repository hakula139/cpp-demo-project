FROM ubuntu:latest

ENV DEBIAN_FRONTEND=noninteractive

ARG LLVM_VERSION=20
ARG GCC_VERSION=14

# Install base dependencies
RUN apt-get update && apt-get install -y \
  ca-certificates \
  curl \
  wget

# Use mirror to speed up
COPY apt/sources.list.d /tmp/sources.list.d
RUN /tmp/sources.list.d/init.sh \
  && rm -rf /tmp/sources.list.d

# Install common dependencies
RUN apt-get update && apt-get install -y \
  unminimize \
  sudo \
  software-properties-common \
  locales \
  build-essential \
  cmake \
  ninja-build \
  cppcheck \
  catch2 \
  fzf \
  git \
  htop \
  jq \
  tree \
  vim \
  zsh \
  autojump

# Unminimize to restore human-friendly tools that are missing from a minimal install
RUN yes | unminimize

# Generate locale
RUN locale-gen en_US.UTF-8

# Set zsh as default shell
RUN chsh -s /bin/zsh

# Use mirror to speed up
COPY pip/pip.conf /root/.pip/pip.conf

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
  && apt-get update && apt-get install -y \
  gcc-${GCC_VERSION} \
  g++-${GCC_VERSION} \
  libunwind-dev \
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
  && update-alternatives --install /usr/bin/clangd clangd /usr/bin/clangd-${LLVM_VERSION} ${LLVM_VERSION}00 \
  && update-alternatives --install /usr/bin/clang-format clang-format /usr/bin/clang-format-${LLVM_VERSION} ${LLVM_VERSION}00 \
  && update-alternatives --install /usr/bin/clang-tidy clang-tidy /usr/bin/clang-tidy-${LLVM_VERSION} ${LLVM_VERSION}00 \
  && update-alternatives --install /usr/bin/lldb lldb /usr/bin/lldb-${LLVM_VERSION} ${LLVM_VERSION}00
