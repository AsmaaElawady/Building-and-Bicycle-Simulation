# Building and Bicycle Simulation Project

This project is a simple simulation using OpenGL that demonstrates a rotating bicycle and a building with interactive elements such as doors and windows. The code utilizes the FreeGLUT library to manage window creation, input handling, and rendering.

## Prerequisites
Before running the program, ensure you have the following installed on your system:
- GCC compiler
- FreeGLUT library
  
## Project Description
The project mainly consists of:
- A building of 2 floors, 2 windows per floor, door, and a roof
- A bicycle that rotates around the building with different radii in a circular road

## Building and Running the Project
- **Compile**: g++ main.cpp -lfreeglut -lopengl32 -lglu32 -lgdi32
- **Execute**: a.exe

## Features
### Camera Control
- **cameraX, cameraY, cameraZ**: Variables to control 3D scene
### Door Control
- **isOpen**: Boolean variable to control the state of the door (open or closed).
### Window Control
- **isOpenW**: Boolean to control the state of the windows (open or closed).
### Bicycle Control
- **isRotateBike**: Boolean to control the rotation of the bicycle.
- **angle**: Variable to store the rotation angle of the bicycle.
- **xChange**: Variable to store the horizontal translation of the bicycle.
- **leftWheelAngle, rightWheelAngle**: Variables to store the rotation angles of the bicycle wheels.
### Mouse Control
- **isLeftButtonPressed, isRightButtonPressed**: Booleans to track mouse button states.

## Interaction
- **Keyboard**:
  - `a`: Move camera left
  - `d`: Move camera right
  - `w`: Move camera up
  - `s`: Move camera down
  - `q`: Move camera forward
  - `e`: Move camera backward
  - `o`: Open door
  - `c`: Close door
  - `O`: Open windows
  - `C`: Close windows
  - `f`: Move bicycle through positive x-axis
  - `b`: Move bicycle through negative x-axis
  - `r`: Rotate front wheel
  - `l`: Rotate back wheel
- **Mouse**:
  - `Right click`: Stop rotating bicycle
  - `Left click`: Start rotating bicycle

