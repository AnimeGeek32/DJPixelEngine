/*
	Filename: TextureManager.h
	Description: Header file for texture manager
*/

#ifndef _TEXTUREMANAGER_H_
#define _TEXTUREMANAGER_H_

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
#include <vector>
using namespace std;

/*
	Class name: TextureManager
*/
class TextureManager
{
public:
	TextureManager();
	TextureManager(const TextureManager&);
	~TextureManager();

#ifdef WIN32
	bool AddTexture(D3DClass*, LPCTSTR);
	IDirect3DTexture9* GetTexture(unsigned int);
#endif
	void Shutdown();

private:
#ifdef WIN32
	vector<IDirect3DTexture9*> m_Textures;
#endif
};

#endif
