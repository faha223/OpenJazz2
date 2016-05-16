#ifndef _ACTOR_H_
#define _ACTOR_H_
#include <cstdint>
#include <map>
#include "Math.h"
#include "Structures.h"
#include "Animation.h"

class Level;
class Tileset;
class Player;

enum ActorState
{
	Still,
	PlayOnce,
	Looping
};

class Actor
{
private:
	EventID eventId;
	uint32_t Sprite;
	vec2 Location;
	vec2 Velocity;
	const Animation *anim;
	float TTL;
	float Age;
	bool DoesNotFloat;
	float PeriodInitialOffset;
	float SpeedModifier;
	bool AffectedByGravity;
	const Level *level;
	const Tileset *tileset;
	uint8_t *GetClipOverlap();
	uint32_t lastTileCoord;
	uint8_t *quad;
	bool animateOnCollision;
	ActorState state;
	float timeSinceStateChanged;
	bool isFlipped;

	void SetState(const ActorState &newState);
public:
	Actor(const Level *level, const Tileset *tileset, const vec2 &location, const EventID &EventID, const Animations *anims, const float &TTL);
	Actor(const Actor &other);
	Actor operator =(const Actor &other);
	vec2 GetPosition() const;
	virtual void Update(const float &timeElapsed);
	const virtual AnimationFrame *GetFrame() const;
	EventID GetEventID() const;
	bool GetDestroyActor() const;
	bool isFood() const;
	uint8_t GetHealthAdd() const;	// Returns the amount of health that this adds to the player on collision
	uint8_t GetMoneyAdd() const;
	uint8_t GetGemValue() const;
	GemType GetGemType() const;
	uint8_t GetLivesAdd() const;
	uint16_t GetPointValue() const;
	bool IsFlipped() const;
	bool IsAnimationEnded() const;
	bool AddsSparkleOnDeath() const;
	bool RenderFromColdSpot() const;
	bool CheckCollision(const Player *player, const std::map<uint32_t, SpriteCoords> &sprites);
	bool CheckCollision();
	~Actor();
};

#endif