#ifndef _ACTOR_H_
#define _ACTOR_H_
#include <cstdint>
#include <map>
#include "Math.h"
#include "Structures.h"
#include "Animation.h"
#include "Player.h"

class Actor
{
private:
	EventID eventId;
	uint32_t Sprite;
	vec2 Location;
	const Animation *anim;
	float TTL;
	float Age;
	bool DoesNotFloat;
	float PeriodInitialOffset;
	float SpeedModifier;
public:
	Actor(const vec2 &location, const EventID &EventID, const Animations *anims, const float &TTL);
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
	uint16_t GetPointValue() const;
	bool AddsSparkleOnDeath() const;
	bool CheckCollision(const Player *player, const std::map<uint32_t, SpriteCoords> &sprites) const;
	~Actor();
};

#endif