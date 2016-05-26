#include "Player.h"
#include "Actor.h"

#include "Event.h"
#include <cstdlib>
//#define SAVE_QUAD
#ifdef SAVE_QUAD
#include <SDL2/SDL.h>
#endif
using namespace std;

template <class T>
T abs (T num)
{
   return ((num<0) ? num*(-1) : num);
}

#define BBOX_WIDTH 24
#define BBOX_HEIGHT 30

Player::Player(const vec2 &startPosition, const Level *level, const Tileset *tileset, const Animations *anims) :
Position(startPosition),
Facing(1),
level(level),
tileset(tileset), 
anims(anims),
grounded(true),
canLoseBalance(true),
timeSinceStateChanged(0),
state(STANDING),
lastTileCoord(0),
quad(nullptr), 
lives(3),
health(5), 
score(0),
food(0),
money(0),
RedGems(0),
GreenGems(0),
BlueGems(0),
PurpleGems(0),
Invincible(true), 
TimeSinceMadeInvincible(0),
SpringInfluenced(false),
bouncerAmmo(0),
hasUpgradedBouncer(false),
toasterAmmo(0),
hasUpgradedToaster(false),
freezerAmmo(0),
hasUpgradedFreezer(false)
{ }

void Player::InitLives(const uint32_t lives)
{
	this->lives = lives;
}

vec2 Player::GetPosition() const
{
	return vec2(Math::Round(Position.x), Math::Round(Position.y));
}

Rect Player::GetBoundingBox() const
{
	return Rect(Position.x - (BBOX_WIDTH/2), Position.y, BBOX_WIDTH, BBOX_HEIGHT);
}

int Player::GetFacing() const
{
	return Facing;
}

PlayerState Player::GetState() const
{
	return state;
}

const AnimationFrame *Player::GetSprite() const
{
	bool FirstFrameOnly = false;
	bool LastFrameOnly = false;
	bool ClampFrame = false;
	uint32_t CurrentAnim = 0;
	float AnimSpeedModifier = 1.0f;
	int32_t LoopCount = 1;
	switch(state)
	{
		case WALKING:
			CurrentAnim = ANIM_JAZZ_WALKING;
			break;
		case RUNNING:
			CurrentAnim = ((Velocity.x * Facing) >= (RunSpeed*0.8)) ? ANIM_JAZZ_RUNNING : ANIM_JAZZ_STARTING_TO_RUN;
			break;
		case JUMPING:
			ClampFrame = true;
			CurrentAnim = (abs(Velocity.x) > 20) ? ANIM_JAZZ_JUMP_SIDE : ANIM_JAZZ_JUMP;
			break;
		case RUNNING_JUMP:
			CurrentAnim = ANIM_JAZZ_BALL;
			break;
		case DESCENDING:
			CurrentAnim = (abs(Velocity.x) > 20) ? ANIM_JAZZ_DESC_SIDE : ANIM_JAZZ_DESC_1;
			break;
		case DESCENDING_GUN:
			CurrentAnim = ANIM_JAZZ_DESC_GUN;
			break;
		case STARTING_SKID:
			CurrentAnim = ANIM_JAZZ_STARTING_SKID;
			break;
		case ENDING_SKID:
			CurrentAnim = ANIM_JAZZ_ENDING_SKID;
			AnimSpeedModifier = 2.0f;
			break;
		case SKIDDING:
			CurrentAnim = ANIM_JAZZ_SKIDDING;
			break;
		case COPTER:
			CurrentAnim = ANIM_JAZZ_COPTER;
			break;
		case STANDING:	// For STANDING, the best I could come up with was to just draw the first frame of the JAZZ_IDLE1 animation
			FirstFrameOnly = true;
			CurrentAnim = ANIM_JAZZ_IDLE1;
			break;
		case IDLE1:
			CurrentAnim = ANIM_JAZZ_IDLE1;
			break;
		case IDLE2:
			CurrentAnim = ANIM_JAZZ_IDLE2;
			break;
		case IDLE3:
			CurrentAnim = ANIM_JAZZ_IDLE3;
			break;
		case HANGING:
			CurrentAnim = ANIM_JAZZ_VINE;
			break;
		case PREPARING_BUTTSTOMP:
			CurrentAnim = ANIM_JAZZ_BUTTSTOMP_PENDING;
			break;
		case SPRING_JUMP:
			CurrentAnim = ANIM_JAZZ_BUTTSTOMP_PENDING;
			AnimSpeedModifier = 3.0f;
			break;
		case BUTTSTOMP:
			CurrentAnim = ANIM_JAZZ_BUTTSTOMP;
			AnimSpeedModifier = 4.0f;
			break;
		case ENDING_BUTTSTOMP:
			CurrentAnim = ANIM_JAZZ_ENDING_BUTTSTOMP;
			break;
		case HANGING_IDLE:
			CurrentAnim = ANIM_JAZZ_VINE_IDLE;
			break;
		case HANGING_MOVING:
			CurrentAnim = ANIM_JAZZ_VINE_MOVING;
			break;
		case IDLE4:
			CurrentAnim = ANIM_JAZZ_IDLE4;
			break;
		case IDLE5:
			CurrentAnim = ANIM_JAZZ_IDLE5;
			break;
		case OFF_BALANCE:
			CurrentAnim = ANIM_JAZZ_OFF_BALANCE;
			break;
		case LANDING:
			CurrentAnim = ANIM_JAZZ_LANDING;
			break;
		case PUSHING:
			CurrentAnim = ANIM_JAZZ_PUSHING;
			break;
		case CROUCHING:
			CurrentAnim = ANIM_JAZZ_CROUCHING;
			break;
		case CROUCHED:
			FirstFrameOnly = true;
			CurrentAnim = ANIM_JAZZ_EXITING_CROUCH;
			break;
		case EXITING_CROUCH:
			CurrentAnim = ANIM_JAZZ_EXITING_CROUCH;
			break;
		case HIGHJUMP:
			LastFrameOnly = true;
			CurrentAnim = ANIM_JAZZ_SUPER_JUMP;
			break;
		case STARTING_HIGHJUMP:
			CurrentAnim = ANIM_JAZZ_SUPER_JUMP;
			break;
		case HPOLE:
			CurrentAnim = ANIM_JAZZ_HPOLE;
			AnimSpeedModifier = 2.0f;
			LoopCount = 2;
			break;
		case VPOLE:
			CurrentAnim = ANIM_JAZZ_VPOLE;
			AnimSpeedModifier = 2.0f;
			LoopCount = 2;
			break;
		case BALL:
			CurrentAnim = ANIM_JAZZ_BALL;
			break;
		case SHOOTING:
			break;
		case HURT:
			CurrentAnim = ANIM_JAZZ_HURT;
			break;
		case DYING:
			CurrentAnim = ANIM_JAZZ_DEATH;
			break;
		case DEAD:
			CurrentAnim = ANIM_JAZZ_DEAD;
			break;
		default:
			CurrentAnim = ANIM_JAZZ_DIZZY;
			break;
	}
	const Animation *anim = anims->GetAnimSet(ANIM_SET_JAZZ)->GetAnim(CurrentAnim);
	int numFrames = anim->GetFrameCount();
	int frameRate = anim->GetFrameRate();
	int Frame = (LastFrameOnly) ? ((LoopCount * numFrames)-1) : ((FirstFrameOnly) ? 0 : (int)Math::Round(timeSinceStateChanged * frameRate * AnimSpeedModifier) % ((ClampFrame) ? INT_MAX : numFrames));
	return anim->GetFrame(min(numFrames-1, max(0, Frame % numFrames)));
}

bool Player::IsInvisible() const
{
	if (Invincible)
	{
		int x = (int)(TimeSinceMadeInvincible * 6);
		return ((x % 2) == 1);
	}
	return false;
}

uint8_t Player::AddHealth(const uint8_t &add)
{
	uint8_t remainder = ((add + health) > 5) ? (add + health - 5) : 0;
	health = min(add + health, 5);
	return remainder;
}

uint8_t Player::AddFood(const uint8_t &add)
{
	food += add;
	if(food > 100)
		food -= 100;
	return 0;
}

uint8_t Player::AddMoney(const uint8_t &add)
{
	money += add;
	return 0;
}

uint8_t Player::AddGems(const GemType &type, const uint8_t &add)
{
	switch(type)
	{
		case GT_None:
			break;
		case GT_Red:
			RedGems += add;
			break;
		case GT_Green:
			GreenGems += add;
			break;
		case GT_Blue:
			BlueGems += add;
			break;
		case GT_Purple:
			PurpleGems += add;
			break;
	}
	return 0;
}

uint8_t Player::AddAmmo(const Weapons &type, const uint8_t &add)
{
	switch (type)
	{
	case Bouncer:
		bouncerAmmo += add;
		return 0;
	case Toaster:
		toasterAmmo += add;
		return 0;
	case Freezer:
		freezerAmmo += add;
		return 0;
	}
	return add;
}

void Player::AddPoints(const uint32_t &points)
{
	score += points;
}

bool Player::HasAnimationEnded() const
{
	uint32_t CurrentAnim = 0;
	float SpeedModifier = 1.0f;
	int32_t LoopCount = 1;
	switch(state)
	{
		case WALKING:
			CurrentAnim = ANIM_JAZZ_WALKING;
			break;
		case RUNNING:
			CurrentAnim = ((Velocity.x * Facing) >= (RunSpeed*0.8)) ? ANIM_JAZZ_RUNNING : ANIM_JAZZ_STARTING_TO_RUN;
			break;
		case JUMPING:
			CurrentAnim = (abs(Velocity.x) > 20) ? ANIM_JAZZ_JUMP_SIDE : ANIM_JAZZ_JUMP;
			break;
		case RUNNING_JUMP:
			CurrentAnim = ANIM_JAZZ_BALL;
			break;
		case DESCENDING:
			CurrentAnim = (abs(Velocity.x) > 20) ? ANIM_JAZZ_DESC_SIDE : ANIM_JAZZ_DESC_1;
			break;
		case STARTING_SKID:
			CurrentAnim = ANIM_JAZZ_STARTING_SKID;
			break;
		case ENDING_SKID:
			CurrentAnim = ANIM_JAZZ_ENDING_SKID;
			SpeedModifier = 2.0f;
			break;
		case SKIDDING:
			CurrentAnim = ANIM_JAZZ_SKIDDING;
			break;
		case COPTER:
			CurrentAnim = ANIM_JAZZ_COPTER;
			break;
		case STANDING:	// For STANDING, the best I could come up with was to just draw the first frame of the JAZZ_IDLE1 animation
			CurrentAnim = ANIM_JAZZ_IDLE1;
			break;
		case IDLE1:
			CurrentAnim = ANIM_JAZZ_IDLE1;
			break;
		case IDLE2:
			CurrentAnim = ANIM_JAZZ_IDLE2;
			break;
		case IDLE3:
			CurrentAnim = ANIM_JAZZ_IDLE3;
			break;
		case IDLE4:
			CurrentAnim = ANIM_JAZZ_IDLE4;
			break;
		case IDLE5:
			CurrentAnim = ANIM_JAZZ_IDLE5;
			break;
		case OFF_BALANCE:
			CurrentAnim = ANIM_JAZZ_OFF_BALANCE;
			break;
		case PREPARING_BUTTSTOMP:
			CurrentAnim = ANIM_JAZZ_BUTTSTOMP_PENDING;
			break;
		case ENDING_BUTTSTOMP:
			CurrentAnim = ANIM_JAZZ_ENDING_BUTTSTOMP;
			break;
		case LANDING:
			CurrentAnim = ANIM_JAZZ_LANDING;
			break;
		case PUSHING:
			CurrentAnim = ANIM_JAZZ_PUSHING;
			break;
		case CROUCHING:
			CurrentAnim = ANIM_JAZZ_CROUCHING;
			break;
		case CROUCHED:
			CurrentAnim = ANIM_JAZZ_EXITING_CROUCH;
			break;
		case EXITING_CROUCH:
			CurrentAnim = ANIM_JAZZ_EXITING_CROUCH;
			break;
		case HIGHJUMP:
			CurrentAnim = ANIM_JAZZ_SUPER_JUMP;
			break;
		case STARTING_HIGHJUMP:
			CurrentAnim = ANIM_JAZZ_SUPER_JUMP;
			break;
		case HPOLE:
			CurrentAnim = ANIM_JAZZ_HPOLE;
			SpeedModifier = 2.0f;
			LoopCount = 2;
			break;
		case VPOLE:
			CurrentAnim = ANIM_JAZZ_VPOLE;
			SpeedModifier = 2.0f;
			LoopCount = 2;
			break;
		case HURT:
			CurrentAnim = ANIM_JAZZ_HURT;
			break;
		case DYING:
			CurrentAnim = ANIM_JAZZ_DEATH;
			break;
		case DEAD:
			CurrentAnim = ANIM_JAZZ_DEAD;
			break;
		default:
			CurrentAnim = ANIM_JAZZ_DIZZY;
			break;
	}
	const Animation *anim = anims->GetAnimSet(ANIM_SET_JAZZ)->GetAnim(CurrentAnim);
	float FrameCount = (float)anim->GetFrameCount();
	float FrameRate = (float)anim->GetFrameRate();
	return ((timeSinceStateChanged*SpeedModifier) > ((LoopCount * FrameCount)-1.0f)/FrameRate);
}

void Player::SetState(const PlayerState &newState)
{
	//printf("Setting State to %i\n", newState);
	state = newState;
	timeSinceStateChanged = 0;
	switch (state)
	{
	case RUNNING:
	case JUMPING:
	case HIGHJUMP:
	case DESCENDING:
		break;
	default:
		SpringInfluenced = false;
		break;
	}
}

void Player::SetSpringInfluenced(const float &timeout)
{
	SpringInfluenced = true;
	timeSinceSpringInfluenced = 0;
	springInfluenceTimeout = timeout;
}

void Player::Update(const float &dt, map<Control, bool> Controls)
{
	timeSinceStateChanged += dt;

#pragma region Update SpringInfluence Values

	timeSinceSpringInfluenced += dt;
	if (timeSinceSpringInfluenced > springInfluenceTimeout)
		SpringInfluenced = false;

#pragma endregion Update SpringInfluence Values

	float rate = 6.0f;
	float origVelocityY = Velocity.y;
	for (int i = 0; i < (int)rate; i++)
	{
		vec2 oldPos = Position;
		vec2 newPos = Position + (Velocity * (dt / rate));
		//printf("Velocity = (%0.2f, %0.2f)\n", Velocity.x, Velocity.y);
		
		if(!grounded)
		{
			Velocity += vec2(0, (dt/rate)*((Velocity.y >= 0.0f) ? FallingGravity : Gravity));
		}
		if((state != BALL) && (state != BALL))
		{
			if (abs((float)Velocity.x) >(float)MaxVelocity)
				Velocity.x *= (MaxVelocity / abs((float)Velocity.x));
			if (Velocity.y > ((state == BUTTSTOMP) ? ButtstompVelocity : MaxVelocity))
				Velocity.y *= (((state == BUTTSTOMP) ? ButtstompVelocity : MaxVelocity) / abs((float)Velocity.y));
		}

		vec2 delta = newPos - oldPos;
		int phase = 0;

		Position.x = newPos.x;
		if (!ResolveCollision(CheckCollision(oldPos)))
		{
			Position.x = Math::Round(oldPos.x);
			Position.y = Math::Round(oldPos.y);
		}
		oldPos = Position;
		Position.y = newPos.y;
		if (!ResolveCollision(CheckCollision(oldPos)))
		{
			Position.y = Math::Round(oldPos.y);
		}
	}
	CollisionInfo coll = CheckCollision(Position);
	int TileYCoord = ((int)Math::Round(Position.y-16))/32;
	int TileXCoord = ((int)Math::Round(Position.x))/32;
	int TileCoord = (level->GetLayerWidth(3) * TileYCoord) + TileXCoord;
	J2L_Event event = level->GetEvents(TileXCoord, TileYCoord);
	if (Invincible)
	{
		TimeSinceMadeInvincible += dt;
		if (TimeSinceMadeInvincible >= InvincibilityDuration)
			Invincible = false;
	}
	switch (state)
	{
	case ENDING_SKID:
	case OFF_BALANCE:
	case STANDING:
	case IDLE1:
	case IDLE2:
	case IDLE3:
	case IDLE4:
	case IDLE5:
	case LANDING:
	case EXITING_CROUCH:
	case ENDING_BUTTSTOMP:
		Velocity.x = 0;
		Velocity.y = 0;
		if(state == STANDING)
		{
			if(timeSinceStateChanged > 1.5)
			{
				int randInt = rand() % 10;
				if(randInt < 5)
					SetState(IDLE1);
				else if(randInt < 7)
					SetState(IDLE2);
				else if(randInt < 8)
					SetState(IDLE3);
				else if(randInt < 9)
					SetState(IDLE4);
				else
					SetState(IDLE5);
			}
		}
		else
		{
			if(HasAnimationEnded())
				SetState(STANDING);
		}
		if ((coll.iBottomCenter > 0) && (coll.iBottomLeft > 0) && (coll.iBottomRight > 0)) // No floor
		{
			SetState(DESCENDING);
			grounded = false;
		}
		else
		{
			J2L_Event e = level->GetEvents(TileXCoord, TileYCoord + 1);
			if ((e.EventID == Hurt) && !Invincible)
			{
				health--;
				if (health > 0)
				{
					SetState(HURT);
					Invincible = true;
					TimeSinceMadeInvincible = 0;
				}
				else
				{
					SetState(DYING);
				}
			}
			if (Controls[LEFT] || Controls[RIGHT])
			{
				Facing = Controls[LEFT] ? -1 : 1;
				canLoseBalance = true;
				if ((Controls[LEFT] && (coll.iLeft <= 1)) || (Controls[RIGHT] && (coll.iRight <= 1)))
					SetState(PUSHING);
				else
					SetState((Controls[RUN]) ? RUNNING : WALKING);
			}
			if (Controls[JUMP] && !PrevControls[JUMP])
			{
				grounded = 0;
				Velocity.y = JumpVelocity;
				SetState(JUMPING);
				canLoseBalance = true;
			}
			if(!Controls[LEFT] && !Controls[RIGHT] && !Controls[JUMP])
			{
				if(canLoseBalance && ((coll.iBottomCenter > 0) && ((coll.iBottomLeft > 0) || (coll.iBottomRight > 0))))
				{
					SetState(OFF_BALANCE);
					canLoseBalance = false;
				}
			}
			if(Controls[DOWN])
				SetState(CROUCHING);
		}
		break;
	case HURT:
		if (HasAnimationEnded())
		{
			SetState(STANDING);
		}
		break;
	case CROUCHING:
	case CROUCHED:
		if(state == CROUCHING)
		{
			if(HasAnimationEnded())
				SetState(CROUCHED);
			else if(Controls[JUMP])
			{
				SetState(Controls[RUN] ? RUNNING_JUMP : JUMPING);
			}
		}
		Velocity.x = (1.0f - Acceleration) * Velocity.x;
		if(!Controls[DOWN])
		{
			if(abs(Velocity.x) < 20)
				SetState(EXITING_CROUCH);
			else
				SetState(SKIDDING);
		}
		if(Controls[JUMP])
		{
			SetState(STARTING_HIGHJUMP);
		}
		break;
	case STARTING_HIGHJUMP:
		if (HasAnimationEnded())
		{
			SetState(HIGHJUMP);
		}
		else
		{
			if (timeSinceStateChanged > 0.2)
			{
				Velocity.y = HighJumpVelocity;
				grounded = false;
			}
		}
		break;
	case HIGHJUMP:
		if (TileYCoord > 0)
		{
			if((coll.CollisionState == ButtStompScenery) || (coll.CollisionState == DestructScenery))
			{
				coll.iTop = INT_MAX;
				Velocity.y = origVelocityY;
			}
		}
		if (SpringInfluenced && (springInfluenceTimeout > timeSinceSpringInfluenced))
		{
			Velocity.y = origVelocityY;
		}
		else if((!Controls[JUMP]) || (Velocity.y >= 0) || (timeSinceStateChanged >= HighJumpDuration))
		{
			SetState(JUMPING);
			timeSinceStateChanged = 1;
		}
		else
		{
			Velocity.y = (HighJumpAcceleration * (HighJumpVelocity)) + ((1.0f - HighJumpAcceleration) * Velocity.y);
		}
		if(event.EventID == VPole)
		{
			grounded = true;
			SetState(VPOLE);
			ExitPoleVelocity = (Velocity.y < 0 ? -1 : 1) * min(MaxPoleExitSpeed, abs(2 * Velocity.y));
		}
		break;
	case SPRING_JUMP:
		if (TileYCoord > 0)
		{
			if ((coll.CollisionState == ButtStompScenery) || (coll.CollisionState == DestructScenery))
			{
				coll.iTop = INT_MAX;
				Velocity.y = origVelocityY;
			}
		}
		if ((coll.CollisionState == Vine) || (coll.CollisionState == Hook))
		{
			grounded = true;
			Velocity.y = 0;
			SetState(HANGING);
		}
		else
		{
			if (coll.iTop < 0)
			{
				Velocity.y = 0;
				SetState(DESCENDING);
			}

			if (Controls[LEFT] || Controls[RIGHT])
			{
				Facing = Controls[LEFT] ? -1 : 1;
				Velocity.x = ((1.0f - AirControl) * Velocity.x) + (AirControl * (Controls[RUN] ? RunSpeed : WalkSpeed) * Facing);
			}
			else
			{
				Velocity.x = (1.0f - AirControl) * Velocity.x;
			}

			if (SpringInfluenced && (springInfluenceTimeout > timeSinceSpringInfluenced))
			{
				Velocity.y = origVelocityY;
			}
			else
			{
				SetState(JUMPING);
				timeSinceStateChanged = 1;
			}
			if (event.EventID == VPole)
			{
				grounded = true;
				SetState(VPOLE);
				ExitPoleVelocity = (Velocity.y < 0 ? -1 : 1) * min(MaxPoleExitSpeed, abs(2 * Velocity.y));
			}
		}
		break;
	case PUSHING:
		if (Controls[LEFT] || Controls[RIGHT])
		{
			Facing = (Controls[LEFT]) ? -1 : 1;
			if (Controls[LEFT])
			{
				if (coll.iLeft >= 1)
					SetState((Controls[RUN]) ? RUNNING : WALKING);
			}
			if (Controls[RIGHT])
			{
				if (coll.iRight >= 1)
					SetState((Controls[RUN]) ? RUNNING : WALKING);
			}
			if(Controls[JUMP])
			{
				SetState((Controls[RUN]) ? RUNNING_JUMP : JUMPING);
				grounded = 0;
				Velocity.y = JumpVelocity;
			}
		}
		else
			SetState(STANDING);
		break;
	case RUNNING_JUMP:
	case JUMPING:
		if (Velocity.y >= 0)
			SetState(DESCENDING);
		else
		{
			if (Controls[JUMP])
			{
				if (timeSinceStateChanged < JumpDuration)
					Velocity.y = JumpVelocity;
			}
			else if (SpringInfluenced)
			{
				if (timeSinceSpringInfluenced < springInfluenceTimeout)
					Velocity.y = origVelocityY;
			}
			else
			{
				SetState(DESCENDING);
			}
			if (Controls[LEFT] || Controls[RIGHT])
			{
				Facing = Controls[LEFT] ? -1 : 1;
				Velocity.x = (Acceleration * Facing * (Controls[RUN] ? RunSpeed : WalkSpeed)) + ((1.0f - Acceleration) * Velocity.x);
			}
			else
			{
				Velocity.x = (1.0f - AirControl) * Velocity.x;
			}
			if(Controls[DOWN] && !PrevControls[DOWN])
			{
				Velocity.y = 0;
				grounded = true;
				SetState(PREPARING_BUTTSTOMP);
			}
			if(event.EventID == VPOLE)
			{
				grounded = true;
				SetState(VPOLE);
				ExitPoleVelocity = (Velocity.y < 0 ? -1 : 1) * min(MaxPoleExitSpeed, abs(2 * Velocity.y));
			}
		}
		break;
	case WALKING:
		Velocity.y = 0;
		if ((coll.iBottomCenter > 0) && (coll.iBottomLeft > 0) && (coll.iBottomRight > 0)) // No floor
		{
			SetState(DESCENDING);
			grounded = false;
		}
		else
		{
			if (!Controls[RUN] && (Controls[LEFT] || Controls[RIGHT]))
			{
				Facing = Controls[LEFT] ? -1 : 1;
				if ((Controls[LEFT] && (coll.iLeft <= 1)) || (Controls[RIGHT] && (coll.iRight <= 1)))
				{
					SetState(STANDING);
				}
				else
				{
					Velocity.x = ((1.0f - Acceleration) * Velocity.x) + (Acceleration * WalkSpeed * Facing);
				}
			}
			else if (Controls[RUN] && (Controls[LEFT] || Controls[RIGHT]))
			{
				SetState(RUNNING);
			}
			else
			{
				SetState((abs(Velocity.x) > 100) ? STARTING_SKID : STANDING);
				if (state == STANDING)
					Velocity.x = 0;
			}
			if (Controls[JUMP] && !PrevControls[JUMP])
			{
				grounded = 0;
				Velocity.y = JumpVelocity;
				SetState(JUMPING);
			}
			if(event.EventID == HPole)
			{
				ExitPoleVelocity = Facing * min(MaxPoleExitSpeed, 2 * WalkSpeed);
				SetState(HPOLE);
			}
		}
		break;
	case RUNNING:
		Velocity.y = 0;
		if ((coll.iBottomCenter > 0) && (coll.iBottomLeft > 0) && (coll.iBottomRight > 0)) // No floor
		{
			SetState(DESCENDING);
			grounded = false;
		}
		else
		{
			if ((Controls[RUN] && (Controls[LEFT] || Controls[RIGHT])) || SpringInfluenced)
			{
				Facing = Controls[LEFT] ? -1 : 1;
				if ((Controls[LEFT] && (coll.iLeft <= 1)) || (Controls[RIGHT] && (coll.iRight <= 1)))
				{
					SetState(STANDING);
				}
				else
				{
					if(!SpringInfluenced)
						Velocity.x = ((1.0f - Acceleration) * Velocity.x) + (Acceleration * RunSpeed * Facing);
				}
			}
			else if (!Controls[RUN] && (Controls[LEFT] || Controls[RIGHT]))
			{
				SetState(WALKING);
			}
			else
			{
				SetState((abs(Velocity.x) > 100) ? STARTING_SKID : STANDING);
				if (state == STANDING)
					Velocity.x = 0;
			}
			if (Controls[JUMP] && !PrevControls[JUMP])
			{
				grounded = 0;
				Velocity.y = JumpVelocity;
				SetState(RUNNING_JUMP);
			}
			if(event.EventID == HPole)
			{
				ExitPoleVelocity = Facing * min(MaxPoleExitSpeed, 2 * RunSpeed);
				SetState(HPOLE);
			}
		}
		break;
	case STARTING_SKID:
	case SKIDDING:
		Velocity.y = 0;
		if ((coll.iBottomCenter > 0) && (coll.iBottomLeft > 0) && (coll.iBottomRight > 0)) // No floor
		{
			SetState(DESCENDING);
			grounded = false;
		}
		else
		{
			if (Controls[LEFT] || Controls[RIGHT])
			{
				Facing = Controls[LEFT] ? -1 : 1;
				SetState((Controls[RUN]) ? RUNNING : WALKING);
			}
			else
			{
				Velocity.x = (1.0f - Acceleration) * Velocity.x;
				if(abs(Velocity.x) < 10)
				{
					Velocity.x = 0;
					SetState(ENDING_SKID);
				}
			}
			if (Controls[JUMP] && !PrevControls[JUMP])
			{
				grounded = 0;
				Velocity.y = JumpVelocity;
				SetState(JUMPING);
			}
			if(Controls[DOWN])
			{
				SetState(CROUCHING);
			}
		}
		if (state == STARTING_SKID)
		{
			const Animation *anim = anims->GetAnimSet(ANIM_SET_JAZZ)->GetAnim(ANIM_JAZZ_STARTING_SKID);
			float FrameCount = (float)anim->GetFrameCount();
			float FrameRate = (float)anim->GetFrameRate();
			if (timeSinceStateChanged > ((FrameCount - 1) / FrameRate))
			{
				SetState(SKIDDING);
			}
		}
		if(event.EventID == HPole)
		{
			ExitPoleVelocity = Facing * min(MaxPoleExitSpeed, 2 * WalkSpeed);
			Velocity.x = 0;
			SetState(HPOLE);
		}
		break;
	case HPOLE:
		Position.x = 16.0f + (32.0f * TileXCoord);
		Velocity.x = 0;
		Velocity.y = 0;
		if(HasAnimationEnded())
		{
			Velocity.x = ExitPoleVelocity;
			Position.x += (32 * Facing);
			SetState(BALL);
		}
		break;
	case VPOLE:
		Position.y = 32.0f * (TileYCoord)+31.0f;
		Velocity.y = 0;
		Velocity.x = 0;
		if(HasAnimationEnded())
		{
			Velocity.y = ExitPoleVelocity;
			Position.y += (32.0f * ((ExitPoleVelocity < 0.0f) ? -1.0f : 1.0f));
			grounded = false;
			SetState(BALL);
		}
		break;
	case BALL:
		if((coll.iBottomRight > 0) && (coll.iBottomCenter > 0) && (coll.iBottomLeft > 0))
		{
			grounded = false;
		} 
		if((abs(Velocity.x) <= WalkSpeed) && (abs(Velocity.y) <= WalkSpeed))
		{
			SetState(DESCENDING);
		}
		if(event.EventID == HPole)
		{
			SetState(HPOLE);
			grounded = true;
			Velocity.x = 0;
		}
		if(event.EventID == VPole)
		{
			SetState(VPOLE);
			ExitPoleVelocity = (Velocity.y < 0 ? -1 : 1) * min(MaxPoleExitSpeed, abs(2 * Velocity.y));
			grounded = true;
			Velocity.y = 0;
		}
		break;
	case DESCENDING:
	case DESCENDING_GUN:
		if (state == DESCENDING_GUN && !Controls[SHOOT] && timeSinceStateChanged > ShootIdleTime)
			SetState(DESCENDING);
		if ((coll.iBottomCenter <= 0) || (coll.iBottomLeft <= 0) || (coll.iBottomRight <= 0))
		{
			grounded = true;
			if (Controls[LEFT] || Controls[RIGHT] || SpringInfluenced)
			{
				SetState(Controls[RUN] ? RUNNING : WALKING);
			}
			else
			{
				SetState((abs(Velocity.x) > 20) ? STARTING_SKID : LANDING);
				if(state == LANDING)
					Velocity.x = 0;
			}
		}
		else
		{
			if((coll.CollisionState == Vine) || (coll.CollisionState == Hook))
			{
				grounded = true;
				Velocity.y = 0;
				SetState(HANGING);
			}
			else
			{
				if (Controls[LEFT] || Controls[RIGHT] || SpringInfluenced)
				{
					Facing = Controls[LEFT] ? -1 : 1;
					Velocity.x = ((1.0f - AirControl) * Velocity.x) + (AirControl * (Controls[RUN] ? RunSpeed : WalkSpeed) * Facing);
				}
				else
				{
					Velocity.x = (1.0f - AirControl) * Velocity.x;
				}
				if (Controls[SHOOT] && !PrevControls[SHOOT])
				{
					SetState(DESCENDING_GUN);
				}
				if (Controls[JUMP] && !PrevControls[JUMP])
				{
					timeInCopter = 0;
					Velocity.y = CopterDescentSpeed;
					SetState(COPTER);
				}
				if(Controls[DOWN] && !PrevControls[DOWN])
				{
					SetState(PREPARING_BUTTSTOMP);
					Velocity.x = 0;
					Velocity.y = 0;
					grounded = true;
				}
				if(event.EventID == VPole)
				{
					grounded = true;
					SetState(VPOLE);
					ExitPoleVelocity = (Velocity.y < 0 ? -1 : 1) * min(MaxPoleExitSpeed, abs(2 * Velocity.y));
				}
			}
		}
		break;
	case PREPARING_BUTTSTOMP:
		if(HasAnimationEnded())
		{
			Velocity.y = ButtstompVelocity;
			SetState(BUTTSTOMP);
		}
		break;
	case BUTTSTOMP:
		if((coll.CollisionState == ButtStompScenery) || (coll.CollisionState == DestructScenery))
		{
			coll.iBottomCenter = INT_MAX;
			coll.iBottomLeft = INT_MAX;
			coll.iBottomRight = INT_MAX;
		}
		if((coll.iBottomCenter <= 0) || (coll.iBottomLeft <= 0) || (coll.iBottomRight <= 0))
		{
			Velocity.y = 0;
			SetState(ENDING_BUTTSTOMP);
		}
		else
		{
			if(coll.CollisionState == Vine)
			{
				Velocity.y = 0;
				grounded = true;
				SetState(HANGING);
			}
			if(event.EventID == VPole)
			{
				grounded = true;
				SetState(VPOLE);
				ExitPoleVelocity = (Velocity.y < 0 ? -1 : 1) * min(MaxPoleExitSpeed, abs(2 * Velocity.y));
				Velocity.y = 0;
			}
		}
		break;
	case HANGING_IDLE:
	case HANGING:
		Velocity.y = 0;
		if ((coll.CollisionState == Vine) || (coll.CollisionState == Hook))
		{
			if(coll.iTop < 10)
				Position.y += (-4 - coll.iTop);
			if(Controls[DOWN])
			{
				Position.y += 12;
				grounded = false;
				SetState(DESCENDING);
			}
			if(Controls[JUMP])
			{
				Position.y -= 1;
				grounded = false;
				Velocity.y = JumpVelocity;
				SetState(JUMPING);
			}
			if(Controls[LEFT] || Controls[RIGHT])
			{
				SetState(HANGING_MOVING);
			}
			else
			{
				Velocity.x = 0;
				if(state == HANGING_IDLE)
				{
					if(HasAnimationEnded())
					{
						SetState(HANGING);
					}
				}
				if(timeSinceStateChanged > 1.5f)
				{
					SetState(HANGING_IDLE);
				}
			}
		}
		else
		{

		}
		break;
	case HANGING_MOVING:
		Velocity.y = 0;
		if(coll.CollisionState != Vine)
		{
			SetState(DESCENDING);
			grounded = false;
		}
		else
		{
			if(coll.iTop < 10)
				Position.y += (-4 - coll.iTop);
			if(Controls[DOWN])
			{
				Position.y += 12;
				SetState(DESCENDING);
				grounded = false;
			}
			if(Controls[JUMP])
			{
				Position.y -= 1;
				grounded = false;
				Velocity.y = JumpVelocity;
				SetState(JUMPING);
			}
			if(Controls[LEFT] || Controls[RIGHT])
			{

				Facing = (Controls[LEFT]) ? -1 : 1;
				float swingMag = ((Controls[RUN] ? MaxFastHangingMoveSpeed : MaxHangingMoveSpeed) - MinHangingMoveSpeed);
				Velocity.x = Facing * ((0.5f * swingMag * Math::Sin(HangingMoveSpeedPeriod * timeSinceStateChanged + HangingMoveSpeedPeriodOffset)) + (0.5f * swingMag) + MinHangingMoveSpeed);
			}
			else
			{
				SetState(HANGING);
			}
		}
		break;
	case COPTER:
		timeInCopter += dt;
		Velocity.y = CopterDescentSpeed;
		if ((coll.iBottomCenter <= 0) || (coll.iBottomLeft <= 0) || (coll.iBottomRight <= 0))
		{
			grounded = true;
			Velocity.y = 0;
			if (abs(Velocity.x) > 0)
			{
				SetState(WALKING);
			}
			else
			{
				SetState(STANDING);
			}
		}
		else
		{
			if(coll.CollisionState == Vine)
			{
				SetState(HANGING);
				grounded = true;
			}
			if (Controls[LEFT] || Controls[RIGHT])
			{
				Facing = Controls[LEFT] ? -1 : 1;
				Velocity.x = (AirControl * (Controls[RUN] ? RunSpeed : WalkSpeed) * Facing) + ((1.0f - AirControl) * Velocity.x);
			}
			else
			{
				Velocity.x = (1.0f - AirControl) * Velocity.x;
			}
			if (Controls[JUMP] & !PrevControls[JUMP])
			{
				timeInCopter = 0;
			}
			if (timeInCopter > CopterTime)
			{
				SetState(DESCENDING);
			}
			if(Controls[DOWN] && !PrevControls[DOWN])
			{
				SetState(PREPARING_BUTTSTOMP);
				Velocity.x = 0;
				Velocity.y = 0;
				grounded = true;
			}
			if(event.EventID == VPole)
			{
				grounded = true;
				SetState(VPOLE);
				ExitPoleVelocity = (Velocity.y < 0 ? -1 : 1) * min(MaxPoleExitSpeed, abs(2 * WalkSpeed));
			}
		}
		break;
	case DYING:
		Velocity.x = 0;
		Velocity.y = 0;
		grounded = true;
		if (HasAnimationEnded())
		{
			SetState(DEAD);
		}
		break;
	case SUCKERTUBE:
		if (event.EventID != SuckerTube)
			SetState(BALL);
		break;
	case DEAD:
		break;
	}

	PrevControls = Controls;

	if ((state == DYING) || (state == DEAD))
	{
		return;
	}
	if (((coll.CollisionState == Hurt) || (event.EventID == Hurt)) && !Invincible)
	{
		health--;
		Velocity.x = -Facing * 16.0f;
		Velocity.y = -16.0f;
		grounded = false;
		if (health > 0)
		{
			TimeSinceMadeInvincible = 0;
			Invincible = true;
			SetState(HURT);
		}
		else
		{
			SetState(DYING);
		}
	}
	if (event.EventID == SuckerTube)
	{
		Event e = Event(event);
		if (e.IsActive())
		{
			float XVelocity = e.GetXVelocity();
			float YVelocity = e.GetYVelocity();
			if (XVelocity != 0)
			{
				auto posy = (TileYCoord + 1) * 32.0f;
				{
					Position.y = posy;
					Velocity = vec2(XVelocity, YVelocity);
					SetState(BALL);
				}
			}
			else if (YVelocity != 0)
			{
				auto posx = ((TileXCoord + 1) * 32.0f) - 16.0f;
				if ((posx - Position.x) * (posx - (Position.x + (Velocity.x * dt))) <= 0)
				{
					Position.x = posx;
					Velocity = vec2(XVelocity, YVelocity);
					SetState(BALL);
				}
			}
		}
	}
	if(event.EventID == AreaEndOfLevel)
	{
		SetState(LEVEL_ENDED);
	}
}

uint32_t Player::GetHealth() const
{
	return max(0, (int)health);
}

uint32_t Player::GetLives() const
{
	return max(0, (int)lives);
}

void Player::AddLives(uint8_t add)
{
	lives += add;
}

uint32_t Player::GetScore() const
{
	return score;
}

bool Player::ResolveCollision(CollisionInfo coll)
{
	if ((grounded && ((coll.iBottomLeft != INT_MAX) || (coll.iBottomRight != INT_MAX) || (coll.iBottomCenter != INT_MAX))) ||
		(!grounded && ((coll.iBottomLeft < 0) || (coll.iBottomCenter < 0) || (coll.iBottomRight < 0))))
	{
		int offset = min(min(coll.iBottomLeft, coll.iBottomRight), coll.iBottomCenter);
		Position.y += offset;
		coll.iBottomCenter = min(0, coll.iBottomCenter - offset);
		coll.iBottomLeft = min(0, coll.iBottomLeft - offset);
		coll.iBottomRight = min(0, coll.iBottomRight - offset);
		Velocity.y = 0;
	}
	if (coll.iTop < 0)
	{
		if(coll.CollisionState == Vine)
		{
			Position.y += (-4 - coll.iTop);
			coll.iTop = -4;
			Velocity.y = 0;
		}
		else
		{
			Position.y -= coll.iTop;
			coll.iTop = 0;
			Velocity.y = max(Velocity.y, 0.0f);
		}
	}
	if (coll.iLeft < 0)
	{
		Position.x -= coll.iLeft;
		coll.iLeft = 0;
		Velocity.x = max(0.0f, Velocity.x);
	}
	if (coll.iRight < 0)
	{
		Position.x += coll.iRight;
		coll.iRight = 0;
		Velocity.x = min(0.0f, Velocity.x);
	}

	coll = CheckCollision(Position);
	if (((coll.iTop < 0) && (coll.CollisionState != Vine)) || (coll.iLeft < 0) || (coll.iRight < 0) || (coll.iBottomCenter < 0) || ((coll.iBottomCenter > 0) && ((coll.iBottomLeft < 0) || (coll.iBottomRight < 0))))
	{
		return false;
	}

	return true;
}

inline void AdjustClipMask(uint8_t id, uint8_t *mask)
{
	uint8_t value = 0xFF;
	switch(id)
	{
		case OneWay:
		case Vine:
			value = id;
			break;
		default:
			break;
	}
	for(int i = 0; i < 1024; i++)
	{
		mask[i] = (mask[i]) ? value : 0x00;
	}
}

uint8_t *Player::GetClipOverlap()
{
	J2L_Tile tileInfo;
	int32_t layerWidth = level->GetLayerWidth(3);
	int32_t layerHeight = level->GetLayerHeight(3);
	int32_t tileXCoord = (int32_t)Math::Floor((Position.x - ((BBOX_WIDTH/2)+6)) / 32);	// Get the tile that contains the pixel 16 pixels to the left
	int32_t tileYCoord = (int32_t)Math::Floor((Position.y - (BBOX_HEIGHT + 6)) / 32);	// and 16 pixels above the collision volume. This will be the upper left corner
	int32_t tileCoord = (((int32_t)layerWidth) * tileYCoord) + tileXCoord;
	if ((tileCoord != lastTileCoord) || (quad == nullptr))
	{
		if (quad != nullptr)
			delete[] quad;
		quad = new uint8_t[96 * 96];
		memset(quad, 0, 96 * 96);

		if((tileXCoord > (-2)) && (tileYCoord > (-2)))
		{
			uint8_t *clip = nullptr;
			if ((tileXCoord >= 0) && (tileXCoord < layerWidth) && (tileYCoord >= 0) && (tileYCoord < layerHeight))
			{
				tileInfo = level->GetTile(3, tileXCoord, tileYCoord, 0);
				clip = tileset->GetClipMask(tileInfo.index, tileInfo.flipped);
				J2L_Event event = level->GetEvents(tileXCoord, tileYCoord);
				if(event.EventID != 0)
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
				if(event.EventID != 0)
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
				if(event.EventID != 0)
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
				if(event.EventID != 0)
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
				if(event.EventID != 0)
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
				if(event.EventID != 0)
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
				if(event.EventID != 0)
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
				if(event.EventID != 0)
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
				if(event.EventID != 0)
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

	uint8_t *overlap = new uint8_t[(BBOX_WIDTH+12) * (BBOX_HEIGHT+12)];
	int xOffset = (int)Math::Round(Position.x - ((BBOX_WIDTH/2)+6)) - (32 * tileXCoord);
	int yOffset = (int)Math::Round(Position.y - (BBOX_HEIGHT+6)) - (32 * tileYCoord);
	for (int i = 0; i < (BBOX_HEIGHT+12); i++)
	{
		memcpy(&overlap[(BBOX_WIDTH+12) * i], &quad[96 * (i + yOffset) + xOffset], (BBOX_WIDTH+12));
	}
	
	return overlap;
}

template<typename T>
bool contains(vector<T> collection, T item)
{
	for (auto it = collection.begin(); it != collection.end(); it++)
	{
		if ((*it) == item)
			return true;
	}
	return false;
}

CollisionInfo Player::CheckCollision(const vec2 &PreviousPosition)
{
	CollisionInfo coll;
	coll.CollisionState = 0;
	coll.iBottomLeft = INT_MAX;
	coll.iBottomCenter = INT_MAX;
	coll.iBottomRight = INT_MAX;
	coll.iLeft = INT_MAX;
	coll.iRight = INT_MAX;
	coll.iTop = INT_MAX;
	uint8_t *clip = GetClipOverlap();

#ifdef SAVE_QUAD
	uint32_t *pixels = new uint32_t[64 * 64];
	for (int i = 0; i < (32 * 32); i++)
	{
		pixels[i] = (clip[i]) ? 0xFF000000 : 0x00000000;
		if ((((i % 32) > 6) && ((i % 32) < 25)) && ((i / 32) > 6) && ((i / 32) < 25))
		{
			pixels[i] |= 0x0000FF00;
		}
	}
	SDL_Surface *quadSurface = SDL_CreateRGBSurfaceFrom(pixels, 32, 32, 32, 32 * 4, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x00000000);
	if (0 < SDL_SaveBMP(quadSurface, "/home/specialfred453/Desktop/quad.bmp"))
	{
		const char *error = SDL_GetError();
		printf("Error saving bmp: %s\n", error);
	}
	SDL_FreeSurface(quadSurface);
	delete[] pixels;
#endif

	bool clipTopReached = false;
	bool clipLeftReached = false;
	bool clipRightReached = false;
	
	if (clip != nullptr)
	{
		// Check for collision to the top coords
		for (int i = 6; i > -6 ; i--)
		{
			uint32_t coord1 = (BBOX_WIDTH + 12) * (6 - i) + 9;
			uint32_t coord2 = ((BBOX_WIDTH + 12) * (6 - i)) + (BBOX_WIDTH + 3);
			
			if (((clip[coord1] == 0xFF) || (clip[coord2] == 0xFF)) && !clipTopReached)
			{
				coll.iTop = i;
			}
			else if ((clip[coord1] == Vine) && (clip[coord2] == Vine) && !clipTopReached)
			{
				coll.iTop = i;
				coll.CollisionState = Vine;
				if (i < -4)
				{
					coll.CollisionState = 0;
					coll.iTop = INT_MAX;
				}
			}
			else if ((clip[coord1] == ButtStompScenery) || (clip[coord2] == ButtStompScenery))
			{
				coll.CollisionState = ButtStompScenery;
			}
			else if ((clip[coord1] == DestructScenery) || (clip[coord2] == DestructScenery))
			{
				coll.CollisionState = DestructScenery;
			}
		}
		// Check for collision to the bottom left coord
		for (int i = 6; i > -6; i--)
		{
			uint32_t coord1 = (BBOX_WIDTH+12) * ((BBOX_HEIGHT+11) - (6-i)) + 9;
			uint32_t coord2 = ((BBOX_WIDTH + 12) * ((BBOX_HEIGHT + 11) - (6 - i))) + ((BBOX_WIDTH / 2) + 6);
			uint32_t coord3 = ((BBOX_WIDTH + 12) * ((BBOX_HEIGHT + 11) - (6 - i))) + (BBOX_WIDTH + 3);
			if ((clip[coord1] == 0xFF) || ((clip[coord1] == OneWay) && (((Position.y - PreviousPosition.y) + i) >= 0)))
				coll.iBottomLeft = i;
			if ((clip[coord2] == 0xFF) || ((clip[coord1] == OneWay) && (((Position.y - PreviousPosition.y) + i) >= 0)))
				coll.iBottomCenter = i;
			if ((clip[coord3] == 0xFF) || ((clip[coord1] == OneWay) && (((Position.y - PreviousPosition.y) + i) >= 0)))
				coll.iBottomRight = i;
		}
		// Check for collision to the bottom right coord
		for (int i = 6; i > -6; i--)
		{
			uint32_t coord1 = ((BBOX_WIDTH + 12) * 20) + (6 - i);
			uint32_t coord2 = ((BBOX_WIDTH + 12) * 20) + ((BBOX_WIDTH + 11) - (6 - i));
			if ((clip[coord1] == 0xFF) && !clipLeftReached)
			{
				coll.iLeft = i;
			}
			if ((clip[coord2] == 0xFF) && !clipRightReached)
			{
				coll.iRight = i;
			}
		}
	}
	delete[] clip;

	return coll;
}

bool Player::CollidedWithActor(const Actor &actor)
{
	bool consumed = false;
	if (actor.isFood())
	{
		AddFood(actor.isFood() ? 1 : 0);
		consumed = true;
	}
	if (actor.GetMoneyAdd() > 0)
	{
		AddMoney(actor.GetMoneyAdd());
		consumed = true;
	}
	if (actor.GetGemValue() > 0)
	{
		AddGems(actor.GetGemType(), actor.GetGemValue());
		consumed = true;
	}
	if (actor.GetLivesAdd() > 0)
	{
		AddLives(actor.GetLivesAdd());
		consumed = true;
	}
	if (actor.GetHealthAdd() > 0)
	{
		auto add = actor.GetHealthAdd();
		auto remainder = AddHealth(actor.GetHealthAdd());
		if(add > remainder)
			consumed = true;
	}
	if (actor.GetAmmoAdd() > 0)
	{
		AddAmmo(actor.GetAmmoType(), actor.GetAmmoAdd());
		consumed = true;
	}
	if (actor.GetEventID() == RedSpring)
	{
		Position.x = actor.GetPosition().x;
		Velocity.x = 0;
		Velocity.y = RedSpringJumpSpeed;
		SetState(SPRING_JUMP);
		grounded = false;
		SetSpringInfluenced(RedSpringJumpDuration);
	}
	else if (actor.GetEventID() == GreenSpring)
	{
		Position.x = actor.GetPosition().x;
		Velocity.x = 0;
		Velocity.y = GreenSpringJumpSpeed;
		SetState(SPRING_JUMP);
		grounded = false;
		SetSpringInfluenced(GreenSpringJumpDuration);
	}
	else if (actor.GetEventID() == BlueSpring)
	{
		Position.x = actor.GetPosition().x;
		Velocity.x = 0;
		Velocity.y = BlueSpringJumpSpeed;
		SetState(SPRING_JUMP);
		grounded = false;
		SetSpringInfluenced(BlueSpringJumpDuration);
	}
	else if (actor.GetEventID() == HorRedSpring)
	{
		SetState(grounded ? RUNNING : RUNNING_JUMP);
		SetSpringInfluenced(FLT_MAX);
		Velocity.x = HorRedSpringVelocity * (actor.IsFlipped() ? -1.0f : 1.0f);
		if (Velocity.x < 0)
			Facing = -1;
		else
			Facing = 1;
	}
	else if (actor.GetEventID() == HorGreenSpring)
	{
		SetState(grounded ? RUNNING : RUNNING_JUMP);
		SetSpringInfluenced(FLT_MAX);
		Velocity.x = HorGreenSpringVelocity * (actor.IsFlipped() ? -1.0f : 1.0f);
		if (Velocity.x < 0)
			Facing = -1;
		else
			Facing = 1;
	}
	else if (actor.GetEventID() == HorBlueSpring)
	{ 
		SetState(grounded ? RUNNING : RUNNING_JUMP);
		SetSpringInfluenced(FLT_MAX);
		Velocity.x = HorBlueSpringVelocity * (actor.IsFlipped() ? -1.0f : 1.0f);
		if (Velocity.x < 0)
			Facing = -1;
		else
			Facing = 1;
	}
	return consumed;
}

Player::~Player()
{
	if (quad != nullptr)
		delete[] quad;
}
