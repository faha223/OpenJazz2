#ifndef _EVENT_H_
#define _EVENT_H_

#include "Structures.h"
#include "Animation.h"
#include <string>
#include <vector>

struct Parameter
{
	std::string Name;
	int32_t Value;
};

enum Difficulty
{
	Easy,
	Normal,
	Hard
};

class Event
{
private:
	EventID id;
	uint32_t TileCoord;
	uint32_t SpriteId;
	Difficulty difficulty;
	bool Illuminate;
	bool Active;
	Difficulty diff;
	std::vector<Parameter> params;
public:	
	Event(const J2L_Event &event, const uint32_t &TileCoord);
	uint32_t GetTileXCoord() const;
	uint32_t GetTileYCoord() const;
	uint32_t GetSprite(const Animations *anims) const;
	void Activate();
};

#endif