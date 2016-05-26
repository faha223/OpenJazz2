#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Animation.h"
#include "Math.h"

enum EnemyState
{
	WALKING,
	STANDING,
	STARTING_REVERSE,
	ENDING_REVERSE,
	ATTACKING
};

class Enemy
{
private:
	uint8_t eventId;
	vec2 position;
	EnemyState state;
	int facing;
	float timeSinceLastStateChange;
	const AnimationSet *animSet;
	void setState(const EnemyState &newState);
public:
	Enemy(const J2L_Event &event, const vec2 &position, const Animations *anims);
	~Enemy();
	vec2 GetPosition() const;
	const AnimationFrame *GetAnimFrame() const;
	void Update(const float &timeElapsed);
};

#endif