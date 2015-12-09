/*
	Filename: main.cpp
	Description: The main source file for DJPixelEngine
*/

#include "SystemClass.h"

#ifdef WIN32
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
#else
int main(int argc, char *argv[])
#endif
{
	SystemClass *System;
	bool result;

	// Create the system object
	System = new SystemClass();
	if(!System)
	{
		return 0;
	}

	// Initialize and run the system object
	result = System->Initialize();
	if(result)
	{
		System->Run();
	}

	// Shutdown and release the system object
	System->Shutdown();
	delete System;
	System = 0;

	return 0;
}
