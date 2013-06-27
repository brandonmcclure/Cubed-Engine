/*
/ Mesh.cpp
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
#include "FPSEngine.h"

Mesh::Mesh( char *name, VertexPos verticies[], int noOfVerticies, DWORD indicies[], int noOfIndicies,char *path ) : Resource< Mesh >( name, path )
{
	sizeOfVerticies = noOfVerticies;
	SetMyVerticies( verticies );

	sizeOfIndicies = noOfIndicies;
	SetMyIndicies( indicies );

	SetVertexBuffer();
	SetIndexBuffer();
}

Mesh::~Mesh()
{
	SAFE_RELEASE(indexBuffer_);
	SAFE_RELEASE(vertexBuffer_);
}

//Issues returning an array, return a pointer maybe?
/*
VertexPos Mesh::GetMyVerticies()
{
	return myVerticies[];
}
*/

int Mesh::GetSizeOfVerticies()
{
	return sizeOfVerticies;
}

/*
DWORD Mesh::GetMyIndicies()
{
	return myIndicies;
}
*/

int	Mesh::GetSizeOfIndicies()
{
	return sizeOfIndicies;
}



void Mesh::SetMyVerticies( VertexPos verticies[] )
{
	for ( int index = 0; index < sizeOfVerticies; index++ )
	{
		myVerticies[index] = verticies[index];
	}
}

void Mesh::SetMyIndicies( DWORD indicies[] )
{
	for ( int index = 0; index < sizeOfIndicies; index++ )
	{
		myIndicies[index] = indicies[index];
	}
}

void Mesh::SetIndexBuffer()
{
	// create the index buffer
		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(DWORD) * GetSizeOfIndicies();
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.MiscFlags = 0;

		_myEngine->GetD3DDevice()->CreateBuffer(&bd, NULL, &indexBuffer_);

		D3D11_MAPPED_SUBRESOURCE ms;
		_myEngine->GetD3DContext()->Map(indexBuffer_, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
		memcpy(ms.pData, myIndicies, sizeof(myIndicies));
		_myEngine->GetD3DContext()->Unmap(indexBuffer_, NULL);
}

void Mesh::SetVertexBuffer()
{
	D3D11_BUFFER_DESC vertexDesc;
	ZeroMemory( &vertexDesc, sizeof( vertexDesc ) );

	vertexDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexDesc.ByteWidth = sizeof( VertexPos ) * sizeOfVerticies;

	//Create subresource data for our verticies
	D3D11_SUBRESOURCE_DATA resourceData;
	ZeroMemory( &resourceData, sizeof( resourceData ) );

	resourceData.pSysMem = myVerticies;

		
	HRESULT myResult = _myEngine->GetD3DDevice()->CreateBuffer( &vertexDesc, &resourceData, &vertexBuffer_ );

	if (  FAILED( myResult ) )
	{
		#ifdef _DEBUG
			MessageBox(0,"Failed to create the Vertex Buffer!","Compile Error", MB_OK );
		#endif
	}
}

MeshManager::MeshManager()
{
}

MeshManager::~MeshManager()
{
}

Mesh* MeshManager::Add( char *name, VertexPos verticies[], int noOfVerticies, DWORD indicies[], int noOfIndicies )
{
	// Ensure the list, the resource name, and its path are valid.
	if( GetList() == NULL || name == NULL )
			return NULL;

		// If the element already exists, then return a pointer to it.
		Mesh *element = GetElement( name );
		if( element != NULL )
		{
			element->IncRef();
			return element;
		}

		// Create the resource, preferably through the application specific
		// function if it is available.
		Mesh *resource = NULL;
			resource = new Mesh( name, verticies, noOfVerticies, indicies, noOfIndicies );

		// Add the new resource to the manager and return a pointer to it.
		return GetList()->Add( resource );
}
