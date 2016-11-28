
# README #

An STL Viewer written in C/C++
A very much work in progress! 
A binary is generated as well as a lib.
The binary is an example on how to use the lib. 

### Quick Guide ### 

Do a out of source cmake build.
For ASCII STL run: ` STLViewer with -i [file.stl] `
For binary STL run: ` STLViewer with -i [file.stl] -b `

### Dependecies ###
	
	* GLFW3
	* GLM
	* OpenGL
	* GLEW
	* A graphics card with OpenGL v3.3 support

### Usage ###

The executable is very plain but easy to extend. 
The following switches can/must be used:
	
	* ` -i [FILE] ` to specify which file to open
	* ` -b ` to specify for binary reading mode
	* ` -s [FILE] ` to save the loaded file as ASCII STL
	* ` -S [FILE ` to save the loaded file as Binray STL

It is possible to use the program as a ASCII to binary convert
and vice versa

### Known Issues ###

The Cmake file is a mess, the dependecie checks are horrible...
Have to sit down for that one day...