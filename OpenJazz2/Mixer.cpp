#include "Mixer.h"

Mixer::Mixer()
{
	if (0 > Mix_Init(MIX_INIT_FLAC | MIX_INIT_MOD | MIX_INIT_MP3 | MIX_INIT_OGG))
	{
		printf("Error Initializing SDL_Mixer: %s\n", SDL_GetError());
		return;
	}
	if (0 > Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096))
	{
		printf("Error Opening Audio Device: %s\n", SDL_GetError());
		return;
	}
}

uint32_t Mixer::LoadChunk(Mix_Chunk *chunk)
{
	soundBytes.push_back(chunk);
	return soundBytes.size() - 1;
}

void Mixer::PlayMusic(const uint32_t &soundId)
{
	Mix_PlayChannel(0, soundBytes[soundId], -1);
}

void Mixer::PlaySound(const uint32_t &soundId)
{
	Mix_PlayChannel(-1, soundBytes[soundId], 0);
}

Mixer::~Mixer()
{
	for (size_t i = 0; i < soundBytes.size(); i++)
	{
		Mix_FreeChunk(soundBytes[i]);
		soundBytes[i] = nullptr;
	}
	soundBytes.clear();
	Mix_CloseAudio();
	Mix_Quit();
}