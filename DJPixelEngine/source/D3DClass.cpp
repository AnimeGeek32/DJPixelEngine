/*
	Filename: D3DClass.cpp
	Description: Source implementation of Direct3D 9 class
*/

#include "D3DClass.h"

D3DClass::D3DClass()
{
	m_d3dObject = 0;
	m_d3dDevice = 0;
	m_d3dSpriteHandler = 0;
}

D3DClass::D3DClass(const D3DClass& other)
{
}

D3DClass::~D3DClass()
{
}

bool D3DClass::Initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear)
{
	float fieldOfView, screenAspect;

	// Store the VSync setting
	m_vsync_enabled = vsync;

	// Create an instance of Direct3D9 object
	if( NULL == (m_d3dObject = Direct3DCreate9(D3D_SDK_VERSION)) )
	{
		MessageBox(NULL, L"DJPixelEngine Error", L"Could not create Direct3D9 instance.", MB_ICONERROR | MB_OK);
		return false;
	}

	// Create a struct to hold various device information
	D3DPRESENT_PARAMETERS d3dpp;

	ZeroMemory(&d3dpp, sizeof(d3dpp));	// Clear out the struct for use
	if(fullscreen)
	{
		d3dpp.Windowed = FALSE;
		d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;	// Set the back buffer format to 32-bit
		d3dpp.BackBufferWidth = screenWidth;		// Set the width of the buffer
		d3dpp.BackBufferHeight = screenHeight;		// Set the height of the buffer
	}
	else
	{
		d3dpp.Windowed = TRUE;
		d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;	// Set the back buffer format to current display mode format on desktop
		d3dpp.BackBufferWidth = 0;
		d3dpp.BackBufferHeight = 0;
	}
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	// Discard old frames
	d3dpp.hDeviceWindow = hwnd;					// Set the window to be used by Direct3D
	if(vsync)
	{
		// Wait for default VSync interval
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	}
	else
	{
		// Do not wait for VSync
		d3dpp.PresentationInterval = D3DPRESENT_DONOTWAIT;
	}

	// Screen depth (needed?)

	// Create a device class using this information and information from the d3dpp struct
	if(FAILED(m_d3dObject->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &m_d3dDevice)))
	{
		MessageBox(hwnd, L"DJPixelEngine Error", L"Could not create Direct3D9 device.", MB_ICONERROR | MB_OK);
		return false;
	}

	// Create an instance of sprite handler
	if(FAILED(D3DXCreateSprite(m_d3dDevice, &m_d3dSpriteHandler)))
	{
		MessageBox(hwnd, L"DJPixelEngine Error", L"Could not create Direct3D9 sprite handler.", MB_ICONERROR | MB_OK);
		return false;
	}

	// Initialize the world matrix to the identity matrix
	D3DXMatrixIdentity(&m_worldMatrix);

	// Setup and create the orthographic projection matrix for 2D rendering
	fieldOfView = (float)D3DX_PI / 4.0f;
	screenAspect = (float)screenWidth / (float)screenHeight;
	D3DXMatrixOrthoLH(&m_orthoMatrix, (float)screenWidth, (float)screenHeight, screenNear, screenDepth);

	// TODO: Video card info?

	return true;
}

void D3DClass::Shutdown()
{
	if(m_d3dSpriteHandler)
	{
		// Close and release the sprite handler
		m_d3dSpriteHandler->Release();
		m_d3dSpriteHandler = 0;
	}
	if(m_d3dDevice)
	{
		// Close and release the 3D device
		m_d3dDevice->Release();
		m_d3dDevice = 0;
	}
	if(m_d3dObject)
	{
		// Close and release Direct3D
		m_d3dObject->Release();
		m_d3dObject = 0;
	}
}

void D3DClass::BeginScene(int red, int green, int blue, int alpha)
{
	// Clear the window to specified color
	m_d3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(alpha, red, green, blue), 1.0f, 0);

	// Begins the 3D scene
	m_d3dDevice->BeginScene();

	// Begins the 2D scene with alpha blending
	m_d3dSpriteHandler->Begin(D3DXSPRITE_ALPHABLEND);
}

void D3DClass::EndScene()
{
	// Ends the 2D scene
	m_d3dSpriteHandler->End();

	// Ends the 3D scene
	m_d3dDevice->EndScene();

	// Displays the created frame
	m_d3dDevice->Present(NULL, NULL, NULL, NULL);
}

IDirect3D9* D3DClass::GetD3DObject()
{
	return m_d3dObject;
}

IDirect3DDevice9* D3DClass::GetD3DDevice()
{
	return m_d3dDevice;
}

ID3DXSprite* D3DClass::GetD3DSpriteHandler()
{
	return m_d3dSpriteHandler;
}

void D3DClass::GetWorldMatrix(D3DXMATRIX& worldMatrix)
{
	worldMatrix = m_worldMatrix;
}

void D3DClass::GetOrthoMatrix(D3DXMATRIX& orthoMatrix)
{
	orthoMatrix = m_orthoMatrix;
}
