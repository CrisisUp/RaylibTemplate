# Use an official Ubuntu as a parent image
FROM mcr.microsoft.com/devcontainers/cpp:1-ubuntu-22.04

# Install Raylib dependencies and other useful tools
RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
    && apt-get -y install --no-install-recommends \
    build-essential \
    cmake \
    git \
    python3 \
    python3-pip \
    libasound2-dev \
    libx11-dev \
    libxrandr-dev \
    libxi-dev \
    libgl1-mesa-dev \
    libglu1-mesa-dev \
    libxcursor-dev \
    libxinerama-dev \
    libwayland-dev \
    libxkbcommon-dev \
    xorg-dev \
    && apt-get autoremove -y && apt-get clean -y && rm -rf /var/lib/apt/lists/*

# Install Python dependencies for the project scripts
RUN pip3 install Pillow
