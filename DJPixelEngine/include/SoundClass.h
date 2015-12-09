/*
	Filename: SoundClass.h
	Description: Header file for the sound class
*/

#ifndef _SOUNDCLASS_H_
#define _SOUNDCLASS_H_

// Include(s)
#ifdef WIN32
#include <AL/al.h>
#include <AL/alc.h>
#elif __APPLE__
#include <al.h>
#include <alc.h>
#else
#include <AL/al.h>
#include <AL/alc.h>
#endif
#include <vector>
using namespace std;

// Linking
#ifdef WIN32
#pragma comment(lib, "libOpenAL32.dll.a")
#endif

// Constants
#define NUM_SND_BUFFERS 96
#define SND_BUFFER_SIZE 4096
#define NUM_SND_SOURCES 32

/*
	Struct name: WavHeader
*/
struct WavHeader
{
	char chunkId[4];
	unsigned long chunkSize;
	char format[4];
	char subChunkId[4];
	unsigned long subChunkSize;
	unsigned short audioFormat;
	unsigned short numChannels;
	unsigned long sampleRate;
	unsigned long bytesPerSecond;
	unsigned short blockAlign;
	unsigned short bitsPerSample;
	char dataChunkId[4];
	unsigned long dataSize;
};

/*
	Class name: SoundClass
*/
class SoundClass
{
public:
	SoundClass();
	SoundClass(const SoundClass& other);
	~SoundClass();

	bool Initialize();
	void Shutdown();
private:
	ALCdevice *m_device;
	ALCcontext *m_context;

	ALuint m_soundSources[NUM_SND_SOURCES];
	ALuint m_soundBuffers[NUM_SND_BUFFERS];
};

#endif
