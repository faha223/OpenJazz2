#ifndef _MIXER_H_
#define _MIXER_H_

#include <SDL2/SDL_mixer.h>
#include <cstdint>
#include <vector>

class Mixer
{
private:
	std::vector<Mix_Chunk*> soundBytes;
	std::vector<Mix_Music*> musicBytes;
public:
	Mixer();
	~Mixer();
	uint32_t LoadChunk(Mix_Chunk *chunk);
	void PlayMusic(const uint32_t &MusicId);
	void PlaySound(const uint32_t &SoundId);
};

#endif