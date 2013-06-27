//-----------------------------------------------------------------------------
// Input.h implementation.
// Refer to the Input.h interface for more details.
//
// Programming a Multiplayer First Person Shooter in DirectX
// Copyright (c) 2004 Vaughan Young
//-----------------------------------------------------------------------------
#include "FPSEngine.h"

Input::Input( HWND window )
{
	// Store the handle to the parent window.
	myWindow = window;

	// Create a DirectInput interface.
	DirectInput8Create( GetModuleHandle( NULL ), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_di, NULL );

	// Create, prepare, and aquire the keyboard device.
	m_di->CreateDevice( GUID_SysKeyboard, &m_keyboard, NULL );
	m_keyboard->SetDataFormat( &c_dfDIKeyboard );
	m_keyboard->SetCooperativeLevel( myWindow, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE );
	m_keyboard->Acquire();

	// Start the press stamp.
	m_pressStamp = 0;
}

Input::~Input()
{
	SAFE_RELEASE( m_di );
	SAFE_RELEASE( m_keyboard );
}

void Input::Update()
{
	static HRESULT result;

	// Poll the keyboard until it succeeds or returns an unknown error.
	while( true )
	{
		m_keyboard->Poll();
		if( SUCCEEDED( result = m_keyboard->GetDeviceState( 256, (LPVOID)&m_keyState ) ) )
			break;
		if( result != DIERR_INPUTLOST && result != DIERR_NOTACQUIRED )
			return;

		// Reacquire the device if the focus was lost.
		if( FAILED( m_keyboard->Acquire() ) )
			return;
	}


	// Increment the press stamp.
	m_pressStamp++;
}

//-----------------------------------------------------------------------------
// Returns true if the given key is pressed.
// Note: Consistent presses will return false when using the press stamp.
//-----------------------------------------------------------------------------
bool Input::GetKeyPress( unsigned char key, bool ignorePressStamp )
{
	if( ( m_keyState[key] & 0x80 ) == false )
		return false;

	bool pressed = true;

	if( ignorePressStamp == false )
		if( m_keyPressStamp[key] == m_pressStamp - 1 || m_keyPressStamp[key] == m_pressStamp )
			pressed = false;

	m_keyPressStamp[key] = m_pressStamp;

	return pressed;
}