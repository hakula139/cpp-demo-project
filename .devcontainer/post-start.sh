#!/bin/bash

set -e

# Display environment information
echo "======== Environment Information ========"
echo "OS: $(lsb_release -d -s | head -n1)"
if command -v clang &> /dev/null; then
  echo "Clang: $(clang --version | head -n1)"
else
  echo "Clang: not installed"
fi
if command -v clang-format &> /dev/null; then
  echo "Clang-Format: $(clang-format --version | head -n1)"
else
  echo "Clang-Format: not installed"
fi
if command -v clang-tidy &> /dev/null; then
  echo "Clang-Tidy: $(clang-tidy --version | head -n1)"
else
  echo "Clang-Tidy: not installed"
fi
if command -v gcc &> /dev/null; then
  echo "GCC: $(gcc --version | head -n1)"
else
  echo "GCC: not installed"
fi
if command -v cppcheck &> /dev/null; then
  echo "Cppcheck: $(cppcheck --version | head -n1)"
else
  echo "Cppcheck: not installed"
fi
if command -v cmake &> /dev/null; then
  echo "CMake: $(cmake --version | head -n1)"
else
  echo "CMake: not installed"
fi
if command -v ninja &> /dev/null; then
  echo "Ninja: $(ninja --version | head -n1)"
else
  echo "Ninja: not installed"
fi
if command -v gersemi &> /dev/null; then
  echo "Gersemi: $(gersemi --version | head -n1)"
else
  echo "Gersemi: not installed"
fi
if dpkg -l | grep -q catch2 &> /dev/null; then
  echo "Catch2: $(dpkg -l | grep catch2 | head -n1 | awk '{print $3}')"
else
  echo "Catch2: not installed"
fi
if command -v python3 &> /dev/null; then
  echo "Python: $(python3 --version | head -n1)"
else
  echo "Python: not installed"
fi
if command -v pre-commit &> /dev/null; then
  echo "Pre-commit: $(pre-commit --version | head -n1)"
else
  echo "Pre-commit: not installed"
fi
echo "Working directory: $(pwd)"
echo "========================================="
echo ""

# Verify compiler setup
echo "🛠 Verifying compiler configuration..."
echo "CC: $CC ($(which $CC))"
echo "CXX: $CXX ($(which $CXX))"
echo ""

# Install pre-commit hooks
if command -v pre-commit &> /dev/null; then
  echo "📦 Installing pre-commit hooks..."
  pre-commit install
  echo "✅ Pre-commit hooks installed!"
else
  echo "🚨 Pre-commit not installed! Please install it manually."
fi
echo ""

# Test CMake configuration
if command -v cmake &> /dev/null; then
  echo "🔍 Testing CMake configuration..."
  cmake --list-presets=configure 2>/dev/null || echo "No configure presets found"
  echo ""

  if cmake --workflow --preset debug-workflow; then
    echo "✅ CMake configuration successful!"
  else
    echo "🚨 CMake configuration failed!"
  fi
else
  echo "🚨 CMake not installed! Please install it manually."
fi
echo ""

# Print final message
echo "🎯 Dev environment setup complete!"
echo ""
echo "📖 Quick start commands:"
echo "  • Check available presets:  cmake --list-presets=configure"
echo "  • Configure project:        cmake --preset <preset>"
echo "  • Build project:            cmake --build --preset <preset>"
echo "  • Run tests:                ctest --preset <preset>"
echo "  • Format code:              pre-commit run --all-files"
echo ""
echo "Happy coding! 🚀✨"
