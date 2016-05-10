#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <cstdint>
#include <map>
#include "Math.h"
#include "Controls.h"
#include "Level.h"
#include "Tileset.h"
#include "Animation.h"

#define MaxVelocity 512.0f			// Measured
#define WalkSpeed 270.0f			// Measured
#define RunSpeed 580.0f				// Measured
#define Gravity (1536.0f)			// Measured
#define FallingGravity (768.0f)		// Measured
#define Acceleration 0.125f			// 
#define JumpVelocity -480.0f		// Measured
#define HighJumpVelocity -480.0f	// Measured
#define JumpDuration 0.125f			// Measured
#define JumpAcceleration 0.025f		// 
#define HighJumpAcceleration 0.075f	// 
#define HighJumpDuration 0.1f		// Measured
#define CopterDescentSpeed 40.0f	// Measured
#define CopterTime 1.0f				// 
#define HighJumpTime 0.25f 			// Measured
#define AirControl 0.15f			// Measured
#define MaxHangingMoveSpeed 128.0f 	// Measured
#define MinHangingMoveSpeed 64.0f 	// Measured
#define HangingMoveSpeedPeriod 7.85f // Measured
#define HangingMoveSpeedPeriodOffset 2.8f
#define MaxFastHangingMoveSpeed 256.0f // Measured
#define ButtstompVelocity 1024.0f 	// Needs Tweaked
#define HPoleExitSpeed 1024.0f 		// Needs Tweaked
#define InvincibilityDuration 2.0f	// Needs Tweaked

enum PlayerState
{
	STANDING,
	IDLE1,
	IDLE2,
	IDLE3,
	IDLE4,
	IDLE5,
	OFF_BALANCE,
	WALKING,
	PUSHING,
	RUNNING,
	STARTING_SKID,
	SKIDDING,
	ENDING_SKID,
	RUNNING_JUMP,
	JUMPING,
	LANDING,
	STARTING_HIGHJUMP,
	HIGHJUMP,
	ENDING_HIGHJUMP,
	ASCENDING,
	DESCENDING,
	COPTER,
	HANGING,
	HANGING_IDLE,
	HANGING_MOVING,
	PREPARING_BUTTSTOMP,
	BUTTSTOMP,
	ENDING_BUTTSTOMP,
	DYING,
	DEAD, 
	CROUCHING, 
	CROUCHED, 
	EXITING_CROUCH,
	VPOLE,
	HPOLE,
	BALL,
	HURT,
	END_REACHED,
	LEVEL_ENDING,
	LEVEL_ENDED
};

struct CollisionInfo
{
	// Interpenetration Values:
	// x < 0: no collision (x is either INT_MAX, or the number of pixels, up to 5, away from adjacent obstacle)
	// x == 0: collision (immediately adjacent to obstacle)
	// x > 0: intepenetration (-x pixels into obstacle)
	uint8_t CollisionState;
	int iLeft;
	int iRight;
	int iBottomLeft;
	int iBottomCenter;
	int iBottomRight;
	int iTop;
};

class Player
{
private:
	PlayerState state;
	int32_t Facing;
	float ExitPoleVelocity;
	vec2 Position;
	vec2 Velocity;
	const Level *level;
	const Tileset *tileset;
	const Animations *anims;
	bool grounded; 
	bool canLoseBalance;
	float timeSinceStateChanged = 0;
	int8_t health;
	uint32_t score;
	int8_t food;
	uint32_t money;
	uint16_t RedGems;
	uint16_t GreenGems;
	uint16_t BlueGems;
	uint16_t PurpleGems;
	bool Invincible;
	float TimeSinceMadeInvincible;

	uint32_t lastTileCoord;
	uint8_t *quad;

	float timeInCopter;
	std::map<Control, bool> PrevControls;

	uint8_t *GetClipOverlap();
	CollisionInfo CheckCollision(const vec2 &PreviousPosition);
	bool ResolveCollision(CollisionInfo coll);
	bool HasAnimationEnded() const;
	void SetState(const PlayerState &newState);
public:
	Player(const vec2 &startPosition, const Level *level, const Tileset *tileset, const Animations *anims);
	~Player();
	PlayerState GetState() const;
	vec2 GetPosition() const;
	Rect GetBoundingBox() const;
	int GetFacing() const;
	const AnimationFrame *GetSprite() const;
	uint32_t GetHealth() const;
	uint32_t GetScore() const;
	uint8_t AddHealth(const uint8_t &add);
	void AddPoints(const uint32_t &points);
	uint8_t AddFood(const uint8_t &food);
	uint8_t AddMoney(const uint8_t &add);
	uint8_t AddGems(const GemType &type, const uint8_t &add);
	bool IsInvisible() const;
	void Update(const float &timeSinceLastUpdate, std::map<Control, bool> Keyboard);
};

#endif