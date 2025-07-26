#!/bin/bash

set -e

# Display environment information
echo "======== Environment Information ========"
echo "OS: $(lsb_release -d -s | head -n1)"
echo "Clang: $(clang --version | head -n1)"
echo "Clang-Format: $(clang-format --version | head -n1)"
echo "Clang-Tidy: $(clang-tidy --version | head -n1)"
echo "GCC: $(gcc --version | head -n1)"
echo "Cppcheck: $(cppcheck --version | head -n1)"
echo "CMake: $(cmake --version | head -n1)"
echo "Ninja: $(ninja --version | head -n1)"
echo "Gersemi: $(gersemi --version | head -n1)"
echo "Catch2: $(dpkg -l | grep catch2 | head -n1 | awk '{print $3}')"
echo "Python: $(python3 --version | head -n1)"
echo "Pre-commit: $(pre-commit --version | head -n1)"
echo "Working directory: $(pwd)"
echo "========================================="
echo ""

# Verify compiler setup
echo "🔧 Verifying compiler configuration..."
echo "CC: $CC ($(which $CC))"
echo "CXX: $CXX ($(which $CXX))"
echo ""

# Install pre-commit hooks
echo "📦 Installing pre-commit hooks..."
pre-commit install
echo "✅ Pre-commit hooks installed!"
echo ""

# Test CMake configuration
echo "🏗️ Testing CMake configuration..."
cmake --list-presets=configure 2>/dev/null || echo "No configure presets found"
echo ""

if cmake --workflow --preset debug-workflow; then
  echo "✅ CMake configuration successful!"
else
  echo "⚠️ CMake configuration failed!"
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
