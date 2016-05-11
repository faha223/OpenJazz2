#ifndef _STRUCTURES_H_
#define _STRUCTURES_H_

/* This logic was copied, almost verbatim, from the following resources:
 * http://www.jazz2online.com/wiki/J2L+File+Format
 * http://www.jazz2online.com/wiki/J2A+File+Format
 * http://www.jazz2online.com/wiki/J2T+File+Format
 */

#include <cstdint>
#include <string>
#include <vector>
#include <map>

#include "FileIO.h"

#define VERSION_1_23 514
#define VERSION_TSF 515
#define VERSION_GIGANTIC 256

#define TILE_VERSION_1_23 0x200
#define TILE_VERSION_1_24 0x201

#define MAX_TILES_1_23 1024
#define MAX_TILES_TSF 4096

#define FLIPPED_1_23 0x0400
#define FLIPPED_TSF 0x1000

#define ANIM_COUNT 109

#pragma region helper functions

template<typename Type>
inline void ReadAndAdvance(Type &value, uint8_t *bytes, int &offset)
{
	memcpy(&value, &bytes[offset], sizeof(Type));
	offset += sizeof(Type);
}

inline void ReadStringAndAdvance(char *buffer, uint32_t bufferLength, uint8_t *bytes, int &offset)
{
	memcpy(buffer, &bytes[offset], bufferLength);
	offset += bufferLength;
}

struct vertex
{
	float x, y, z;
	float s, t;
};

struct SpriteCoords
{
	int TextureIndex;
	float x;
	float y;
	float width;
	float height;
};

enum GemType
{
	GT_None,
	GT_Red,
	GT_Blue,
	GT_Green,
	GT_Purple
};

#pragma endregion

#pragma region File Data Structures

#pragma region Level Structures

enum EventID : uint8_t
{
	None = 0,
	OneWay = 1,
	Hurt = 2,
	Vine = 3,
	Hook = 4,
	Slide = 5,
	HPole = 6,
	VPole = 7,
	AreaFlyOff = 8,
	Ricochet = 9,
	BeltRight = 10,
	BeltLeft = 11,
	AccBeltR = 12,
	AccBeltL = 13,
	StopEnemy = 14,
	WindLeft = 15,
	WindRight = 16,
	AreaEndOfLevel = 17,
	AreaWarpEOL = 18,
	AreaRevertMorph = 19,
	AreaFloatUp = 20,
	TriggerRock = 21,
	DimLight = 22,
	SetLight = 23,
	LimitXScroll = 24,
	ResetLight = 25,
	AreaWarpSecret = 26,
	Echo = 27,
	ActivateBoss = 28,
	JazzLevelStart = 29,
	SpazLevelStart = 30,
	MultiplayerLevelStart = 31,
	FreezerAmmoPlus3 = 33,
	BouncerAmmoPlus3 = 34,
	SeekerAmmoPlus3 = 35,
	ThreeWayAmmoPlus3 = 36,
	ToasterAmmoPlus3 = 37,
	TNTAmmoPlus3 = 38,
	Gun8AmmoPlus3 = 39,
	Gun9AmmoPlus3 = 40,
	StillTurtleshell = 41,
	SwingingVine = 42,
	Bomb = 43,
	SilverCoin = 44,
	GoldCoin = 45,
	Guncrate = 46,
	Carrotcrate = 47,
	OneUpcrate = 48,
	Gembarrel = 49,
	Carrotbarrel = 50,
	OneUpBarrel = 51,
	BombCrate = 52,
	FreezerAmmoPlus15 = 53,
	BouncerAmmoPlus15 = 54,
	SeekerAmmoPlus15 = 55,
	ThreeWayAmmoPlus15 = 56,
	ToasterAmmoPlus15 = 57,
	TNT = 58,
	Airboard = 59,
	FrozenGreenSpring = 60,
	GunFastFire = 61,
	SpringCrate = 62,
	RedGemPlus1 = 63,
	GreenGemPlus1 = 64,
	BlueGemPlus1 = 65,
	PurpleGemPlus1 = 66,
	SuperRedGem = 67,
	Birdy = 68,
	GunBarrel = 69,
	GemCrate = 70,
	JazzSpaz = 71,
	CarrotEnergyPlus1 = 72,
	FullEnergy = 73,
	FireShield = 74,
	WaterShield = 75,
	LightningShield = 76,
	MaxWeapon = 77,
	Autofire = 78,
	FastFeet = 79,
	ExtraLive = 80,
	EndofLevelsignpost = 81,
	Sparkle = 82,
	Savepointsignpost = 83,
	BonusLevelsignpost = 84,
	RedSpring = 85,
	GreenSpring = 86,
	BlueSpring = 87,
	Invincibility = 88,
	ExtraTime = 89,
	FreezeEnemies = 90,
	HorRedSpring = 91,
	HorGreenSpring = 92,
	HorBlueSpring = 93,
	MorphIntoBird = 94,
	SceneryTriggerCrate = 95,
	Flycarrot = 96,
	RectGemRed = 97,
	RectGemGreen = 98,
	RectGemBlue = 99,
	TufTurt = 100,
	TufBoss = 101,
	LabRat = 102,
	Dragon = 103,
	Lizard = 104,
	Bee = 105,
	Rapier = 106,
	Sparks = 107,
	Bat = 108,
	Sucker = 109,
	Caterpillar = 110,
	Cheshire1 = 111,
	Cheshire2 = 112,
	Hatter = 113,
	BilsyBoss = 114,
	Skeleton = 115,
	DoggyDogg = 116,
	NormTurtle = 117,
	Helmut = 118,
	Leaf = 119,
	Demon = 120,
	Fire = 121,
	Lava = 122,
	DragonFly = 123,
	Monkey = 124,
	FatChick = 125,
	Fencer = 126,
	Fish = 127,
	Moth = 128,
	Steam = 129,
	RotatingRock = 130,
	BlasterPowerUp = 131,
	BouncyPowerUp = 132,
	IcegunPowerUp = 133,
	SeekPowerUp = 134,
	RFPowerUp = 135,
	ToasterPowerUP = 136,
	PINLeftPaddle = 137,
	PINRightPaddle = 138,
	PIN500Bump = 139,
	PINCarrotBump = 140,
	Apple = 141,
	Banana = 142,
	Cherry = 143,
	Orange = 144,
	Pear = 145,
	Pretzel = 146,
	Strawberry = 147,
	SteadyLight = 148,
	PulzeLight = 149,
	FlickerLight = 150,
	QueenBoss = 151,
	FloatingSucker = 152,
	Bridge = 153,
	Lemon = 154,
	Lime = 155,
	Thing = 156,
	Watermelon = 157,
	Peach = 158,
	Grapes = 159,
	Lettuce = 160,
	Eggplant = 161,
	Cucumb = 162,
	SoftDrink = 163,
	SodaPop = 164,
	Milk = 165,
	Pie = 166,
	Cake = 167,
	Donut = 168,
	Cupcake = 169,
	Chips = 170,
	Candy = 171,
	Chocbar = 172,
	Icecream = 173,
	Burger = 174,
	Pizza = 175,
	Fries = 176,
	ChickenLeg = 177,
	Sandwich = 178,
	Taco = 179,
	Weenie = 180,
	Ham = 181,
	Cheese = 182,
	FloatLizard = 183,
	StandMonkey = 184,
	DestructScenery = 185,
	DestructSceneryBOMB = 186,
	CollapsingScenery = 187,
	ButtStompScenery = 188,
	InvisibleGemStomp = 189,
	Raven = 190,
	TubeTurtle = 191,
	GemRing = 192,
	SmallTree = 193,
	AmbientSound = 194,
	Uterus = 195,
	Crab = 196,
	Witch = 197,
	RocketTurtle = 198,
	Bubba = 199,
	Devildevanboss = 200,
	Devan = 201,
	Robot = 202,
	Carrotuspole = 203,
	Psychpole = 204,
	Diamonduspole = 205,
	SuckerTube = 206,
	Text = 207,
	WaterLevel = 208,
	FruitPlatform = 209,
	BollPlatform = 210,
	GrassPlatform = 211,
	PinkPlatform = 212,
	SonicPlatform = 213,
	SpikePlatform = 214,
	SpikeBoll = 215,
	Generator = 216,
	Eva = 217,
	Bubbler = 218,
	TNTPowerup = 219,
	Gun8Powerup = 220,
	Gun9Powerup = 221,
	MorphFrog = 222,
	ThreeDSpikeBoll = 223,
	Springcord = 224,
	Bees = 225,
	Copter = 226,
	LaserShield = 227,
	Stopwatch = 228,
	JunglePole = 229,
	Warp = 230,
	BigRock = 231,
	BigBox = 232,
	WaterBlock = 233,
	TriggerScenery = 234,
	BollyBoss = 235,
	Butterfly = 236,
	BeeBoy = 237,
	Snow = 238,
	WarpTarget = 240,
	TweedleBoss = 241,
	AreaId = 242,
	CTFBasePlusFlag = 244,
	NoFireZone = 245
};

struct LEVL_Header
{
	char Copyright[180];
	char Magic[4]; // Should be 'LEVL'
	uint8_t PasswordHash[4]; // 0x00BABE00 for no password
	char LevelName[32];
	uint16_t Version;
	uint32_t FileSize;
	uint32_t CRC32;
	uint32_t CData1;            // compressed size of Data1
	uint32_t UData1;            // uncompressed size of Data1
	uint32_t CData2;            // compressed size of Data2
	uint32_t UData2;            // uncompressed size of Data2
	uint32_t CData3;            // compressed size of Data3
	uint32_t UData3;            // uncompressed size of Data3
	uint32_t CData4;            // compressed size of Data4
	uint32_t UData4;            // uncompressed size of Data4
};	

struct TILE_Header
{
	char Copyright[180];
	char Magic[4]; // Should be 'TILE'
	uint8_t Signature[4]; // 0xDEADBEAF
	char TilesetName[32];
	uint16_t Version;
	uint32_t FileSize;
	uint32_t CRC32;
	uint32_t CData1;            // compressed size of Data1
	uint32_t UData1;            // uncompressed size of Data1
	uint32_t CData2;            // compressed size of Data2
	uint32_t UData2;            // uncompressed size of Data2
	uint32_t CData3;            // compressed size of Data3
	uint32_t UData3;            // uncompressed size of Data3
	uint32_t CData4;            // compressed size of Data4
	uint32_t UData4;            // uncompressed size of Data4
};

struct Animated_Tile
{
	uint16_t FrameWait;
	uint16_t RandomWait;
	uint16_t PingPongWait;
	bool PingPong;
	uint8_t Speed;
	uint8_t FrameCount;
	uint16_t Frame[64]; // this can be a flipped tile or another animated tile
};

struct J2L_Data1_1_23
{
	uint16_t JCSHorizontalOffset; // In pixels
	uint16_t Security1; // 0xBA00 if passworded, 0x0000 otherwise
	uint16_t JCSVerticalOffset; // In pixels
	uint16_t Security2; // 0xBE00 if passworded, 0x0000 otherwise
	uint8_t SecAndLayer; // Upper 4 bits are set if passworded, zero otherwise. Lower 4 bits represent the layer number as last saved in JCS.
	uint8_t MinLight; // Multiply by 1.5625 to get value seen in JCS
	uint8_t StartLight; // Multiply by 1.5625 to get value seen in JCS
	uint16_t AnimCount;
	bool VerticalSplitscreen;
	bool IsLevelMultiplayer;
	uint32_t BufferSize;
	char LevelName[32];
	char Tileset[32];
	char BonusLevel[32];
	char NextLevel[32];
	char SecretLevel[32];
	char MusicFile[32];
	char HelpString[16][512];
	uint32_t LayerMiscProperties[8]; // Each property is a bit in the following order: Tile Width, Tile Height, Limit Visible Region, Texture Mode, Parallax Stars. This leaves 27 (32-5) unused bits for each layer?
	uint8_t Type[8]; // name from Michiel; function unknown
	bool DoesLayerHaveAnyTiles[8]; // must always be set to true for layer 4, or JJ2 will crash
	uint32_t LayerWidth[8];
	uint32_t LayerRealWidth[8]; // for when "Tile Width" is checked. The lowest common multiple of LayerWidth and 4.
	uint32_t LayerHeight[8];
	int32_t LayerZAxis[8]; // Should be { -300, -200, -100, 0, 100, 200, 300, 400 }, but nothing happens when you change these
	uint8_t DetailLevel[8]; // is set to 02 for layer 5 in Battle1 and Battle3, but is 00 the rest of the time, at least for JJ2 levels. No clear effect of altering. Name from Michiel.
	int32_t WaveX[8]; // name from Michiel; function unknown
	int32_t WaveY[8]; // name from Michiel; function unknown
	uint32_t LayerXSpeed[8]; // Divide by 65536 to get value seen in JCS
	uint32_t LayerYSpeed[8]; // Divide by 65536 to get value seen in JCSvalue
	uint32_t LayerAutoXSpeed[8]; // Divide by 65536 to get value seen in JCS
	uint32_t LayerAutoYSpeed[8]; // Divide by 65536 to get value seen in JCS
	uint8_t LayerTextureMode[8];
	uint8_t LayerTextureParams[8][3]; // Red, Green, Blue
	uint16_t AnimOffset; // MAX_TILES minus AnimCount, also called StaticTiles
	uint32_t TilesetEvents[MAX_TILES_1_23]; // same format as in Data2, for tiles
	bool IsEachTileFlipped[MAX_TILES_1_23]; // set to 1 if a tile appears flipped anywhere in the level
	uint8_t TileTypes[MAX_TILES_1_23]; // translucent=1 or caption=4, basically. Doesn't work on animated tiles.
	uint8_t XMask[MAX_TILES_1_23]; // tested to equal all zeroes in almost 4000 different levels, and editing it has no appreciable effect.  // Name from Michiel, who claims it is totally unused.
	Animated_Tile Anim[128]; // or [256] in TSF.
	// only the first [AnimCount] are needed; JCS will save all 128/256, but JJ2 will run your level either way.
	uint8_t Padding[512]; //all zeroes; only in levels saved with JCS
};

struct J2L_Data1_TSF
{
	uint16_t JCSHorizontalOffset; // In pixels
	uint16_t Security1; // 0xBA00 if passworded, 0x0000 otherwise
	uint16_t JCSVerticalOffset; // In pixels
	uint16_t Security2; // 0xBE00 if passworded, 0x0000 otherwise
	uint8_t SecAndLayer; // Upper 4 bits are set if passworded, zero otherwise. Lower 4 bits represent the layer number as last saved in JCS.
	uint8_t MinLight; // Multiply by 1.5625 to get value seen in JCS
	uint8_t StartLight; // Multiply by 1.5625 to get value seen in JCS
	uint16_t AnimCount;
	bool VerticalSplitscreen;
	bool IsLevelMultiplayer;
	uint32_t BufferSize;
	char LevelName[32];
	char Tileset[32];
	char BonusLevel[32];
	char NextLevel[32];
	char SecretLevel[32];
	char MusicFile[32];
	char HelpString[16][512];
	uint32_t LayerMiscProperties[8]; // Each property is a bit in the following order: Tile Width, Tile Height, Limit Visible Region, Texture Mode, Parallax Stars. This leaves 27 (32-5) unused bits for each layer?
	uint8_t Type[8]; // name from Michiel; function unknown
	bool DoesLayerHaveAnyTiles[8]; // must always be set to true for layer 4, or JJ2 will crash
	uint32_t LayerWidth[8];
	uint32_t LayerRealWidth[8]; // for when "Tile Width" is checked. The lowest common multiple of LayerWidth and 4.
	uint32_t LayerHeight[8];
	int32_t LayerZAxis[8]; // Should be { -300, -200, -100, 0, 100, 200, 300, 400 }, but nothing happens when you change these
	uint8_t DetailLevel[8]; // is set to 02 for layer 5 in Battle1 and Battle3, but is 00 the rest of the time, at least for JJ2 levels. No clear effect of altering. Name from Michiel.
	int32_t WaveX[8]; // name from Michiel; function unknown
	int32_t WaveY[8]; // name from Michiel; function unknown
	uint32_t LayerXSpeed[8]; // Divide by 65536 to get value seen in JCS
	uint32_t LayerYSpeed[8]; // Divide by 65536 to get value seen in JCSvalue
	uint32_t LayerAutoXSpeed[8]; // Divide by 65536 to get value seen in JCS
	uint32_t LayerAutoYSpeed[8]; // Divide by 65536 to get value seen in JCS
	uint8_t LayerTextureMode[8];
	uint8_t LayerTextureParams[8][3]; // Red, Green, Blue
	uint16_t AnimOffset; // MAX_TILES minus AnimCount, also called StaticTiles
	uint32_t TilesetEvents[MAX_TILES_TSF]; // same format as in Data2, for tiles
	bool IsEachTileFlipped[MAX_TILES_TSF]; // set to 1 if a tile appears flipped anywhere in the level
	uint8_t TileTypes[MAX_TILES_TSF]; // translucent=1 or caption=4, basically. Doesn't work on animated tiles.
	uint8_t XMask[MAX_TILES_TSF]; // tested to equal all zeroes in almost 4000 different levels, and editing it has no appreciable effect.  // Name from Michiel, who claims it is totally unused.
	Animated_Tile Anim[256]; // or [256] in TSF.
	// only the first [AnimCount] are needed; JCS will save all 128/256, but JJ2 will run your level either way.
	uint8_t Padding[512]; //all zeroes; only in levels saved with JCS
};

struct J2L_Data1_AGA
{
	uint16_t JCSHorizontalOffset; // In pixels
	uint16_t Security1; // 0xBA00 if passworded, 0x0000 otherwise
	uint16_t JCSVerticalOffset; // In pixels
	uint16_t Security2; // 0xBE00 if passworded, 0x0000 otherwise
	uint8_t SecAndLayer; // Upper 4 bits are set if passworded, zero otherwise. Lower 4 bits represent the layer number as last saved in JCS.
	uint8_t MinLight; // Multiply by 1.5625 to get value seen in JCS
	uint8_t StartLight; // Multiply by 1.5625 to get value seen in JCS
	uint16_t AnimCount;
	bool VerticalSplitscreen;
	bool IsLevelMultiplayer;
	uint32_t BufferSize;
	char LevelName[32];
	char Tileset[32];
	char BonusLevel[32];
	char NextLevel[32];
	char SecretLevel[32];
	char MusicFile[32];
	char HelpString[16][512];
	char SoundEffectPointer[48][64]; // only in version 256 (AGA)
	uint32_t LayerMiscProperties[8]; // Each property is a bit in the following order: Tile Width, Tile Height, Limit Visible Region, Texture Mode, Parallax Stars. This leaves 27 (32-5) unused bits for each layer?
	uint8_t Type[8]; // name from Michiel; function unknown
	bool DoesLayerHaveAnyTiles[8]; // must always be set to true for layer 4, or JJ2 will crash
	uint32_t LayerWidth[8];
	uint32_t LayerRealWidth[8]; // for when "Tile Width" is checked. The lowest common multiple of LayerWidth and 4.
	uint32_t LayerHeight[8];
	int32_t LayerZAxis[8]; // Should be { -300, -200, -100, 0, 100, 200, 300, 400 }, but nothing happens when you change these
	uint8_t DetailLevel[8]; // is set to 02 for layer 5 in Battle1 and Battle3, but is 00 the rest of the time, at least for JJ2 levels. No clear effect of altering. Name from Michiel.
	int32_t WaveX[8]; // name from Michiel; function unknown
	int32_t WaveY[8]; // name from Michiel; function unknown
	uint32_t LayerXSpeed[8]; // Divide by 65536 to get value seen in JCS
	uint32_t LayerYSpeed[8]; // Divide by 65536 to get value seen in JCSvalue
	uint32_t LayerAutoXSpeed[8]; // Divide by 65536 to get value seen in JCS
	uint32_t LayerAutoYSpeed[8]; // Divide by 65536 to get value seen in JCS
	uint8_t LayerTextureMode[8];
	uint8_t LayerTextureParams[8][3]; // Red, Green, Blue
	uint16_t AnimOffset; // MAX_TILES minus AnimCount, also called StaticTiles
	uint32_t TilesetEvents[MAX_TILES_TSF]; // same format as in Data2, for tiles
	bool IsEachTileFlipped[MAX_TILES_TSF]; // set to 1 if a tile appears flipped anywhere in the level
	uint8_t TileTypes[MAX_TILES_TSF]; // translucent=1 or caption=4, basically. Doesn't work on animated tiles.
	uint8_t XMask[MAX_TILES_TSF]; // tested to equal all zeroes in almost 4000 different levels, and editing it has no appreciable effect.  // Name from Michiel, who claims it is totally unused.
	uint8_t UnknownAGA[32768]; // only in version 256 (AGA)
	Animated_Tile Anim[256]; // or [256] in TSF.
	// only the first [AnimCount] are needed; JCS will save all 128/256, but JJ2 will run your level either way.
	uint8_t Padding[512]; //all zeroes; only in levels saved with JCS
};

struct J2L_Event
{
	uint8_t EventID;
	uint8_t EventData[3];
};

class J2L_Data2_1_23
{
public:
	inline J2L_Data2_1_23() {}
	inline J2L_Data2_1_23(const J2L_Data2_1_23 &other)
	{
		for (size_t i = 0; i < other.Events.size(); i++)
		{
			Events.push_back(other.Events[i]);
		}
	}
	std::vector<J2L_Event> Events;
};

typedef char EventName[64];

struct Data2AGAPart1
{
	uint16_t NumberOfDistinctEvents;
	EventName *Events;
};

class AGAString
{
private:
	uint32_t StringLength; //including null byte
	char *String; //ends with a null byte
public:
	AGAString(uint8_t *bytes, uint32_t length);
	~AGAString();
	std::string ToString();
};

struct AGAEvent
{
	uint16_t XPos;
	uint16_t YPos;
	uint16_t EventID;
	uint32_t Marker;
	//the rest of the structure is only included if the highest bit of Marker is set.
	uint32_t LengthOfParameterSection; //including its own four bytes
	uint16_t AreThereStrings; //02 if yes, 00 otherwise?
	uint16_t NumberOfLongs;
	uint32_t *Parameter;
	AGAString *parameters; //I guess it just keeps looking for strings until it hits the LengthOfParameterSection length?
};

struct J2L_Data2_AGA
{
	Data2AGAPart1 part1;
	std::vector<AGAEvent> Events;
};

struct Word
{
	uint16_t Tiles[4];
};

struct J2L_Data3
{
	std::vector<Word> Dictionary;
};

class J2L_Data4
{
public:
	std::vector<uint16_t> Layers[8];
};

#pragma endregion

#pragma region Tileset Structures

struct J2T_TilesetInfo_1_23 {
	uint32_t PaletteColor[256];        //arranged RGBA
	uint32_t TileCount;                 //number of tiles, always a multiple of 10
	uint8_t FullyOpaque[MAX_TILES_1_23];    //1 if no transparency at all, otherwise 0
	uint8_t Unknown1[MAX_TILES_1_23];       //appears to be all zeros
	uint32_t ImageAddress[MAX_TILES_1_23];
	uint32_t Unknown2[MAX_TILES_1_23];       //appears to be all zeros
	uint32_t TMaskAddress[MAX_TILES_1_23];   //Transparency masking, for bitblt
	uint32_t Unknown3[MAX_TILES_1_23];       //appears to be all zeros
	uint32_t MaskAddress[MAX_TILES_1_23];    //Clipping or tile mask
	uint32_t FMaskAddress[MAX_TILES_1_23];   //flipped version of the above
};

struct J2T_TilesetInfo_1_24 {
	uint32_t PaletteColor[256];        //arranged RGBA
	uint32_t TileCount;                 //number of tiles, always a multiple of 10
	uint8_t FullyOpaque[MAX_TILES_TSF];    //1 if no transparency at all, otherwise 0
	uint8_t Unknown1[MAX_TILES_TSF];       //appears to be all zeros
	uint32_t ImageAddress[MAX_TILES_TSF];
	uint32_t Unknown2[MAX_TILES_TSF];       //appears to be all zeros
	uint32_t TMaskAddress[MAX_TILES_TSF];   //Transparency masking, for bitblt
	uint32_t Unknown3[MAX_TILES_TSF];       //appears to be all zeros
	uint32_t MaskAddress[MAX_TILES_TSF];    //Clipping or tile mask
	uint32_t FMaskAddress[MAX_TILES_TSF];   //flipped version of the above
};

struct J2T_TileImage {
	uint8_t ImageData[1024];	// Stores the palette color index for each pixel for a 32x32 image
};

struct J2T_TileTransparency {
	uint8_t TransparencyMask[128];
};

struct J2T_TileClip {
	uint8_t ClippingMask[128];
};

#pragma endregion

#pragma region Animation Structures

struct ALIB_Header
{
	char Magic[4];						// Magic number, should be 'ALIB'
	uint32_t Signature = 0x00BABE00;	// Signature
	uint32_t HeaderSize;				// Equals 464 bytes for v1.23 Anims.j2a
	uint16_t Version = 0x0200;			// Probably means v2.0
	uint16_t Unknown2 = 0x1808;			// Unknown purpose
	uint32_t FileSize;					// Equals 8182764 bytes for v1.23 anims.j2a
	uint32_t CRC32;						// Note: CRC buffer starts after the end of header
	uint32_t SetCount;					// Number of sets in the Anims.j2a (109 in v1.23)
	uint32_t SetAddress[ANIM_COUNT];	// Each set's starting address within the file
};

struct ANIM_Header
{
	char Magic[4];						// Magic number, should be 'ANIM'
	uint8_t AnimationCount;				// Number of animations in set
	uint8_t SampleCount;				// Number of sound samples in set
	uint16_t FrameCount;				// Total number of frames in set
	uint32_t PriorSampleCount;          // Total number of sound sample across all sets preceding this one
	uint32_t CData1;                    // Compressed size of Data1
	uint32_t UData1;                    // Uncompressed size of Data1
	uint32_t CData2;                    // Compressed size of Data2
	uint32_t UData2;                    // Uncompressed size of Data2
	uint32_t CData3;                    // Compressed size of Data3
	uint32_t UData3;                    // Uncompressed size of Data3
	uint32_t CData4;                    // Compressed size of Data4
	uint32_t UData4;                    // Uncompressed size of Data4
};

struct AnimInfo
{
	uint16_t FrameCount;
	uint16_t FrameRate;
	uint32_t Reserved;
};

struct FrameInfo
{
	uint16_t Width;
	uint16_t Height;
	int16_t ColdspotX;    // Relative to hotspot
	int16_t ColdspotY;    // Relative to hotspot
	int16_t HotspotX;
	int16_t HotspotY;
	int16_t GunspotX;     // Relative to hotspot
	int16_t GunspotY;     // Relative to hotspot
	uint32_t ImageAddress;  // Address in Data3 where image starts
	uint32_t MaskAddress;   // Address in Data3 where mask starts
};

#pragma endregion

#pragma region Episode Structures

struct J2E_Header
{
	uint32_t HeaderSize;
	uint32_t Position;
	uint32_t IsRegistered;
	uint32_t Unknown;
	char EpisodeName[128];
	char FirstLevel[32];
	uint32_t Width;
	uint32_t Height;
	uint32_t Unknown2;
	uint32_t Unknown3;
	uint32_t TitleWidth;
	uint32_t TitleHeight;
	uint32_t Unknown4;
	uint32_t Unknown5;
};

#pragma endregion

#pragma endregion

#pragma region Loader Functions

inline void ReadAnimatedTile(Animated_Tile &Anim, uint8_t *bytes, int &offset)
{
	ReadAndAdvance(Anim.FrameWait, bytes, offset);
	ReadAndAdvance(Anim.RandomWait, bytes, offset);
	ReadAndAdvance(Anim.PingPongWait, bytes, offset);
	ReadAndAdvance(Anim.PingPong, bytes, offset);
	ReadAndAdvance(Anim.Speed, bytes, offset);
	ReadAndAdvance(Anim.FrameCount, bytes, offset);
	ReadStringAndAdvance((char*)Anim.Frame, 64 * sizeof(uint16_t), bytes, offset); // this can be a flipped tile or another animated tile
}

inline void ReadJ2L_Data_1_23(J2L_Data1_1_23 *Data1, uint8_t *bytes)
{
	int offset = 0;
	ReadAndAdvance(Data1->JCSHorizontalOffset, bytes, offset); // In pixels
	ReadAndAdvance(Data1->Security1, bytes, offset); // 0xBA00 if passworded, 0x0000 otherwise
	ReadAndAdvance(Data1->JCSVerticalOffset, bytes, offset); // In pixels
	ReadAndAdvance(Data1->Security2, bytes, offset); // 0xBE00 if passworded, 0x0000 otherwise
	ReadAndAdvance(Data1->SecAndLayer, bytes, offset); // Upper 4 bits are set if passworded, zero otherwise. Lower 4 bits represent the layer number as last saved in JCS.
	ReadAndAdvance(Data1->MinLight, bytes, offset); // Multiply by 1.5625 to get value seen in JCS
	ReadAndAdvance(Data1->StartLight, bytes, offset); // Multiply by 1.5625 to get value seen in JCS
	ReadAndAdvance(Data1->AnimCount, bytes, offset);
	ReadAndAdvance(Data1->VerticalSplitscreen, bytes, offset);
	ReadAndAdvance(Data1->IsLevelMultiplayer, bytes, offset);
	ReadAndAdvance(Data1->BufferSize, bytes, offset);
	ReadStringAndAdvance(Data1->LevelName, 32, bytes, offset);
	ReadStringAndAdvance(Data1->Tileset, 32, bytes, offset);
	ReadStringAndAdvance(Data1->BonusLevel, 32, bytes, offset);
	ReadStringAndAdvance(Data1->NextLevel, 32, bytes, offset);
	ReadStringAndAdvance(Data1->SecretLevel, 32, bytes, offset);
	ReadStringAndAdvance(Data1->MusicFile, 32, bytes, offset);
	ReadStringAndAdvance((char*)Data1->HelpString, 16 * 512, bytes, offset);
	ReadStringAndAdvance((char*)Data1->LayerMiscProperties, 8 * sizeof(uint32_t), bytes, offset); // Each property is a bit in the following order: Tile Width, Tile Height, Limit Visible Region, Texture Mode, Parallax Stars. This leaves 27 (32-5) unused bits for each layer?
	ReadStringAndAdvance((char*)Data1->Type, 8, bytes, offset); // name from Michiel; function unknown
	ReadStringAndAdvance((char*)Data1->DoesLayerHaveAnyTiles, 8 * sizeof(bool), bytes, offset); // must always be set to true for layer 4, or JJ2 will crash
	ReadStringAndAdvance((char*)Data1->LayerWidth, 8 * sizeof(uint32_t), bytes, offset);
	ReadStringAndAdvance((char*)Data1->LayerRealWidth, 8 * sizeof(uint32_t), bytes, offset); // for when "Tile Width" is checked. The lowest common multiple of LayerWidth and 4.
	ReadStringAndAdvance((char*)Data1->LayerHeight, 8 * sizeof(uint32_t), bytes, offset);
	ReadStringAndAdvance((char*)Data1->LayerZAxis, 8 * sizeof(uint32_t), bytes, offset); // Should be { -300, -200, -100, 0, 100, 200, 300, 400 }, but nothing happens when you change these
	ReadStringAndAdvance((char*)Data1->DetailLevel, 8 * sizeof(uint8_t), bytes, offset); // is set to 02 for layer 5 in Battle1 and Battle3, but is 00 the rest of the time, at least for JJ2 levels. No clear effect of altering. Name from Michiel.
	ReadStringAndAdvance((char*)Data1->WaveX, 8 * sizeof(int32_t), bytes, offset); // name from Michiel; function unknown
	ReadStringAndAdvance((char*)Data1->WaveY, 8 * sizeof(int32_t), bytes, offset); // name from Michiel; function unknown
	ReadStringAndAdvance((char*)Data1->LayerXSpeed, 8 * sizeof(uint32_t), bytes, offset); // Divide by 65536 to get value seen in JCS
	ReadStringAndAdvance((char*)Data1->LayerYSpeed, 8 * sizeof(uint32_t), bytes, offset); // Divide by 65536 to get value seen in JCSvalue
	ReadStringAndAdvance((char*)Data1->LayerAutoXSpeed, 8 * sizeof(uint32_t), bytes, offset); // Divide by 65536 to get value seen in JCS
	ReadStringAndAdvance((char*)Data1->LayerAutoYSpeed, 8 * sizeof(uint32_t), bytes, offset); // Divide by 65536 to get value seen in JCS
	ReadStringAndAdvance((char*)Data1->LayerTextureMode, 8 * sizeof(uint8_t), bytes, offset);
	ReadStringAndAdvance((char*)Data1->LayerTextureParams, 8 * 3 * sizeof(uint8_t), bytes, offset); // Red, Green, Blue
	ReadAndAdvance(Data1->AnimOffset, bytes, offset); // MAX_TILES minus AnimCount, also called StaticTiles
	ReadStringAndAdvance((char*)Data1->TilesetEvents, MAX_TILES_1_23 * sizeof(uint32_t), bytes, offset); // same format as in Data2, for tiles
	ReadStringAndAdvance((char*)Data1->IsEachTileFlipped, MAX_TILES_1_23 * sizeof(bool), bytes, offset); // set to 1 if a tile appears flipped anywhere in the level
	ReadStringAndAdvance((char*)Data1->TileTypes, MAX_TILES_1_23 * sizeof(uint8_t), bytes, offset); // translucent=1 or caption=4, basically. Doesn't work on animated tiles.
	ReadStringAndAdvance((char*)Data1->XMask, MAX_TILES_1_23 * sizeof(uint8_t), bytes, offset); // tested to equal all zeroes in almost 4000 different levels, and editing it has no appreciable effect.  // Name from Michiel, who claims it is totally unused.
	for (int i = 0; i < 128; i++)
		ReadAnimatedTile(Data1->Anim[i], bytes, offset); // or [256] in TSF.
}

inline void ReadJ2L_Data_TSF(J2L_Data1_TSF *Data1, uint8_t *bytes)
{
	int offset = 0;
	ReadAndAdvance(Data1->JCSHorizontalOffset, bytes, offset); // In pixels
	ReadAndAdvance(Data1->Security1, bytes, offset); // 0xBA00 if passworded, 0x0000 otherwise
	ReadAndAdvance(Data1->JCSVerticalOffset, bytes, offset); // In pixels
	ReadAndAdvance(Data1->Security2, bytes, offset); // 0xBE00 if passworded, 0x0000 otherwise
	ReadAndAdvance(Data1->SecAndLayer, bytes, offset); // Upper 4 bits are set if passworded, zero otherwise. Lower 4 bits represent the layer number as last saved in JCS.
	ReadAndAdvance(Data1->MinLight, bytes, offset); // Multiply by 1.5625 to get value seen in JCS
	ReadAndAdvance(Data1->StartLight, bytes, offset); // Multiply by 1.5625 to get value seen in JCS
	ReadAndAdvance(Data1->AnimCount, bytes, offset);
	ReadAndAdvance(Data1->VerticalSplitscreen, bytes, offset);
	ReadAndAdvance(Data1->IsLevelMultiplayer, bytes, offset);
	ReadAndAdvance(Data1->BufferSize, bytes, offset);
	ReadStringAndAdvance(Data1->LevelName, 32, bytes, offset);
	ReadStringAndAdvance(Data1->Tileset, 32, bytes, offset);
	ReadStringAndAdvance(Data1->BonusLevel, 32, bytes, offset);
	ReadStringAndAdvance(Data1->NextLevel, 32, bytes, offset);
	ReadStringAndAdvance(Data1->SecretLevel, 32, bytes, offset);
	ReadStringAndAdvance(Data1->MusicFile, 32, bytes, offset);
	ReadStringAndAdvance((char*)Data1->HelpString, 16 * 512, bytes, offset);
	ReadStringAndAdvance((char*)Data1->LayerMiscProperties, 8 * sizeof(uint32_t), bytes, offset); // Each property is a bit in the following order: Tile Width, Tile Height, Limit Visible Region, Texture Mode, Parallax Stars. This leaves 27 (32-5) unused bits for each layer?
	ReadStringAndAdvance((char*)Data1->Type, 8, bytes, offset); // name from Michiel; function unknown
	ReadStringAndAdvance((char*)Data1->DoesLayerHaveAnyTiles, 8 * sizeof(bool), bytes, offset); // must always be set to true for layer 4, or JJ2 will crash
	ReadStringAndAdvance((char*)Data1->LayerWidth, 8 * sizeof(uint32_t), bytes, offset);
	ReadStringAndAdvance((char*)Data1->LayerRealWidth, 8 * sizeof(uint32_t), bytes, offset); // for when "Tile Width" is checked. The lowest common multiple of LayerWidth and 4.
	ReadStringAndAdvance((char*)Data1->LayerHeight, 8 * sizeof(uint32_t), bytes, offset);
	ReadStringAndAdvance((char*)Data1->LayerZAxis, 8 * sizeof(uint32_t), bytes, offset); // Should be { -300, -200, -100, 0, 100, 200, 300, 400 }, but nothing happens when you change these
	ReadStringAndAdvance((char*)Data1->DetailLevel, 8 * sizeof(uint8_t), bytes, offset); // is set to 02 for layer 5 in Battle1 and Battle3, but is 00 the rest of the time, at least for JJ2 levels. No clear effect of altering. Name from Michiel.
	ReadStringAndAdvance((char*)Data1->WaveX, 8 * sizeof(int32_t), bytes, offset); // name from Michiel; function unknown
	ReadStringAndAdvance((char*)Data1->WaveY, 8 * sizeof(int32_t), bytes, offset); // name from Michiel; function unknown
	ReadStringAndAdvance((char*)Data1->LayerXSpeed, 8 * sizeof(uint32_t), bytes, offset); // Divide by 65536 to get value seen in JCS
	ReadStringAndAdvance((char*)Data1->LayerYSpeed, 8 * sizeof(uint32_t), bytes, offset); // Divide by 65536 to get value seen in JCSvalue
	ReadStringAndAdvance((char*)Data1->LayerAutoXSpeed, 8 * sizeof(uint32_t), bytes, offset); // Divide by 65536 to get value seen in JCS
	ReadStringAndAdvance((char*)Data1->LayerAutoYSpeed, 8 * sizeof(uint32_t), bytes, offset); // Divide by 65536 to get value seen in JCS
	ReadStringAndAdvance((char*)Data1->LayerTextureMode, 8 * sizeof(uint8_t), bytes, offset);
	ReadStringAndAdvance((char*)Data1->LayerTextureParams, 8 * 3 * sizeof(uint8_t), bytes, offset); // Red, Green, Blue
	ReadAndAdvance(Data1->AnimOffset, bytes, offset); // MAX_TILES minus AnimCount, also called StaticTiles
	ReadStringAndAdvance((char*)Data1->TilesetEvents, MAX_TILES_TSF * sizeof(uint32_t), bytes, offset); // same format as in Data2, for tiles
	ReadStringAndAdvance((char*)Data1->IsEachTileFlipped, MAX_TILES_TSF * sizeof(bool), bytes, offset); // set to 1 if a tile appears flipped anywhere in the level
	ReadStringAndAdvance((char*)Data1->TileTypes, MAX_TILES_TSF * sizeof(uint8_t), bytes, offset); // translucent=1 or caption=4, basically. Doesn't work on animated tiles.
	ReadStringAndAdvance((char*)Data1->XMask, MAX_TILES_TSF * sizeof(uint8_t), bytes, offset); // tested to equal all zeroes in almost 4000 different levels, and editing it has no appreciable effect.  // Name from Michiel, who claims it is totally unused.
	for (int i = 0; i < 256; i++)
		ReadAnimatedTile(Data1->Anim[i], bytes, offset); // or [256] in TSF.
}

inline void ReadJ2L_Data_AGA(J2L_Data1_AGA *Data1, uint8_t *bytes)
{
	int offset = 0;
	ReadAndAdvance(Data1->JCSHorizontalOffset, bytes, offset); // In pixels
	ReadAndAdvance(Data1->Security1, bytes, offset); // 0xBA00 if passworded, 0x0000 otherwise
	ReadAndAdvance(Data1->JCSVerticalOffset, bytes, offset); // In pixels
	ReadAndAdvance(Data1->Security2, bytes, offset); // 0xBE00 if passworded, 0x0000 otherwise
	ReadAndAdvance(Data1->SecAndLayer, bytes, offset); // Upper 4 bits are set if passworded, zero otherwise. Lower 4 bits represent the layer number as last saved in JCS.
	ReadAndAdvance(Data1->MinLight, bytes, offset); // Multiply by 1.5625 to get value seen in JCS
	ReadAndAdvance(Data1->StartLight, bytes, offset); // Multiply by 1.5625 to get value seen in JCS
	ReadAndAdvance(Data1->AnimCount, bytes, offset);
	ReadAndAdvance(Data1->VerticalSplitscreen, bytes, offset);
	ReadAndAdvance(Data1->IsLevelMultiplayer, bytes, offset);
	ReadAndAdvance(Data1->BufferSize, bytes, offset);
	ReadStringAndAdvance(Data1->LevelName, 32, bytes, offset);
	ReadStringAndAdvance(Data1->Tileset, 32, bytes, offset);
	ReadStringAndAdvance(Data1->BonusLevel, 32, bytes, offset);
	ReadStringAndAdvance(Data1->NextLevel, 32, bytes, offset);
	ReadStringAndAdvance(Data1->SecretLevel, 32, bytes, offset);
	ReadStringAndAdvance(Data1->MusicFile, 32, bytes, offset);
	ReadStringAndAdvance((char*)Data1->HelpString, 16 * 512, bytes, offset);
	ReadStringAndAdvance((char*)Data1->SoundEffectPointer, 48 * 64, bytes, offset); // only in version 256 (AGA)
	ReadStringAndAdvance((char*)Data1->LayerMiscProperties, 8 * sizeof(uint32_t), bytes, offset); // Each property is a bit in the following order: Tile Width, Tile Height, Limit Visible Region, Texture Mode, Parallax Stars. This leaves 27 (32-5) unused bits for each layer?
	ReadStringAndAdvance((char*)Data1->Type, 8, bytes, offset); // name from Michiel; function unknown
	ReadStringAndAdvance((char*)Data1->DoesLayerHaveAnyTiles, 8 * sizeof(bool), bytes, offset); // must always be set to true for layer 4, or JJ2 will crash
	ReadStringAndAdvance((char*)Data1->LayerWidth, 8 * sizeof(uint32_t), bytes, offset);
	ReadStringAndAdvance((char*)Data1->LayerRealWidth, 8 * sizeof(uint32_t), bytes, offset); // for when "Tile Width" is checked. The lowest common multiple of LayerWidth and 4.
	ReadStringAndAdvance((char*)Data1->LayerHeight, 8 * sizeof(uint32_t), bytes, offset);
	ReadStringAndAdvance((char*)Data1->LayerZAxis, 8 * sizeof(uint32_t), bytes, offset); // Should be { -300, -200, -100, 0, 100, 200, 300, 400 }, but nothing happens when you change these
	ReadStringAndAdvance((char*)Data1->DetailLevel, 8 * sizeof(uint8_t), bytes, offset); // is set to 02 for layer 5 in Battle1 and Battle3, but is 00 the rest of the time, at least for JJ2 levels. No clear effect of altering. Name from Michiel.
	ReadStringAndAdvance((char*)Data1->WaveX, 8 * sizeof(int32_t), bytes, offset); // name from Michiel; function unknown
	ReadStringAndAdvance((char*)Data1->WaveY, 8 * sizeof(int32_t), bytes, offset); // name from Michiel; function unknown
	ReadStringAndAdvance((char*)Data1->LayerXSpeed, 8 * sizeof(uint32_t), bytes, offset); // Divide by 65536 to get value seen in JCS
	ReadStringAndAdvance((char*)Data1->LayerYSpeed, 8 * sizeof(uint32_t), bytes, offset); // Divide by 65536 to get value seen in JCSvalue
	ReadStringAndAdvance((char*)Data1->LayerAutoXSpeed, 8 * sizeof(uint32_t), bytes, offset); // Divide by 65536 to get value seen in JCS
	ReadStringAndAdvance((char*)Data1->LayerAutoYSpeed, 8 * sizeof(uint32_t), bytes, offset); // Divide by 65536 to get value seen in JCS
	ReadStringAndAdvance((char*)Data1->LayerTextureMode, 8 * sizeof(uint8_t), bytes, offset);
	ReadStringAndAdvance((char*)Data1->LayerTextureParams, 8 * 3 * sizeof(uint8_t), bytes, offset); // Red, Green, Blue
	ReadAndAdvance(Data1->AnimOffset, bytes, offset); // MAX_TILES minus AnimCount, also called StaticTiles
	ReadStringAndAdvance((char*)Data1->TilesetEvents, MAX_TILES_TSF * sizeof(uint32_t), bytes, offset); // same format as in Data2, for tiles
	ReadStringAndAdvance((char*)Data1->IsEachTileFlipped, MAX_TILES_TSF * sizeof(bool), bytes, offset); // set to 1 if a tile appears flipped anywhere in the level
	ReadStringAndAdvance((char*)Data1->TileTypes, MAX_TILES_TSF * sizeof(uint8_t), bytes, offset); // translucent=1 or caption=4, basically. Doesn't work on animated tiles.
	ReadStringAndAdvance((char*)Data1->XMask, MAX_TILES_TSF * sizeof(uint8_t), bytes, offset); // tested to equal all zeroes in almost 4000 different levels, and editing it has no appreciable effect.  // Name from Michiel, who claims it is totally unused.
	ReadStringAndAdvance((char*)Data1->UnknownAGA, 32768, bytes, offset); // only in version 256 (AGA)
	for (int i = 0; i < 256; i++)
		ReadAnimatedTile(Data1->Anim[i], bytes, offset); // or [256] in TSF.
}

inline void ReadJ2L_Data2_1_23(J2L_Data2_1_23 *Data2, uint8_t *bytes, uint32_t length)
{
	for (uint32_t i = 0; i < length; i += 4)
	{
		J2L_Event e;
		memcpy(&e.EventID, &bytes[i], 1);
		memcpy(e.EventData, &bytes[i + 1], 3);
		if (e.EventID == 0x1F)
			printf("MP Start at %d, %d\n", (i / 4) % 128, (i / 4) / 128);
		Data2->Events.push_back(e);
	}
}

inline void ReadData2AGAPart1(Data2AGAPart1 *part1, uint8_t *bytes, uint32_t &offset)
{
	offset = sizeof(uint16_t);
	memcpy(&part1->NumberOfDistinctEvents, bytes, sizeof(uint16_t));
	part1->Events = new EventName[part1->NumberOfDistinctEvents];
	for (int i = 0; i < part1->NumberOfDistinctEvents; i++)
	{
		memcpy(&part1->Events[i], &bytes[offset], 64);
		offset += 64;
	}
}

inline void ReadJ2T_TilesetInfo_1_23(J2T_TilesetInfo_1_23 *tilesetInfo, uint8_t *bytes)
{
	int offset = 0;
	ReadStringAndAdvance((char*)tilesetInfo->PaletteColor, 256 * sizeof(uint32_t), bytes, offset);
	ReadAndAdvance(tilesetInfo->TileCount, bytes, offset);
	ReadStringAndAdvance((char*)tilesetInfo->FullyOpaque, MAX_TILES_1_23 * sizeof(uint8_t), bytes, offset);
	ReadStringAndAdvance((char*)tilesetInfo->Unknown1, MAX_TILES_1_23 * sizeof(uint8_t), bytes, offset);
	ReadStringAndAdvance((char*)tilesetInfo->ImageAddress, MAX_TILES_1_23 * sizeof(uint32_t), bytes, offset);
	ReadStringAndAdvance((char*)tilesetInfo->Unknown2, MAX_TILES_1_23 * sizeof(uint32_t), bytes, offset);
	ReadStringAndAdvance((char*)tilesetInfo->TMaskAddress, MAX_TILES_1_23 * sizeof(uint32_t), bytes, offset);
	ReadStringAndAdvance((char*)tilesetInfo->Unknown3, MAX_TILES_1_23 * sizeof(uint32_t), bytes, offset);
	ReadStringAndAdvance((char*)tilesetInfo->FMaskAddress, MAX_TILES_1_23 * sizeof(uint32_t), bytes, offset);
	ReadStringAndAdvance((char*)tilesetInfo->MaskAddress, MAX_TILES_1_23 * sizeof(uint32_t), bytes, offset);
}

inline void ReadJ2T_TilesetInfo_1_24(J2T_TilesetInfo_1_24 *tilesetInfo, uint8_t *bytes)
{
	int offset = 0;
	ReadStringAndAdvance((char*)tilesetInfo->PaletteColor, 256 * sizeof(uint32_t), bytes, offset);
	ReadAndAdvance(tilesetInfo->TileCount, bytes, offset);
	ReadStringAndAdvance((char*)tilesetInfo->FullyOpaque, MAX_TILES_TSF * sizeof(uint8_t), bytes, offset);
	ReadStringAndAdvance((char*)tilesetInfo->Unknown1, MAX_TILES_TSF * sizeof(uint8_t), bytes, offset);
	ReadStringAndAdvance((char*)tilesetInfo->ImageAddress, MAX_TILES_TSF * sizeof(uint32_t), bytes, offset);
	ReadStringAndAdvance((char*)tilesetInfo->Unknown2, MAX_TILES_TSF * sizeof(uint32_t), bytes, offset);
	ReadStringAndAdvance((char*)tilesetInfo->TMaskAddress, MAX_TILES_TSF * sizeof(uint32_t), bytes, offset);
	ReadStringAndAdvance((char*)tilesetInfo->Unknown3, MAX_TILES_TSF * sizeof(uint32_t), bytes, offset);
	ReadStringAndAdvance((char*)tilesetInfo->FMaskAddress, MAX_TILES_TSF * sizeof(uint32_t), bytes, offset);
	ReadStringAndAdvance((char*)tilesetInfo->MaskAddress, MAX_TILES_TSF * sizeof(uint32_t), bytes, offset);
}

inline void ReadLEVLHeader(FILE *fi, LEVL_Header *header)
{
	fread((char*)header->Copyright, sizeof(char), 180, fi);
	fread((char*)header->Magic, sizeof(char), 4, fi); // Should be 'LEVL'
	fread((char*)header->PasswordHash, sizeof(uint8_t), 4, fi); // 0x00BABE00 for no password
	fread((char*)header->LevelName, sizeof(char), 32, fi);
	fread(&header->Version, sizeof(uint16_t), 1, fi);
	fread(&header->FileSize, sizeof(uint32_t), 1, fi);
	fread(&header->CRC32, sizeof(uint32_t), 1, fi);
	fread(&header->CData1, sizeof(uint32_t), 1, fi);            // compressed size of Data1
	fread(&header->UData1, sizeof(uint32_t), 1, fi);
	fread(&header->CData2, sizeof(uint32_t), 1, fi);            // compressed size of Data1
	fread(&header->UData2, sizeof(uint32_t), 1, fi);
	fread(&header->CData3, sizeof(uint32_t), 1, fi);            // compressed size of Data1
	fread(&header->UData3, sizeof(uint32_t), 1, fi);
	fread(&header->CData4, sizeof(uint32_t), 1, fi);            // compressed size of Data1
	fread(&header->UData4, sizeof(uint32_t), 1, fi);
}

inline void ReadTILEHeader(FILE *fi, TILE_Header *header)
{
	fread(&header->Copyright, sizeof(char), 180, fi);
	fread(&header->Magic, sizeof(char), 4, fi); // Should be 'LEVL'
	fread((char*)header->Signature, sizeof(uint8_t), 4, fi); // 0x00BABE00 for no password
	fread((char*)header->TilesetName, sizeof(char), 32, fi);
	fread(&header->Version, sizeof(uint16_t), 1, fi);
	fread(&header->FileSize, sizeof(uint32_t), 1, fi);
	fread(&header->CRC32, sizeof(uint32_t), 1, fi);
	fread(&header->CData1, sizeof(uint32_t), 1, fi);            // compressed size of Data1
	fread(&header->UData1, sizeof(uint32_t), 1, fi);
	fread(&header->CData2, sizeof(uint32_t), 1, fi);            // compressed size of Data1
	fread(&header->UData2, sizeof(uint32_t), 1, fi);
	fread(&header->CData3, sizeof(uint32_t), 1, fi);            // compressed size of Data1
	fread(&header->UData3, sizeof(uint32_t), 1, fi);
	fread(&header->CData4, sizeof(uint32_t), 1, fi);            // compressed size of Data1
	fread(&header->UData4, sizeof(uint32_t), 1, fi);
}

inline void ReadALIBHeader(FILE *fi, ALIB_Header *alib)
{
	fread(alib->Magic, 1, 4, fi);
	fread(&alib->Signature, 4, 1, fi);
	fread(&alib->HeaderSize, 4, 1, fi);
	fread(&alib->Version, 2, 1, fi);
	fread(&alib->Unknown2, 2, 1, fi);
	fread(&alib->FileSize, 4, 1, fi);
	fread(&alib->CRC32, 4, 1, fi);
	fread(&alib->SetCount, 4, 1, fi);
	fread(alib->SetAddress, 4, ANIM_COUNT, fi);
}

inline void ReadANIMHeader(FILE *fi, ANIM_Header *anim)
{
	fread(anim->Magic, 1, 4, fi);
	fread(&anim->AnimationCount, 1, 1, fi);
	fread(&anim->SampleCount, 1, 1, fi);
	fread(&anim->FrameCount, 2, 1, fi);
	fread(&anim->PriorSampleCount, 4, 1, fi);
	fread(&anim->CData1, 4, 1, fi);
	fread(&anim->UData1, 4, 1, fi);
	fread(&anim->CData2, 4, 1, fi);
	fread(&anim->UData2, 4, 1, fi); 
	fread(&anim->CData3, 4, 1, fi);
	fread(&anim->UData3, 4, 1, fi);
	fread(&anim->CData4, 4, 1, fi);
	fread(&anim->UData4, 4, 1, fi);
}

inline void ReadJ2EHeader(FILE *fi, J2E_Header *header)
{
	fread(&header->HeaderSize, sizeof(uint32_t), 1, fi);
	fread(&header->Position, sizeof(uint32_t), 1, fi);
	fread(&header->IsRegistered, sizeof(uint32_t), 1, fi);
	fread(&header->Unknown, sizeof(uint32_t), 1, fi);
	fread(header->EpisodeName, sizeof(char), 128, fi);
	for(int i = 0; header->EpisodeName[i] != '\0'; i++)
	{
		header->EpisodeName[i] = (header->EpisodeName[i+1] == '@') ? ' ' : header->EpisodeName[i+1];
	}
	fread(header->FirstLevel, sizeof(char), 32, fi);
	for(int i = 1; header->FirstLevel[i] != '\0'; i++)
	{
		if((header->FirstLevel[i] >= 'A') && (header->FirstLevel[i] <= 'Z'))
		{
			header->FirstLevel[i] += ('a' - 'A');
		}
	}
	fread(&header->Width, sizeof(uint32_t), 1, fi);
	fread(&header->Height, sizeof(uint32_t), 1, fi);
	fread(&header->Unknown2, sizeof(uint32_t), 1, fi);
	fread(&header->Unknown3, sizeof(uint32_t), 1, fi);
	fread(&header->TitleWidth, sizeof(uint32_t), 1, fi);
	fread(&header->TitleHeight, sizeof(uint32_t), 1, fi);
	fread(&header->Unknown4, sizeof(uint32_t), 1, fi);
	fread(&header->Unknown5, sizeof(uint32_t), 1, fi);
}

#pragma endregion

#endif
