//-----------------------------------------------------------------------------
// A wrapper class for handling input via DirectInput. Both the keyboard and
// mouse devices are supported through this class.
//
// Programming a Multiplayer First Person Shooter in DirectX
// Copyright (c) 2004 Vaughan Young
//-----------------------------------------------------------------------------
#ifndef INPUT_H
#define INPUT_H


//-----------------------------------------------------------------------------
// Input Class
//-----------------------------------------------------------------------------
class Input
{
public:
	Input( HWND window );
	virtual ~Input();

	void Update();

	bool GetKeyPress( unsigned char key, bool ignorePressStamp = false );


private:
	HWND myWindow; // Handle of the parent window.

	IDirectInput8* m_di; // DirectInput object.
	unsigned long m_pressStamp; // Current press stamp (incremented every frame).

	IDirectInputDevice8* m_keyboard;
	unsigned char m_keyState[256]; // Stores the state of the keyboard keys.
	unsigned long m_keyPressStamp[256]; // Stamps the last frame each key was preseed.
};

#endif