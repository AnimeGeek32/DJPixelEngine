/*
	Filename: GraphicsClass.h
	Description: Header file of graphics class
*/

#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

// Includes
#ifdef WIN32
#include "D3DClass.h"
#elif __APPLE__
#include "TargetConditionals.h"
#if TARGET_OS_IPHONE
// iOS Device stuff
#elif TARGET_IPHONE_SIMULATOR
// iOS Simulator stuff
#elif TARGET_OS_MAC
// Mac OS X stuff
#endif
#endif
#include "InputClass.h"
#include "CameraClass.h"
#include "TextureManager.h"
#include "SpriteClass.h"
#include "SpriteManager.h"
#include "FontClass.h"
#include "TextClass.h"

// Globals
const float screenDepth = 1000.0f;
const float screenNear = 0.1f;

/*
	Class name: GraphicsClass
*/
class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass& other);
	~GraphicsClass();

#ifdef WIN32
	bool Initialize(int, int, HWND);
#else
	bool Initialize(int, int);
#endif
	void Shutdown();
	bool Frame(InputClass* inputClass);
	
	bool getFullscreen();
	bool getVSyncEnabled();
	int getScreenWidth();
	int getScreenHeight();

	bool Render();
private:

#ifdef WIN32
	D3DClass* m_D3D;
#endif
	CameraClass* m_Camera;
	TextureManager* m_textureManager;
	SpriteManager* m_spriteManager;
	FontClass* m_font;
	TextClass* m_textObject;

	bool bFullScreen;
	bool bVSyncEnabled;
	int m_screenWidth;
	int m_screenHeight;
};

#endif
