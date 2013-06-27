/*
/ Mesh.h
/ Author: Brandon McClure

/ A custom class which contains all of the information needed to draw a mesh. Currently it is very
/ simplistic, but should scale well to recieve more data to draw more complex meshes. This also contains
/ a mesh manager which inherits from the resource manager.
/
/ TODO: Implement skinning, importing meshes from a file. Also, I am unsure if it is wise to store the index and vertex buffers
/		with the meshes. Part of my uncertainty is because I am unsure if there are more than one copies of the mesh in memory,
/		there shouldn't be because the objects recieve their mesh data via look up from the mesh manager. I dunno
/		Also need to implement more freedom over buffer controls ( MAYBE?)
/
*/
#ifndef _MESH_
#define _MESH_

class Mesh : public BoundingVolume, public Resource< Mesh >
{
	/*
	/ ----- ----- Functions ----- -----/
	*/
public:
	Mesh( char *name, VertexPos verticies[], int noOfVerticies, DWORD indicies[], int noOfIndicies, char *path = "./" );
	virtual ~Mesh();


	/*
	/ ----- ----- Get Functions ----- -----/
	*/
		// I am haveing issues returning the array so verticies and indecies are public
		VertexPos	GetMyVerticies();
		int			GetSizeOfVerticies();

		DWORD		GetMyIndicies();
		int			GetSizeOfIndicies();

	/*
	/ ----- ----- Set Functions----- -----/
	*/
			void SetMyVerticies( VertexPos verticies[] );
			void SetMyIndicies( DWORD indicies[] );
			void SetIndexBuffer();
			void SetVertexBuffer();
private:

	/*
	/ ----- ---- Variables ----- -----/
	*/
public:
	VertexPos myVerticies[50];
	//Learn how to keep track of the size better
	int sizeOfVerticies;

	DWORD myIndicies[50];
	int sizeOfIndicies;
	ID3D11Buffer *vertexBuffer_;
	ID3D11Buffer *indexBuffer_;
private:
};

class MeshManager : public ResourceManager<Mesh>
{
public:
	MeshManager();
	~MeshManager();
	Mesh* Add( char *name, VertexPos verticies[], int noOfVerticies, DWORD indicies[], int noOfIndicies );
};

#endif