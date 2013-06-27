/*
/ Object.cpp
/ Author: Brandon McClure

/ A class which contains all of the information needed for a generic game object
/ it will be inherited from later on to specify functionality for each object group
/ Also this file contains the code for the Object Manager 
/
/ TODO: The object manager could inherit from the resource manager, but first I need to simplify the 
/		resource manager, it is to complicated for being a base class
/
*/
#ifndef _OBJECT_
#define _OBJECT_

class Object
{
	/*
	/ ----- ----- Functions ----- -----/
	*/
public:

	Object( char* Name, unsigned long ID );
	virtual ~Object();

	void DrawFrame();

	void Update();

		/*
		/ ----- ----- Get Functions ----- -----/
		*/
			unsigned long		GetID();
			char 				*GetName();
			unsigned long		GetRefCount();

			D3DXMATRIX			GetmatFinal();
			D3DXMATRIX			GetmatRotateY();
			D3DXMATRIX			GetmatRotateZ();
			D3DXMATRIX			GetmatRotateX();
			D3DXMATRIX			GetmatTranslate();
			D3DXMATRIX			GetmatView();
			D3DXMATRIX			GetmatProjection();
			D3DXMATRIX			GetmatScale();

			WorldPos			GetWorldPos();

			Mesh				*GetMyMesh();

			unsigned long		GetFlag();
		/*
		/ ----- ----- Set Functions ----- -----/
		*/
			void				IncRef();
			void				DecRef();

			void				SetMyMesh( char* meshName );

			void				SetmatFinal( D3DXMATRIX tempMatrix );
			void				SetmatRotateY( D3DXMATRIX tempMatrix );
			void				SetmatRotateZ( D3DXMATRIX tempMatrix );
			void				SetmatRotateX( D3DXMATRIX tempMatrix );
			void				SetmatTranslate( D3DXMATRIX tempMatrix );
			void				SetmatView( D3DXMATRIX tempMatrix );
			void				SetmatProjection( D3DXMATRIX tempMatrix );
			void				SetmatScale( D3DXMATRIX tempMatrix );

			void				SetWorldPos( XMFLOAT3 newPos );
			void				UpdateWorldPosX( float x );
			void				UpdateWorldPosY( float y );
			void				UpdateWorldPosZ( float z );

			void				SetFlag( unsigned long flag );

private:

	/*
	/ ----- ----- Variables ----- -----/
	*/
public:
	// Matricies for the objects, matView and matProjection will ultimatly be set by a special class
	// of camera objects
	D3DXMATRIX matFinal_, matRotateX_, matRotateY_, matRotateZ_, matTranslate_, matView_, matProjection_, matScale_;
private:
	unsigned long ID_;
	char* myName_;
	char* myMeshName_;
	unsigned long refCount;
	unsigned long flags_;

	WorldPos worldPos_;

	//A flag wich tells the engine if the object has been updated ( moved, rotated, etc ) this frame
	bool Updated_;
};

class ObjectManager
{
	/*
	/ ----- ----- Function ----- -----/
	*/
public:
	ObjectManager();
	~ObjectManager();

	Object *Add( char *name );
	void Remove( Object **object );
	void EmptyList();
	LinkedList< Object > *GetList();
	Object* GetElement( char *name );
	Object* GetElement( unsigned long ID );

	void Update();
private:

	/*
	/ ----- ----- Variables ----- -----/
	*/
public:
private:
	LinkedList< Object > *m_list;
};

#endif