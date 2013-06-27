/*
/ A FPS Engine, following the book:
/	Programming a Multiplayer First Person Shooter in DirectX
/		Copyright (c) 2004 Vaughan Young
/
/ Used extensivly in my GAM420 class at UAT.
/ All new code authored by Brandon McClure
*/
#include "FPSEngine.h"

Engine * _myEngine = NULL;


LRESULT CALLBACK WindowProc( HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam )
{
	switch( msg )
	{
		case WM_ACTIVATEAPP:
			_myEngine->SetDeactiveFlag( !wparam );
			return 0;

		case WM_DESTROY:
			PostQuitMessage( 0 );
			return 0;

		default:
			return DefWindowProc( wnd, msg, wparam, lparam );
	}
}

Engine::Engine( EngineSetup *setup ) : d3dDevice_( 0 ), d3dContext_( 0 ), swapChain_( 0 ), backBufferTarget_( 0 )
{
	engineLoaded = false;

	// If no setup structure was passed in, then create a default one.
	// Otehrwise, make a copy of the passed in structure.
	myEngineSetup = new EngineSetup;
	if( setup != NULL )
	{
		memcpy( myEngineSetup, setup, sizeof( EngineSetup ) );
	}

	_myEngine = this;

	// Prepare and register the window class.
	WNDCLASSEX wcex;
	wcex.cbSize        = sizeof( WNDCLASSEX );
	wcex.style         = CS_CLASSDC;
	wcex.lpfnWndProc   = WindowProc;
	wcex.cbClsExtra    = 0;
	wcex.cbWndExtra    = 0;
	wcex.hInstance     = myEngineSetup->instance;
	wcex.hIcon         = LoadIcon( NULL, IDI_APPLICATION );
	wcex.hCursor       = LoadCursor( NULL, IDC_ARROW );
	wcex.hbrBackground = NULL;
	wcex.lpszMenuName  = NULL;
	wcex.lpszClassName = "WindowClass";
	wcex.hIconSm       = LoadIcon( NULL, IDI_APPLICATION );
	RegisterClassEx( &wcex );

	// Initialise the COM using multithreaded concurrency.
	CoInitializeEx( NULL, COINIT_MULTITHREADED );

	// Create the window and retrieve a handle to it.
	// Note: Later the window will be created using a windowed/fullscreen flag.
	myWindow = CreateWindow( "WindowClass", myEngineSetup->name, WS_OVERLAPPED, 0, 0, 800, 600, NULL, NULL, myEngineSetup->instance, NULL );

	sMultiSamplerCount = 1;

	//Set Up the states list
	m_states = new LinkedList< State >;
	m_currentState = NULL;

	// Set up the primative mesh manager
	primativeManager_ = new MeshManager();

	//Runs the DX Init class
	_DX11Init = new DX11Init;
	myResult = _DX11Init->Initialize();
	if ( FAILED( myResult ) )
	{
		MessageBox(0,"DX Initialization Failed.","Compile Error", MB_OK );
	}

	// Seed the random number generator with the current time.
	srand( timeGetTime() );

	// Set up the script manager
	m_scriptManager = new ResourceManager< LuaScript >;

	objects_ = new ObjectManager();

	// Create the input object.
	myInput = new Input( myWindow );

	// Allow the application to perform any state setup now.
	if( myEngineSetup->StateSetup != NULL )
		myEngineSetup->StateSetup();

	// The engine is fully loaded and ready to go.
	engineLoaded = true;

}

Engine::~Engine()
{
	// Ensure the engine is loaded.
	if( engineLoaded == true )
	{
		// Destroy the states linked lists.
		if( m_currentState != NULL )
			m_currentState->Close();
		SAFE_DELETE( m_states );

		// Destroy everything.
		SAFE_DELETE( myInput );
		SAFE_DELETE( objects_ );
		SAFE_DELETE( primativeManager_ );
		SAFE_DELETE( m_scriptManager );
	}

	CoUninitialize();

	UnregisterClass( "WindowClass", myEngineSetup->instance );

	GarbageCollect();

	// Destroy the engine setup structure.
	SAFE_DELETE( myEngineSetup );
	
}

void Engine::GarbageCollect()
{
	swapChain_->SetFullscreenState( false, NULL );
	
	SAFE_RELEASE( zBuffer_ );
	SAFE_RELEASE( inputLayout_ );
	SAFE_RELEASE( constantBuffer_ );
	SAFE_RELEASE( solidColorPS_ );
	SAFE_RELEASE( solidColorVS_ );
	SAFE_RELEASE( backBufferTarget_ );
	SAFE_RELEASE( swapChain_ );
	SAFE_RELEASE( d3dContext_ );
	SAFE_RELEASE( d3dDevice_ );

}

void Engine::Run()
{
	// Ensure the engine is loaded.
	if( engineLoaded == true )
	{
		// Show the window.
		ShowWindow( myWindow, SW_NORMAL );

		// Enter the message loop.
		MSG msg;
		ZeroMemory( &msg, sizeof( MSG ) );
		while( msg.message != WM_QUIT )
		{
			if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
			{
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
			else if( !m_deactive )
			{				
				if ( Update() != 0 )
				{
					continue;
				}
	
			}
		}
	}

	// Destroy the engine.
	SAFE_DELETE( _myEngine );
}

HWND Engine::GetWindow()
{
	return myWindow;
}

void Engine::SetDeactiveFlag( bool deactive )
{
	m_deactive = deactive;
}

void Engine::DX11Initialize()
{
}

ObjectManager* Engine::GetObjectManager()
{
	return objects_;
}

ID3D11DeviceContext *Engine::GetD3DContext()
{
	return d3dContext_;
}

ID3D11Device *Engine::GetD3DDevice()
{
	return d3dDevice_;
}

ID3D11RenderTargetView *Engine::GetBackBufferTarget()
{
	return backBufferTarget_;
}

IDXGISwapChain* Engine::GetSwapChain()
{
	return swapChain_;
}

ID3D11Buffer* Engine::GetConstantBuffer( int indexNo )
{
	return constantBuffer_ ;
}

ID3D11VertexShader* Engine::GetSolidColorVS()
{
	return solidColorVS_;
}

ID3D11PixelShader* Engine::GetSolidColorPS()
{
	return solidColorPS_;
}

ID3D11InputLayout* Engine::GetInputLayout()
{
	return inputLayout_;
}

unsigned int Engine::SCREEN_WIDTH()
{
	return sWidth;
}

unsigned int Engine::SCREEN_HEIGHT()
{
	return sHeight;
}

unsigned int Engine::MUTI_SAMPLE_COUNT()
{
	return sMultiSamplerCount;
}

ID3D11DepthStencilView *Engine::GetZBuffer()
{
	return zBuffer_;
}

void Engine::SetSCREEN_WIDTH( unsigned int width )
{
	sWidth = width;
}

void Engine::SetSCREEN_HEIGHT( unsigned int height )
{
	sHeight = height;
}

void Engine::SetMULTI_SAMPLE_COUNT( unsigned int count )
{
	sMultiSamplerCount = count;
}

void Engine::AddState( State *state, bool change )
{
	m_states->Add( state );

	if( change == false )
		return;

	if( m_currentState != NULL )
		m_currentState->Close();

	m_currentState = m_states->GetLast();
	m_currentState->Load();
}

void Engine::RemoveState( State *state )
{
	m_states->Remove( &state );
}

void Engine::ChangeState( unsigned long id )
{
	// Iterate through the list of states and find the new state to change to.
	m_states->Iterate( true );
	while( m_states->Iterate() != NULL )
	{
		if( m_states->GetCurrent()->GetID() == id )
		{
			// Close the old state.
			if( m_currentState != NULL )
				m_currentState->Close();

			// Set the new current state and load it.
			m_currentState = m_states->GetCurrent();
			m_currentState->Load();

			// Indicate that the state has changed.
			m_stateChanged = true;

			break;
		}
	}
}

State *Engine::GetCurrentState()
{
	return m_currentState;
}

ResourceManager< LuaScript > *Engine::GetScriptManager()
{
	return m_scriptManager;
}

Input *Engine::GetInput()
{
	return myInput;
}

int Engine::Update()
{
	// Calculate the elapsed time.
	unsigned long currentTime = timeGetTime();
	static unsigned long lastTime = currentTime;
	float elapsed = ( currentTime - lastTime ) / 1000.0f;
	lastTime = currentTime;

	// Update the input object, reading the keyboard and mouse.
	myInput->Update();

	// Request the viewer from the current state, if there is one.
	if( m_currentState != NULL )
		m_currentState->RequestViewer( &viewer );

	// Update the current state (if there is one), taking state
	// changes into account.
	m_stateChanged = false;
	if( m_currentState != NULL )
		m_currentState->Update( elapsed );

	if( m_stateChanged == true )
		return 1;

	objects_.Update();

	return 0;
}

MeshManager* Engine::GetPrimativeManager()
{
	return primativeManager_;
}
