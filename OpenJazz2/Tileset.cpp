#include "Tileset.h"
#include "FileIO.h"
using namespace std;

void Tileset::ReadTilesetInfo()
{
	if (Data1 != nullptr)
	{
		switch (header.Version)
		{
		case TILE_VERSION_1_23:
			TilesetInfo_1_23 = new J2T_TilesetInfo_1_23();
			ReadJ2T_TilesetInfo_1_23(TilesetInfo_1_23, Data1);
			break;
		case TILE_VERSION_1_24:
			TilesetInfo_1_24 = new J2T_TilesetInfo_1_24();
			ReadJ2T_TilesetInfo_1_24(TilesetInfo_1_24, Data1);
			break;
		}
		delete[] Data1;
		Data1 = nullptr;
	}
}

void Tileset::ReadTileImages()
{
	if (Data2 != nullptr)
	{
		std::map<uint32_t, uint32_t> ImageIndexMap;
		if (TilesetInfo_1_23 != nullptr)
		{
			for (uint32_t i = 0; i < MAX_TILES_1_23; i++)
			{
				if (ImageIndexMap.count(TilesetInfo_1_23->ImageAddress[i]))
				{
					TilesetInfo_1_23->ImageAddress[i] = ImageIndexMap[TilesetInfo_1_23->ImageAddress[i]];
				}
				else
				{
					J2T_TileImage image;
					memcpy(image.ImageData, &Data2[TilesetInfo_1_23->ImageAddress[i]], 1024);
					uint32_t newIndex = TileImages.size();
					ImageIndexMap[TilesetInfo_1_23->ImageAddress[i]] = newIndex;
					TilesetInfo_1_23->ImageAddress[i] = newIndex;
					TileImages.push_back(image);
				}
			}
		}
		else if (TilesetInfo_1_24 != nullptr)
		{
			for (uint32_t i = 0; i < MAX_TILES_TSF; i++)
			{
				if (ImageIndexMap.count(TilesetInfo_1_24->ImageAddress[i]))
				{
					TilesetInfo_1_24->ImageAddress[i] = ImageIndexMap[TilesetInfo_1_24->ImageAddress[i]];
				}
				else
				{
					J2T_TileImage image;
					memcpy(image.ImageData, &Data2[TilesetInfo_1_24->ImageAddress[i]], 1024);
					uint32_t newIndex = TileImages.size();
					ImageIndexMap[TilesetInfo_1_24->ImageAddress[i]] = newIndex;
					TilesetInfo_1_24->ImageAddress[i] = newIndex;
					TileImages.push_back(image);
				}
			}
		}
		delete[] Data2;
		Data2 = nullptr;
	}
}

void Tileset::ReadTileTransparency()
{
	if (Data3 != nullptr)
	{
		std::map<uint32_t, uint32_t> ImageIndexMap;
		if (TilesetInfo_1_23 != nullptr)
		{
			for (uint32_t i = 0; i < MAX_TILES_1_23; i++)
			{
				if (ImageIndexMap.count(TilesetInfo_1_23->TMaskAddress[i]))
				{
					TilesetInfo_1_23->TMaskAddress[i] = ImageIndexMap[TilesetInfo_1_23->TMaskAddress[i]];
				}
				else
				{
					J2T_TileTransparency image;
					memcpy(image.TransparencyMask, &Data3[TilesetInfo_1_23->TMaskAddress[i]], 128);
					uint32_t newIndex = TileTransparencyMasks.size();
					ImageIndexMap[TilesetInfo_1_23->TMaskAddress[i]] = newIndex;
					TilesetInfo_1_23->TMaskAddress[i] = newIndex;
					TileTransparencyMasks.push_back(image);
				}
			}
		}
		else if (TilesetInfo_1_24 != nullptr)
		{
			for (uint32_t i = 0; i < MAX_TILES_TSF; i++)
			{
				if (ImageIndexMap.count(TilesetInfo_1_24->TMaskAddress[i]))
				{
					TilesetInfo_1_24->TMaskAddress[i] = ImageIndexMap[TilesetInfo_1_24->TMaskAddress[i]];
				}
				else
				{
					J2T_TileTransparency image;
					memcpy(image.TransparencyMask, &Data3[TilesetInfo_1_24->TMaskAddress[i]], 128);
					uint32_t newIndex = TileTransparencyMasks.size();
					ImageIndexMap[TilesetInfo_1_24->TMaskAddress[i]] = newIndex;
					TilesetInfo_1_24->TMaskAddress[i] = newIndex;
					TileTransparencyMasks.push_back(image);
				}
			}
		}
		delete[] Data3;
		Data3 = nullptr;
	}
}

void Tileset::ReadTileClipping()
{
	if (Data4 != nullptr)
	{
		std::map<uint32_t, uint32_t> ImageIndexMap;
		if (TilesetInfo_1_23 != nullptr)
		{
			for (uint32_t i = 0; i < MAX_TILES_1_23; i++)
			{
				if (ImageIndexMap.count(TilesetInfo_1_23->MaskAddress[i]))
				{
					TilesetInfo_1_23->MaskAddress[i] = ImageIndexMap[TilesetInfo_1_23->MaskAddress[i]];
				}
				else
				{
					J2T_TileClip image;
					memcpy(image.ClippingMask, &Data4[TilesetInfo_1_23->MaskAddress[i]], 128);
					uint32_t newIndex = TileClippingMasks.size();
					ImageIndexMap[TilesetInfo_1_23->MaskAddress[i]] = newIndex;
					TilesetInfo_1_23->MaskAddress[i] = newIndex;
					TileClippingMasks.push_back(image);
				}
			}
			for (uint32_t i = 0; i < MAX_TILES_1_23; i++)
			{
				if (ImageIndexMap.count(TilesetInfo_1_23->FMaskAddress[i]))
				{
					TilesetInfo_1_23->FMaskAddress[i] = ImageIndexMap[TilesetInfo_1_23->FMaskAddress[i]];
				}
				else
				{
					J2T_TileClip image;
					memcpy(image.ClippingMask, &Data4[TilesetInfo_1_23->FMaskAddress[i]], 128);
					uint32_t newIndex = TileClippingMasks.size();
					ImageIndexMap[TilesetInfo_1_23->FMaskAddress[i]] = newIndex;
					TilesetInfo_1_23->FMaskAddress[i] = newIndex;
					TileClippingMasks.push_back(image);
				}
			}
		}
		else if (TilesetInfo_1_24 != nullptr)
		{
			for (uint32_t i = 0; i < MAX_TILES_TSF; i++)
			{
				if (ImageIndexMap.count(TilesetInfo_1_24->MaskAddress[i]))
				{
					TilesetInfo_1_24->MaskAddress[i] = ImageIndexMap[TilesetInfo_1_24->MaskAddress[i]];
				}
				else
				{
					J2T_TileClip image;
					memcpy(image.ClippingMask, &Data4[TilesetInfo_1_24->MaskAddress[i]], 128);
					uint32_t newIndex = TileClippingMasks.size();
					ImageIndexMap[TilesetInfo_1_24->MaskAddress[i]] = newIndex;
					TilesetInfo_1_24->MaskAddress[i] = newIndex;
					TileClippingMasks.push_back(image);
				}
			}
			for (uint32_t i = 0; i < MAX_TILES_TSF; i++)
			{
				if (ImageIndexMap.count(TilesetInfo_1_24->FMaskAddress[i]))
				{
					TilesetInfo_1_24->FMaskAddress[i] = ImageIndexMap[TilesetInfo_1_24->FMaskAddress[i]];
				}
				else
				{
					J2T_TileClip image;
					memcpy(image.ClippingMask, &Data4[TilesetInfo_1_24->FMaskAddress[i]], 128);
					uint32_t newIndex = TileClippingMasks.size();
					ImageIndexMap[TilesetInfo_1_24->FMaskAddress[i]] = newIndex;
					TilesetInfo_1_24->FMaskAddress[i] = newIndex;
					TileClippingMasks.push_back(image);
				}
			}
		}
		delete[] Data4;
		Data4 = nullptr;
	}
}

Tileset::Tileset(const char *filename) :
TilesetInfo_1_23(nullptr),
TilesetInfo_1_24(nullptr)
{
	FILE *fi = openFile(filename, "rb");
	if (fi == nullptr)
	{
		return;
	}
	ReadTILEHeader(fi, &header);

	uint8_t *compressed = new uint8_t[header.CData1];
	fread(compressed, 1, header.CData1, fi);
	Data1 = new uint8_t[header.UData1];
	uLongf ucompSize = (uLongf)header.UData1;
	uncompress(Data1, &ucompSize, compressed, header.CData1);
	delete[] compressed;

	ReadTilesetInfo();

	compressed = new uint8_t[header.CData2];
	fread(compressed, 1, header.CData2, fi);
	Data2 = new uint8_t[header.UData2];
	ucompSize = (uLongf)header.UData2;
	uncompress(Data2, &ucompSize, compressed, header.CData2);
	delete[] compressed;

	ReadTileImages();

	compressed = new uint8_t[header.CData3];
	fread(compressed, 1, header.CData3, fi);
	Data3 = new uint8_t[header.UData3];
	ucompSize = (uLongf)header.UData3;
	uncompress(Data3, &ucompSize, compressed, header.CData3);
	delete[] compressed;

	ReadTileTransparency();

	compressed = new uint8_t[header.CData4];
	fread(compressed, 1, header.CData4, fi);
	Data4 = new uint8_t[header.UData4];
	ucompSize = (uLongf)header.UData4;
	uncompress(Data4, &ucompSize, compressed, header.CData4);
	delete[] compressed;

	ReadTileClipping();

	fclose(fi);
}

uint32_t Tileset::NumTiles() const
{
	if (TilesetInfo_1_23 != nullptr)
		return TilesetInfo_1_23->TileCount;
	else if (TilesetInfo_1_24 != nullptr)
		return TilesetInfo_1_24->TileCount;
	return 0;
}

static uint8_t bitmap[8] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };

uint8_t *Tileset::GetTile(const uint32_t &index, const bool &flipped) const
{
	if (index >= NumTiles())
		return nullptr;
	uint32_t ImageIndex = -1;
	uint32_t TransparencyIndex = -1;
	if (TilesetInfo_1_23 != nullptr)
	{
		ImageIndex = TilesetInfo_1_23->ImageAddress[index];
		TransparencyIndex = TilesetInfo_1_23->TMaskAddress[index];
	}
	if (TilesetInfo_1_24 != nullptr)
	{
		ImageIndex = TilesetInfo_1_23->ImageAddress[index];
		TransparencyIndex = TilesetInfo_1_23->TMaskAddress[index];
	}
	J2T_TileImage image = TileImages[ImageIndex];
	J2T_TileTransparency transparency = TileTransparencyMasks[TransparencyIndex];
	if (TilesetInfo_1_23 != nullptr)
	{
		uint32_t *Pixels = new uint32_t[1024];
		for (int i = 0; i < 1024; i++)
		{
			Pixels[i] = TilesetInfo_1_23->PaletteColor[image.ImageData[i]];
		}
		for (int i = 0; i < 1024; i++)
		{
			Pixels[i] |= (transparency.TransparencyMask[i / 8] & bitmap[i % 8]) ? CHANNEL_ALPHA : 0x00000000;
		}
		if (flipped)
		{
			for (int i = 0; i < 32; i++)
			{
				for (int j = 0; j < 16; j++)
				{
					swap(Pixels[32 * i + j], Pixels[32 * i + (31 - j)]);
				}
			}
		}
		return (uint8_t*)Pixels;
	}
	else if (TilesetInfo_1_24 != nullptr)
	{
		uint32_t *Pixels = new uint32_t[1024];
		for (int i = 0; i < 1024; i++)
		{
			Pixels[i] = TilesetInfo_1_24->PaletteColor[image.ImageData[i]];
		}
		for (int i = 0; i < 1024; i++)
		{
			Pixels[i] |= (transparency.TransparencyMask[i / 8] & bitmap[i % 8]) ? CHANNEL_ALPHA : 0x00000000;
		}
		if (flipped)
		{
			for (int i = 0; i < 32; i++)
			{
				for (int j = 0; j < 16; j++)
				{
					swap(Pixels[32 * i + j], Pixels[32 * i + (31 - j)]);
				}
			}
		}
		return (uint8_t*)Pixels;
	}
	return nullptr;
}

uint8_t *Tileset::GetClipMask(const uint32_t &index, const bool &flipped) const
{
	if ((index >= NumTiles()) || (index < 0))
	{
		return nullptr;
	}
	uint32_t ClipIndex = -1;
	if (TilesetInfo_1_23 != nullptr)
	{
		ClipIndex = TilesetInfo_1_23->MaskAddress[index];
	}
	if (TilesetInfo_1_24 != nullptr)
	{
		ClipIndex = TilesetInfo_1_24->MaskAddress[index];
	}
	J2T_TileClip clip = TileClippingMasks[ClipIndex];
	uint8_t *Pixels = new uint8_t[1024];
	for (int i = 0; i < 1024; i++)
	{
		int row = i / 32;
		int col = (flipped) ? (i%32) : (31 - (i % 32));
		Pixels[(32 * row) + col] = (clip.ClippingMask[i / 8] & bitmap[i % 8]) ? 0xFF : 0x00;
	}
	//if (flipped)
	//{
	//	for (int i = 0; i < 32; i++)
	//	{
	//		for (int j = 0; j < 16; j++)
	//		{
	//			swap(Pixels[32 * i + j], Pixels[32 * i + (31 - j)]);
	//		}
	//	}
	//}
	return Pixels;
}

uint32_t *Tileset::GetPalette() const
{
	uint32_t *palette = new uint32_t[256];
	if (TilesetInfo_1_23 != nullptr)
	{
		memcpy(palette, TilesetInfo_1_23->PaletteColor, 256 * sizeof(uint32_t));
	}
	else if (TilesetInfo_1_24 != nullptr)
	{
		memcpy(palette, TilesetInfo_1_24->PaletteColor, 256 * sizeof(uint32_t));
	}
	return palette;
}

Tileset::~Tileset()
{
	if (Data1 != nullptr)
	{
		delete[] Data1;
		Data1 = nullptr;
	}
	if (Data2 != nullptr)
	{
		delete[] Data2;
		Data2 = nullptr;
	}
	if (Data3 != nullptr)
	{
		delete[] Data3;
		Data3 = nullptr;
	}
	if (Data4 != nullptr)
	{
		delete[] Data4;
		Data4 = nullptr;
	}
	if (TilesetInfo_1_23 != nullptr)
	{
		delete[] TilesetInfo_1_23;
		TilesetInfo_1_23 = nullptr;
	}
	if (TilesetInfo_1_24 != nullptr)
	{
		delete[] TilesetInfo_1_24;
		TilesetInfo_1_24 = nullptr;
	}
	for (map<uint32_t, uint8_t*>::iterator it = ExpandedClipMasks.begin(); it != ExpandedClipMasks.end(); it++)
	{
		delete[] it->second;
	}
}

