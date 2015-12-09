/*
	Filename: SpriteClass.cpp
*/

#include "SpriteClass.h"

SpriteClass::SpriteClass()
{
	m_Texture = 0;
	m_positionX = 0;
	m_positionY = 0;
	m_positionZ = 0;
	m_centerX = 0;
	m_centerY = 0;
	m_clipLeft = 0;
	m_clipTop = 0;
	m_clipRight = 0;
	m_clipBottom = 0;
}

SpriteClass::SpriteClass(const SpriteClass& other)
{
}

SpriteClass::~SpriteClass()
{
}

bool SpriteClass::Initialize(TextureManager* pTexManager, unsigned int texID)
{
#ifdef WIN32
	m_Texture = pTexManager->GetTexture(texID);

	if(!m_Texture)
	{
		return false;
	}

	D3DSURFACE_DESC desc;
	m_Texture->GetLevelDesc(0, &desc);
	m_centerX = (float)desc.Width / 2;
	m_centerY = (float)desc.Height / 2;
	m_clipLeft = 0;
	m_clipTop = 0;
	m_clipRight = desc.Width;
	m_clipBottom = desc.Height;
#endif

	return true;
}

void SpriteClass::SetPosition(float x, float y, float z)
{
	m_positionX = x;
	m_positionY = y;
	m_positionZ = z;
}

void SpriteClass::SetClipRect(long clipLeft, long clipTop, long clipRight, long clipBottom)
{
	m_clipLeft = clipLeft;
	m_clipTop = clipTop;
	m_clipRight = clipRight;
	m_clipBottom = clipBottom;
	m_centerX = (float)abs((m_clipRight - m_clipLeft) / 2);
	m_centerY = (float)abs((m_clipBottom - m_clipTop) / 2);
}

void SpriteClass::Shutdown()
{
	m_Texture = 0;
}

#ifdef WIN32
IDirect3DTexture9* SpriteClass::GetTexture()
{
	return m_Texture;
}

D3DXVECTOR3 SpriteClass::GetPosition()
{
	return D3DXVECTOR3(m_positionX, m_positionY, m_positionZ);
}

D3DXVECTOR2 SpriteClass::GetCenter()
{
	return D3DXVECTOR2(m_centerX, m_centerY);
}

RECT SpriteClass::GetClipRect()
{
	RECT tempRect;

	tempRect.left = m_clipLeft;
	tempRect.top = m_clipTop;
	tempRect.right = m_clipRight;
	tempRect.bottom = m_clipBottom;

	return tempRect;
}
#endif
