Cubed-Engine
============

This is a pet project of a 3D game engine using DirectX 11, C++, and a Lua scripting engine

TODO:

3/19/2012
ObjectManager class 
  -needs to inheret from resource manager
	(done)-Needs to make mesh identifier optional
	-Implement the DrawFrame() with more control over the process.
	-Implement collision flag
ResourceManagement class
	-Simplify? Allow for greater modularity for different types of resources ( files, meshes, objects )
Mesh Class
	-Manage the size of the verticies and indecies buffer automatically
	-Implement Get functions for verticies and indicies arrays
Constant Buffer
	-Allow for the color of an object to be updated after creation
General Goals
	-Encapsulate area in a room
	-Implement Collision between objects which allow it
	-2nd Cube is AI controlled, moves away from the player
	(Done)-Eliminate RGBAColor, find a better way to pass color data around. XMFLOAT4?
