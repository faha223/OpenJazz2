#ifndef _EVENT_H_
#define _EVENT_H_

#include "Structures.h"
#include "Animation.h"
#include <string>
#include <vector>

class Parameter
{
public:
	Parameter();
	Parameter(const Parameter &p);
	Parameter(const std::string &name, const int32_t &value);
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
	uint32_t SpriteId;
	Difficulty difficulty;
	bool Illuminate;
	bool Active;
	Difficulty diff;
	std::vector<Parameter> params;
public:	
	Event(const J2L_Event &event);
	const Animation *GetSprite(const Animations *anims) const;
	bool IsActive() const;
	float GetXVelocity() const;
	float GetYVelocity() const;
};

#endif