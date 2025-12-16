# CAD Viewer

Industrial-grade 3D CAD Scene Viewer built with C++/OpenGL/Qt.

## Requirements

### Build-Time Dependencies

| Dependency | Version | Notes |
|------------|---------|-------|
| CMake | 3.25+ | Build system generator |
| C++ Compiler | GCC 12+, Clang 15+, MSVC 2022 | Must support C++23 |
| Qt 6 SDK | 6.5+ | Components: Widgets, OpenGLWidgets |
| OpenGL | 4.1+ | Core Profile |
| Doxygen | 1.9+ | Optional, for documentation |

## Build & Run

```bash
# Configure
cmake --preset arm_darwin_generic_mach_o_64bit-Debug

# Build
cmake --build build/arm_darwin_generic_mach_o_64bit-Debug --parallel

# Run (macOS)
./build/arm_darwin_generic_mach_o_64bit-Debug/veidelmarov_technologies_cad_scene.app/Contents/MacOS/veidelmarov_technologies_cad_scene
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
> Welcom default 3D primitives scene 
![default 3D primitives scene](https://github.com/saintsonoff/cad_scene/blob/develop/readme_src/1.png)
> Add .stl 3D model to scene
![stl 3D model](https://github.com/saintsonoff/cad_scene/blob/develop/readme_src/2.png)
> Change models visability
![change visability](https://github.com/saintsonoff/cad_scene/blob/develop/readme_src/3.png)
> Rotate and Scale models
![rotating and scaling](https://github.com/saintsonoff/cad_scene/blob/develop/readme_src/4.png)
> Change models color
![change color](https://github.com/saintsonoff/cad_scene/blob/develop/readme_src/5.png)
> Ranging models Shininess
![change shininess](https://github.com/saintsonoff/cad_scene/blob/develop/readme_src/6.png)
> ![change shininess](https://github.com/saintsonoff/cad_scene/blob/develop/readme_src/7.png)

## Documentation

Generate Doxygen documentation:

```bash
doxygen Doxyfile
open docs/html/index.html
```

## License

GNU General Public License v3.0 (GPLv3)
