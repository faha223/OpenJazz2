#ifndef _TILESET_H_
#define _TILESET_H_

#include "Structures.h"

class Tileset
{
private:
	TILE_Header header;
	uint8_t *Data1;
	uint8_t *Data2;
	uint8_t *Data3;
	uint8_t *Data4;

	J2T_TilesetInfo_1_23 *TilesetInfo_1_23;
	J2T_TilesetInfo_1_24 *TilesetInfo_1_24;

	std::vector<J2T_TileImage> TileImages;
	std::vector<J2T_TileTransparency> TileTransparencyMasks;
	std::vector<J2T_TileClip> TileClippingMasks;

	std::map<uint32_t, uint8_t*> ExpandedClipMasks;

	void ReadTilesetInfo();
	void ReadTileImages();
	void ReadTileTransparency();
	void ReadTileClipping();
public:
	Tileset(const char *filename);
	uint32_t NumTiles() const;
	uint8_t *GetTile(const uint32_t &index, const bool &flipped) const;
	uint8_t *GetClipMask(const uint32_t &index, const bool &flipped) const;
	uint32_t *GetPalette() const;
	~Tileset();
};

#endif