/*
/ Definitions of structs used in the Cubed Engine
/ Brandon McClure
*/
#ifndef _DEFINE_STRUCTS_
#define _DEFINE_STRUCTS_

struct EngineSetup
{
	HINSTANCE instance;
	char *name;
	float scale;
	unsigned char totalBackBuffers;
	void (*StateSetup)();

	EngineSetup()
	{
		instance = NULL;
		name = "Application";
		scale = 1.0f;
		totalBackBuffers = 1;
		StateSetup = NULL;
	}
};

struct ViewerSetup
{
	unsigned long viewClearFlags; // Flags used for clearing the view.

	ViewerSetup()
	{
		viewClearFlags = 0;
	};
};

struct VertexPos
{
    XMFLOAT3 pos;
    D3DXCOLOR Color;
};

// Constant buffers
struct CBPerFrame
{
	D3DXCOLOR Color;
	float X,Y,Z;
};

struct CBPerObject
{
};

//This struct is utilized for my objects
// The plan is for it to automatically keep track of the previous position 
// of the object via its Update function. On creation defaults to 0,0,0
struct WorldPos
{
	XMFLOAT3 newPos;
	XMFLOAT3 oldPos;

	WorldPos()
	{
		newPos.x = 0.0f;
		newPos.y = 0.0f;
		newPos.z = 0.0f;

		oldPos.x = 0.0f;
		oldPos.y = 0.0f;
		oldPos.z = 0.0f;
	};

	void WorldPos::Update( XMFLOAT3 pos )
	{
		oldPos = newPos;
		newPos = pos;
	};
};

struct CollisionData
{
	float scale;
	float elapsed;
	unsigned int frameStamp;

	//SceneObject *object;

	D3DXVECTOR3 translation;
	D3DXVECTOR3 velocity;
	D3DXVECTOR3 normilizedVelocity;

	D3DXVECTOR3 gravity;

	bool collisionFound;
	float distance;

	D3DXVECTOR3 intersection;
}

#endif