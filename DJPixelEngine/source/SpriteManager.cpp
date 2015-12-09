/*
	Filename: SpriteManager.cpp
	Description: Source implementation of sprite manager
*/

#include "SpriteManager.h"

SpriteManager::SpriteManager()
{
}

SpriteManager::SpriteManager(const SpriteManager& other)
{
}

SpriteManager::~SpriteManager()
{
}

void SpriteManager::AddSprite(SpriteClass* spriteToAdd)
{
	m_sprites.push_back(spriteToAdd);
}

void SpriteManager::Shutdown()
{
	while(m_sprites.size() > 0)
	{
		int lastSprite = m_sprites.size() - 1;
		m_sprites[lastSprite]->Shutdown();
		delete m_sprites[lastSprite];
		m_sprites[lastSprite] = 0;
		m_sprites.pop_back();
	}

	m_sprites.clear();
}

SpriteClass* SpriteManager::GetSprite(unsigned int index)
{
	return m_sprites[index];
}

int SpriteManager::GetSize()
{
	return m_sprites.size();
}
