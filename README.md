# Snipper

**Snipper** is a powerful, cross-platform, command-line snippet manager built in C++.
It helps developers save, search, tag, edit, and manage reusable code snippets using a cargo-style CLI.

## Features

- Add, remove, rename, and edit snippets
- Tag support and tag-based listing
- Full-text search
- Export and import to/from JSON
- Snippet ID normalization
- Clean CLI experience
- Cross-platform support (Windows, Linux, macOS)
- Built-in test suite using GoogleTest

## Getting Started

### Prerequisites

- CMake >= 3.14
- C++17 compatible compiler (MSVC, g++, clang++)
- Git
- Make or Ninja (Linux/macOS)
- PowerShell or CMD (Windows)

### Clone the repository

```bash
git clone https://github.com/yourusername/snipper.git
cd snipper
```

### Build Instructions

#### Windows (PowerShell)

```powershell
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Release
```

#### Linux / macOS

```bash
mkdir build
cd build
cmake ..
make -j$(nproc)
```

### Auto Path Setup

After successful build, the CLI binary (`snipper-cli` or `snipper-cli.exe`) is moved to the `scripts/` directory, which is automatically added to your system `PATH`.

You can now use `snipper` globally from any terminal session.

If it does not work immediately, restart your terminal or VS Code.

## Usage

```bash
snipper init                 # Initialize the database/config (optional)
snipper add                  # Add a new snippet
snipper list                 # List all snippets
snipper show <id>            # Show a snippet by ID
snipper remove <id>          # Remove snippet by ID
snipper rename <id> <title>  # Rename snippet title
snipper edit <id>            # Edit a snippet in-place
snipper search <query>       # Search snippets (by content/tags)
snipper list-by-tags <tag>   # List snippets by tag
snipper export <file>        # Export to JSON file
snipper import <file>        # Import from JSON file
snipper stats                # Show usage statistics
snipper clear                # Clear all snippets (use with caution)
```

## Testing

Snipper uses [GoogleTest](https://github.com/google/googletest) for unit testing.

To build and run tests:

```bash
cd build
ctest --verbose
```

Or run specific test executables inside `build/bin` directory:

```bash
./build/bin/test_snipper_app
```

## Directory Structure

```
snipper/
├── include/           # Public headers
│   ├── app/
│   ├── core/
│   ├── models/
│   ├── platform/
│   ├── search/
│   └── external/json.hpp
├── src/               # Implementation files
│   ├── app/
│   ├── core/
│   ├── cli/
│   ├── models/
│   ├── platform/
│   └── search/
├── config/            # Default config JSON
├── data/              # Snippets database
├── tests/             # GoogleTest-based unit tests
├── scripts/           # CLI binary is output here
├── CMakeLists.txt     # CMake build file
└── README.md          # You're here!
```

## Compatibility

Snipper is tested and works on:

- Windows 10/11 (PowerShell, CMD)
- Ubuntu Linux (bash, zsh)
- macOS (zsh)

The CLI binary works globally once built, and the scripts directory is included in PATH automatically during build.

## License

MIT License. See [LICENSE](LICENSE) for details.

---

**Happy Snipping!**