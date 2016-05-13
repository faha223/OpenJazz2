#include "Event.h"
using namespace std;

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
Active(true),
difficulty(Normal)
{
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
	}
	Illuminate = (event.EventData[0] & 0x20) != 0;
	//Active = (event.EventData[0] & 0x10) != 0;

	int32_t xVel = 0;
	int32_t yVel = 0;
	switch(id)
	{
	case SuckerTube:
		difficulty = Normal;
		Illuminate = false;
		if ((event.EventData[0] == 0x60) && (event.EventData[1] == 0x07) && (event.EventData[2] == 0x00))
		{
			xVel = -500;
		}
		else if ((event.EventData[0] == 0xa0) && (event.EventData[1] == 0x00) && (event.EventData[2] == 0x00))
		{
			xVel = 500;
		}
		else if ((event.EventData[0] == 0x00) && (event.EventData[1] == 0xb0) && (event.EventData[2] == 0x03))
		{
			yVel = -500;
		}
		else if((event.EventData[0] == 0x00) && (event.EventData[1] == 0x50) && (event.EventData[2] == 0x00))
		{
			yVel = 500;
		}
		else
		{
			yVel = 0;
			xVel = 0;
		}
		params.push_back(Parameter("X-Velocity", xVel));
		params.push_back(Parameter("Y-Velocity", yVel));
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