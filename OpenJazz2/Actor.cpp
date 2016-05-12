#include "Actor.h"
#include <time.h>
#include <SDL2/SDL.h>
using namespace std;

#define ActorFloatingRadius 4.0f  // Number of pixels it oscillates per second
#define ActorFloatingPeriod 6.75f // Number of radians it cycles of oscillation per second

static int32_t nextOffsetPoint = 0;

Actor::Actor(const vec2 &location, const EventID &eventId, const Animations *anims, const float &TTL) : eventId(eventId), Location(location), anim(nullptr), TTL(TTL), Age(0), DoesNotFloat(false), SpeedModifier(1.0f)
{
	const AnimationSet *animSet = anims->GetAnimSet(ANIM_SET_ITEMS);
	switch (eventId)
	{
	case None:
		return;
	case Apple:
		anim = animSet->GetAnim(ANIM_APPLE);
		break;
	case Milk:
		anim = animSet->GetAnim(ANIM_MILK);
		break;
	case Pear:
		anim = animSet->GetAnim(ANIM_PEAR);
		break;
	case CarrotEnergyPlus1:
		anim = animSet->GetAnim(ANIM_CARROT);
		break;
	case GoldCoin:
		anim = animSet->GetAnim(ANIM_GOLD_COIN);
		SpeedModifier = -2.0f;
		break;
	case SilverCoin:
		anim = animSet->GetAnim(ANIM_SILVER_COIN);
		SpeedModifier = -2.0f;
		break;
	case Cake:
		anim = animSet->GetAnim(ANIM_CAKE);
		break;
	case Cupcake:
		anim = animSet->GetAnim(ANIM_CUPCAKE);
		break;
	case Candy:
		anim = animSet->GetAnim(ANIM_CANDY);
		break;
	case Chocbar:
		anim = animSet->GetAnim(ANIM_CHOCOLATE);
		break;
	case ChickenLeg:
		anim = animSet->GetAnim(ANIM_CHICKEN);
		break;
	case RedGemPlus1:
	case PurpleGemPlus1:
	case BlueGemPlus1:
	case GreenGemPlus1:
		anim = animSet->GetAnim(ANIM_GEM);
		SpeedModifier = -2.0f;
		break;
	case Donut:
		anim = animSet->GetAnim(ANIM_DONUT);
		break;
	case GemCrate:
		anim = animSet->GetAnim(ANIM_CRATE);
		DoesNotFloat = true;
		break;
	case ExtraLive:
		anim = animSet->GetAnim(ANIM_1UP);
		break;
	case Lettuce:
		anim = animSet->GetAnim(ANIM_LETTUCE);
		break;
	case Watermelon:
		anim = animSet->GetAnim(ANIM_WATERMELON);
		break;
	case Peach:
		anim = animSet->GetAnim(ANIM_PEACH);
		break;
	case Sparkle:
		anim = animSet->GetAnim(ANIM_SPARKLE);
		this->TTL = anim->GetFrameCount() / (float)anim->GetFrameRate();
		DoesNotFloat = true;
		break;
	}
	PeriodInitialOffset = (nextOffsetPoint++) * (ActorFloatingPeriod / 4.0f);
	nextOffsetPoint %= 4;
}

Actor::Actor(const Actor &other) : eventId(other.eventId), Location(other.Location), anim(other.anim), TTL(other.TTL), Age(other.Age), DoesNotFloat(other.DoesNotFloat), PeriodInitialOffset(other.PeriodInitialOffset), SpeedModifier(other.SpeedModifier)
{
}

Actor Actor::operator =(const Actor &other)
{
	eventId = other.eventId;
	Location = other.Location;
	anim = other.anim;
	TTL = other.TTL;
	Age = other.Age;
	DoesNotFloat = other.DoesNotFloat;
	PeriodInitialOffset = other.PeriodInitialOffset;
	SpeedModifier = other.SpeedModifier;
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

bool Actor::CheckCollision(const Player *player, const map<uint32_t, SpriteCoords> &sprites) const
{
	SDL_Rect aRect, pRect;
	int flipped = (player->GetFacing() == -1);
	vec2 playerPos = player->GetPosition();
	const AnimationFrame *aFrame = GetFrame();
	const AnimationFrame *pFrame = player->GetSprite();
	vec2 aHotspot = aFrame->getHotSpot();
	vec2 pHotspot = pFrame->getHotSpot();
	if (!flipped)
		playerPos += vec2(pHotspot.x, pFrame->getHeight() + pHotspot.y - 20);
	else
		playerPos += vec2(-pHotspot.x, pFrame->getHeight() + pHotspot.y - 20);
	
	aRect.x = Location.x + aHotspot.x;
	aRect.y = Location.y + aFrame->getHeight() + aHotspot.y - 12;
	aRect.w = aFrame->getWidth();
	aRect.h = aFrame->getHeight();	
	pRect.x = playerPos.x - ((flipped) ? (pFrame->getWidth()) : 0);
	pRect.y = playerPos.y - (float)pFrame->getHeight();
	pRect.w = pFrame->getWidth();
	pRect.h = pFrame->getHeight();
	return (SDL_HasIntersection(&aRect, &pRect) == SDL_TRUE);
}

Actor::~Actor()
{
}