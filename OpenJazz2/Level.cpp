#include "Level.h"
#include "FileIO.h"
#include "Math.h"

Level::Level(const char *filename) :
Data1_1_23(nullptr),
Data1_TSF(nullptr),
Data1_AGA(nullptr),
Data2_1_23(nullptr),
Data2_AGA(nullptr),
Data3_1_23(nullptr),
TileFrame(nullptr)
{
	FILE *fi = openFile(filename, "rb");
	if (fi == nullptr)
		return;
	//fread(&header, sizeof(LEVL_Header), 1, fi);

	ReadLEVLHeader(fi, &header);

	uint8_t *buffer = new uint8_t[(uint32_t)header.UData1];
	uint8_t *compressed = new uint8_t[(uint32_t)header.CData1];
	fread(compressed, sizeof(uint8_t), (uint32_t)header.CData1, fi);
	uLongf uncompressedSize = (uLongf)header.UData1;
	uncompress(buffer, &uncompressedSize, compressed, header.CData1);
	delete[] compressed;

	LoadData1(buffer);
	delete[] buffer;

	buffer = new uint8_t[(uint32_t)header.UData2];
	compressed = new uint8_t[(uint32_t)header.CData2];
	fread(compressed, sizeof(uint8_t), (uint32_t)header.CData2, fi);
	uncompressedSize = (uLongf)header.UData2;
	uncompress(buffer, &uncompressedSize, compressed, header.CData2);
	delete[] compressed;

	LoadData2(buffer);
	delete[] buffer;

	buffer = new uint8_t[(uint32_t)header.UData3];
	compressed = new uint8_t[(uint32_t)header.CData3];
	fread(compressed, sizeof(uint8_t), (uint32_t)header.CData3, fi);
	uncompressedSize = (uLongf)header.UData3;
	uncompress(buffer, &uncompressedSize, compressed, header.CData3);
	delete[] compressed;

	LoadData3(buffer);
	delete[] buffer;

	buffer = new uint8_t[(uint32_t)header.UData4];
	compressed = new uint8_t[(uint32_t)header.CData4];
	fread(compressed, sizeof(uint8_t), (uint32_t)header.CData4, fi);
	uncompressedSize = (uLongf)header.UData4;
	uncompress(buffer, &uncompressedSize, compressed, header.CData4);
	delete[] compressed;

	LoadData4(buffer);
	delete[] buffer;

	TileFrame = new uint8_t[GetLayerWidth(3) * GetLayerHeight(3)];
	memset(TileFrame, 0, GetLayerWidth(3) * GetLayerHeight(3));

	fclose(fi);
}

Level::Level(const Level &other) :
Data1_1_23(nullptr), 
Data1_TSF(nullptr),
Data1_AGA(nullptr), 
Data2_1_23(nullptr), 
Data2_AGA(nullptr),
Data3_1_23(nullptr), 
Data4_1_23(nullptr)
{
	memcpy(&header, &other.header, sizeof(LEVL_Header));

	if (other.Data1_1_23 != nullptr)
	{
		Data1_1_23 = new J2L_Data1_1_23();
		memcpy(Data1_1_23, other.Data1_1_23, sizeof(J2L_Data1_1_23));
	}
	else if (other.Data1_TSF != nullptr)
	{
		Data1_TSF = new J2L_Data1_TSF();
		memcpy(Data1_TSF, other.Data1_TSF, sizeof(J2L_Data1_TSF));
	}
	else if (other.Data1_AGA != nullptr)
	{
		Data1_AGA = new J2L_Data1_AGA();
		memcpy(Data1_AGA, other.Data1_AGA, sizeof(J2L_Data1_AGA));
	}

	if (other.Data2_1_23 != nullptr)
	{
		Data2_1_23 = new J2L_Data2_1_23(*other.Data2_1_23);
	}
	else if (other.Data2_AGA != nullptr)
	{
		Data2_AGA = new J2L_Data2_AGA(*other.Data2_AGA);
	}
}

void Level::LoadData1(uint8_t *Data1)
{
	if (Data1 != nullptr)
	{
		switch (header.Version)
		{
		case VERSION_1_23:
			Data1_1_23 = new J2L_Data1_1_23();
			ReadJ2L_Data_1_23(Data1_1_23, Data1);
			break;
		case VERSION_TSF:
			Data1_TSF = new J2L_Data1_TSF();
			ReadJ2L_Data_TSF(Data1_TSF, Data1);
			break;
		case VERSION_GIGANTIC:
			Data1_AGA = new J2L_Data1_AGA();
			ReadJ2L_Data_AGA(Data1_AGA, Data1);
			break;
		}
	}
}

void Level::LoadData2(uint8_t *Data2)
{
	if (Data2 != nullptr)
	{
		switch (header.Version)
		{
		case VERSION_1_23:
		case VERSION_TSF:
			Data2_1_23 = new J2L_Data2_1_23();
			ReadJ2L_Data2_1_23(Data2_1_23, Data2, header.UData2);
			break;
		case VERSION_GIGANTIC:
			Data2_AGA = new J2L_Data2_AGA();
			ReadJ2L_Data2_1_23(Data2_1_23, Data2, header.UData2);
			break;
		}
	}
}

void Level::LoadData3(uint8_t *Data3)
{
	if (Data3 != nullptr)
	{
		Data3_1_23 = new J2L_Data3();

		for (uint32_t i = 0; i < header.UData3; i += 8)
		{
			Word w;
			memcpy((char*)w.Tiles, &Data3[i], 4 * sizeof(uint16_t));
			Data3_1_23->Dictionary.push_back(w);
		}
	}
}

void Level::LoadData4(uint8_t *Data4)
{
	if (Data4 != nullptr)
	{
		Data4_1_23 = new J2L_Data4();
		int offset = 0;
		for (int i = 0; i < 8; i++)
		{
			bool TilesDefinedForLayer = false;
			uint32_t LayerWidth = 0;
			uint32_t LayerHeight = 0;
			if (Data1_1_23 != nullptr)
			{
				TilesDefinedForLayer = Data1_1_23->DoesLayerHaveAnyTiles[i];
				LayerWidth = Data1_1_23->LayerRealWidth[i];
				LayerHeight = Data1_1_23->LayerHeight[i];
			}
			else if (Data1_TSF != nullptr)
			{
				TilesDefinedForLayer = Data1_TSF->DoesLayerHaveAnyTiles[i];
				LayerWidth = Data1_TSF->LayerRealWidth[i];
				LayerHeight = Data1_TSF->LayerHeight[i];
			}
			else if (Data1_AGA != nullptr)
			{
				TilesDefinedForLayer = Data1_AGA->DoesLayerHaveAnyTiles[i];
				LayerWidth = Data1_AGA->LayerRealWidth[i];
				LayerHeight = Data1_AGA->LayerHeight[i];
			}
			if (TilesDefinedForLayer)
			{
				uint32_t wordWidth = LayerWidth / 4;
				if ((LayerWidth % 4) > 0)
					wordWidth += 1;
				for (uint32_t s = 0; s < LayerHeight; s++)
				{
					for (uint32_t t = 0; t < wordWidth; t++)
					{
						uint16_t word;
						ReadAndAdvance(word, Data4, offset);
						Data4_1_23->Layers[i].push_back(word);
					}
				}
			}
		}
	}
}

std::string Level::GetMusic() const
{
	if (Data1_1_23 != nullptr)
	{
		return std::string(Data1_1_23->MusicFile) + std::string(".j2b");
	}
	else if (Data1_TSF != nullptr)
	{
		return std::string(Data1_TSF->MusicFile) + std::string(".j2b");
	}
	else if (Data1_AGA != nullptr)
	{
		return std::string(Data1_AGA->MusicFile) + std::string(".j2b");
	}
	return std::string();
}

std::string Level::GetTileset() const
{
	if (Data1_1_23 != nullptr)
	{
		return std::string(Data1_1_23->Tileset);
	}
	else if (Data1_TSF != nullptr)
	{
		return std::string(Data1_TSF->Tileset);
	}
	else if (Data1_AGA != nullptr)
	{
		return std::string(Data1_AGA->Tileset);
	}
	return std::string();
}

std::string Level::GetName() const
{
	return std::string(header.LevelName);
}

std::string Level::GetNextLevel() const
{
	std::string level = "";
	if (Data1_1_23 != nullptr)
	{
		level = std::string(Data1_1_23->NextLevel);
	}
	else if (Data1_TSF != nullptr)
	{
		level = std::string(Data1_TSF->NextLevel);
	}
	else if (Data1_AGA != nullptr)
	{
		level = std::string(Data1_AGA->NextLevel);
	}
	if(level.length() > 0)
	{
		level[0] = ((level[0] >= 'a') && (level[0] <= 'z')) ? level[0]  + ('A' - 'a') : level[0];
		for(int i = 1; i < level.length(); i++)
		{
			if((level[i] >= 'A') && (level[i] <= 'Z'))
				level[i] += ('a' - 'A');
		}
	}
	return level;
}

uint32_t Level::GetLayerWidth(const uint32_t &layer) const
{
	if (layer < 8)
	{
		if (Data1_1_23 != nullptr)
			return (Data1_1_23->LayerRealWidth[layer]);
		else if (Data1_TSF != nullptr)
			return (Data1_TSF->LayerRealWidth[layer]);
		else if (Data1_AGA != nullptr)
			return (Data1_AGA->LayerRealWidth[layer]);
	}
	return 0;
}

uint32_t Level::GetLayerHeight(const uint32_t &layer) const
{
	if (layer < 8)
	{
		if (Data1_1_23 != nullptr)
			return (Data1_1_23->LayerHeight[layer]);
		else if (Data1_TSF != nullptr)
			return (Data1_TSF->LayerHeight[layer]);
		else if (Data1_AGA != nullptr)
			return (Data1_AGA->LayerHeight[layer]);
	}
	return 0;
}

float Level::GetLayerXSpeed(const uint32_t &layer) const
{
	if (layer < 7)
	{
		if (Data1_1_23 != nullptr)
		{
			return (float)Data1_1_23->LayerXSpeed[layer] / (float)Data1_1_23->LayerXSpeed[3];
		}
	}
	return 0;
}

float Level::GetLayerYSpeed(const uint32_t &layer) const
{
	if (layer < 7)
	{
		if (Data1_1_23 != nullptr)
		{
			return (float)Data1_1_23->LayerYSpeed[layer] / (float)Data1_1_23->LayerYSpeed[3];
		}
	}
	return 0;
}

int32_t Level::GetLayerZ(const uint32_t &layer) const
{
	if (layer < 8)
	{
		if (Data1_1_23 != nullptr)
		{
			return Data1_1_23->LayerZAxis[layer];
		}
	}
	return -INT_MAX;
}

uint32_t Level::GetTileCount(const uint32_t &layer) const
{
	if (layer < 8)
	{
		if (Data1_1_23 != nullptr)
			return (Data1_1_23->DoesLayerHaveAnyTiles[layer]) ? Data1_1_23->LayerRealWidth[layer] * Data1_1_23->LayerHeight[layer] : 0;
		else if (Data1_AGA != nullptr)
			return (Data1_AGA->DoesLayerHaveAnyTiles[layer]) ? Data1_AGA->LayerRealWidth[layer] * Data1_AGA->LayerHeight[layer] : 0;
		else if (Data1_TSF != nullptr)
			return (Data1_TSF->DoesLayerHaveAnyTiles[layer]) ? Data1_TSF->LayerRealWidth[layer] * Data1_TSF->LayerHeight[layer] : 0;
	}
	return 0;
}

J2L_Tile Level::GetTile(const uint32_t &layer, const uint32_t &tileXCoord, const uint32_t &tileYCoord, const float &timeElapsed) const
{
	uint32_t stride = GetLayerWidth(layer);
	stride += (((stride % 4) > 0) ? (4 - (stride % 4)) : 0);
	int32_t ind = (tileYCoord * stride) + tileXCoord;

	J2L_Tile tile;
	tile.flipped = false;
	tile.index = -1;
	if (layer > 7)
		return tile;
	if (Data4_1_23 != nullptr)
	{
		uint16_t bitmask = ~((header.Version == VERSION_1_23) ? FLIPPED_1_23 : FLIPPED_TSF);
		int DictionaryIndex = Data4_1_23->Layers[layer][ind/4];
		tile.index = Data3_1_23->Dictionary[DictionaryIndex].Tiles[ind % 4];
		if (tile.index & ~bitmask)
		{
			tile.index &= bitmask;
			tile.flipped = true;
		}
		if (tile.index >= Data1_1_23->AnimOffset)
		{
			auto anim = Data1_1_23->Anim[tile.index - Data1_1_23->AnimOffset];
			if (anim.PingPong)
			{
				int frame = (int)(Math::Round(anim.Speed * max(timeElapsed, 0.0f))) % (2*anim.FrameCount - 2);
				if (frame >= anim.FrameCount)
				{
					frame = (2 * anim.FrameCount) - frame - 1;
				}
				tile.index = anim.Frame[frame];
			}
			else
			{
				int frame = (int)(Math::Round(anim.Speed * max(timeElapsed, 0.0f))) % anim.FrameCount;
				if((anim.Speed == 0) && (layer == 3))
				{
					frame = TileFrame[ind];
				}
				tile.index = anim.Frame[frame];
			}
			if (tile.index & ~bitmask)
			{
				tile.index &= bitmask;
				tile.flipped = !tile.flipped;
			}
		}
	}
	return tile;
}

void Level::SetTileFrame(const uint32_t &tile, const uint8_t &newVal)
{
	printf("Setting Tile %d to frame %d\n", tile, newVal);
	TileFrame[tile] = newVal;
}

TileCoord Level::GetPlayerStart(bool Multiplayer)
{
	TileCoord coord;
	coord.x = 0;
	coord.y = 0;
	for (uint32_t i = 0; i < (Data1_1_23->LayerRealWidth[3] * Data1_1_23->LayerHeight[3]); i++)
	{
		
		if (Multiplayer)
		{
			if (Data2_1_23->Events[i].EventID == MultiplayerLevelStart)
			{
				coord.x = i % Data1_1_23->LayerRealWidth[3];
				coord.y = i / Data1_1_23->LayerRealWidth[3];
			}
		}
		else
		{
			if ((Data2_1_23->Events[i].EventID == JazzLevelStart) || (Data2_1_23->Events[i].EventID == SpazLevelStart))
			{
				coord.x = i % Data1_1_23->LayerRealWidth[3];
				coord.y = i / Data1_1_23->LayerRealWidth[3];
				break;
			}
		}
	}
	return coord;
}

bool Level::IsLayerTiledHorizontally(const uint32_t &layer) const
{
	if (layer < 8)
	{
		if (Data1_1_23 != nullptr)
		{
			return (Data1_1_23->LayerMiscProperties[layer] & 1) ? true : false;
		}
		else if (Data1_TSF != nullptr)
		{
			return (Data1_TSF->LayerMiscProperties[layer] & 1) ? true : false;
		}
		else if (Data1_AGA != nullptr)
		{
			return (Data1_AGA->LayerMiscProperties[layer] & 1) ? true : false;
		}
	}
	return false;
}

bool Level::IsLayerTiledVertically(const uint32_t &layer) const
{
	if (layer < 8)
	{
		if (Data1_1_23 != nullptr)
		{
			return (Data1_1_23->LayerMiscProperties[layer] & 2) ? true : false;
		}
		else if (Data1_TSF != nullptr)
		{
			return (Data1_TSF->LayerMiscProperties[layer] & 2) ? true : false;
		}
		else if (Data1_AGA != nullptr)
		{
			return (Data1_AGA->LayerMiscProperties[layer] & 2) ? true : false;
		}
	}
	return false;
}

bool Level::IsLayerLimitedToVisibleRegion(const uint32_t &layer) const
{
	if (layer < 8)
	{
		return ((Data1_1_23->LayerMiscProperties[layer] & 4) ? true : false);
	}
	return true;
}

J2L_Event Level::GetEvents(const uint32_t &tileXCoord, const uint32_t &tileYCoord) const
{
	uint32_t tileCoord = GetLayerWidth(3) * tileYCoord + tileXCoord;
	return Data2_1_23->Events[tileCoord];
}

//void Level::DumpLayerTilesRaw(const uint32_t &layer, const char *filename) const 
//{
//	FILE *fi = openFile(filename, "wb");
//	if (fi != nullptr)
//	{
//		if (GetTileCount(layer) > 0)
//		{
//			for (int i = 0; i < GetLayerHeight(layer); i++)
//			{
//				for (int j = 0; j < GetLayerWidth(layer); j++)
//				{
//					int index = i*GetLayerWidth(layer) + j;
//					int DictionaryIndex = Data4_1_23->Layers[layer][index / 4];
//					uint16_t tile = Data3_1_23->Dictionary[DictionaryIndex].Tiles[index % 4];
//					fprintf(fi, "%d(0x%X), ", tile, tile);
//				}
//				fprintf(fi, "\n");
//			}
//		}
//		fclose(fi);
//	}
//}

Level::~Level()
{
	if (Data1_1_23 != nullptr)
	{
		delete Data1_1_23;
		Data1_1_23 = nullptr;
	}

	if (Data1_TSF != nullptr)
	{
		delete Data1_TSF;
		Data1_TSF = nullptr;
	}

	if (Data1_AGA != nullptr)
	{
		delete Data1_AGA;
		Data1_AGA = nullptr;
	}

	if (Data2_1_23 != nullptr)
	{
		delete Data2_1_23;
		Data2_1_23 = nullptr;
	}

	if (Data2_AGA != nullptr)
	{
		delete Data2_AGA;
		Data2_AGA = nullptr;
	}

	if (Data3_1_23 != nullptr)
	{
		delete Data3_1_23;
		Data3_1_23 = nullptr;
	}

	if (Data4_1_23 != nullptr)
	{
		delete Data4_1_23;
		Data4_1_23 = nullptr;
	}
}
