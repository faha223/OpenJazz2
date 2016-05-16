#include "Actor.h"
#include "Player.h"
#include "Level.h"
#include "Tileset.h"
#include <time.h>
#include <SDL2/SDL.h>
using namespace std;

#define ActorFloatingRadius 4.0f  // Number of pixels it oscillates per second
#define ActorFloatingPeriod 6.75f // Number of radians it cycles of oscillation per second
#ifndef FallingGravity
#define FallingGravity 768.0f	  // Measured
#endif

static int32_t nextOffsetPoint = 0;

Actor::Actor(const Level *level, const Tileset *tileset, const vec2 &location, const EventID &eventId, const Animations *anims, const float &TTL) :
	eventId(eventId),
	Location(location),
	Velocity(vec2()),
	anim(nullptr),
	TTL(TTL),
	Age(0),
	DoesNotFloat(false),
	SpeedModifier(1.0f),
	AffectedByGravity(false),
	level(level),
	tileset(tileset),
	lastTileCoord(0),
	quad(nullptr)
{
	const AnimationSet *items = anims->GetAnimSet(ANIM_SET_ITEMS);
	const AnimationSet *springs = anims->GetAnimSet(ANIM_SET_SPRINGS);
	switch (eventId)
	{
	case None:
		return;
	case Apple:
		anim = items->GetAnim(ANIM_APPLE);
		break;
	case Milk:
		anim = items->GetAnim(ANIM_MILK);
		break;
	case Pear:
		anim = items->GetAnim(ANIM_PEAR);
		break;
	case CarrotEnergyPlus1:
		anim = items->GetAnim(ANIM_CARROT);
		break;
	case GoldCoin:
		anim = items->GetAnim(ANIM_GOLD_COIN);
		SpeedModifier = -2.0f;
		break;
	case SilverCoin:
		anim = items->GetAnim(ANIM_SILVER_COIN);
		SpeedModifier = -2.0f;
		break;
	case Cake:
		anim = items->GetAnim(ANIM_CAKE);
		break;
	case Cupcake:
		anim = items->GetAnim(ANIM_CUPCAKE);
		break;
	case Candy:
		anim = items->GetAnim(ANIM_CANDY);
		break;
	case Chocbar:
		anim = items->GetAnim(ANIM_CHOCOLATE);
		break;
	case ChickenLeg:
		anim = items->GetAnim(ANIM_CHICKEN);
		break;
	case RedGemPlus1:
	case PurpleGemPlus1:
	case BlueGemPlus1:
	case GreenGemPlus1:
		anim = items->GetAnim(ANIM_GEM);
		SpeedModifier = -2.0f;
		break;
	case Donut:
		anim = items->GetAnim(ANIM_DONUT);
		break;
	case GemCrate:
		anim = items->GetAnim(ANIM_CRATE);
		DoesNotFloat = true;
		break;
	case ExtraLive:
		anim = items->GetAnim(ANIM_1UP);
		break;
	case Lettuce:
		anim = items->GetAnim(ANIM_LETTUCE);
		break;
	case Watermelon:
		anim = items->GetAnim(ANIM_WATERMELON);
		break;
	case Peach:
		anim = items->GetAnim(ANIM_PEACH);
		break;
	case Sparkle:
		anim = items->GetAnim(ANIM_SPARKLE);
		this->TTL = anim->GetFrameCount() / (float)anim->GetFrameRate();
		DoesNotFloat = true;
		break;
	case RedSpring:
		anim = springs->GetAnim(ANIM_SPRING_UP_RED);
		DoesNotFloat = true;
		break;
	case GreenSpring:
		anim = springs->GetAnim(ANIM_SPRING_UP_GREEN);
		DoesNotFloat = true;
		break;
	case BlueSpring:
		anim = springs->GetAnim(ANIM_SPRING_UP_GREEN);
		DoesNotFloat = true;
		break;
	case HorRedSpring:
		anim = springs->GetAnim(ANIM_SPRING_RT_RED);
		DoesNotFloat = true;
		break;
	case HorGreenSpring:
		anim = springs->GetAnim(ANIM_SPRING_RT_GREEN);
		DoesNotFloat = true;
		break;
	case HorBlueSpring:
		anim = springs->GetAnim(ANIM_SPRING_RT_BLUE);
		DoesNotFloat = true;
		break;
	}
	PeriodInitialOffset = (nextOffsetPoint++) * (ActorFloatingPeriod / 4.0f);
	nextOffsetPoint %= 4;
}

Actor::Actor(const Actor &other) :
	eventId(other.eventId),
	Location(other.Location),
	Velocity(other.Velocity),
	anim(other.anim),
	TTL(other.TTL), Age(other.Age),
	DoesNotFloat(other.DoesNotFloat),
	PeriodInitialOffset(other.PeriodInitialOffset),
	SpeedModifier(other.SpeedModifier),
	AffectedByGravity(other.AffectedByGravity),
	level(other.level),
	tileset(other.tileset),
	lastTileCoord(other.lastTileCoord),
	quad(other.quad)
{
	if (quad != nullptr)
	{
		quad = new uint8_t[96 * 96];
		memcpy(quad, other.quad, 96 * 96);
	}
}

Actor Actor::operator =(const Actor &other)
{
	eventId = other.eventId;
	Location = other.Location;
	Velocity = other.Velocity;
	anim = other.anim;
	TTL = other.TTL;
	Age = other.Age;
	DoesNotFloat = other.DoesNotFloat;
	PeriodInitialOffset = other.PeriodInitialOffset;
	SpeedModifier = other.SpeedModifier;
	AffectedByGravity = other.AffectedByGravity;
	level = other.level;
	tileset = other.tileset;
	lastTileCoord = other.lastTileCoord;
	quad = other.quad;
	if (quad != nullptr)
	{
		quad = new uint8_t[96 * 96];
		memcpy(quad, other.quad, 96 * 96);
	}
	return *this;
}

void Actor::Update(const float &timeElapsed)
{
	Age += timeElapsed;
}

vec2 Actor::GetPosition() const
{
	if (DoesNotFloat)
		return Location;
	return Location + vec2(0.0f, (Math::Sin(Age * ActorFloatingPeriod + PeriodInitialOffset) + 1) * ActorFloatingRadius);
}

EventID Actor::GetEventID() const
{
	return eventId;
}

bool Actor::GetDestroyActor() const
{
	return ((TTL != 0) && (Age >= TTL));
}

uint8_t Actor::GetHealthAdd() const
{
	switch(eventId)
	{
		case CarrotEnergyPlus1:
			return 1;
		default:
			return 0;
	}
}

uint8_t Actor::GetMoneyAdd() const
{
	switch(eventId)
	{
		case GoldCoin:
			return 5;
		case SilverCoin:
			return 1;
		default:
			return 0;
	}
}

uint8_t Actor::GetLivesAdd() const
{
	switch (eventId)
	{
	case ExtraLive:
		return 1;
	default:
		return 0;
	}
}

uint8_t Actor::GetGemValue() const
{
	switch(eventId)
	{
		case RedGemPlus1:
		case GreenGemPlus1:
		case PurpleGemPlus1:
		case BlueGemPlus1:
			return 1;
		default:
			return 0;
	}
}

GemType Actor::GetGemType() const
{
	switch(eventId)
	{
		case RedGemPlus1:
			return GT_Red;
		case GreenGemPlus1:
			return GT_Green;
		case BlueGemPlus1:
			return GT_Blue;
		case PurpleGemPlus1:
			return GT_Purple;
		default:
			return GT_None;
	}
}

bool Actor::IsFlipped() const
{
	switch (eventId)
	{
	case RedSpring:
	case GreenSpring:
	case BlueSpring:
		return false;
	case HorRedSpring:
	case HorGreenSpring:
	case HorBlueSpring:
		return false;
	default:
		return false;
	}
}

uint16_t Actor::GetPointValue() const
{
	switch(eventId)
	{
		case BlueGemPlus1:
		case GoldCoin:
			return 1000;
		case CarrotEnergyPlus1:
		case SilverCoin:
		case GreenGemPlus1:
			return 500;
		case RedGemPlus1:
		case PurpleGemPlus1:
			return 100;
		case ExtraLive:
			return 2000;
		default:
			return isFood() ? 50 : 0;
	}
}

bool Actor::isFood() const
{
	switch(eventId)
	{
		case Apple:
		case Chocbar:
		case Chips:
		case Candy:
		case ChickenLeg:
		case Milk:
		case Pear:
		case Cake:
		case Cupcake:
		case Lettuce:
		case Watermelon:
		case Peach:
		case Donut:
			return true;
		default:
			return false;
	}
}

bool Actor::AddsSparkleOnDeath() const
{
	switch(eventId)
	{
		case BlueGemPlus1:
		case GoldCoin:
		case SilverCoin:
		case GreenGemPlus1:
		case RedGemPlus1:
		case PurpleGemPlus1:
		case ExtraLive:
			return true;
		default:
			return isFood();	
	}
}

const AnimationFrame *Actor::GetFrame() const
{
	if (anim != nullptr)
	{
		uint32_t frameCount = anim->GetFrameCount();
		uint32_t frameRate = anim->GetFrameRate();
		int frame = ((int)Math::Floor(Age * frameRate * SpeedModifier)) % frameCount;
		return anim->GetFrame(max(0, min(frameCount, frame)));
	}
	return nullptr;
}

bool Actor::RenderFromColdSpot() const
{
	// Return true only for the events that are rendered from their coldspot
	switch (eventId)
	{
	case RedSpring:
	case GreenSpring:
	case BlueSpring:
	case GemCrate:
	case Guncrate:
	case BigBox:
		return true;
	default:
		break;
	}
	return false;
}

inline bool HotspotCollision(const Player *player, vec2 hotspot)
{
	SDL_Rect aRect, pRect;
	vec2 playerPos = player->GetPosition();

	pRect.x = playerPos.x - 16;
	pRect.y = playerPos.y - 30;
	pRect.w = 32;
	pRect.h = 32;
	return ((hotspot.x >= pRect.x) && (hotspot.x <= (pRect.x + pRect.w))
		&& (hotspot.y >= pRect.y) && (hotspot.y <= (pRect.y + pRect.h)));
}

bool Actor::CheckCollision(const Player *player, const map<uint32_t, SpriteCoords> &sprites) const
{
	const AnimationFrame *aFrame = GetFrame();
	vec2 hotspot = GetPosition();
	if (RenderFromColdSpot())
	{
		hotspot = GetPosition() + vec2(0,16) + aFrame->getHotSpot() - aFrame->getColdSpot();
	}
	return HotspotCollision(player, hotspot);
}

inline void AdjustClipMask(uint8_t id, uint8_t *mask)
{
	uint8_t value = 0xFF;
	switch (id)
	{
	case OneWay:
	case Vine:
	case Hook:
		value = id;
		break;
	default:
		break;
	}
	for (int i = 0; i < 1024; i++)
	{
		mask[i] = (mask[i]) ? value : 0x00;
	}
}

uint8_t *Actor::GetClipOverlap()
{
	auto BBOX_WIDTH = 8;
	auto BBOX_HEIGHT = 8;
	J2L_Tile tileInfo;
	int32_t layerWidth = level->GetLayerWidth(3);
	int32_t layerHeight = level->GetLayerHeight(3);
	int32_t tileXCoord = (int32_t)Math::Floor((Location.x - ((BBOX_WIDTH / 2) + 6)) / 32);	// Get the tile that contains the pixel 16 pixels to the left
	int32_t tileYCoord = (int32_t)Math::Floor((Location.y - (BBOX_HEIGHT + 6)) / 32);	// and 16 pixels above the collision volume. This will be the upper left corner
	int32_t tileCoord = (((int32_t)layerWidth) * tileYCoord) + tileXCoord;
	if ((tileCoord != lastTileCoord) || (quad == nullptr))
	{
		if (quad != nullptr)
			delete[] quad;
		quad = new uint8_t[96 * 96];
		memset(quad, 0, 96 * 96);

		if ((tileXCoord > (-2)) && (tileYCoord > (-2)))
		{
			uint8_t *clip = nullptr;
			if ((tileXCoord >= 0) && (tileXCoord < layerWidth) && (tileYCoord >= 0) && (tileYCoord < layerHeight))
			{
				tileInfo = level->GetTile(3, tileXCoord, tileYCoord, 0);
				clip = tileset->GetClipMask(tileInfo.index, tileInfo.flipped);
				J2L_Event event = level->GetEvents(tileXCoord, tileYCoord);
				if (event.EventID != 0)
				{
					AdjustClipMask(event.EventID, clip);
				}
			}
			else
			{
				clip = new uint8_t[1024];
				memset(clip, 0xFF, 1024);
			}
			for (int i = 0; i < 32; i++)
			{
				memcpy(&quad[(96 * i)], &clip[32 * i], 32);
			}
			delete[] clip;
			if (((tileXCoord + 1) >= 0) && ((tileXCoord + 1) < layerWidth) && (tileYCoord >= 0) && (tileYCoord < layerHeight))
			{
				tileInfo = level->GetTile(3, tileXCoord + 1, tileYCoord, 0);
				clip = tileset->GetClipMask(tileInfo.index, tileInfo.flipped);
				J2L_Event event = level->GetEvents(tileXCoord + 1, tileYCoord);
				if (event.EventID != 0)
				{
					AdjustClipMask(event.EventID, clip);
				}
			}
			else
			{
				clip = new uint8_t[1024];
				memset(clip, 0xFF, 1024);
			}
			for (int i = 0; i < 32; i++)
			{
				memcpy(&quad[(96 * i) + 32], &clip[32 * i], 32);
			}
			delete[] clip;
			if (((tileXCoord + 2) >= 0) && ((tileXCoord + 2) < layerWidth) && (tileYCoord >= 0) && (tileYCoord < layerHeight))
			{
				tileInfo = level->GetTile(3, tileXCoord + 2, tileYCoord, 0);
				clip = tileset->GetClipMask(tileInfo.index, tileInfo.flipped);
				J2L_Event event = level->GetEvents(tileXCoord + 2, tileYCoord);
				if (event.EventID != 0)
				{
					AdjustClipMask(event.EventID, clip);
				}
			}
			else
			{
				clip = new uint8_t[1024];
				memset(clip, 0xFF, 1024);
			}
			for (int i = 0; i < 32; i++)
			{
				memcpy(&quad[(96 * i) + 64], &clip[32 * i], 32);
			}
			delete[] clip;
			if ((tileXCoord >= 0) && (tileXCoord < layerWidth) && ((tileYCoord + 1) >= 0) && ((tileYCoord + 1) < layerHeight))
			{
				tileInfo = level->GetTile(3, tileXCoord, tileYCoord + 1, 0);
				clip = tileset->GetClipMask(tileInfo.index, tileInfo.flipped);
				J2L_Event event = level->GetEvents(tileXCoord, tileYCoord + 1);
				if (event.EventID != 0)
				{
					AdjustClipMask(event.EventID, clip);
				}
			}
			else
			{
				clip = new uint8_t[1024];
				memset(clip, 0xFF, 1024);
			}
			for (int i = 0; i < 32; i++)
			{
				memcpy(&quad[(96 * (i + 32))], &clip[32 * i], 32);
			}
			delete[] clip;
			if (((tileXCoord + 1) >= 0) && ((tileXCoord + 1) < layerWidth) && ((tileYCoord + 1) >= 0) && ((tileYCoord + 1) < layerHeight))
			{
				tileInfo = level->GetTile(3, tileXCoord + 1, tileYCoord + 1, 0);
				clip = tileset->GetClipMask(tileInfo.index, tileInfo.flipped);
				J2L_Event event = level->GetEvents(tileXCoord + 1, tileYCoord + 1);
				if (event.EventID != 0)
				{
					AdjustClipMask(event.EventID, clip);
				}
			}
			else
			{
				clip = new uint8_t[1024];
				memset(clip, 0xFF, 1024);
			}
			for (int i = 0; i < 32; i++)
			{
				memcpy(&quad[(96 * (i + 32) + 32)], &clip[32 * i], 32);
			}
			delete[] clip;
			if (((tileXCoord + 2) >= 0) && ((tileXCoord + 2) < layerWidth) && ((tileYCoord + 1) >= 0) && ((tileYCoord + 1) < layerHeight))
			{
				tileInfo = level->GetTile(3, tileXCoord + 2, tileYCoord + 1, 0);
				clip = tileset->GetClipMask(tileInfo.index, tileInfo.flipped);
				J2L_Event event = level->GetEvents(tileXCoord + 2, tileYCoord + 1);
				if (event.EventID != 0)
				{
					AdjustClipMask(event.EventID, clip);
				}
			}
			else
			{
				clip = new uint8_t[1024];
				memset(clip, 0xFF, 1024);
			}
			for (int i = 0; i < 32; i++)
			{
				memcpy(&quad[(96 * (i + 32) + 64)], &clip[32 * i], 32);
			}
			delete[] clip;
			if (((tileXCoord) >= 0) && ((tileXCoord) < layerWidth) && ((tileYCoord + 2) >= 0) && ((tileYCoord + 2) < layerHeight))
			{
				tileInfo = level->GetTile(3, tileXCoord, tileYCoord + 2, 0);
				clip = tileset->GetClipMask(tileInfo.index, tileInfo.flipped);
				J2L_Event event = level->GetEvents(tileXCoord, tileYCoord + 2);
				if (event.EventID != 0)
				{
					AdjustClipMask(event.EventID, clip);
				}
			}
			else
			{
				clip = new uint8_t[1024];
				memset(clip, 0xFF, 1024);
			}
			for (int i = 0; i < 32; i++)
			{
				memcpy(&quad[(96 * (i + 64))], &clip[32 * i], 32);
			}
			delete[] clip;
			if (((tileXCoord + 1) >= 0) && ((tileXCoord + 1) < layerWidth) && ((tileYCoord + 2) >= 0) && ((tileYCoord + 2) < layerHeight))
			{
				tileInfo = level->GetTile(3, tileXCoord + 1, tileYCoord + 2, 0);
				clip = tileset->GetClipMask(tileInfo.index, tileInfo.flipped);
				J2L_Event event = level->GetEvents(tileXCoord + 1, tileYCoord + 2);
				if (event.EventID != 0)
				{
					AdjustClipMask(event.EventID, clip);
				}
			}
			else
			{
				clip = new uint8_t[1024];
				memset(clip, 0xFF, 1024);
			}
			for (int i = 0; i < 32; i++)
			{
				memcpy(&quad[(96 * (i + 64) + 32)], &clip[32 * i], 32);
			}
			delete[] clip;
			if (((tileXCoord + 2) >= 0) && ((tileXCoord + 2) < layerWidth) && ((tileYCoord + 2) >= 0) && ((tileYCoord + 2) < layerHeight))
			{
				tileInfo = level->GetTile(3, tileXCoord + 2, tileYCoord + 2, 0);
				clip = tileset->GetClipMask(tileInfo.index, tileInfo.flipped);
				J2L_Event event = level->GetEvents(tileXCoord + 2, tileYCoord + 2);
				if (event.EventID != 0)
				{
					AdjustClipMask(event.EventID, clip);
				}
			}
			else
			{
				clip = new uint8_t[1024];
				memset(clip, 0xFF, 1024);
			}
			for (int i = 0; i < 32; i++)
			{
				memcpy(&quad[(96 * (i + 64) + 64)], &clip[32 * i], 32);
			}
			delete[] clip;
		}
	}

	uint8_t *overlap = new uint8_t[(BBOX_WIDTH + 12) * (BBOX_HEIGHT + 12)];
	int xOffset = (int)Math::Round(Location.x - ((BBOX_WIDTH / 2) + 6)) - (32 * tileXCoord);
	int yOffset = (int)Math::Round(Location.y - (BBOX_HEIGHT + 6)) - (32 * tileYCoord);
	for (int i = 0; i < (BBOX_HEIGHT + 12); i++)
	{
		memcpy(&overlap[(BBOX_WIDTH + 12) * i], &quad[96 * (i + yOffset) + xOffset], (BBOX_WIDTH + 12));
	}

	return overlap;
}

bool Actor::CheckCollision()
{
	bool colliding = false;
	auto clipMask = GetClipOverlap();

	// Determine if we're clipping with the level

	for (int i = 0; i < 1024; i++)
	{
		if (clipMask[i] == 0xFF)
			colliding = true;
	}

	delete[] clipMask;
	return colliding;
}

Actor::~Actor()
{
	if (quad != nullptr)
	{
		delete[] quad;
		quad = nullptr;
	}
}