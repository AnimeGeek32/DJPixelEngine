/*
	Filename: SystemClass.h
	Description: Header file containing core aspects of the engine
*/

#ifndef _SYSTEMCLASS_H_
#define _SYSTEMCLASS_H_

#ifdef WIN32
// Pre-processing Directives
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
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

// My class includes
#include "InputClass.h"
#include "GraphicsClass.h"

/*
	Class name: SystemClass
*/
class SystemClass
{
public:
	SystemClass();
	SystemClass(const SystemClass&);
	~SystemClass();

	bool Initialize();
	void Shutdown();
	void Run();

#ifdef WIN32
	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);
#endif

private:
	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();

#ifdef WIN32
	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;
#endif

	InputClass *m_Input;
	GraphicsClass *m_Graphics;
};

// Function Prototypes
#ifdef WIN32
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
#endif

// Globals
static SystemClass *ApplicationHandle = 0;

#endif
