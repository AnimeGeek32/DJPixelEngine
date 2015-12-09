/*
	Filename: TextureManager.cpp
	Description: Source implementation of the texture manager
*/

#include "TextureManager.h"

TextureManager::TextureManager()
{
}

TextureManager::TextureManager(const TextureManager& other)
{
}

TextureManager::~TextureManager()
{
}

#ifdef WIN32
bool TextureManager::AddTexture(D3DClass* pD3DClass, LPCTSTR pSrcFile)
{
	HRESULT result;
	LPDIRECT3DTEXTURE9 newTexture;

	// Load the texture into memory
	result = D3DXCreateTextureFromFile(pD3DClass->GetD3DDevice(), pSrcFile, &newTexture);
	if(FAILED(result))
	{
		return false;
	}

	m_Textures.push_back(newTexture);

	return true;
}

IDirect3DTexture9* TextureManager::GetTexture(unsigned int texID)
{
	if(texID >= m_Textures.size())
	{
		return 0;
	}
	else
	{
		return m_Textures[texID];
	}
}
#endif

void TextureManager::Shutdown()
{
#ifdef WIN32
	while(m_Textures.size() > 0)
	{
		LPDIRECT3DTEXTURE9 tempTex = m_Textures.back();
		tempTex->Release();
		tempTex = 0;
		m_Textures.pop_back();
	}
#endif
}
