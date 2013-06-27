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
#include "FPSEngine.h"

Object::Object( char* Name,  unsigned long ID )
{
	ID_ = ID;
	myName_ = Name;
	flags_ = 0;
}

Object::~Object()
{
}

// TODO: Find a better way to draw objects, currently all drawing code is done via main.cpp
void Object::DrawFrame()
{
}

void Object::Update()
{
}

// ----- ----- ----- Get Functions ----- ----- ----- //

Mesh* Object::GetMyMesh()
{
	return _myEngine->GetPrimativeManager()->GetElement( myMeshName_ );
}

unsigned long Object::GetID()
{
	return ID_;
}

char *Object::GetName()
{
	return myName_;
}

unsigned long Object::GetRefCount()
{
	return refCount;
}

D3DXMATRIX Object::GetmatFinal()
{
	return matFinal_;
}

D3DXMATRIX Object::GetmatRotateY()
{
	return matRotateY_;
}

D3DXMATRIX Object::GetmatRotateZ()
{
	return matRotateZ_;
}

D3DXMATRIX Object::GetmatRotateX()
{
	return matRotateX_;
}

D3DXMATRIX Object::GetmatTranslate()
{
	return matTranslate_;
}

D3DXMATRIX Object::GetmatView()
{
	return matView_;
}

D3DXMATRIX Object::GetmatProjection()
{
	return matProjection_;
}

D3DXMATRIX Object::GetmatScale()
{
	return matScale_;
}

WorldPos Object::GetWorldPos()
{
	return worldPos_;
}

unsigned long Object::GetFlag()
{
	return flags_;
}

// ----- ----- ----- Set Functions ----- ----- ----- //

void Object::IncRef()
{
	refCount++;
}

void Object::DecRef()
{
	refCount--;
}

void Object::SetMyMesh( char* meshName )
{
	myMeshName_ = meshName;
}

void Object::SetmatFinal( D3DXMATRIX tempMatrix )
{
	matFinal_ = tempMatrix;
}

void Object::SetmatRotateY( D3DXMATRIX tempMatrix )
{
	matRotateY_ = tempMatrix;
}

void Object::SetmatRotateZ( D3DXMATRIX tempMatrix )
{
	matRotateZ_ = tempMatrix;
}

void Object::SetmatRotateX( D3DXMATRIX tempMatrix )
{
	matRotateX_ = tempMatrix;
}

void Object::SetmatTranslate( D3DXMATRIX tempMatrix )
{
	matTranslate_ = tempMatrix;
}

void Object::SetmatView( D3DXMATRIX tempMatrix )
{
	matView_ = tempMatrix;
}

void Object::SetmatProjection( D3DXMATRIX tempMatrix )
{
	matProjection_ = tempMatrix;
}

void Object::SetmatScale( D3DXMATRIX tempMatrix )
{
	matScale_ = tempMatrix;
}

void Object::SetWorldPos( XMFLOAT3 newPos )
{
	worldPos_.Update( newPos );
}

void Object::UpdateWorldPosX( float x )
{
	XMFLOAT3 tempPos;
	tempPos.x = x;
	tempPos.y = worldPos_.newPos.y;
	tempPos.z = worldPos_.newPos.z;

	worldPos_.Update( tempPos );
}

void Object::UpdateWorldPosY( float y )
{
	XMFLOAT3 tempPos;
	tempPos.x = worldPos_.newPos.x;
	tempPos.y = y;
	tempPos.z = worldPos_.newPos.z;

	worldPos_.Update( tempPos );
}

void Object::UpdateWorldPosZ( float z )
{
	XMFLOAT3 tempPos;
	tempPos.x = worldPos_.newPos.x;
	tempPos.y = worldPos_.newPos.y;
	tempPos.z = z;

	worldPos_.Update( tempPos );
}

void Object::SetFlag( unsigned long flag )
{
	flags_ = flag;
}

// ----- ----- ----- ----- Object Manager ----- ----- ----- ----- // 

ObjectManager::ObjectManager()
{
	m_list = new LinkedList< Object >;
}

ObjectManager::~ObjectManager()
{
	SAFE_DELETE( m_list );
}

Object *ObjectManager::Add( char *name )
{
	// Ensure the list, the resource name, and its path are valid.
	if( m_list == NULL || name == NULL )
		return NULL;

	// If the element already exists, then return a pointer to it.
	Object *element = GetElement( name );
	if( element != NULL )
	{
		element->IncRef();
		return element;
	}

	// Create the resource, preferably through the application specific
	// function if it is available.
	Object *tempObject = NULL;
	tempObject = new Object( name, m_list->GetTotalElements() );

	// Add the new resource to the manager and return a pointer to it.
	return m_list->Add( tempObject );
}

void ObjectManager::Remove( Object **object )
{
	// Ensure the resource to be removed and the list is valid.
	if( *object == NULL || m_list == NULL )
		return;

	// Decrement the resource's reference count.
	(*object)->DecRef();

	// If the resource is no long being used then destroy it.
	if( (*object)->GetRefCount() == 0 )
		m_list->Remove( object );
}

void ObjectManager::EmptyList()
{
	if( m_list != NULL )
		m_list->Empty();
}

LinkedList< Object > *ObjectManager::GetList()
{
	return m_list;
}

Object *ObjectManager::GetElement( char *name )
{
	// Ensure the name and path are valid, and the list is valid and not empty.
	if( name == NULL )
		return NULL;

	if( m_list->GetFirst() == NULL )
		return NULL;

	// Iterate the list looking for the specified resource.
	m_list->Iterate( true );
	while( m_list->Iterate() )
		if( strcmp( m_list->GetCurrent()->GetName(), name ) == 0 )
			return m_list->GetCurrent();

	// Return NULL if the resource was not found.
	return NULL;
}

Object *ObjectManager::GetElement( unsigned long ID )
{
	// Ensure the name and path are valid, and the list is valid and not empty.
	if( m_list->GetFirst() == NULL )
		return NULL;

	// Iterate the list looking for the specified resource.
	m_list->Iterate( true );
	while( m_list->Iterate() )
		if(  m_list->GetCurrent()->GetID() == ID )
			return m_list->GetCurrent();

	// Return NULL if the resource was not found.
	return NULL;
}

void ObjectManager::Update()
{
	for ( int i = 0; i < GetList().GetTotalElements(); i++
	{
		GetElement( i ).Update();
	}
}