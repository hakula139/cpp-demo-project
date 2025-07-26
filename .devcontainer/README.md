# Development Container Configuration

This directory contains the configuration for a VS Code development container that provides a complete
C++23 development environment based on Ubuntu 24.04 with LLVM 20 / GCC 14.

## 🚀 Features

### Core Environment

| Component | Version | Description                            |
| --------- | ------- | -------------------------------------- |
| Ubuntu    | 24.04   | Latest stable Ubuntu release           |
| LLVM      | 20.1    | Latest LLVM / Clang with C++23 support |
| GCC       | 14.2    | Latest GCC compiler with C++23 support |
| CMake     | 4.0     | Modern build system                    |
| Ninja     | 1.13    | Fast build system backend              |
| Catch2    | 3.9     | C++ testing framework                  |

### Development Tools

| Component    | Version | Description                 |
| ------------ | ------- | --------------------------- |
| clang-format | 20.1    | Code formatting             |
| clang-tidy   | 20.1    | Static analysis and linting |
| cppcheck     | 2.13    | Additional static analysis  |
| pre-commit   | 4.2     | Git hooks for code quality  |
| gersemi      | 0.21    | CMake formatter             |

### VS Code Extensions

| Name                            | Extension ID                            | Description                          |
| ------------------------------- | --------------------------------------- | ------------------------------------ |
| Kylin Clangd                    | `KylinIdeTeam.kylin-clangd`             | C/C++ Language Server Protocol (LSP) |
| CodeLLDB                        | `vadimcn.vscode-lldb`                   | C/C++ Debugger                       |
| CMake Intellisence              | `KylinIdeTeam.cmake-intellisence`       | CMake format, completion, etc.       |
| CMake Tools                     | `KylinIdeTeam.kylin-cmake-tools`        | CMake support                        |
| Doxygen Documentation Generator | `cschlosser.doxdocgen`                  | Doxygen documentation generation     |
| Markdown All in One             | `yzhang.markdown-all-in-one`            | Markdown formatting                  |
| Markdown Preview Enhanced       | `shd101wyy.markdown-preview-enhanced`   | Markdown preview                     |
| markdownlint                    | `DavidAnson.vscode-markdownlint`        | Markdown linting                     |
| GitLens                         | `eamodio.gitlens`                       | Git integration                      |
| Error Lens                      | `usernamehw.errorlens`                  | Error highlighting                   |
| Code Spell Checker              | `streetsidesoftware.code-spell-checker` | Spell checking                       |

## 🛠️ Setup & Usage

### Prerequisites

- **Docker** installed and running
- **VS Code** with the **Dev Containers** extension

### Quick Start

1. **Open the project in VS Code**

2. **Open in Dev Container**
   - VS Code will detect the devcontainer configuration
   - Click `Reopen in Container` when prompted
   - Or use Command Palette: `Dev Containers: Reopen in Container`

3. **Wait for setup** (it may take a while for first build)
   - Docker builds the container image
   - Post-create script sets up the environment
   - Recommended VS Code extensions are installed

4. **Test the setup** once the container is ready

   ```bash
   # Run the complete debug workflow
   cmake --workflow --preset debug-workflow
   ```

## 🔍 Troubleshooting

### Container Build Issues

If the container fails to build:

1. **Check Docker**: Ensure Docker is running and has sufficient resources

   ```bash
   docker ps -a                # List all containers
   docker logs <container-id>  # View Docker build logs for specific errors
   ```

2. **Clean rebuild**: Use Command Palette → `Dev Containers: Rebuild Without Cache and Reopen in Container`

### Compilation Issues

If C++23 features don't work:

1. **Verify compiler**: Ensure the compiler is C++23 compatible

   ```bash
   clang++ --version  # Should show version 20
   g++ --version      # Should show version 14
   ```

2. **Check standard**: Use `-std=c++23` flag explicitly

3. **Library support**: Some C++23 features may not be supported by the compiler yet
   - Check [C++23 compiler support](https://en.cppreference.com/w/cpp/23.html) for more information

### VS Code Integration

If IntelliSense isn't working:

1. **Check `compile_commands.json`**: Verify that the file is present in the build directory and up to date
   - If not, configure the project to generate it:

     ```bash
     cmake --preset debug
     ```

2. **Restart language server**: Command Palette → `Kylin Clangd: Restart the clangd language server`

3. **Reload window**: Command Palette → `Developer: Reload Window`
