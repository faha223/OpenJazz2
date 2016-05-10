#ifndef _MUSIC_H_
#define _MUSIC_H_

#include <cstdint>
#include <SDL2/SDL_mixer.h>

class Music
{
private:
	uint32_t ID; // Should be 'MUSE' (0x4553554D or 0x4D555345 depending on architecture)
	uint32_t ID2;	// Should be 0xDEADBEAF (RIFF AM) or 0xDEADBABE (RIFF AMFF).
	uint32_t FileSize;
	uint32_t ChkSum;
	uint32_t CompressedSize;
	uint32_t UncompressedSize;
	//uint8_t *CompressedData;
	//uint8_t *UncompressedData;
	Mix_Chunk *chunk;
public:
	Music(const char *filename);
	Mix_Chunk *GetChunk();
	~Music();
};


#endif