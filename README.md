
# OBJViewer

OBJViewer is a simple application designed to load and display 3D object files. It uses modern OpenGL for rendering and provides a basic framework for handling input, shaders, and textures.

## Features

- Load and render 3D object files
- Basic input handling for object manipulation
- Shader management for customizable rendering effects
- Texture loading and application

## Dependencies

This project relies on the following libraries, which are managed using [vcpkg](https://github.com/microsoft/vcpkg):

- **OpenGL**: Core rendering API
- **GLEW**: OpenGL Extension Wrangler Library
- **GLFW**: Library for handling window and input events
- **GLM**: OpenGL Mathematics library
- **stb**: Header-only library for image loading
- **FreeGLUT**: Alternative to GLUT for managing window context and input

## Getting Started

### Prerequisites

- **CMake** 3.10 or higher
- **vcpkg**: Package manager for installing dependencies
- **Visual Studio 2019/2022** or a similar C++20 compliant compiler

### Installing Dependencies

1. **Clone the vcpkg repository**:

   ```bash
   git clone https://github.com/microsoft/vcpkg.git
   cd vcpkg
   ```

2. **Bootstrap vcpkg**:

   - **On Windows**:

     ```bash
     .\bootstrap-vcpkg.bat
     ```

   - **On Unix**:

     ```bash
     ./bootstrap-vcpkg.sh
     ```

3. **Install required packages**:

   ```bash
   .\vcpkg install glew:x64-windows glfw3:x64-windows glm:x64-windows stb:x64-windows freeglut:x64-windows
   ```

### Building the Project

1. **Clone the OBJViewer repository**:

   ```bash
   git clone https://github.com/zaedynk/OBJ-Viewer
   cd OBJViewer
   ```

2. **Create a build directory**:

   ```bash
   mkdir build
   cd build
   ```

3. **Configure the project with CMake**:

   ```bash
   cmake .. -DCMAKE_TOOLCHAIN_FILE=C:/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
   ```

   Replace `C:/path/to/vcpkg` with the actual path to your vcpkg installation.

4. **Build the project**:

   ```bash
   cmake --build .
   ```

### Running the Application

After building, navigate to the `bin` directory inside your build folder and run the `OBJViewer.exe`:

```bash
cd bin
OBJViewer.exe
```

### Project Structure

- `Main.cpp`: Entry point for the application
- `InputManager.cpp`: Handles user input
- `Shader.cpp`: Manages shader programs
- `Texture.cpp`: Manages texture loading and binding
- `CMakeLists.txt`: Build configuration file

### Contributing

Contributions are welcome! Please fork the repository and submit a pull request for any features or bug fixes you would like to add.

### License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

### Acknowledgments

This project uses open-source libraries provided by the community. Special thanks to the contributors of GLEW, GLFW, GLM, stb, and FreeGLUT.
