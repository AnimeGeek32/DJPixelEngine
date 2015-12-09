/*
	Filename: SpriteManager.h
	Description: Header file for the sprite manager
*/

#ifndef _SPRITEMANAGER_H_
#define _SPRITEMANAGER_H_

#include <vector>
#include "SpriteClass.h"
using namespace std;

/*
	Class name: SpriteManager
*/
class SpriteManager
{
public:
	SpriteManager();
	SpriteManager(const SpriteManager& other);
	~SpriteManager();

	void AddSprite(SpriteClass* spriteToAdd);
	void Shutdown();

	SpriteClass* GetSprite(unsigned int index);
	int GetSize();

private:
	vector<SpriteClass*> m_sprites;
};

#endif
