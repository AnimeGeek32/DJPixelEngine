/*
	Filename: SoundClass.cpp
	Description: Source implementation of the sound class
*/

#include "SoundClass.h"

SoundClass::SoundClass()
{
	m_device = 0;
	m_context = 0;
}

SoundClass::SoundClass(const SoundClass& other)
{
}

SoundClass::~SoundClass()
{
}

bool SoundClass::Initialize()
{
	// Initialize preferred OpenAL device
	m_device = alcOpenDevice(NULL);

	if(!m_device)
	{
		return false;
	}

	m_context = alcCreateContext(m_device, NULL);
	alcMakeContextCurrent(m_context);

	// Clear error code
	alGetError();

	// Generate sound buffers
	alGenBuffers(NUM_SND_BUFFERS, m_soundBuffers);
	if(alGetError() != AL_NO_ERROR)
	{
		return false;
	}

	// Generate sound sources
	alGenSources(NUM_SND_SOURCES, m_soundSources);
	if(alGetError() != AL_NO_ERROR)
	{
		return false;
	}

	return true;
}

void SoundClass::Shutdown()
{
	m_context = alcGetCurrentContext();
	m_device = alcGetContextsDevice(m_context);
	alcMakeContextCurrent(NULL);
	alcDestroyContext(m_context);
	alcCloseDevice(m_device);
}
