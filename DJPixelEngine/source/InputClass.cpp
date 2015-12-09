/*
	Filename: InputClass.cpp
	Description: Source implementation of the input class
*/

#include "InputClass.h"

InputClass::InputClass()
{
#ifdef WIN32
	m_directInput = 0;
	m_keyboard = 0;
	m_mouse = 0;
	m_diGamepad = 0;
	m_bGamepadIsXInput = false;
#endif
}

InputClass::InputClass(const InputClass& other)
{
}

InputClass::~InputClass()
{
}

#ifdef WIN32
bool InputClass::Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight)
#else
bool InputClass::Initialize(int screenWidth, int screenHeight)
#endif
{
	// Store the screen size which will be used for positioning the mouse cursor
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	// Initialize the mouse cursor position
	m_mouseX = 0;
	m_mouseY = 0;

#ifdef WIN32
	HRESULT result;
	// Initialize the main DirectInput interface
	result = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL);
	if(FAILED(result))
	{
		return false;
	}

	// Initialize the DirectInput interface for keyboard
	result = m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
	if(FAILED(result))
	{
		return false;
	}

	// Set the data format. In this case since it is a keyboard we can use the predefined data format
	result = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
	if(FAILED(result))
	{
		return false;
	}

	// Set the cooperative level of the keyboard to not share with other programs
	result = m_keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if(FAILED(result))
	{
		return false;
	}

	// Now acquire the keyboard
	result = m_keyboard->Acquire();
	if(FAILED(result))
	{
		return false;
	}

	// Initialize the DirectInput interface for the mouse
	result = m_directInput->CreateDevice(GUID_SysMouse, &m_mouse, NULL);
	if(FAILED(result))
	{
		return false;
	}

	// Set the data format for the mouse using the pre-determined mouse data format.
	result = m_mouse->SetDataFormat(&c_dfDIMouse);
	if(FAILED(result))
	{
		return false;
	}

	// Set the cooperative level of the mouse to share with other programs
	result = m_mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if(FAILED(result))
	{
		return false;
	}

	// Acquire the mouse
	result = m_mouse->Acquire();
	if(FAILED(result))
	{
		return false;
	}

	// Enumerate joysticks or gamepads
	result = m_directInput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback, this, DIEDFL_ATTACHEDONLY);
	if(FAILED(result))
	{
		return false;
	}

	if(m_diGamepad == NULL) {
		//MessageBox(hwnd, L"Gamepad not found.", L"DJPixelEngine", MB_OK);
#ifdef _DEBUG
		OutputDebugString(L"Gamepad not found.");
#endif
		return true;
	}

	// Set the joystick/gamepad data format
	result = m_diGamepad->SetDataFormat(&c_dfDIJoystick2);
	if(FAILED(result))
	{
		return false;
	}

	// Set the cooperative level of the joystick/gamepad to not share with other programs
	result = m_diGamepad->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if(FAILED(result))
	{
		return false;
	}

	// Gather information about the capabilities of the joystick/gamepad
	m_diDevCaps.dwSize = sizeof(DIDEVCAPS);
	result = m_diGamepad->GetCapabilities(&m_diDevCaps);
	if(FAILED(result))
	{
		return false;
	}

	// Enumerate the axes on joystick/gamepad and set a range for each
	result = m_diGamepad->EnumObjects(EnumObjectsCallback, this, DIDFT_ALL);
	if(FAILED(result))
	{
		return false;
	}

	// Acquire the joystick/gamepad
	result = m_diGamepad->Poll();
	if(FAILED(result))
	{
		result = m_diGamepad->Acquire();
		while(result == DIERR_INPUTLOST)
		{
			result = m_diGamepad->Acquire();
		}

		return true;
	}
#endif

	return true;
}

void InputClass::Shutdown()
{
#ifdef WIN32
	// Release the gamepad
	if(m_diGamepad)
	{
		m_diGamepad->Unacquire();
		m_diGamepad->Release();
		m_diGamepad = 0;
	}

	// Release the mouse
	if(m_mouse)
	{
		m_mouse->Unacquire();
		m_mouse->Release();
		m_mouse = 0;
	}

	// Release the keyboard
	if(m_keyboard)
	{
		m_keyboard->Unacquire();
		m_keyboard->Release();
		m_mouse = 0;
	}

	// Release the main interface to DirectInput
	if(m_directInput)
	{
		m_directInput->Release();
		m_directInput = 0;
	}
#endif
}

bool InputClass::Frame()
{
	bool result;

	// Read the current state of the keyboard
	result = ReadKeyboard();
	if(!result)
	{
		return false;
	}

	// Read the current state of the mouse
	result = ReadMouse();
	if(!result)
	{
		return false;
	}

	// Read the current state of the joystick/gamepad if available
	if(m_diGamepad)
	{
		result = ReadGamepad();
		if(!result)
		{
			return false;
		}
	}

	// Process the changes in the mouse and keyboard
	ProcessInput();

	return true;
}

bool InputClass::ReadKeyboard()
{
	// Read the keyboard device
#ifdef WIN32
	HRESULT result;

	result = m_keyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
	if(FAILED(result))
	{
		// If the keyboard lost focus or was not acquired, then try to get control back
		if((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_keyboard->Acquire();
		}
		else
		{
			return false;
		}
	}
#endif

	return true;
}

bool InputClass::ReadMouse()
{
	// Read the mouse device
#ifdef WIN32
	HRESULT result;
	result = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);
	if(FAILED(result))
	{
		// If the mouse lost focus or was not acquired, then try to get control back
		if((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_mouse->Acquire();
		}
		else
		{
			return false;
		}
	}
#endif

	return true;
}

bool InputClass::ReadGamepad()
{
	//char strText[128];
#ifdef WIN32
	HRESULT result;

	if(0 == m_diGamepad)
		return false;

	// Poll the device to the current state
	result = m_diGamepad->Poll();
	if(FAILED(result))
	{
		result = m_diGamepad->Acquire();
		while(result == DIERR_INPUTLOST)
		{
			result = m_diGamepad->Acquire();
		}

		return true;
	}

	// Get the gamepad/joystick's device state
	result = m_diGamepad->GetDeviceState(sizeof(DIJOYSTATE2), &m_joystickState);
	if(FAILED(result))
	{
#ifdef _DEBUG
		OutputDebugString(L"Controller may be disconnected.");
#endif
		return true;
	}
#endif

	return true;
}

void InputClass::ProcessInput()
{
	// Update the location of the mouse cursor based on the change of the mouse location during the frame
	m_mouseX += m_mouseState.lX;
	m_mouseY += m_mouseState.lY;

	// Ensure the mouse location doesn't exceed the screen bounds
	if(m_mouseX < 0)
	{
		m_mouseX = 0;
	}
	else if(m_mouseX > m_screenWidth)
	{
		m_mouseX = m_screenWidth;
	}

	if(m_mouseY < 0)
	{
		m_mouseY = 0;
	}
	else if(m_mouseY > m_screenHeight)
	{
		m_mouseY = m_screenHeight;
	}

	return;
}

bool InputClass::IsEscapePressed()
{
	// Do a bitwise and on the keyboard state to check if the escape key is currently being pressed
	if(m_keyboardState[DIK_ESCAPE] & 0x80)
	{
		return true;
	}

	return false;
}

void InputClass::GetMouseLocation(int& mouseX, int& mouseY)
{
	mouseX = m_mouseX;
	mouseY = m_mouseY;
	return;
}

void InputClass::GetJoystickState(float& leftAnalogStickX, float& leftAnalogStickY, float& rightAnalogStickX, float& rightAnalogStickY)
{
	if(m_diGamepad)
	{
		leftAnalogStickX = (float)m_joystickState.lX / 1000.0f;
		leftAnalogStickY = (float)m_joystickState.lY / 1000.0f;
		rightAnalogStickX = (float)m_joystickState.lZ / 1000.0f;
		rightAnalogStickY = (float)m_joystickState.lRz / 1000.0f;

		// Make sure these axes are above their dead values
		if(abs(leftAnalogStickX) < 0.19f) {
			leftAnalogStickX = 0;
		}
		if(abs(leftAnalogStickY) < 0.19f) {
			leftAnalogStickY = 0;
		}
		if(abs(rightAnalogStickX) < 0.19f) {
			rightAnalogStickX = 0;
		}
		if(abs(rightAnalogStickY) < 0.19f) {
			rightAnalogStickY = 0;
		}
	}
	else
	{
		leftAnalogStickX = 0;
		leftAnalogStickY = 0;
		rightAnalogStickX = 0;
		rightAnalogStickY = 0;
	}
	return;
}

#ifdef WIN32
bool InputClass::IsXInputDevice(const GUID* pGuidProductFromDirectInput)
{
	IWbemLocator*			pIWbemLocator = NULL;
	IEnumWbemClassObject*	pEnumDevices = NULL;
	IWbemClassObject*		pDevices[20] = {0};
	IWbemServices*			pIWbemServices = NULL;
	BSTR					bstrNamespace = NULL;
	BSTR					bstrDeviceID = NULL;
	BSTR					bstrClassName = NULL;
	DWORD					uReturned = 0;
	bool					bIsXInputDevice = false;
	UINT					iDevice = 0;
	VARIANT					var;
	HRESULT					hr;

	// Co-init if needed
	hr = CoInitialize(NULL);
	bool bCleanupCOM = SUCCEEDED(hr);

	// Create WMI
	hr = CoCreateInstance(__uuidof(WbemLocator),
						NULL,
						CLSCTX_INPROC_SERVER,
						__uuidof(IWbemLocator),
						(LPVOID*) &pIWbemLocator);
	if( FAILED(hr) || pIWbemLocator == NULL )
		goto LCleanup;

	bstrNamespace = SysAllocString( L"\\\\.\\root\\cimv2" );	if( bstrNamespace == NULL ) goto LCleanup;
	bstrClassName = SysAllocString( L"Win32_PNPEntity" );		if( bstrClassName == NULL ) goto LCleanup;
	bstrDeviceID = SysAllocString( L"DeviceID" );				if( bstrDeviceID == NULL ) goto LCleanup;

	// Connect to WMI
	hr = pIWbemLocator->ConnectServer( bstrNamespace, NULL, NULL, 0L, 0L, NULL, NULL, &pIWbemServices );
	if( FAILED(hr) || pIWbemServices == NULL )
		goto LCleanup;

	// Switch security level to IMPERSONATE
	CoSetProxyBlanket( pIWbemServices, RPC_C_AUTHN_WINNT, RPC_C_AUTHN_NONE, NULL, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE );

	hr = pIWbemServices->CreateInstanceEnum( bstrClassName, 0, NULL, &pEnumDevices );
	if( FAILED(hr) || pEnumDevices == NULL )
		goto LCleanup;

	// Loop over all devices
	for(;;)
	{
		// Get 20 at a time
		hr = pEnumDevices->Next( 10000, 20, pDevices, &uReturned );
		if( FAILED(hr) )
			goto LCleanup;
		if( uReturned == 0 )
			break;

		for( iDevice = 0; iDevice < uReturned; iDevice++ )
		{
			// For each device, get its device ID
			hr = pDevices[iDevice]->Get( bstrDeviceID, 0L, &var, NULL, NULL );
			if( SUCCEEDED(hr) && var.vt == VT_BSTR && var.bstrVal != NULL )
			{
				// Check if the device ID contains "IG_". If it does, then it's an XInput device
				// This information cannot be found from DirectInput
				if( wcsstr( var.bstrVal, L"IG_" ) )
				{
					// If it does, then get the VID/PID from var.bstrVal
					DWORD dwPid = 0, dwVid = 0;
					WCHAR* strVid = wcsstr( var.bstrVal, L"VID_" );
					if( strVid && swscanf( strVid, L"VID_%4X", &dwVid ) != 1 )
						dwVid = 0;
					WCHAR* strPid = wcsstr( var.bstrVal, L"PID_" );
					if( strPid && swscanf( strVid, L"VID_%4X", &dwPid ) != 1 )
						dwPid = 0;

					// Compare the VID/PID to the DInput device
					DWORD dwVidPid = MAKELONG( dwVid, dwPid );
					if( dwVidPid == pGuidProductFromDirectInput->Data1 )
					{
						bIsXInputDevice = true;
						goto LCleanup;
					}
				}
			}
			if( pDevices[iDevice] )
			{
				pDevices[iDevice]->Release();
				pDevices[iDevice] = NULL;
			}
		}
	}

LCleanup:
	if(bstrNamespace)
		SysFreeString(bstrNamespace);
	if(bstrDeviceID)
		SysFreeString(bstrDeviceID);
	if(bstrClassName)
		SysFreeString(bstrClassName);
	for( iDevice = 0; iDevice < 20; iDevice++ )
	{
		if( pDevices[iDevice] )
		{
			pDevices[iDevice]->Release();
			pDevices[iDevice] = NULL;
		}
	}
	if( pEnumDevices )
	{
		pEnumDevices->Release();
		pEnumDevices = NULL;
	}
	if( pIWbemLocator )
	{
		pIWbemLocator->Release();
		pIWbemLocator = NULL;
	}
	if( pIWbemServices )
	{
		pIWbemServices->Release();
		pIWbemServices = NULL;
	}

	if( bCleanupCOM )
		CoUninitialize();

	return bIsXInputDevice;
}

BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext)
{
	HRESULT hr;

	InputClass* inputClass = (InputClass*)pContext;

	if( inputClass->IsXInputDevice( &pdidInstance->guidProduct ) )
	{
#ifdef _DEBUG
		OutputDebugString(L"XInput device detected.");
#endif
		inputClass->m_bGamepadIsXInput = true;
		return DIENUM_CONTINUE;
	}

	// Obtain an interface to the enumerated joystick
#ifdef _DEBUG
	OutputDebugString(L"DirectInput device detected.");
#endif
	hr = inputClass->m_directInput->CreateDevice(pdidInstance->guidInstance, &(inputClass->m_diGamepad), NULL);

	if(FAILED(hr))
	{
		return DIENUM_CONTINUE;
	}

	return DIENUM_STOP;
}

BOOL CALLBACK EnumObjectsCallback(const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext)
{
	InputClass* inputClass = (InputClass*)pContext;

	if(pdidoi->dwType & DIDFT_AXIS)
	{
		DIPROPRANGE diprg;

		diprg.diph.dwSize = sizeof(DIPROPRANGE);
		diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		diprg.diph.dwHow = DIPH_BYID;
		diprg.diph.dwObj = pdidoi->dwType;
		diprg.lMin = -1000;
		diprg.lMax = +1000;

		// Set the range for the axis
		if(FAILED((inputClass->m_diGamepad)->SetProperty(DIPROP_RANGE, &diprg.diph)))
		{
			return DIENUM_STOP;
		}
	}

	return DIENUM_CONTINUE;
}
#endif
