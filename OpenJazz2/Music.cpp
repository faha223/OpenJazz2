#include "Music.h"
#include "FileIO.h"

Music::Music(const char *filename) :
ID(0),
ID2(0),
FileSize(0),
ChkSum(0), 
chunk(nullptr)
{
	FILE *fi = openFile(filename, "rb");
	if (fi == nullptr)
	{
		return;
	}

	fread(&ID, sizeof(uint32_t), 1, fi);
	fread(&ID2, sizeof(uint32_t), 1, fi);
	fread(&FileSize, sizeof(uint32_t), 1, fi);
	fread(&ChkSum, sizeof(uint32_t), 1, fi);
	uint32_t CompressedSize = 0;
	uint32_t UncompressedSize = 0;
	fread(&CompressedSize, sizeof(uint32_t), 1, fi);
	fread(&UncompressedSize, sizeof(uint32_t), 1, fi);

	uint8_t *CompressedData = new uint8_t[CompressedSize];
	uint8_t *UncompressedData = new uint8_t[UncompressedSize];

	fread(CompressedData, sizeof(uint8_t), CompressedSize, fi);

	uLongf ucompSize = (uLongf)UncompressedSize;
	uncompress(UncompressedData, &ucompSize, CompressedData, (uLong)CompressedSize);

	chunk = Mix_QuickLoad_RAW(UncompressedData, UncompressedSize);
	delete[] CompressedData;
	delete[] UncompressedData;
}

Mix_Chunk *Music::GetChunk()
{
	return chunk;
}

Music::~Music()
{
	FileSize = 0;
	UncompressedSize = 0;
	CompressedSize = 0;
	if (chunk != nullptr)
	{
		Mix_FreeChunk(chunk);
	}
}