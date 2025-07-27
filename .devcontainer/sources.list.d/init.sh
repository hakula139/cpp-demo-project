#!/bin/bash

SCRIPT_DIR=$(dirname "$0")
DST_PATH="/etc/apt/sources.list.d/ubuntu.sources"
ARCH=$(dpkg --print-architecture)

if [[ "$ARCH" = "amd64" ]]; then
  cp "$SCRIPT_DIR/ubuntu.sources" "$DST_PATH"
else
  cp "$SCRIPT_DIR/ubuntu-ports.sources" "$DST_PATH"
fi
