#include "Enemy.h"

Enemy::Enemy(const J2L_Event &Event, const vec2 &position, const Animations *anims) :
	eventId(Event.EventID),
	position(position),
	facing(1),
	state(STANDING),
	timeSinceLastStateChange(0)
{
	switch (Event.EventID)
	{
	case NormTurtle:
		animSet = anims->GetAnimSet(ANIM_SET_NORM_TURTLE);
		break;
	}
}

void Enemy::setState(const EnemyState &newState)
{
	if (state != newState)
	{
		state = newState;
		timeSinceLastStateChange = 0;
	}
}

void Enemy::Update(const float &timeElapsed)
{
	timeSinceLastStateChange += timeElapsed;

	switch (state)
	{
	case STANDING:
		break;
	case WALKING:
		break;
	case STARTING_REVERSE:
		break;
	case ENDING_REVERSE:
		break;
	case ATTACKING:
		break;
	}
}

vec2 Enemy::GetPosition() const
{
	return position;
}

const AnimationFrame *Enemy::GetAnimFrame() const
{
	const Animation *anim = nullptr;
	bool firstFrameOnly = false;
	bool loop = true;
	switch (state)
	{
	case STANDING:
		anim = animSet->GetAnim(ANIM_NORM_TURT_WALK);
		firstFrameOnly = true;
		break;
	case WALKING:
		anim = animSet->GetAnim(ANIM_NORM_TURT_WALK);
		break;
	case ATTACKING:
		anim = animSet->GetAnim(ANIM_NORM_TURT_BITE);
		loop = false;
		break;
	case STARTING_REVERSE:
		anim = animSet->GetAnim(ANIM_NORM_TURT_START_REVERSE);
		loop = false;
		break;
	case ENDING_REVERSE:
		anim = animSet->GetAnim(ANIM_NORM_TURT_END_REVERSE);
		loop = false;
		break;
	}

	int frameIndex = (firstFrameOnly ? 0 : (int)Math::Floor(anim->GetFrameRate() * timeSinceLastStateChange));
	if (loop)
		frameIndex %= anim->GetFrameCount();
	else
		frameIndex = min(frameIndex, anim->GetFrameCount());

	return anim->GetFrame(frameIndex);
}

Enemy::~Enemy()
{

}