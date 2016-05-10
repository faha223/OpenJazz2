#include "Event.h"
using namespace std;

Event::Event(const J2L_Event &event, const uint32_t &TileCoord) : 
id((EventID)event.EventID),
TileCoord(TileCoord),
SpriteId(0),
Illuminate(false),
Active(false),
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
	Illuminate = (event.EventData[0] & 0x20);
	Active = (event.EventData[0] & 0x10);
	switch(id)
	{
		default:
			break;
	}
}