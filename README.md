# Snipper

**Snipper** is a powerful, cross-platform, command-line snippet manager built in C++.
It helps developers save, search, tag, edit, and manage reusable code snippets (currently using a cargo-style CLI).

## Features

- Add, remove, rename, and edit snippets
- Tag support and tag-based listing
- Full-text search
- Export and import to/from JSON
- Snippet ID normalization
- Clean CLI experience
- Cross-platform support (Windows, Linux, macOS)
- Built-in test suite using GoogleTest

### Adding Snippets (with multiline paste support)

Use the interactive `snipper add` command to add a new snippet.

```bash
snipper add
```

- You'll be prompted to enter ID, Title, Content, and Tags.
- For content, you can paste multiline code (like from VS Code, Sublime, etc.).
- After pasting, press:
- Ctrl+D on Linux/macOS
- Ctrl+Z then Enter on Windows
to finish content input.

Example:

```
Enter Content (paste multiple lines and press Ctrl+D (Linux/macOS) or Ctrl+Z then Enter (Windows) to finish):
for (int i = 0; i < 10; ++i) {
    std::cout << i << std::endl;
}
[Ctrl+D]->[press Enter afterwords]
```

## Getting Started

### Prerequisites

- CMake >= 3.14
- C++17 compatible compiler (MSVC, g++, clang++)
- Git
- Make or Ninja (Linux/macOS)
- PowerShell or CMD (Windows)

### Clone the repository

```bash
git clone https://github.com/Peeyush-04/snipper.git
cd snipper
```

### Build Instructions

#### Windows (PowerShell)

```powershell
mkdir -p build
cd build
cmake -G Ninja -DCMAKE_BUILD_TYPE=Release ..
ninja
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
snipper init                 # Initialize the database/config (optional) - creates backup-db if [name] is promted
# (do not use [name] currently - only for backup purposes, use init only)
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
[start from project root]
mkdir -p data
cd build
ctest --verbose
[OR]
ninja test (To do complete testing)
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
