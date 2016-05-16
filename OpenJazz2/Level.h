#ifndef _LEVEL_H_
#define _LEVEL_H_

#include "Structures.h"

class Tileset;

struct J2L_Tile
{
	int32_t index;
	bool flipped;
};

struct TileCoord
{
	uint32_t x;
	uint32_t y;
};

class Level
{
private:
	LEVL_Header header;

	J2L_Data1_1_23 *Data1_1_23;
	J2L_Data1_TSF *Data1_TSF;
	J2L_Data1_AGA *Data1_AGA;
	J2L_Data2_1_23 *Data2_1_23;
	J2L_Data2_AGA *Data2_AGA;
	J2L_Data3 *Data3_1_23;
	J2L_Data4 *Data4_1_23;

	uint8_t *TileFrame;

	void LoadData1(uint8_t *Data1);
	void LoadData2(uint8_t *Data2);
	void LoadData3(uint8_t *Data3);
	void LoadData4(uint8_t *Data4);
public:
	Level(const char *filename);
	Level(const Level &other);

	Level operator =(const Level &other);

	std::string GetName() const;
	std::string GetTileset() const;
	std::string GetMusic() const;
	std::string GetNextLevel() const;

	uint32_t GetLayerWidth(const uint32_t &layer) const;
	uint32_t GetLayerHeight(const uint32_t &layer) const;
	float GetLayerXSpeed(const uint32_t &layer) const;
	float GetLayerYSpeed(const uint32_t &layer) const;
	float GetLayerAutoXSpeed(const uint32_t &layer) const;
	float GetLayerAutoYSpeed(const uint32_t &layer) const;
	int32_t GetLayerZ(const uint32_t &layer) const;
	uint32_t GetTileCount(const uint32_t &layer) const;
	J2L_Tile GetTile(const uint32_t &layer, const uint32_t &tileXCoord, const uint32_t &tileYCoord, const float &timeElapsed) const;
	TileCoord GetPlayerStart(bool Multiplayer = false);
	bool IsLayerTiledHorizontally(const uint32_t &layer) const;
	bool IsLayerTiledVertically(const uint32_t &layer) const;
	bool IsLayerLimitedToVisibleRegion(const uint32_t &layer) const;
	void SetTileFrame(const uint32_t &tilecoord, const uint8_t &newVal);	// This allows the user to destroy destructible tiles
	J2L_Event GetEvents(const uint32_t &tileXCoord, const uint32_t &tileYCoord) const;
	bool IsWalkable(const uint32_t &tileXCoord, const uint32_t &tileYCoord, const Tileset *tileset) const;

	//void DumpLayerTilesRaw(const uint32_t &layer, const char *filename) const;
	~Level();
};

#endif