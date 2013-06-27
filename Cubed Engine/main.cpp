#include <Windows.h>

#include "FPSEngine.h"

#define TEST_STATE 1


class TestState : public State
{
	virtual void Load()
	{

		_myEngine->GetScriptManager()->Add( "myScript", "luaFile.lua" );

		//Adds objects, updates their world position
		// ----- ----- Camera ----- ----- //
			_myEngine->GetObjectManager()->Add( "Camera1" );
				UpdateMove.x = 5.0f;
				UpdateMove.y = 10.0f;
				UpdateMove.z = 0.0f;
				_myEngine->GetObjectManager()->GetElement( "Camera1" )->SetWorldPos( UpdateMove );
			_myEngine->GetObjectManager()->GetElement( "Camera1" )->SetFlag( 0 );

		// ----- ----- Cubes ------ ----- //
			_myEngine->GetObjectManager()->Add( "Cube1" );
			_myEngine->GetObjectManager()->GetElement( "Cube1" )->SetMyMesh( "Cube" );
				UpdateMove.x = 5.0f;
				UpdateMove.y = 0.0f;
				UpdateMove.z = 0.0f;
			_myEngine->GetObjectManager()->GetElement( "Cube1" )->SetWorldPos( UpdateMove );
			_myEngine->GetObjectManager()->GetElement( "Cube1" )->SetFlag( 1 );

			_myEngine->GetObjectManager()->Add( "Cube2" );
			_myEngine->GetObjectManager()->GetElement( "Cube2" )->SetMyMesh( "Cube" );
				UpdateMove.x = -5.0f;
				UpdateMove.y = 0.0f;
				UpdateMove.z = 0.0f;
			_myEngine->GetObjectManager()->GetElement( "Cube2" )->SetWorldPos( UpdateMove );
			_myEngine->GetObjectManager()->GetElement( "Cube2" )->SetFlag( 1 );

		// ----- ----- World Box ----- ----- //
			_myEngine->GetObjectManager()->Add( "Floor" );
			_myEngine->GetObjectManager()->GetElement( "Floor" )->SetMyMesh( "Plane" );
				UpdateMove.x = 0.0f;
				UpdateMove.y = -1.0f;
				UpdateMove.z = 0.0f;
				_myEngine->GetObjectManager()->GetElement( "Floor" )->SetWorldPos( UpdateMove );
				_myEngine->GetObjectManager()->GetElement( "Floor" )->SetFlag( 2 );

	};

	virtual void RequestViewer( ViewerSetup *viewer )
	{
	};

	virtual void Update( float elapsed )
	{
		// Check if the user wants to exit.
		if( _myEngine->GetInput()->GetKeyPress( DIK_Q ) )
			PostQuitMessage( 0 );

		// The Keys "a" and "s" read from luafile.lua and allow greater control of the camera dureing runtime
		if (  _myEngine->GetInput()->GetKeyPress( DIK_A ) )
		{
			CameraPos = _myEngine->GetScriptManager()->GetElement( "myScript", "luaFile.lua" )->GetVectorData("CameraPosDefault");
			_myEngine->GetObjectManager()->GetElement( "Camera1" )->SetWorldPos( CameraPos );
		}
		if (  _myEngine->GetInput()->GetKeyPress( DIK_S ) )
		{
			CameraPos = _myEngine->GetScriptManager()->GetElement( "myScript", "luaFile.lua" )->GetVectorData("CameraPosUpdate");
			_myEngine->GetObjectManager()->GetElement( "Camera1" )->SetWorldPos( CameraPos );
		}
		// I used this script for tweaking the floor rotation to get it right, ended up being an issue with other matricies, not the rotation
		if (  _myEngine->GetInput()->GetKeyPress( DIK_Z ) )
		{
			FloorRotation = _myEngine->GetScriptManager()->GetElement( "myScript", "luaFile.lua" )->GetVectorData("FloorRotation");
		}
		/*
		/
		/ ----- ------ ----- ------ ----- Arrow Keys ------ ----- ------ ----- ------ /
		/ Checks for user input and updates the world position for the player's object /
		/
		*/
			// ----- ------ Left ----- ----- //
			if ( _myEngine->GetInput()->GetKeyPress( DIK_LEFT ) )
			{
				UpdateMove.x = _myEngine->GetObjectManager()->GetElement( "Cube1" )->GetWorldPos().newPos.x + 0.5f;
				UpdateMove.y = _myEngine->GetObjectManager()->GetElement( "Cube1" )->GetWorldPos().newPos.y;
				UpdateMove.z = _myEngine->GetObjectManager()->GetElement( "Cube1" )->GetWorldPos().newPos.z;

				_myEngine->GetObjectManager()->GetElement( "Cube1" )->SetWorldPos( UpdateMove );

				UpdateMove.x = 0.0f;
				UpdateMove.y = 0.0f;
				UpdateMove.z = 0.0f;
			}
			// ----- ------ Right ----- ----- //
			if ( _myEngine->GetInput()->GetKeyPress( DIK_RIGHT ) )
			{
				UpdateMove.x = _myEngine->GetObjectManager()->GetElement( "Cube1" )->GetWorldPos().newPos.x - 0.5f;
				UpdateMove.y = _myEngine->GetObjectManager()->GetElement( "Cube1" )->GetWorldPos().newPos.y;
				UpdateMove.z = _myEngine->GetObjectManager()->GetElement( "Cube1" )->GetWorldPos().newPos.z;

				_myEngine->GetObjectManager()->GetElement( "Cube1" )->SetWorldPos( UpdateMove );

				UpdateMove.x = 0.0f;
				UpdateMove.y = 0.0f;
				UpdateMove.z = 0.0f;
			}
			// ----- ------ Up ----- ----- //
			if ( _myEngine->GetInput()->GetKeyPress( DIK_UP ) )
			{
				UpdateMove.x = _myEngine->GetObjectManager()->GetElement( "Cube1" )->GetWorldPos().newPos.x;
				UpdateMove.y = _myEngine->GetObjectManager()->GetElement( "Cube1" )->GetWorldPos().newPos.y + 0.5f;
				UpdateMove.z = _myEngine->GetObjectManager()->GetElement( "Cube1" )->GetWorldPos().newPos.z;

				_myEngine->GetObjectManager()->GetElement( "Cube1" )->SetWorldPos( UpdateMove );

				UpdateMove.x = 0.0f;
				UpdateMove.y = 0.0f;
				UpdateMove.z = 0.0f;
			}
			// ----- ------ Down ----- ----- //
			if ( _myEngine->GetInput()->GetKeyPress( DIK_DOWN ) )
			{
				UpdateMove.x = _myEngine->GetObjectManager()->GetElement( "Cube1" )->GetWorldPos().newPos.x;
				UpdateMove.y = _myEngine->GetObjectManager()->GetElement( "Cube1" )->GetWorldPos().newPos.y - 0.5f;
				UpdateMove.z = _myEngine->GetObjectManager()->GetElement( "Cube1" )->GetWorldPos().newPos.z;

				_myEngine->GetObjectManager()->GetElement( "Cube1" )->SetWorldPos( UpdateMove );

				UpdateMove.x = 0.0f;
				UpdateMove.y = 0.0f;
				UpdateMove.z = 0.0f;
			}
		/*
		/
		/ ----- ------ ----- ------ ----- Numpad Arrow Keys ------ ----- ------ ----- ------ /
		/ Updates the camera position /
		/
		*/
			if ( _myEngine->GetInput()->GetKeyPress( DIK_NUMPAD8 ))
			{
				float y = _myEngine->GetObjectManager()->GetElement( "Camera1" )->GetWorldPos().newPos.y + 0.5f;

				_myEngine->GetObjectManager()->GetElement( "Camera1" )->UpdateWorldPosY( y );
			}
			if ( _myEngine->GetInput()->GetKeyPress( DIK_NUMPAD2 ))
			{
				float y = _myEngine->GetObjectManager()->GetElement( "Camera1" )->GetWorldPos().newPos.y - 0.5f;

				_myEngine->GetObjectManager()->GetElement( "Camera1" )->UpdateWorldPosY( y );
			}
				if ( _myEngine->GetInput()->GetKeyPress( DIK_NUMPAD6 ))
			{
				float x = _myEngine->GetObjectManager()->GetElement( "Camera1" )->GetWorldPos().newPos.x + 0.5f;

				_myEngine->GetObjectManager()->GetElement( "Camera1" )->UpdateWorldPosX( x );
			}
			if ( _myEngine->GetInput()->GetKeyPress( DIK_NUMPAD4 ))
			{
				float x = _myEngine->GetObjectManager()->GetElement( "Camera1" )->GetWorldPos().newPos.x - 0.5f;

				_myEngine->GetObjectManager()->GetElement( "Camera1" )->UpdateWorldPosX( x );
			}
			if ( _myEngine->GetInput()->GetKeyPress( DIK_NUMPAD7 ))
			{
				float z = _myEngine->GetObjectManager()->GetElement( "Camera1" )->GetWorldPos().newPos.z + 0.5f;

				_myEngine->GetObjectManager()->GetElement( "Camera1" )->UpdateWorldPosZ( z );
			}
			if ( _myEngine->GetInput()->GetKeyPress( DIK_NUMPAD1 ))
			{
				float z = _myEngine->GetObjectManager()->GetElement( "Camera1" )->GetWorldPos().newPos.z - 0.5f;

				_myEngine->GetObjectManager()->GetElement( "Camera1" )->UpdateWorldPosZ( z );
			}

		//----- ----- ----- ----- ----- ----- Matricies ----- ----- ----- ----- ----- -----//

		// ----- Camera matricies ----- //
		// The camera stays focused on cube1, the position of the camera object is an offset from cube1
		D3DXMATRIX matView, matProjection;  

		XMFLOAT3 cubePos = _myEngine->GetObjectManager()->GetElement( "Cube1" )->GetWorldPos().newPos;
		XMFLOAT3 cameraPos = _myEngine->GetObjectManager()->GetElement( "Camera1" )->GetWorldPos().newPos;

		D3DXMatrixLookAtLH(&matView,
                       &D3DXVECTOR3( cubePos.x + cameraPos.x, cubePos.y + cameraPos.y, cubePos.z + cameraPos.z ),    // the camera position
					   &D3DXVECTOR3( cubePos.x, cubePos.y, cubePos.z ),    // the look-at position
                       &D3DXVECTOR3(0.0f, 1.0f, 0.0f));   // the up direction

		D3DXMatrixPerspectiveFovLH(&matProjection,
                               (FLOAT)D3DXToRadian(45),                    // field of view
							   (FLOAT)_myEngine->SCREEN_WIDTH() / (FLOAT)_myEngine->SCREEN_HEIGHT(), // aspect ratio
                               1.0f,                                       // near view-plane
                               100.0f);                                    // far view-plane

		// Loops through all objects, applies matricies as needed per object type.
		for ( unsigned long index = 0; index < _myEngine->GetObjectManager()->GetList()->GetTotalElements(); index++ )
		{
			D3DXMATRIX matRotateX, matRotateY, matRotateZ, matTranslate, matScale;
			for ( unsigned long num = 0; num < _myEngine->GetObjectManager()->GetList()->GetTotalElements(); num++ )
			{
				if ( index == num )
					continue;
				if ( _myEngine->GetObjectManager()->GetElement( index )->GetFlag() == 0 || _myEngine->GetObjectManager()->GetElement( num )->GetFlag() == 0 ) 
					continue;

				CheckCollision( _myEngine->GetObjectManager()->GetElement(index), _myEngine->GetObjectManager()->GetElement(num) );
			}

			//Camera
			if ( _myEngine->GetObjectManager()->GetElement( index )->GetFlag() == 0 )
			{
				_myEngine->GetObjectManager()->GetElement( index )->SetmatView( matView);
				_myEngine->GetObjectManager()->GetElement( index )->SetmatProjection( matProjection );
				D3DXMatrixTranslation(&_myEngine->GetObjectManager()->GetElement(index)->matTranslate_, _myEngine->GetObjectManager()->GetElement( index )->GetWorldPos().newPos.x, 
												_myEngine->GetObjectManager()->GetElement( index )->GetWorldPos().newPos.y,
												_myEngine->GetObjectManager()->GetElement( index )->GetWorldPos().newPos.z );

				_myEngine->GetObjectManager()->GetElement( index )->SetmatFinal( _myEngine->GetObjectManager()->GetElement( index )->GetmatTranslate() *
																				_myEngine->GetObjectManager()->GetElement( index )->GetmatView() *
																				_myEngine->GetObjectManager()->GetElement( index )->GetmatProjection());
			}
			// Cubes
			else if ( _myEngine->GetObjectManager()->GetElement( index )->GetFlag() == 1 )
			{
				_myEngine->GetObjectManager()->GetElement( index )->SetmatView( matView);
				_myEngine->GetObjectManager()->GetElement( index )->SetmatProjection( matProjection );
				D3DXMatrixTranslation(&_myEngine->GetObjectManager()->GetElement(index)->matTranslate_, _myEngine->GetObjectManager()->GetElement( index )->GetWorldPos().newPos.x, 
												_myEngine->GetObjectManager()->GetElement( index )->GetWorldPos().newPos.y,
												_myEngine->GetObjectManager()->GetElement( index )->GetWorldPos().newPos.z );

				_myEngine->GetObjectManager()->GetElement( index )->SetmatFinal( _myEngine->GetObjectManager()->GetElement( index )->GetmatTranslate() *
																			_myEngine->GetObjectManager()->GetElement( index )->GetmatView() *
																			_myEngine->GetObjectManager()->GetElement( index )->GetmatProjection());
			}
			// Floor
			else if ( _myEngine->GetObjectManager()->GetElement( index )->GetFlag() == 2 )
			{
				D3DXMatrixRotationX(&matRotateX, D3DXToRadian( 90 ) );
				_myEngine->GetObjectManager()->GetElement( index )->SetmatRotateX( matRotateX );

				D3DXMatrixScaling(&matScale, 50.0f,50.0f,50.0f );
				_myEngine->GetObjectManager()->GetElement(index)->SetmatScale( matScale );

				_myEngine->GetObjectManager()->GetElement( index )->SetmatView( matView);
				_myEngine->GetObjectManager()->GetElement( index )->SetmatProjection( matProjection );
				D3DXMatrixTranslation(&_myEngine->GetObjectManager()->GetElement(index)->matTranslate_, _myEngine->GetObjectManager()->GetElement( index )->GetWorldPos().newPos.x, 
												_myEngine->GetObjectManager()->GetElement( index )->GetWorldPos().newPos.y,
												_myEngine->GetObjectManager()->GetElement( index )->GetWorldPos().newPos.z );

				_myEngine->GetObjectManager()->GetElement( index )->SetmatFinal( _myEngine->GetObjectManager()->GetElement( index )->GetmatRotateX() *
																				_myEngine->GetObjectManager()->GetElement( index )->GetmatScale() *
																			_myEngine->GetObjectManager()->GetElement( index )->GetmatTranslate() *
																			_myEngine->GetObjectManager()->GetElement( index )->GetmatView() *
																			_myEngine->GetObjectManager()->GetElement( index )->GetmatProjection());
			}
		}


		Render();
	};

	virtual void Render()
	{
		// Leftovers from script example where I changed to background color
		float temp[4];

		temp[0] = _myEngine->backgroundColor.r;
		temp[1] = _myEngine->backgroundColor.g;
		temp[2] = _myEngine->backgroundColor.b;
		temp[3] = _myEngine->backgroundColor.a;

		_myEngine->GetD3DContext()->ClearRenderTargetView( _myEngine->GetBackBufferTarget(), temp );

		   // clear the depth buffer
		_myEngine->GetD3DContext()->ClearDepthStencilView(_myEngine->GetZBuffer(), D3D11_CLEAR_DEPTH, 1.0f, 0);


		//Draws Each Object in the manager
		for ( unsigned long index = 1; index < _myEngine->GetObjectManager()->GetList()->GetTotalElements(); index++ )
		{
			unsigned int stride = sizeof( VertexPos );
			unsigned int offset = 0;
			_myEngine->GetD3DContext()->IASetVertexBuffers( 0, 1, &_myEngine->GetObjectManager()->GetElement(index)->GetMyMesh()->vertexBuffer_, &stride, &offset );

			_myEngine->GetD3DContext()->IASetIndexBuffer(_myEngine->GetObjectManager()->GetElement(index)->GetMyMesh()->indexBuffer_, DXGI_FORMAT_R32_UINT, 0);
			_myEngine->GetD3DContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			_myEngine->GetD3DContext()->UpdateSubresource(_myEngine->GetConstantBuffer( 0 ), 0, 0, &_myEngine->GetObjectManager()->GetElement(index)->matFinal_, 0, 0);
			_myEngine->GetD3DContext()->DrawIndexed( _myEngine->GetObjectManager()->GetElement(index)->GetMyMesh()->GetSizeOfIndicies(), 0, 0);
		}
		
		_myEngine->GetSwapChain()->Present( 0, 0 );
	};

	//Globals
	CBPerFrame CBPerFrame;
	XMFLOAT3 UpdateMove;
	XMFLOAT3 CameraPos;
	XMFLOAT3 FloorRotation;
};

//-----------------------------------------------------------------------------
// Application specific state setup.
//-----------------------------------------------------------------------------
void StateSetup()
{
	_myEngine->AddState( new TestState, true );
}





int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmdLine )
{

	// Create the engine setup structure.
	EngineSetup setup;
	setup.instance = hInstance;
	setup.name = "Engine Control Test";
	setup.StateSetup = StateSetup;

	// Create the engine (using the setup structure), then run it.
	new Engine( &setup );
	_myEngine->Run();

	return 0;

}
