/*
	Filename: SpriteClass.h
	Description: Header file for the sprite class
*/

#ifndef _SPRITECLASS_H_
#define _SPRITECLASS_H_

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
#include "TextureManager.h"

/*
	Class name: SpriteClass
*/
class SpriteClass
{
public:
	SpriteClass();
	SpriteClass(const SpriteClass&);
	~SpriteClass();

	bool Initialize(TextureManager* pTexManager, unsigned int texID);
	void SetPosition(float x, float y, float z);
	void SetClipRect(long clipLeft, long clipTop, long clipRight, long clipBottom);
	void Shutdown();

#ifdef WIN32
	IDirect3DTexture9* GetTexture();
	D3DXVECTOR3 GetPosition();
	D3DXVECTOR2 GetCenter();
	RECT GetClipRect();
#endif

private:
#ifdef WIN32
	IDirect3DTexture9* m_Texture;
#endif
	float m_positionX, m_positionY, m_positionZ;
	float m_centerX, m_centerY;
	long m_clipLeft, m_clipTop, m_clipRight, m_clipBottom;
};

#endif
