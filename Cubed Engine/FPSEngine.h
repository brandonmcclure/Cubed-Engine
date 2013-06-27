/*
/ A FPS Engine, following the book:
/	Programming a Multiplayer First Person Shooter in DirectX
/		Copyright (c) 2004 Vaughan Young
/
/ Used extensivly in my GAM420 class at UAT.
/ All new code authored by Brandon McClure
*/
#ifndef _FPS_ENGINE_
#define _FPS_ENGINE_

#define _XM_NO_INTRINSICS_
#define DIRECTINPUT_VERSION 0x0800

/////////////
// LINKING //
/////////////
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "D3DX10.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment (lib, "Winmm.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib,"lua51.lib")

//-----------------------------------------------------------------------------
// System Includes
//-----------------------------------------------------------------------------
#include <Windows.h>
#include <WindowsX.h>
#include <time.h>
#include <stdio.h>
#include <tchar.h>
#include<xnamath.h>
#include<memory>
#include<vector>

extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

//-----------------------------------------------------------------------------
// DirectX Includes
//-----------------------------------------------------------------------------
#include <d3d11.h>
#include <d3dx11.h>
#include <d3d10.h>
#include <d3dx10.h>
#include <dinput.h>
#include <DxErr.h>
#include <D3Dcompiler.h>

//-----------------------------------------------------------------------------
// Macros
//-----------------------------------------------------------------------------
#define SAFE_DELETE( p )		{ if( p ) { delete ( p );     ( p ) = NULL; } }
#define SAFE_DELETE_ARRAY( p )	{ if( p ) { delete[] ( p );   ( p ) = NULL; } }
#define SAFE_RELEASE( p )		{ if( p ) { ( p )->Release(); ( p ) = NULL; } }

//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#include "DefineStructs.h"
#include "Input.h"
#include "LinkedList.h"
#include "Geometry.h"
#include "ResourceManagement.h"
#include "State.h"
#include "LuaScript.h"
#include "DeviceEnum.h"
#include "DX11Init.h"
#include "BoundingVolume.h"
#include "Mesh.h"
#include "Object.h"
#include "CollisionDetection.h"

class Engine
{
	
	// ----- ----- Functions ----- ----- //
	
public:
	Engine( EngineSetup *setup = NULL );
	virtual ~Engine();
	void GarbageCollect();

	void Run();

	HWND GetWindow();
	void SetDeactiveFlag( bool deactive );

	void DX11Initialize();

	/*
	/ ----- ----- Get Functions ----- -----/
	*/
		ObjectManager* GetObjectManager();
		// ----- ----- DX Components ----- ----- //
		ID3D11DeviceContext *GetD3DContext();
		ID3D11Device *GetD3DDevice();
		ID3D11RenderTargetView *GetBackBufferTarget();
		IDXGISwapChain* GetSwapChain();
		ID3D11VertexShader* GetSolidColorVS();
		ID3D11PixelShader* GetSolidColorPS();
		ID3D11InputLayout* GetInputLayout();
		ID3D11Buffer *GetConstantBuffer( int indexNo );
		ID3D11DepthStencilView *GetZBuffer();

		unsigned int SCREEN_WIDTH();
		unsigned int SCREEN_HEIGHT();
		unsigned int MUTI_SAMPLE_COUNT();

	/*
	/ ----- ----- Set Functions ----- -----/
	*/
		void SetSCREEN_WIDTH( unsigned int width );
		void SetSCREEN_HEIGHT( unsigned int height );
		void SetMULTI_SAMPLE_COUNT( unsigned int count );

	// Finite State Machine
	void AddState( State *state, bool change = true );
	void RemoveState( State *state );
	void ChangeState( unsigned long id );
	State *GetCurrentState();

	ResourceManager< LuaScript > *GetScriptManager();

	Input *GetInput();
	D3DXCOLOR  backgroundColor;

	int Update();

	MeshManager* GetPrimativeManager();

private:

	// ----- ----- Variables ----- ----- //
private:
	HRESULT myResult;
	bool engineLoaded;
	HWND myWindow;
	bool m_deactive;

	EngineSetup *myEngineSetup;

	LinkedList< State > *m_states;
	State *m_currentState; 
	bool m_stateChanged; 
	ViewerSetup viewer;

	ResourceManager< LuaScript > *m_scriptManager;

	Input *myInput;
	MeshManager* primativeManager_;
	ObjectManager* objects_;

	/*
	/ ----- ----- DirectX Variables ----- -----/
	*/
		ID3D11Buffer* mvpCB_;
		XMMATRIX vpMatrix_;

		ID3D11ShaderResourceView* colorMap_;
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;

		ID3D11SamplerState* colorMapSampler_;
		ID3D11Texture2D* texture_;
		D3D11_TEXTURE2D_DESC desc;
		ID3D11BlendState* alphaBlendState_;

		// ----- ----- DX Settings ----- ----- //
		unsigned int sWidth;
		unsigned int sHeight;
		unsigned int sMultiSamplerCount;

public:
	/*
	/ ----- ----- Returnable DX varialbes ----- -----/
	*/
	ID3D11Device* d3dDevice_;
	ID3D11DeviceContext* d3dContext_;
	IDXGISwapChain* swapChain_;
	ID3D11RenderTargetView* backBufferTarget_;
	ID3D11Buffer *constantBuffer_;
	ID3D11VertexShader* solidColorVS_;
	ID3D11PixelShader* solidColorPS_;
	ID3D11InputLayout* inputLayout_;
	ID3D11DepthStencilView *zBuffer_;

		

};

//-----------------------------------------------------------------------------
// Externals
//-----------------------------------------------------------------------------
extern Engine * _myEngine;

#endif