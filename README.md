# CAD Viewer

Industrial-grade 3D CAD Scene Viewer built with C++/OpenGL/QT.

## Requirements

- **C++23** compiler (GCC 13+, Clang 17+, MSVC 2022)
- **Qt 6.8+**
- **CMake 3.25+**
- **OpenGL 4.1+** Core Profile

## Build & Run

```bash
# Configure
cmake --preset arm_darwin_generic_mach_o_64bit-Debug

# Build
cmake --build build/arm_darwin_generic_mach_o_64bit-Debug --parallel

# Run (macOS)
./build/arm_darwin_generic_mach_o_64bit-Debug/veidemlmarov_technologies_cad_scene.app/Contents/MacOS/veidemlmarov_technologies_cad_scene
```

## Architecture

- **Modular Design** — Core, Scene, Commands, Renderer, IO, GUI
- **Qt Widgets + OpenGL** — Native cross-platform rendering
- **Command Pattern** — Full undo/redo support
- **Type-Safe** — `std::expected`, Concepts, strong typing

## Key Features

- **Scalable** — Efficient mesh caching, lazy loading
- **Thread-Safe Ready** — Atomic ID generation, RAII patterns
- **Strictly Typed** — Zero raw pointers in public API

## Usage Examples

*потом допилить*

## Documentation

Generate Doxygen documentation:

```bash
doxygen Doxyfile
open docs/html/index.html
```

## License

GNU General Public License v3.0 (GPLv3)
