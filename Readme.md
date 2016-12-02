
# README #

An STL Viewer written in C/C++
A very much work in progress! 
A binary is generated as well as a lib.
The binary is an example on how to use the lib. 

### QuickGuide ###

Do a out of source cmake build.
View an ASCII STL run: ` STLViewer with -i [file.stl] `
View a binary STL run: ` STLViewer with -i [file.stl] -b `

### Dependecies ###
	
* GLFW3
* GLM
* OpenGL
* GLEW
* A graphics card with OpenGL v3.3 support

### Usage ###

The executable is very plain but easy to extend. 
The following switches can/must be used:
	
* `-i [FILE]` 	open `[FILE]`
* `-b`  		for binary reading mode
* `-s [FILE]` 	save the loaded file as ASCII STL
* `-S [FILE]`	save the loaded file as Binray STL
* `-fe [FILE]`  save the loaded file as evolver fe file

### Exmaple ###

* `STLViewer -i data.stl -b -s data_ascii.stl ` Stores as ASCII.
* `STLViewer -i data.stl -S data_ascii.stl ` Stores as binary.


The window_Gl class responds to key and mouse events. 
Pressing `p` will change to polygon view
Pressing `i` will *invert* the lighting direction.
The `w,a,s,d` keys will move the camera.
The mouse wheel will move you forward and backward
and moving with left mouse button pressed wil rotate the camera
pressing the right button and moving will strafe.


It is possible to use the program as a ASCII to binary convert
and vice versa

### Known Issues ###

The Cmake file is a mess, the dependecie checks are horrible...
Have to sit down for that one day...

# License #
All Code and material Licensed on GPL v3.0
See `LICENSE`