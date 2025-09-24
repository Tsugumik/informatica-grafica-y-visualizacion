# pr2b — OpenGL/GLUT practice application

A small C++14 application that opens an interactive OpenGL window using GLUT. It includes basic building blocks for a simple 3D scene: a camera, points, and scene/interaction utilities. The main program initializes the UI, registers callbacks, and starts the display loop.

This repository is configured with CMake and uses Conan to manage OpenGL-related dependencies (where applicable). A CLion CMake profile is present for local development.


## Stack
- Language: C++14
- Build system: CMake (minimum 3.25)
- Dependency manager: Conan 2
- Graphics: OpenGL, GLUT (FreeGLUT)
- IDE support: CLion (Debug profile provided)


## Project layout
```
.
├── CMakeLists.txt                # CMake configuration (target: pr2b)
├── conanfile.py                  # Conan application recipe (CMakeDeps + toolchain)
├── conandata.yml                 # Conan requirements (opengl-registry, opengl/system, freeglut)
├── src/
│   ├── cgvCamera.cpp/.h
│   ├── cgvInterface.cpp/.h
│   ├── cgvPoint.cpp/.h
│   ├── cgvScene3D.cpp/.h
│   └── pr2b.cpp                  # Entry point (main)
└── cmake-build-debug/            # CLion-generated build directory (Debug)
```

Entry point: src/pr2b.cpp (defines main, sets up window and starts the GLUT loop).

CMake target(s):
- pr2b (executable)


## Platform notes
CMakeLists.txt contains platform-specific linking logic:
- macOS (APPLE): links the system frameworks "OpenGL" and "GLUT" and defines GL_SILENCE_DEPRECATION.
- Linux (LINUX): finds OpenGL and GLUT via CMake packages (OpenGL, GLUT).
- Windows (WIN32): expects Conan-provided packages (opengl/system, opengl-registry, freeglut).

Conan requirements listed in conandata.yml:
- opengl-registry/cci.20220929
- opengl/system
- freeglut/3.4.0


## Requirements
- CMake >= 3.25
- A C++14 compiler (e.g., clang, GCC, or MSVC)
- Conan 2.x
- Platform-specific SDKs/toolchains able to build OpenGL/GLUT applications
  - macOS: Xcode command line tools; Apple OpenGL/GLUT frameworks available
  - Linux: dev packages for OpenGL and FreeGLUT (or use Conan to supply GLUT)
  - Windows: Visual Studio Build Tools; Conan for OpenGL registry and FreeGLUT


## Build and run
You can build either with command-line CMake/Conan or with CLion. The examples below assume a fresh, out-of-source build.

### Using CMake + Conan (command line)
1) Install dependencies with Conan and generate the toolchain files. One common pattern is:

- Option A (single build folder):
  - conan install . -of build -b missing
  - cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=build/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
  - cmake --build build --target pr2b --config Release

- Option B (Debug build):
  - conan install . -of build-debug -s build_type=Debug -b missing
  - cmake -S . -B build-debug -DCMAKE_TOOLCHAIN_FILE=build-debug/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Debug
  - cmake --build build-debug --target pr2b --config Debug

2) Run the executable:
- On macOS/Linux: ./build/pr2b (or ./build-debug/pr2b)
- On Windows: .\build\Debug\pr2b.exe (or corresponding configuration folder)

Note: If your platform already has system OpenGL/GLUT and CMake is configured for it (e.g., macOS), Conan may not be strictly necessary. However, the project provides Conan metadata to ease portability, especially on Windows.

### Using CLion
This repository already contains a CLion CMake profile folder:
- Profile: Debug
- Build directory: cmake-build-debug
- Target: pr2b

Open the project in CLion and use the Run/Debug configurations to build and run the pr2b target. The resulting binary is placed at cmake-build-debug/pr2b (on Windows, in a config-named subdirectory).


## Scripts and developer tasks
There are no custom shell scripts in this repo. Developer tasks are exposed via CMake targets:
- pr2b: build the application

Typical commands:
- cmake --build <build-dir> --target pr2b
- <build-dir>/pr2b (run)


## Configuration and environment variables
- No required environment variables are defined by the project at this time.
- TODO: Document any runtime options, command-line arguments, or environment toggles if/when they are added.


## Tests
No tests are present in this repository.
- TODO: Add unit or integration tests (e.g., using GoogleTest or Catch2) and document how to run them.


## Troubleshooting
- macOS: If you see deprecation warnings for OpenGL/GLUT, note that CMake defines GL_SILENCE_DEPRECATION to reduce noise. Ensure Command Line Tools/Xcode are installed.
- Linux: Make sure OpenGL and FreeGLUT development packages are available, or use Conan to provide FreeGLUT.
- Windows: Ensure Conan is installed and able to fetch opengl/system, opengl-registry, and freeglut; use a Developer Command Prompt for MSVC builds.


## License
No license file is present in the repository.
- TODO: Add a LICENSE file and update this section accordingly (e.g., MIT, Apache-2.0, GPL-3.0, etc.).
