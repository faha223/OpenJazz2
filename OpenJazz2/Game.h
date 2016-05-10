#ifndef _GAME_H_
#define _GAME_H_

#include "Level.h"
#include "Animation.h"
#include "Tileset.h"
#include "Music.h"
#include "FileSystem.h"
#include "Math.h"
#include "Controls.h"
#include "Player.h"
#include "Event.h"
#include "Actor.h"
#include <vector>
#include <map>
#include <chrono>

#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

#define SPRITESHEET_SIZE 4096
#define ASPECT_RATIO ((float)WindowWidth/(float)WindowHeight)

enum GameState
{
	StartScreen,
	MainMenu,
	NewGame,
	SingleOrMultiplayerMenu,
	EpisodeMenu,
	MultiplayerMenu,
	GameModeMenu,
	InternetMenu,
	ConnectIPMenu
};

class Game
{
private:
	uint32_t WindowWidth;
	uint32_t WindowHeight;
	bool Fullscreen;
	bool BorderlessFullscreen;
	Level *level;
	Tileset *tileset;
	Animations *anims;
	Player *player;
	GameState state;
	GLuint Tilesheet;
	GLuint Layers[8];
	size_t LayerVertexCount[8];
	GLuint framebuffer;
	GLuint framebufferTexture;
	GLuint framebufferVBO;
	std::string Path;
	std::string DumpPath;
	std::map<uint32_t, SpriteCoords> Sprites;
	std::vector<GLuint> SpriteSheets;
	GLuint playerVBO;
	GLuint ActorsVBO;
	std::vector<uint32_t> LayerTiles[8];
	float startTime;
	float timeElapsed = 0.0f;
	std::vector<Event> Events;
	std::vector<Actor> BackgroundActors;
	std::vector<Actor> ForegroundActors;

	SDL_Window *window;
	SDL_GLContext glContext;
	std::map<uint32_t, bool> keyboard;
	std::map<uint32_t, bool> joystick;
	std::multimap<Control, uint32_t> KeyBindings;
	std::multimap<Control, uint32_t> JoyBindings;

	void StartEpisode(const std::string &Episode);
	void StartLevel(const std::string &level);
	void GotoNextLevel();
	void RestartLevel();

	void BlitSurface(SDL_Surface *src, SDL_Surface *dst, float offsetX, float offsetY, bool tileX, bool tileY);
	void BuildLevelVBOs(Level *level);
	void CreateTilesheetTexture();
	void CreateSpritesheets();
	void PrepareFramebuffer();
	void RecalculateLevelVBOs(Level *level, const float &timeElapsed);
	void LoadSpriteCoords();
	void LoadSettings();
public:
	Game(int argc, char *argv[]);
	~Game();
	void Run();
};

#endif
