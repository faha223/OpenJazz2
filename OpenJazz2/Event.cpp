#include "Event.h"
using namespace std;

#define SuckerTubeSpeedScale 50

Parameter::Parameter()
{
	Name = "";
	Value = 0;
}

Parameter::Parameter(const Parameter &p)
{
	Name = p.Name;
	Value = p.Value;
}

Parameter::Parameter(const string & name, const int32_t & value)
{
	Name = name;
	Value = value;
}

Event::Event(const J2L_Event &event) : 
id((EventID)event.EventID),
SpriteId(0),
Illuminate(false),
Active(false),
difficulty(Normal)
{
	if (event.EventID == None) // Check for No Event
		return;

	switch(event.EventData[0] & 0xC0)
	{
		case 0x40:
			difficulty = Easy;
			break;
		case 0x80:
			difficulty = Normal;
			break;
		case 0x60:
			difficulty = Hard;
			break;
		default:
			difficulty = Normal;
			break;
	}

	Illuminate = (event.EventData[0] & 0x20) != 0;

	int8_t sbyte = 0;
	FILE *fp = nullptr;
	switch(id)
	{
	case SuckerTube:
		Active = true;
		difficulty = Normal;
		Illuminate = false;

#pragma region Get Parameters

		sbyte = (((0xF0 & event.EventData[0]) >> 4) | ((0x07 & event.EventData[1]) << 4) | ((0x04 & event.EventData[1]) << 5));
		params.push_back(Parameter("X-Velocity", ((int32_t)sbyte) * SuckerTubeSpeedScale));
		sbyte = (((0x03 & event.EventData[2]) << 5) | ((0xF8 & event.EventData[1]) >> 3) | ((0x02 & event.EventData[2]) << 6));
		params.push_back(Parameter("Y-Velocity", ((int32_t)sbyte) * SuckerTubeSpeedScale));

#pragma endregion Get Parameters

		break;
	default:
		break;
	}
}

const Animation *Event::GetSprite(const Animations *anims) const
{
	switch (id)
	{
	case SilverCoin:
		return anims->GetAnimSet(ANIM_SET_ITEMS)->GetAnim(ANIM_SILVER_COIN);
	default:
		return nullptr; 
	}
}

bool Event::IsActive() const
{
	return Active;
}

float Event::GetXVelocity() const
{
	for (auto it = params.begin(); it != params.end(); it++)
	{
		if (it->Name == "X-Velocity")
			return (float)it->Value;
	}
	return 0;
}

float Event::GetYVelocity() const
{
	for (auto it = params.begin(); it != params.end(); it++)
	{
		if (it->Name == "Y-Velocity")
			return (float)it->Value;
	}
	return 0;
}