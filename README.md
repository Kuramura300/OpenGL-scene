# OpenGL-scene
A simple OpenGL scene demonstrating some shader techniques.

## Prerequisites
Building OpenGL-scene requires either [MinGW/MSYS2](https://msys2.org/) or [Visual Studio 2019](https://visualstudio.microsoft.com/downloads/) to be installed on your system.

The list of build dependencies for MinGW/MSYS2 is listed below.
```bash
pacman -S mingw-w64-x86_64-gcc make mingw-w64-x86_64-glfw mingw-w64-x86_64-glm
```

## Compilation
### MinGW/MSYS2
Once you've set up your build environment with all the necessary dependencies, open a MinGW/MSYS2 Terminal instance and run the following:
```bash
git clone --recursive git@github.com:Kuramura300/OpenGL-scene.git
cd OpenGL-scene/
make
make test # This will automatically run the built binary.
```
The built binaries, as well as all required dynamic libraries and data files will be copied to the `build/` directory.

### Visual Studio 2019
Clone the Git repository using the following invocation in a Terminal instance:
```bash
git clone --recursive git@github.com:Kuramura300/OpenGL-scene.git
```
Then, open `Project_Template.sln` and build the project.

The built binaries, as well as all required dynamic libraries and data files will be copied to the `x64/` directory.

## How does the program work?
### User Interaction
* The camera can be moved around using WASD
  * W moves the camera down
  * A moves the camera left
  * S moves the camera up
  * D moves the camera right
* The camera can be zoomed in and out
  * I moves the camera in
  * K moves the camera out
* Automatic camera rotation is toggled by pressing R
* Fog is toggled by pressing F
* Clouds are toggled by pressing C
* The application can be closed by pressing ESCAPE

### Shading
* Phong shading is applied to each object in the vertex shader using the phongModel() function
* Fog is calculated based on a minimum and maximum distance and used if it is enabled
* A skybox using a cubemap is added in the fragment shader
* Clouds are added to the skybox using a noise texture if it is enabled
