#ifndef _ANIM_H_
#define _ANIM_H_

#include "Structures.h"
#include "Math.h"

#pragma region Animation Set Defines

#pragma region Ammo Animations

#define ANIM_SET_AMMO 0
#define ANIM_AMMO_BOUNCER1 25

#pragma endregion AmmoAnimations

#pragma region Jazz Player Animations

#define ANIM_SET_JAZZ 54
#define ANIM_JAZZ_HOVERBOARD 1
#define ANIM_JAZZ_HOVERBOARD_TURN_AROUND 2
#define ANIM_JAZZ_ENDING_BUTTSTOMP 3
#define ANIM_JAZZ_DEAD 4
#define ANIM_JAZZ_DEATH 5
#define ANIM_JAZZ_CROUCHING 6
#define ANIM_JAZZ_EXITING_CROUCH 9
#define ANIM_JAZZ_VINE_MOVING 11
#define ANIM_JAZZ_LEVEL_EXIT 12
#define ANIM_JAZZ_DESC_1 13
#define ANIM_JAZZ_BUTTSTOMP 14
#define ANIM_JAZZ_LANDING 15
#define ANIM_JAZZ_SHOOT 16
#define ANIM_JAZZ_SHOOT_UP 17
#define ANIM_JAZZ_DONE_SHOOTING 18
#define ANIM_JAZZ_VINE_SHOOT_UP 23
#define ANIM_JAZZ_VINE 24
#define ANIM_JAZZ_VINE_IDLE 25
#define ANIM_JAZZ_VINE_SHOOT 27
#define ANIM_JAZZ_COPTER 28
#define ANIM_JAZZ_COPTER_SHOOT 30
#define ANIM_JAZZ_HPOLE 31
#define ANIM_JAZZ_HURT 32
#define ANIM_JAZZ_IDLE1 33
#define ANIM_JAZZ_IDLE2 34
#define ANIM_JAZZ_IDLE3 35
#define ANIM_JAZZ_IDLE4 36
#define ANIM_JAZZ_IDLE5 37
#define ANIM_JAZZ_DESC_GUN 39
#define ANIM_JAZZ_DESC 40
#define ANIM_JAZZ_JUMP 41
#define ANIM_JAZZ_ASCENDING 42
#define ANIM_JAZZ_OFF_BALANCE 43
#define ANIM_JAZZ_LOOK_UP 47
#define ANIM_JAZZ_DIZZY 55
#define ANIM_JAZZ_PUSHING 56
#define ANIM_JAZZ_DESC_SIDE 61
#define ANIM_JAZZ_JUMP_SIDE 63
#define ANIM_JAZZ_BALL 67
#define ANIM_JAZZ_WALKING 68
#define ANIM_JAZZ_STARTING_TO_RUN 70
#define ANIM_JAZZ_RUNNING 71
#define ANIM_JAZZ_SKIDDING 72
#define ANIM_JAZZ_STARTING_SKID 73
#define ANIM_JAZZ_ENDING_SKID 74
#define ANIM_JAZZ_BUTTSTOMP_PENDING 75
#define ANIM_JAZZ_SUPER_JUMP 79
#define ANIM_JAZZ_DRUNK 80
#define ANIM_JAZZ_SWIM_DOWN 81
#define ANIM_JAZZ_SWIM 82
#define ANIM_JAZZ_SWIM_TRANSITION_DOWN 83
#define ANIM_JAZZ_SWIM_TRANSITION_UP 84
#define ANIM_JAZZ_SWIM_UP 85
#define ANIM_JAZZ_SWINGING 86
#define ANIM_JAZZ_VPOLE 92

#pragma endregion Jazz Player Animations

#pragma region Spaz Player Animations

#define ANIM_SET_SPAZ 85

#pragma endregion Spaz Player Animations

#pragma region Item Animations

#define ANIM_SET_ITEMS 67
#define ANIM_1UP 0
#define ANIM_APPLE 1
#define ANIM_BANANA 2
#define ANIM_BARREL 3
#define ANIM_CRATE 5
#define ANIM_FLAME_UPGRADE 10
#define ANIM_CAKE 11
#define ANIM_BURGER 12
#define ANIM_CANDY 13
#define ANIM_CHECKPOINT 14
#define ANIM_CHEESE 15
#define ANIM_CHERRY 16
#define ANIM_CHICKEN 17
#define ANIM_CHIPS 18
#define ANIM_CHOCOLATE 19
#define ANIM_COKE 20
#define ANIM_CARROT 21
#define ANIM_GEM 22
#define ANIM_PICKLE 23
#define ANIM_CUPCAKE 24
#define ANIM_DONUT 25
#define ANIM_EGGPLANT 26
#define ANIM_UNKNOWN_1 27
#define ANIM_EXIT 28
#define ANIM_GUN 29
#define ANIM_GUN2 30
#define ANIM_FLAME_UPGRADE_2 31
#define ANIM_FRIES 32
#define ANIM_SUPER_SHOES_POWERUP 33
#define ANIM_HOVERBOARD 36
#define ANIM_GOLD_COIN 37
#define ANIM_GRAPES 38
#define ANIM_HAM 39
#define ANIM_FLYING_CARROT 40
#define ANIM_HEART 41
#define ANIM_HOURGLASS 42
#define ANIM_ICE_CREAM 43
#define ANIM_PEAR 48
#define ANIM_LETTUCE 49
#define ANIM_PEAR_2 50
#define ANIM_LIGHTNING_POWERUP 51
#define ANIM_CRATE_2 52
#define ANIM_MILK 53
#define ANIM_BOUNCE_AMMO_CRATE 54
#define ANIM_ICE_AMMO_CRATE 55
#define ANIM_ROCKET_AMMO_CRATE 56
#define ANIM_ROCKET2_AMMO_CRATE 57
#define ANIM_FLAME_AMMO_CRATE 58
#define ANIM_TNT_AMMO_CRATE 59
#define ANIM_GUN_UPGRADE 60
#define ANIM_BOUNCE_AMMO_UPGRADE 61
#define ANIM_ICE_AMMO_UPGRADE 62
#define ANIM_ROCKET_AMMO_UPGRADE 63
#define ANIM_ROCKET2_AMMO_UPGRADE 64
#define ANIM_JAZZSPAZ 70
#define ANIM_TOMATO 71
#define ANIM_CARROT_2 72
#define ANIM_PEACH 73
#define ANIM_PEAR_3 74
#define ANIM_JAZZ_SODA 75
#define ANIM_PIE 76
#define ANIM_PIZZA 77
#define ANIM_POTION 78
#define ANIM_PRETZEL 79
#define ANIM_SANDWICH 80
#define ANIM_STRAWBERRY 81
#define ANIM_CARROT_FLOP 82
#define ANIM_SPAZ_BLASTER_POWERUP 83
#define ANIM_SILVER_COIN 84
#define ANIM_UNKNOWN_2 85
#define ANIM_SPARKLE 86
#define ANIM_CLOCK 87
#define ANIM_TACO 88
#define ANIM_MEATBALL 89
#define ANIM_BOMB 90
#define ANIM_HOTDOG 91
#define ANIM_WATERMELON 92
#define ANIM_WOODEN_SHARD_1 93
#define ANIM_WOODEN_SHARD_2 94

#pragma endregion Item Animations

#pragma region Game Setup Menu Animations

#define ANIM_SET_GAME_SETUP_MENU 60

#pragma endregion Game Setup Menu Animations

#pragma region Head Animations

#define ANIM_SET_HEADS 38
#define ANIM_HEAD_JAZZ 3
#define ANIM_HEAD_SPAZ 4

#pragma endregion Head Animations

#pragma region Menu Animations

#define ANIM_SET_MENU 61
#define FONT_OFFSET -32
#define ANIM_SPRITEFONT_LARGE 0
#define ANIM_SPRITEFONT_SMALL 1

#define ANIM_TITLE_BIG 2
#define ANIM_TITLE_SMALL 3

#pragma endregion Menu Animations

#pragma region Spring Animations

#define ANIM_SET_SPRINGS 92
#define ANIM_SPRING_UP_BLUE 0
#define ANIM_SPRING_RT_BLUE 1
#define ANIM_SPRING_DN_BLUE 2
#define ANIM_SPRING_DN_GREEN 3
#define ANIM_SPRING_DN_RED 4
#define ANIM_SPRING_UP_GREEN 5
#define ANIM_SPRING_RT_GREEN 6
#define ANIM_SPRING_UP_RED 7
#define ANIM_SPRING_RT_RED 8

#pragma endregion Spring Animations

#pragma region NPCs

#pragma region Bonus

// Each of these only have 1 animation, so just one define for the animation should be sufficient
#define ANIM_BONUS 0 
#define ANIM_BONUS_VACANT 1

#define ANIM_SET_BONUS_VACANT 11
#define ANIM_SET_BONUS_10 103
#define ANIM_SET_BONUS_100 104
#define ANIM_SET_BONUS_20 105
#define ANIM_SET_BONUS_50 106

#pragma endregion Bonus

#pragma endregion NPCs

#pragma region Enemies

#define ANIM_SET_WITCH 108
#define ANIM_WITCH_CAST 0
#define ANIM_WITCH_DEATH 1
#define ANIM_WITCH_FLY 2
#define ANIM_WITCH_SPELL 3

#pragma endregion Enemies

#pragma endregion Animation Set Defines

class FrameImage
{
private:
	uint16_t width;
	uint16_t height;
	uint32_t *pixels;	// Image Pixels
	uint8_t *clip;

public:
	FrameImage(uint8_t *bytes, uint32_t length, const uint32_t *palette);
	FrameImage(const FrameImage &other);
	~FrameImage();
	uint16_t getWidth() const;
	uint16_t getHeight() const;
	uint32_t *getPixels() const;
	uint8_t *getClip() const;
	void SaveBMP(const char *filename) const;
};

class AnimationFrame
{
private:
	FrameInfo info;
	FrameImage image;
	uint32_t frameIndex;
public:
	AnimationFrame(FrameInfo info, FrameImage image);
	~AnimationFrame();
	uint32_t getIndex() const;
	uint32_t getWidth() const;
	uint32_t getHeight() const;
	uint32_t *getImage() const;
	uint8_t *getClip() const;
	vec2 getHotSpot() const;
	vec2 getColdSpot() const;
	vec2 getGunSpot() const;
	void SaveBMP(const char *filename) const;
};

class Animation
{
private:
	AnimInfo info;
	std::vector<AnimationFrame> frames;
public:
	Animation(AnimInfo info, std::vector<AnimationFrame> frames);
	uint32_t GetFrameCount() const;
	uint32_t GetFrameRate() const;
	const AnimationFrame *GetFrame(const uint32_t &index) const;
	~Animation();
};

class AnimationSet
{
private:
	ANIM_Header header;
	std::vector<Animation> Anims;
public:
	AnimationSet(FILE *fi, const uint32_t *palette);
	uint32_t GetAnimCount() const;
	const Animation *GetAnim(const uint32_t &index) const;
	~AnimationSet();
};

class Animations
{
private:
	std::vector<AnimationSet> animSets;
public:
	Animations(const char *filename);
	void DumpToDisk(const char *directory) const;
	uint32_t GetAnimSetCount() const;
	const AnimationSet *GetAnimSet(const uint32_t &index) const;
	~Animations();
};

#endif
