/*
	Filename: InputClass.h
	Description: Header file for the input class
*/

#ifndef _INPUTCLASS_H_
#define _INPUTCLASS_H_

// Pre-processing Directive(s)
#ifdef WIN32
#define DIRECTINPUT_VERSION 0x0800
#endif

// Linking
#ifdef WIN32
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#endif

// Include(s)
#ifdef WIN32
#include <cmath>
#include <cstdio>
#include <dinput.h>
#include <Windows.h>
#include <WbemIdl.h>
#include <OleAuto.h>
#endif

/*
	Class name: InputClass
*/
class InputClass
{
public:
	InputClass();
	InputClass(const InputClass& other);
	~InputClass();

#ifdef WIN32
	bool Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight);
	bool IsXInputDevice(const GUID* pGuidProductFromDirectInput);
	friend BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext);
	friend BOOL CALLBACK EnumObjectsCallback(const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext);
#else
	bool Initialize(int screenWidth, int screenHeight);
#endif

	void Shutdown();
	bool Frame();

	bool IsEscapePressed();
	void GetMouseLocation(int& mouseX, int& mouseY);
	void GetJoystickState(float& leftAnalogStickX, float& leftAnalogStickY, float& rightAnalogStickX, float& rightAnalogStickY);

private:
	bool ReadKeyboard();
	bool ReadMouse();
	bool ReadGamepad();
	void ProcessInput();

#ifdef WIN32
	IDirectInput8* m_directInput;
	IDirectInputDevice8* m_keyboard;
	IDirectInputDevice8* m_mouse;
	IDirectInputDevice8* m_diGamepad;
#endif

	unsigned char m_keyboardState[256];
#ifdef WIN32
	DIMOUSESTATE m_mouseState;
	DIDEVCAPS m_diDevCaps;
	DIJOYSTATE2 m_joystickState;
#endif

	bool m_bGamepadIsXInput;
	int m_screenWidth, m_screenHeight;
	int m_mouseX, m_mouseY;
};

#endif
