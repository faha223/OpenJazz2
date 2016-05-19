#include "Game.h"
#include <vector>
#include <algorithm>
using namespace std;

#define DEBUG_KEYS

//#define SAVE_SPRITESHEETS
//#define DRAW_PLAYER_BOUNDING_BOX

static string VertexShader = string("#version 330\n") +
string("uniform mat4 m_Projection;\n") +
string("uniform mat4 m_Modelview;\n") +
string("atribute vec3 texCoord;\n") +
string("attribute vec3 vertex;\n") +
string("void main() {\n") +
string("gl_Position = m_Projection * m_Modelview * vertex;\n") +
string("v_texCoord = texCoord.st;\n") +
string("}");

static string FragmentShader = string("#version 330\n") +
string("uniform sampler2D texture;\n") +
string("varying vec2 texCoord;\n") +
string("void main() {\n") +
string("gl_FragColor = vec4(texture2D(texture, texCoord.st).rgb, 1);\n") +
string("}");

enum TextAlignment
{
	Left,
	Center,
	Right
};

void DrawText(const Animation *font, const map<uint32_t, SpriteCoords> *sprites, const vec2 &textPos, const TextAlignment &alignment, const char *fmt, ...)
{
	if(fmt == nullptr)
		return;
	char buffer[256];
	va_list ap;
	va_start(ap, fmt);
	vsprintf_s(buffer, 255, fmt, ap);
	va_end(ap); 

	string text(buffer);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);
	vertex *charVerts = new vertex[text.length() * 4];
	int length = 0;
	for (size_t i = 0; i < text.length(); i++)
	{
		if ((int)text[i] == ' ')
		{
			charVerts[4 * i].x = 0;
			charVerts[4 * i].y = 0;
			charVerts[4 * i].z = 0;
			charVerts[4 * i].s = 0;
			charVerts[4 * i].t = 0;
			charVerts[4 * i + 1].x = 0;
			charVerts[4 * i + 1].y = 0;
			charVerts[4 * i + 1].z = 0;
			charVerts[4 * i + 1].s = 0;
			charVerts[4 * i + 1].t = 0;
			charVerts[4 * i + 2].x = 0;
			charVerts[4 * i + 2].y = 0;
			charVerts[4 * i + 2].z = 0;
			charVerts[4 * i + 2].s = 0;
			charVerts[4 * i + 2].t = 0;
			charVerts[4 * i + 3].x = 0;
			charVerts[4 * i + 3].y = 0;
			charVerts[4 * i + 3].z = 0;
			charVerts[4 * i + 3].s = 0;
			charVerts[4 * i + 3].t = 0;
			length += 10;
		}
		else
		{
			const AnimationFrame *frame = font->GetFrame(FONT_OFFSET + (int)text[i]);
			SpriteCoords coords = sprites->at(frame->getIndex());
			vec2 Hotspot = frame->getHotSpot();
			vec2 Position(Hotspot.x, coords.height + Hotspot.y);
			charVerts[4 * i].x = length + Position.x;
			charVerts[4 * i].y = Position.y;
			charVerts[4 * i].z = 0;
			charVerts[4 * i].s = ((float)coords.x) / (float)SPRITESHEET_SIZE;
			charVerts[4 * i].t = ((float)coords.y + coords.height) / (float)SPRITESHEET_SIZE;
			charVerts[4 * i + 1].x = length + Position.x;
			charVerts[4 * i + 1].y = Position.y - coords.height;
			charVerts[4 * i + 1].z = 0;
			charVerts[4 * i + 1].s = ((float)coords.x) / (float)SPRITESHEET_SIZE;
			charVerts[4 * i + 1].t = ((float)coords.y) / (float)SPRITESHEET_SIZE;
			charVerts[4 * i + 2].x = length + Position.x + coords.width;
			charVerts[4 * i + 2].y = Position.y - coords.height;
			charVerts[4 * i + 2].z = 0;
			charVerts[4 * i + 2].s = ((float)coords.x + coords.width) / (float)SPRITESHEET_SIZE;
			charVerts[4 * i + 2].t = ((float)coords.y) / (float)SPRITESHEET_SIZE;
			charVerts[4 * i + 3].x = length + Position.x + coords.width;
			charVerts[4 * i + 3].y = Position.y;
			charVerts[4 * i + 3].z = 0;
			charVerts[4 * i + 3].s = ((float)coords.x + coords.width) / (float)SPRITESHEET_SIZE;
			charVerts[4 * i + 3].t = ((float)coords.y + coords.height) / (float)SPRITESHEET_SIZE;
			length += coords.width;
		}
	}

	switch (alignment)
	{
	case Left:
		glTranslatef(textPos.x, textPos.y, 0);
		break;
	case Center:
		glTranslatef(textPos.x - (length * 0.5f), textPos.y, 0);
		break;
	case Right:
		glTranslatef(textPos.x - length, textPos.y, 0);
		break;
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glVertexPointer(3, GL_FLOAT, sizeof(vertex), &charVerts[0].x);
	glTexCoordPointer(2, GL_FLOAT, sizeof(vertex), &charVerts[0].s);
	glDrawArrays(GL_QUADS, 0, 4*text.length());
	glEnable(GL_DEPTH_TEST);
	delete [] charVerts;
}

void DrawHealth(int Health, const SpriteCoords &coords, const float &x, const float &y, const TextAlignment &alignment)
{
	if(Health > 0)
	{
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		vertex *healthVerts = new vertex[4*Health];
		int length = 0;
		for(int i = 0; i < Health; i++)
		{
			healthVerts[4*i].x = length;
			healthVerts[4*i].y = 0;
			healthVerts[4*i].z = 0;
			healthVerts[4*i].s = ((float)(coords.x))/(float)SPRITESHEET_SIZE;
			healthVerts[4*i].t = ((float)(coords.y))/(float)SPRITESHEET_SIZE;
			healthVerts[4*i+1].x = length;
			healthVerts[4*i+1].y = coords.height;
			healthVerts[4*i+1].z = 0;
			healthVerts[4*i+1].s = ((float)(coords.x))/(float)SPRITESHEET_SIZE;
			healthVerts[4*i+1].t = ((float)(coords.y + coords.height))/(float)SPRITESHEET_SIZE;
			healthVerts[4*i+2].x = length + coords.width;
			healthVerts[4*i+2].y = coords.height;
			healthVerts[4*i+2].z = 0;
			healthVerts[4*i+2].s = ((float)(coords.x + coords.width))/(float)SPRITESHEET_SIZE;
			healthVerts[4*i+2].t = ((float)(coords.y + coords.height))/(float)SPRITESHEET_SIZE;
			healthVerts[4*i+3].x = length + coords.width;
			healthVerts[4*i+3].y = 0;
			healthVerts[4*i+3].z = 0;
			healthVerts[4*i+3].s = ((float)(coords.x + coords.width))/(float)SPRITESHEET_SIZE;
			healthVerts[4*i+3].t = ((float)(coords.y))/(float)SPRITESHEET_SIZE;
			length += coords.width;
		}
		glDisable(GL_DEPTH_TEST);
		switch (alignment)
		{
		case Left:
			glTranslatef(x, y, 0);
			break;
		case Center:
			glTranslatef(x - (length * 0.5f), y, 0);
			break;
		case Right:
			glTranslatef(x - length, y, 0);
			break;
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glVertexPointer(3, GL_FLOAT, sizeof(vertex), &healthVerts[0].x);
		glTexCoordPointer(2, GL_FLOAT, sizeof(vertex), &healthVerts[0].s);
		glDrawArrays(GL_QUADS, 0, 4*Health);
		glEnable(GL_DEPTH_TEST);
		delete [] healthVerts;
	}
}

void DrawLives(int lives, const SpriteCoords &coords, const Animation *font, const map<uint32_t, SpriteCoords> *sprites, const float &x, const float &y)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	vertex *headVerts = new vertex[4];
	int length = 0;

	#pragma region Draw Player Head 

	headVerts[0].x = length;
	headVerts[0].y = 0;
	headVerts[0].z = 0;
	headVerts[0].s = ((float)(coords.x)) / (float)SPRITESHEET_SIZE;
	headVerts[0].t = ((float)(coords.y)) / (float)SPRITESHEET_SIZE;
	headVerts[1].x = length;
	headVerts[1].y = coords.height;
	headVerts[1].z = 0;
	headVerts[1].s = ((float)(coords.x)) / (float)SPRITESHEET_SIZE;
	headVerts[1].t = ((float)(coords.y + coords.height)) / (float)SPRITESHEET_SIZE;
	headVerts[2].x = length + coords.width;
	headVerts[2].y = coords.height;
	headVerts[2].z = 0;
	headVerts[2].s = ((float)(coords.x + coords.width)) / (float)SPRITESHEET_SIZE;
	headVerts[2].t = ((float)(coords.y + coords.height)) / (float)SPRITESHEET_SIZE;
	headVerts[3].x = length + coords.width;
	headVerts[3].y = 0;
	headVerts[3].z = 0;
	headVerts[3].s = ((float)(coords.x + coords.width)) / (float)SPRITESHEET_SIZE;
	headVerts[3].t = ((float)(coords.y)) / (float)SPRITESHEET_SIZE;
	length += coords.width;

	#pragma endregion Draw Player Head

	glDisable(GL_DEPTH_TEST);
		
	glTranslatef(x, y-coords.height, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glVertexPointer(3, GL_FLOAT, sizeof(vertex), &headVerts[0].x);
	glTexCoordPointer(2, GL_FLOAT, sizeof(vertex), &headVerts[0].s);
	glDrawArrays(GL_QUADS, 0, 4);
	glEnable(GL_DEPTH_TEST);
	delete[] headVerts;

	DrawText(font, sprites, vec2(x + coords.width, y-24), Left, "x%d", lives);
}

Game::Game(int argc, char *argv[]) :
WindowWidth(640), 
WindowHeight(480), 
Fullscreen(false), 
BorderlessFullscreen(false), 
level(nullptr),
tileset(nullptr),
anims(nullptr),
player(nullptr),
Tilesheet(0),
state(MainMenu)
{
	string LevelFile = "";
	string EpisodeFile = "";
	for (int i = 1; i < argc; i++)
	{
		string arg = argv[i];
		if (arg == string("-Windowed"))
			Fullscreen = false;
		else if(arg == string("-Fullscreen"))
			Fullscreen = true;
		else if (arg.substr(arg.length() - 3) == string("j2l"))
			LevelFile = arg;
	}
	Layers[0] = 0;
	Layers[1] = 0;
	Layers[2] = 0;
	Layers[3] = 0;
	Layers[4] = 0;
	Layers[5] = 0;
	Layers[6] = 0;
	Layers[7] = 0;
	auto now = std::chrono::system_clock::now();
	auto duration = now.time_since_epoch();
	startTime = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() * 0.001f;
	LoadSettings();

	//LevelFile = Path::Combine(Path, "test.j2l");

	int SDL_Error = SDL_Init(SDL_INIT_EVERYTHING);
	if (SDL_Error < 0)
	{
		return;
	}

	if (Fullscreen)
	{
		SDL_ShowCursor(0);
	}

	char buffer[256];
	SDL_DisplayMode disp;
	SDL_GetDesktopDisplayMode(0, &disp);
	
	if(Fullscreen)
	{
		WindowWidth = disp.w;
		WindowHeight = disp.h;
	}

	window = SDL_CreateWindow("Jazz Jackrabbit 2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, max(640, WindowWidth), max(480, WindowHeight), SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | ((Fullscreen) ? ((BorderlessFullscreen) ? SDL_WINDOW_BORDERLESS : SDL_WINDOW_FULLSCREEN_DESKTOP) : 0));
	glContext = SDL_GL_CreateContext(window);

	//printf("Game Controller: %s\n", SDL_GameControllerNameForIndex(0));

	glewInit();

	//glClearColor(0.28f, 0.19f, 0.72f, 1.0f);
	glClearColor(0, 0, 0, 1);

	glEnable(GL_TEXTURE_2D);		// We need this for textures

	glEnable(GL_DEPTH_TEST);		// We need this so the layers get drawn in the right order
	glDepthFunc(GL_LEQUAL);

	glEnable(GL_ALPHA_TEST);		// We need this so that the alpha works properly
	glAlphaFunc(GL_GREATER, 0);

	glEnableClientState(GL_VERTEX_ARRAY);			// We use VBOs to send the tiling information to the gfx card
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	// Sounds are NOT READY
	//Mixer mixer;
	//Music music(Path::Combine(Path, level->GetMusic()).c_str());
	//mixer.PlayMusic(mixer.LoadChunk(music.GetChunk()));

	//SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	//SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
	//if(0 != SDL_GL_SetSwapInterval(-1))
	//{
	//	SDL_GL_SetSwapInterval(1);
	//}

	SDL_GameControllerOpen(0);

	LoadControls("KeyBindings.ini", KeyBindings, JoyBindings);

	if (EpisodeFile.length() == 0)
	{
		auto Files = Directory::GetFiles(Path, ".j2e");
		if (Files.size() > 0)
			EpisodeFile = Files[0];
	}
	EpisodeFile = Path::Combine(Path, Episode);
	anims = new Animations(Path::Combine(Path, "Anims.j2a").c_str());
#ifdef DUMP_ANIMATIONS
	anims.DumpToDisk(DumpPath);
#endif
	CreateSpritesheets();

	PrepareFramebuffer();

	glGenBuffers(1, &playerVBO);
	glBindBuffer(GL_ARRAY_BUFFER, playerVBO);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(vertex), nullptr, GL_STREAM_DRAW);

	glGenBuffers(1, &ActorsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, ActorsVBO);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(vertex), nullptr, GL_STREAM_DRAW);

	if (LevelFile.length() > 0)
	{
		StartLevel(LevelFile);
		Run();
	}
	else if (EpisodeFile.length() > 0)
	{
		StartEpisode(EpisodeFile);
	}
}

void Game::StartEpisode(const string &Episode)
{
	printf("Starting Episode %s\n", Episode.c_str());
	if (level != nullptr)
	{
		delete level;
		level = nullptr;
	}
	if (tileset != nullptr)
	{
		delete tileset;
		tileset = nullptr;
	}
	if (Episode.length() > 0)
	{
		string LevelFile = "";
		FILE *fi = openFile(Episode.c_str(), "rb");
		if (fi != nullptr)
		{
			J2E_Header episodeHeader;
			ReadJ2EHeader(fi, &episodeHeader);
			LevelFile = Path::Combine(Path, episodeHeader.FirstLevel);
			fclose(fi);
		}

		if (LevelFile.length() > 0)
		{
			StartLevel(LevelFile);
			Run();
		}
	}
}

void Game::StartLevel(const string &LevelFile)
{
	printf("Starting Level %s\n", LevelFile.c_str());
	level = new Level(LevelFile.c_str());
	tileset = new Tileset(Path::Combine(Path, level->GetTileset()).c_str());
	CreateTilesheetTexture();
	BuildLevelVBOs(level);

	RestartLevel();
}

void Game::GotoNextLevel()
{
	printf("Clearing Current Level to load Next Level\n");
	string NextLevelName = level->GetNextLevel();
	if (NextLevelName == "Endepis")
	{
		printf("End of Episode\n");
		exit(0);
	}
	string nextLevel = Path::Combine(Path, NextLevelName + ".j2l");
	delete tileset;
	tileset = nullptr;
	delete level;
	level = nullptr;
	StartLevel(nextLevel);
}

void Game::RestartLevel()
{
	printf("Bust a Move!\n");
	int lives = -1;
	if (player != nullptr)
	{
		lives = player->GetLives();
		delete player;
	}
	player = nullptr;

	for (int i = 0; i < level->GetTileCount(3); i++)
		level->SetTileFrame(i, 0);
	Events.clear();
	BackgroundActors.clear();
	
	TileCoord playerStart = level->GetPlayerStart(false);
	printf("Player starting at tile %d, %d\n", playerStart.x, playerStart.y);
	player = new Player(vec2(playerStart.x*32.0f, (playerStart.y + 1)*32.0f), level, tileset, anims);
	if(lives >= 0)
		player->InitLives(lives);

	for (uint32_t i = 0; i < level->GetLayerHeight(3); i++)
	{
		for (uint32_t j = 0; j < level->GetLayerWidth(3); j++)
		{
			uint32_t tc = (level->GetLayerWidth(3)*i + j);
			Events.push_back(Event(level->GetEvents(j, i)));
			EventID eventId = (EventID)level->GetEvents(j, i).EventID;
			if (eventId != EventID::None)
			{
				BackgroundActors.push_back(Actor(level, tileset, vec2((j*32.0f) + 16.0f, (i*32.0f)), eventId, anims, 0));
			}
		}
	}	
}

void Game::Run()
{
	int OffsetX = 0;
	int OffsetY = 0;
	{
		char buffer[256];
		sprintf_s(buffer, 255, "Jazz Jackrabbit 2 - %s", level->GetName().c_str());
		SDL_SetWindowTitle(window, buffer);
	}

	bool quit = false;
	float lastUpdate = SDL_GetTicks() * 0.001f;
	glClearColor(0, 0, 0, 0);
	while (!quit)
	{
		timeElapsed = SDL_GetTicks()*0.001f;
		float sinceLastUpdate = timeElapsed - lastUpdate;
		lastUpdate = timeElapsed;

		OffsetX = (int)Math::Round(min(max(0.0f, player->GetPosition().x - (ASPECT_RATIO * 240.0f)), (level->GetLayerWidth(3)  * 32.0f) - (480.0f * ASPECT_RATIO)));
		OffsetY = (int)Math::Round(min(max(0.0f, player->GetPosition().y - 261.0f), (level->GetLayerHeight(3) * 32.0f) - 480.0f));

		glBindFramebuffer(GL_FRAMEBUFFER_EXT, framebuffer);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, WindowWidth, WindowHeight - (float)(WindowHeight - 480), -(float)(WindowHeight - 480), 500, -500);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		SDL_Rect s, d;
		s.x = 0;
		s.y = 0;

		RecalculateLevelVBOs(level, timeElapsed);

		glBindTexture(GL_TEXTURE_2D, Tilesheet);

		#pragma region Draw the Layers

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		for (int i = 7; i >= 0; i--)
		{
			glColor4f(0, 0, 0, 1);
			if ((LayerVertexCount[i] > 0) && (player->GetHealth() > 0))
			{
				bool TileX = level->IsLayerTiledHorizontally(i);
				bool TileY = level->IsLayerTiledVertically(i);
				bool limitToVisibleRegion = level->IsLayerLimitedToVisibleRegion(i);
				TileY = TileY && !limitToVisibleRegion;
				float LayerWidth = 32.0f * level->GetLayerWidth(i);
				float LayerHeight = 32.0f * level->GetLayerHeight(i);
				int LayerXOffset = (int)Math::Round(-OffsetX * level->GetLayerXSpeed(i) + level->GetLayerAutoXSpeed(i));
				int LayerYOffset = (int)Math::Round(-OffsetY * level->GetLayerYSpeed(i) + level->GetLayerAutoYSpeed(i));

				glBindBuffer(GL_ARRAY_BUFFER, Layers[i]);
				glVertexPointer(3, GL_FLOAT, 5 * sizeof(float), (void*)0);
				glTexCoordPointer(2, GL_FLOAT, 5 * sizeof(float), (void*)(3 * sizeof(float)));
				if (!TileX && !TileY)
				{
					glLoadIdentity();
					glTranslatef(float(LayerXOffset), float(LayerYOffset), 0.0f);
					glDrawArrays(GL_QUADS, 0, LayerVertexCount[i]);
				}
				else if (TileX && TileY)
				{
					int LayerXOffset = (int)Math::Round(-OffsetX * level->GetLayerXSpeed(i)) + ((level->GetLayerWidth(3) - level->GetLayerWidth(i)));
					int LayerYOffset = (int)Math::Round(-OffsetY * level->GetLayerYSpeed(i)) + ((level->GetLayerHeight(i) - level->GetLayerHeight(3)));
					int startx = 0;
					while ((startx * LayerWidth + LayerXOffset) > 0)
						startx--;
					int starty = 0;
					while ((starty * LayerHeight + LayerXOffset) > 0)
						starty--;
					for (int j = starty; (LayerHeight * j) + LayerYOffset < 480; j++)
					{
						for (int k = startx; ((LayerWidth * k) + LayerXOffset) < (ASPECT_RATIO * 480); k++)
						{
							glLoadIdentity();
							glTranslatef(LayerXOffset + (k * LayerWidth), LayerYOffset + (j * LayerHeight), 0.0f);
							glDrawArrays(GL_QUADS, 0, LayerVertexCount[i]);
						}
					}
				}
				else if (TileX && !TileY)
				{
					int LayerXOffset = (int)Math::Round(-OffsetX * level->GetLayerXSpeed(i));
					int LayerYOffset = (int)Math::Round((limitToVisibleRegion) ? (480 - (level->GetLayerHeight(i) * 32)) : (-OffsetY * level->GetLayerYSpeed(i)));
					int startx = 0;
					while ((startx * LayerWidth + LayerXOffset) > 0)
						startx--;
					for (int k = startx; ((LayerWidth * k) + LayerXOffset) < (ASPECT_RATIO * 480); k++)
					{
						glLoadIdentity();
						glTranslatef((float)(LayerXOffset + (k * LayerWidth)), (float)LayerYOffset, 0.0f);
						glDrawArrays(GL_QUADS, 0, LayerVertexCount[i]);
					}
				}
				else if (TileY && !TileX)
				{
					int LayerXOffset = (int)Math::Round((limitToVisibleRegion) ? 0 : -OffsetX * level->GetLayerXSpeed(i));
					int LayerYOffset = (int)Math::Round(-OffsetY * level->GetLayerYSpeed(i));
					int starty = 0;
					while ((starty * LayerHeight + LayerXOffset) > 0)
						starty--;
					for (int j = starty; (LayerHeight * j) + LayerYOffset < 480; j++)
					{
						glLoadIdentity();
						glTranslatef(LayerXOffset, LayerYOffset + (j * LayerHeight), 0.0f);
						glDrawArrays(GL_QUADS, 0, LayerVertexCount[i]);
					}
				}
			}

			if (i == 3)
			{
				glLoadIdentity();
				
				glBindTexture(GL_TEXTURE_2D, SpriteSheets[0]);
				float depth = level->GetLayerZ(3);
				int LayerXOffset = (int)Math::Round(-OffsetX * level->GetLayerXSpeed(3));
				int LayerYOffset = (int)Math::Round(-OffsetY * level->GetLayerYSpeed(3));
				glTranslatef(LayerXOffset, LayerYOffset, 0.0f);

				if (player->GetHealth() > 0)
				{
					glBindBuffer(GL_ARRAY_BUFFER, ActorsVBO);
					for (size_t actor_i = 0; actor_i < BackgroundActors.size(); actor_i++)
					{
						Actor *actor = &BackgroundActors[actor_i];

						if (actor->GetEventID() == RedGemPlus1)
						{
							glColor4f(0.25, 0, 0.125, 0.8); // Red
						}
						else if (actor->GetEventID() == GreenGemPlus1)
						{
							glColor4f(0.125, 0.5, 0.25, 0.8); // Green
						}
						else if (actor->GetEventID() == BlueGemPlus1)
						{
							glColor4f(0, 0.25, 0.5, 0.8); // Blue
						}
						else
						{
							glColor4f(0, 0, 0, 1);
						}

						const AnimationFrame *frame = actor->GetFrame();
						if (frame != nullptr)
						{
							
							SpriteCoords coords = Sprites[frame->getIndex()];
							vec2 position = actor->GetPosition();
							vertex actorVerts[4];

							bool isFlippedX = false;
							bool isFlippedY = false;
							if (actor->IsFlipped())
							{
								switch (actor->GetEventID())
								{
								case RedSpring:
								case GreenSpring:
								case BlueSpring:
									isFlippedY = true;
									break;
								case HorRedSpring:
								case HorGreenSpring:
								case HorBlueSpring:
									isFlippedX = true;
									break;
								}
							}

							if (actor->RenderFromColdSpot())
							{
								vec2 Coldspot = frame->getColdSpot();
								position += vec2(0, 32);
								position += vec2(Coldspot.x, Coldspot.y);
							}
							else
							{
								vec2 Hotspot = frame->getHotSpot();
								position += vec2(Hotspot.x, Hotspot.y);
							}
							//if (isFlippedY)
							//	/*position.y = position.y - 32 + coords.height;*/
							if (isFlippedX)
								position.x += 32 - coords.width;

							actorVerts[0].x = position.x;
							actorVerts[0].y = position.y;
							actorVerts[0].z = depth;
							actorVerts[0].s = isFlippedX ? (((float)coords.x + coords.width) / (float)SPRITESHEET_SIZE) : (((float)coords.x)/(float)SPRITESHEET_SIZE);
							actorVerts[0].t = (((float)coords.y)/(float)SPRITESHEET_SIZE);

							actorVerts[1].x = position.x;
							actorVerts[1].y = position.y + coords.height;
							actorVerts[1].z = depth;
							actorVerts[1].s = isFlippedX ? (((float)(coords.x + coords.width)) / (float)SPRITESHEET_SIZE) : (((float)(coords.x))/(float)SPRITESHEET_SIZE);
							actorVerts[1].t = (((float)coords.y + coords.height) / (float)SPRITESHEET_SIZE);

							actorVerts[2].x = position.x + (coords.width);
							actorVerts[2].y = position.y + coords.height;
							actorVerts[2].z = depth;
							actorVerts[2].s = isFlippedX ? (((float)(coords.x)) / (float)SPRITESHEET_SIZE) : (((float)(coords.x + coords.width))/(float)SPRITESHEET_SIZE);
							actorVerts[2].t = (((float)coords.y + coords.height) / (float)SPRITESHEET_SIZE);

							actorVerts[3].x = position.x + (coords.width);
							actorVerts[3].y = position.y;
							actorVerts[3].z = depth;
							actorVerts[3].s = isFlippedX ? (((float)coords.x) / (float)SPRITESHEET_SIZE) : (((float)coords.x + coords.width)/(float)SPRITESHEET_SIZE);
							actorVerts[3].t = (((float)(coords.y))/(float)SPRITESHEET_SIZE);

							actor->Update(max(1.0f / 60.0f, sinceLastUpdate));
							bool consumed = false;
							if(actor->CheckCollision(player, Sprites))
							{
								consumed = player->CollidedWithActor(*actor);
								
								if(consumed)
								{
									player->AddPoints(actor->GetPointValue());
								}
							}

							if(consumed || actor->GetDestroyActor())
							{
								if(actor->AddsSparkleOnDeath())
								{
									vec2 sparklePos = actor->GetPosition();
									ForegroundActors.push_back(Actor(level, tileset, vec2(sparklePos.x, sparklePos.y+16), Sparkle, anims, 0));
								}
								BackgroundActors[actor_i] = BackgroundActors.back();
								BackgroundActors.pop_back();
							}
							
							glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(vertex), actorVerts, GL_STREAM_DRAW);
							glVertexPointer(3, GL_FLOAT, 5 * sizeof(float), (void*)0);
							glTexCoordPointer(2, GL_FLOAT, 5 * sizeof(float), (void*)(3 * sizeof(float)));
							glDrawArrays(GL_QUADS, 0, 4);
						}
					}
				}

				if (!player->IsInvisible())
				{
					const AnimationFrame *frame = player->GetSprite();
					vec2 Hotspot = frame->getHotSpot();

					SpriteCoords coord = Sprites[frame->getIndex()];
					int flipped = (player->GetFacing() == -1);

					vec2 playerPos = player->GetPosition();

#ifdef DRAW_PLAYER_BOUNDING_BOX
					glBindTexture(GL_TEXTURE_2D, 0);
					glColor3f(1, 0, 0);
					vertex BoxVerts[4];
					BoxVerts[0].x = playerPos.x - 12;
					BoxVerts[0].y = playerPos.y;
					BoxVerts[0].z = depth;
					BoxVerts[1].x = playerPos.x - 12;
					BoxVerts[1].y = playerPos.y - 30;
					BoxVerts[1].z = depth;
					BoxVerts[2].x = playerPos.x + 12;
					BoxVerts[2].y = playerPos.y - 30;
					BoxVerts[2].z = depth;
					BoxVerts[3].x = playerPos.x + 12;
					BoxVerts[3].y = playerPos.y;
					BoxVerts[3].z = depth;
					glBindBuffer(GL_ARRAY_BUFFER, playerVBO);
					glBufferData(GL_ARRAY_BUFFER, 20 * sizeof(float), BoxVerts, GL_STREAM_DRAW);
					glVertexPointer(3, GL_FLOAT, 5 * sizeof(float), (void*)0);
					glDrawArrays(GL_QUADS, 0, 4);
					glBindTexture(GL_TEXTURE_2D, SpriteSheets[0]);
					glColor3f(1, 1, 1);
#endif

					if (!flipped)
						playerPos += vec2(Hotspot.x, coord.height + Hotspot.y - 20);//Hotspot.y);
					else
					{
						playerPos += vec2(-Hotspot.x, coord.height + Hotspot.y - 20);
						coord.x += coord.width;
						coord.width *= -1;
					}
					vertex PlayerVerts[4];
					PlayerVerts[0].x = playerPos.x;
					PlayerVerts[0].y = playerPos.y;
					PlayerVerts[0].z = depth;
					PlayerVerts[0].s = ((float)(!flipped) ? coord.x : coord.x + coord.width) / (float)SPRITESHEET_SIZE;
					PlayerVerts[0].t = ((float)(coord.y + coord.height)) / (float)SPRITESHEET_SIZE;
					PlayerVerts[1].x = playerPos.x;
					PlayerVerts[1].y = playerPos.y - coord.height;
					PlayerVerts[1].z = depth;
					PlayerVerts[1].s = ((float)(!flipped) ? coord.x : coord.x + coord.width) / (float)SPRITESHEET_SIZE;
					PlayerVerts[1].t = ((float)(coord.y)) / (float)SPRITESHEET_SIZE;
					PlayerVerts[2].x = playerPos.x + coord.width;
					PlayerVerts[2].y = playerPos.y - coord.height;
					PlayerVerts[2].z = depth;
					PlayerVerts[2].s = ((float)((!flipped) ? coord.x + coord.width : coord.x)) / (float)SPRITESHEET_SIZE;
					PlayerVerts[2].t = ((float)(coord.y)) / (float)SPRITESHEET_SIZE;
					PlayerVerts[3].x = playerPos.x + coord.width;
					PlayerVerts[3].y = playerPos.y;
					PlayerVerts[3].z = depth;
					PlayerVerts[3].s = ((float)((!flipped) ? coord.x + coord.width : coord.x)) / (float)SPRITESHEET_SIZE;
					PlayerVerts[3].t = ((float)coord.y + coord.height) / (float)SPRITESHEET_SIZE;
					glBindBuffer(GL_ARRAY_BUFFER, playerVBO);
					glBufferData(GL_ARRAY_BUFFER, 20 * sizeof(float), PlayerVerts, GL_STREAM_DRAW);
					glVertexPointer(3, GL_FLOAT, 5 * sizeof(float), (void*)0);
					glTexCoordPointer(2, GL_FLOAT, 5 * sizeof(float), (void*)(3 * sizeof(float)));
					glDrawArrays(GL_QUADS, 0, 4);
				}

				if (player->GetHealth() > 0)
				{
					glBindBuffer(GL_ARRAY_BUFFER, ActorsVBO);
					for (size_t actor_i = 0; actor_i < ForegroundActors.size(); actor_i++)
					{
						Actor *actor = &ForegroundActors[actor_i];
						const AnimationFrame *frame = actor->GetFrame();
						if (frame != nullptr)
						{
							vec2 Hotspot = frame->getHotSpot();
							SpriteCoords coords = Sprites[frame->getIndex()];
							vec2 position = actor->GetPosition();
							position += vec2(Hotspot.x, coords.height + Hotspot.y - 20);
							vertex actorVerts[4];

							actorVerts[0].x = position.x;
							actorVerts[0].y = position.y;
							actorVerts[0].z = depth;
							actorVerts[0].s = (((float)coords.x)/(float)SPRITESHEET_SIZE);
							actorVerts[0].t = (((float)(coords.y))/(float)SPRITESHEET_SIZE);

							actorVerts[1].x = position.x;
							actorVerts[1].y = position.y + coords.height;
							actorVerts[1].z = depth;
							actorVerts[1].s = (((float)(coords.x))/(float)SPRITESHEET_SIZE);
							actorVerts[1].t = (((float)coords.y + coords.height) / (float)SPRITESHEET_SIZE);

							actorVerts[2].x = position.x + (coords.width);
							actorVerts[2].y = position.y + coords.height;
							actorVerts[2].z = depth;
							actorVerts[2].s = (((float)(coords.x + coords.width))/(float)SPRITESHEET_SIZE);
							actorVerts[2].t = (((float)coords.y + coords.height) / (float)SPRITESHEET_SIZE);

							actorVerts[3].x = position.x + (coords.width);
							actorVerts[3].y = position.y;
							actorVerts[3].z = depth;
							actorVerts[3].s = (((float)coords.x + coords.width)/(float)SPRITESHEET_SIZE);
							actorVerts[3].t = (((float)(coords.y))/(float)SPRITESHEET_SIZE);

							actor->Update(max(1.0f / 60.0f, sinceLastUpdate));
							
							if(actor->GetDestroyActor())
							{
								ForegroundActors[actor_i] = ForegroundActors.back();
								ForegroundActors.pop_back();
							}
						
							glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(vertex), actorVerts, GL_STREAM_DRAW);
							glVertexPointer(3, GL_FLOAT, 5 * sizeof(float), (void*)0);
							glTexCoordPointer(2, GL_FLOAT, 5 * sizeof(float), (void*)(3 * sizeof(float)));
							glDrawArrays(GL_QUADS, 0, 4);
						}
					}
				}

				glBindTexture(GL_TEXTURE_2D, Tilesheet);
			}
		}

		#pragma endregion Draw the Layers

		#pragma region Draw the HUD

		glLoadIdentity();
		glBindTexture(GL_TEXTURE_2D, SpriteSheets[0]);
		float depth = level->GetLayerZ(3);
		int LayerXOffset = (int)Math::Round(-OffsetX * level->GetLayerXSpeed(3));
		int LayerYOffset = (int)Math::Round(-OffsetY * level->GetLayerYSpeed(3));
		glTranslatef(LayerXOffset, LayerYOffset, 0.0f);
		DrawText(anims->GetAnimSet(ANIM_SET_MENU)->GetAnim(ANIM_SPRITEFONT_LARGE), &Sprites, vec2(7, 2), Left, "%07d", player->GetScore());
		
		const AnimationFrame *animFrame = anims->GetAnimSet(ANIM_SET_ITEMS)->GetAnim(ANIM_HEART)->GetFrame(0);
		DrawHealth(player->GetHealth(), Sprites[animFrame->getIndex()], (ASPECT_RATIO * 480.0f) - 8, 3, Right);

		const Animation *anim = anims->GetAnimSet(ANIM_SET_HEADS)->GetAnim(ANIM_HEAD_JAZZ);
		animFrame = anim->GetFrame((int)floor(anim->GetFrameRate() * timeElapsed) % anim->GetFrameCount());
		DrawLives(player->GetLives(), Sprites[animFrame->getIndex()], anims->GetAnimSet(ANIM_SET_MENU)->GetAnim(ANIM_SPRITEFONT_LARGE), &Sprites, 4, 476);
		
 		#pragma endregion Draw the HUD

		#pragma region Draw the Frame to the Window

		glBindFramebuffer(GL_FRAMEBUFFER_EXT, 0);
		glClear(GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, 1, 1, 0, -500, 500);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glBindTexture(GL_TEXTURE_2D, framebufferTexture);
		glBindBuffer(GL_ARRAY_BUFFER, framebufferVBO);
		glVertexPointer(3, GL_FLOAT, 5 * sizeof(float), (void*)0);
		glTexCoordPointer(2, GL_FLOAT, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glDrawArrays(GL_QUADS, 0, 4);

		#pragma endregion Draw the Frame to the Window

		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_KEYDOWN:
				keyboard[e.key.keysym.sym] = true;
				break;
			case SDL_KEYUP:
				keyboard[e.key.keysym.sym] = false;
				break;
			case SDL_CONTROLLERBUTTONDOWN:
				joystick[e.cbutton.button] = true;
				break;
			case SDL_CONTROLLERBUTTONUP:
				joystick[e.cbutton.button] = false;
				break;
			}
		}

		map<Control, bool> ControlState;
		ControlState[LEFT] = false;
		ControlState[RIGHT] = false;
		ControlState[UP] = false;
		ControlState[DOWN] = false;
		ControlState[JUMP] = false;
		ControlState[SHOOT] = false;
		ControlState[RUN] = false;

		for (multimap<Control, uint32_t>::iterator i = KeyBindings.begin(); i != KeyBindings.end(); i++)
			ControlState[LEFT] |= ((i->first == LEFT) ? keyboard[i->second] : false);
		for (multimap<Control, uint32_t>::iterator i = JoyBindings.begin(); i != JoyBindings.end(); i++)
			ControlState[LEFT] |= ((i->first == LEFT) ? joystick[i->second] : false);
		for (multimap<Control, uint32_t>::iterator i = KeyBindings.begin(); i != KeyBindings.end(); i++)
			ControlState[RIGHT] |= ((i->first == RIGHT) ? keyboard[i->second] : false);
		for (multimap<Control, uint32_t>::iterator i = JoyBindings.begin(); i != JoyBindings.end(); i++)
			ControlState[RIGHT] |= ((i->first == RIGHT) ? joystick[i->second] : false);
		for (multimap<Control, uint32_t>::iterator i = KeyBindings.begin(); i != KeyBindings.end(); i++)
			ControlState[UP] |= ((i->first == UP) ? keyboard[i->second] : false);
		for (multimap<Control, uint32_t>::iterator i = JoyBindings.begin(); i != JoyBindings.end(); i++)
			ControlState[UP] |= ((i->first == UP) ? joystick[i->second] : false);
		for (multimap<Control, uint32_t>::iterator i = KeyBindings.begin(); i != KeyBindings.end(); i++)
			ControlState[DOWN] |= ((i->first == DOWN) ? keyboard[i->second] : false);
		for (multimap<Control, uint32_t>::iterator i = JoyBindings.begin(); i != JoyBindings.end(); i++)
			ControlState[DOWN] |= ((i->first == DOWN) ? joystick[i->second] : false);
		for (multimap<Control, uint32_t>::iterator i = KeyBindings.begin(); i != KeyBindings.end(); i++)
			ControlState[JUMP] |= ((i->first == JUMP) ? keyboard[i->second] : false);
		for (multimap<Control, uint32_t>::iterator i = JoyBindings.begin(); i != JoyBindings.end(); i++)
			ControlState[JUMP] |= ((i->first == JUMP) ? joystick[i->second] : false);
		for (multimap<Control, uint32_t>::iterator i = KeyBindings.begin(); i != KeyBindings.end(); i++)
			ControlState[SHOOT] |= ((i->first == SHOOT) ? keyboard[i->second] : false);
		for (multimap<Control, uint32_t>::iterator i = JoyBindings.begin(); i != JoyBindings.end(); i++)
			ControlState[SHOOT] |= ((i->first == SHOOT) ? joystick[i->second] : false);
		for (multimap<Control, uint32_t>::iterator i = KeyBindings.begin(); i != KeyBindings.end(); i++)
			ControlState[RUN] |= ((i->first == RUN) ? keyboard[i->second] : false);
		for (multimap<Control, uint32_t>::iterator i = JoyBindings.begin(); i != JoyBindings.end(); i++)
			ControlState[RUN] |= ((i->first == RUN) ? joystick[i->second] : false);

		player->Update(max(1.0f / 60.0f, sinceLastUpdate), ControlState);
		if(player->GetState() == BUTTSTOMP)
		{
			vec2 playerPos = player->GetPosition();
			int tileXCoord = Math::Floor((playerPos.x - 12) / 32);
			int tileYCoord = Math::Floor(playerPos.y / 32);
			J2L_Event event = level->GetEvents(tileXCoord, tileYCoord + 1);
			if((event.EventID == ButtStompScenery) || (event.EventID == DestructScenery))
				level->SetTileFrame(level->GetLayerWidth(3) * (tileYCoord + 1) + tileXCoord, 1);
			tileXCoord = Math::Floor((playerPos.x + 12) / 32);
			event = level->GetEvents(tileXCoord, tileYCoord + 1);
			if ((event.EventID == ButtStompScenery) || (event.EventID == DestructScenery))
				level->SetTileFrame(level->GetLayerWidth(3) * (tileYCoord + 1) + tileXCoord, 1);
		}
		else if (player->GetState() == HIGHJUMP)
		{
			vec2 playerPos = player->GetPosition();
			int tileXCoord = Math::Floor((playerPos.x - 12) / 32);
			int tileYCoord = Math::Floor(playerPos.y / 32);
			if (tileYCoord > 0)
			{
				J2L_Event event = level->GetEvents(tileXCoord, tileYCoord - 1);
				if ((event.EventID == ButtStompScenery) || (event.EventID == DestructScenery))
					level->SetTileFrame(level->GetLayerWidth(3) * (tileYCoord - 1) + tileXCoord, 1);
				tileXCoord = Math::Floor((playerPos.x + 12) / 32);
				event = level->GetEvents(tileXCoord, tileYCoord - 1);
				if ((event.EventID == ButtStompScenery) || (event.EventID == DestructScenery))
					level->SetTileFrame(level->GetLayerWidth(3) * (tileYCoord - 1) + tileXCoord, 1);
			}
		}
		
		if (sinceLastUpdate < (1.0f / 60.0f))
		{
			SDL_Delay(1000 * ((1.0f / 60.0f) - sinceLastUpdate));
		}

		for (multimap<Control, uint32_t>::iterator i = KeyBindings.find(MENU); i != KeyBindings.end(); i++)
		{
			if (keyboard[i->second])
				quit = true;
		}
		for (multimap<Control, uint32_t>::iterator i = JoyBindings.find(MENU); i != JoyBindings.end(); i++)
		{
			if (joystick[i->second])
				quit = true;
		}

		if (player->GetState() == DEAD)
		{
			if (player->GetLives() == 0)
			{
				quit = true;
			}
			else
			{
				player->AddLives(255);
				RestartLevel();
			}
		}

		if(player->GetState() == LEVEL_ENDED)
		{
			GotoNextLevel();
		}

#ifdef DEBUG_KEYS
		if (keyboard[SDLK_F1])
		{
			keyboard[SDLK_F1] = false;
			RestartLevel();
		}
		if(keyboard[SDLK_F2])
		{
			keyboard[SDLK_F2] = false;
			GotoNextLevel();
		}
#endif

		SDL_GL_SwapWindow(window);
	}
}

void Game::BuildLevelVBOs(Level *level)
{
	printf("Building Level VBOs\n");
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	if (Layers[0] != 0)
		glDeleteBuffers(8, Layers);
	glGenBuffers(8, Layers);
	
	for (int layer = 0; layer < 8; layer++)
	{
		vector<vertex> layerVertices;
		if (level->GetTileCount(layer) > 0)
		{
			uint32_t layerHeight = level->GetLayerHeight(layer);
			uint32_t layerWidth = level->GetLayerWidth(layer);
			int32_t layerZ = level->GetLayerZ(layer);
			for (uint32_t t = 0; t < layerHeight; t++)
			{
				for (uint32_t s = 0; s < layerWidth; s++)
				{
					uint32_t i = (layerWidth * t) + s;
					if (i < level->GetTileCount(layer))
					{
						J2L_Tile tileInfo = level->GetTile(layer, s, t, 0.0f);
						LayerTiles[layer].push_back(tileInfo.index);
						if (tileInfo.index != -1)
						{
							vertex v1, v2, v3, v4;
							v1.x = s * 32.0f;
							v1.y = t * 32.0f;
							v1.z = (float)layerZ;
							v2.x = s * 32.0f;
							v2.y = (t + 1) * 32.0f;
							v2.z = (float)layerZ;
							v3.x = (s + 1) * 32.0f;
							v3.y = (t + 1) * 32.0f;
							v3.z = (float)layerZ;
							v4.x = (s + 1) * 32.0f;
							v4.y = t * 32.0f;
							v4.z = (float)layerZ;
							if (tileInfo.flipped)
							{
								v1.s = (tileInfo.index % 32) / 32.0f + (1.0f / 32.0f) - (1.0f / 4096.0f);
								v1.t = (tileInfo.index / 32) / 32.0f + (1.0f / 4096.0f);
								v2.s = (tileInfo.index % 32) / 32.0f + (1.0f / 32.0f) - (1.0f / 4096.0f);
								v2.t = (tileInfo.index / 32) / 32.0f + (1.0f / 32.0f) - (1.0f / 4096.0f);
								v3.s = (tileInfo.index % 32) / 32.0f + (1.0f / 4096.0f);
								v3.t = (tileInfo.index / 32) / 32.0f + (1.0f / 32.0f) - (1.0f / 4096.0f);
								v4.s = (tileInfo.index % 32) / 32.0f + (1.0f / 4096.0f);
								v4.t = (tileInfo.index / 32) / 32.0f + (1.0f / 4096.0f);
							}
							else
							{
								v1.s = (tileInfo.index % 32) / 32.0f + (1.0f / 4096.0f);
								v1.t = (tileInfo.index / 32) / 32.0f + (1.0f / 4096.0f);
								v2.s = (tileInfo.index % 32) / 32.0f + (1.0f / 4096.0f);
								v2.t = (tileInfo.index / 32) / 32.0f + (1.0f / 32.0f) - (1.0f / 4096.0f);
								v3.s = (tileInfo.index % 32) / 32.0f + (1.0f / 32.0f) - (1.0f / 4096.0f);
								v3.t = (tileInfo.index / 32) / 32.0f + (1.0f / 32.0f) - (1.0f / 4096.0f);
								v4.s = (tileInfo.index % 32) / 32.0f + (1.0f / 32.0f) - (1.0f / 4096.0f);
								v4.t = (tileInfo.index / 32) / 32.0f + (1.0f / 4096.0f);
							}
							layerVertices.push_back(v1);
							layerVertices.push_back(v2);
							layerVertices.push_back(v3);
							layerVertices.push_back(v4);
						}
					}
				}
			}
			vertex *vertexArray = new vertex[layerVertices.size()];
			for (size_t i = 0; i < layerVertices.size(); i++)
			{
				vertexArray[i] = layerVertices[i];
			}
			glBindBuffer(GL_ARRAY_BUFFER, Layers[layer]);
			glBufferData(GL_ARRAY_BUFFER, layerVertices.size() * sizeof(vertex), vertexArray, GL_DYNAMIC_DRAW);
			LayerVertexCount[layer] = layerVertices.size();
			delete[] vertexArray;
		}
		else
		{
			LayerVertexCount[layer] = 0;
		}
	}
}

void Game::RecalculateLevelVBOs(Level *level, const float &timeElapsed)
{
	for (int layer = 0; layer < 8; layer++)
	{
		struct vertex
		{
			float x, y, z;
			float s, t;
		};
		vector<vertex> layerVertices;
		if (level->GetTileCount(layer) > 0)
		{
			uint32_t layerHeight = level->GetLayerHeight(layer);
			uint32_t layerWidth = level->GetLayerWidth(layer);
			int32_t layerZ = level->GetLayerZ(layer);
			glBindBuffer(GL_ARRAY_BUFFER, Layers[layer]);
			for (uint32_t t = 0; t < layerHeight; t++)
			{
				for (uint32_t s = 0; s < layerWidth; s++)
				{
					uint32_t i = (layerWidth * t) + s;
					if (i < level->GetTileCount(layer))
					{
						J2L_Tile tileInfo = level->GetTile(layer, s, t, timeElapsed);
						if (LayerTiles[layer][i] != tileInfo.index)
						{
							LayerTiles[layer][i] = tileInfo.index;
							if (tileInfo.index != -1)
							{
								vertex v1, v2, v3, v4;
								v1.x = s * 32;
								v1.y = t * 32;
								v1.z = (float)layerZ;
								v2.x = s * 32;
								v2.y = (t + 1) * 32;
								v2.z = (float)layerZ;
								v3.x = (s + 1) * 32;
								v3.y = (t + 1) * 32;
								v3.z = (float)layerZ;
								v4.x = (s + 1) * 32;
								v4.y = t * 32;
								v4.z = (float)layerZ;
								if (tileInfo.flipped)
								{
									v1.s = (tileInfo.index % 32) / 32.0f + (1.0f / 32.0f) - (1.0f / 4096.0f);
									v1.t = (tileInfo.index / 32) / 32.0f + (1.0f / 4096.0f);
									v2.s = (tileInfo.index % 32) / 32.0f + (1.0f / 32.0f) - (1.0f / 4096.0f);
									v2.t = (tileInfo.index / 32) / 32.0f + (1.0f / 32.0f) - (1.0f / 4096.0f);
									v3.s = (tileInfo.index % 32) / 32.0f + (1.0f / 4096.0f);
									v3.t = (tileInfo.index / 32) / 32.0f + (1.0f / 32.0f) - (1.0f / 4096.0f);
									v4.s = (tileInfo.index % 32) / 32.0f + (1.0f / 4096.0f);
									v4.t = (tileInfo.index / 32) / 32.0f + (1.0f / 4096.0f);
								}
								else
								{
									v1.s = (tileInfo.index % 32) / 32.0f + (1.0f / 4096.0f);
									v1.t = (tileInfo.index / 32) / 32.0f + (1.0f / 4096.0f);
									v2.s = (tileInfo.index % 32) / 32.0f + (1.0f / 4096.0f);
									v2.t = (tileInfo.index / 32) / 32.0f + (1.0f / 32.0f) - (1.0f / 4096.0f);
									v3.s = (tileInfo.index % 32) / 32.0f + (1.0f / 32.0f) - (1.0f / 4096.0f);
									v3.t = (tileInfo.index / 32) / 32.0f + (1.0f / 32.0f) - (1.0f / 4096.0f);
									v4.s = (tileInfo.index % 32) / 32.0f + (1.0f / 32.0f) - (1.0f / 4096.0f);
									v4.t = (tileInfo.index / 32) / 32.0f + (1.0f / 4096.0f);
								}
								layerVertices.push_back(v1);
								layerVertices.push_back(v2);
								layerVertices.push_back(v3);
								layerVertices.push_back(v4);
							}
							vertex *vertexArray = new vertex[layerVertices.size()];
							for (size_t j = 0; j < layerVertices.size(); j++)
							{
								vertexArray[j] = layerVertices[j];
							}
							layerVertices.clear();
							glBufferSubData(GL_ARRAY_BUFFER, i * 4 * sizeof(vertex), 4 * sizeof(vertex), vertexArray);
							delete[] vertexArray;
						}
					}
				}
			}
		}
	}
}

void Game::CreateTilesheetTexture()
{
	printf("Creating Tilesheets\n");
	glBindTexture(GL_TEXTURE_2D, 0);
	SDL_Surface *TileSheet = SDL_CreateRGBSurface(0, 1024, 1024, 32, CHANNEL_RED, CHANNEL_GREEN, CHANNEL_BLUE, CHANNEL_ALPHA);
	if (TileSheet != nullptr)
	{
		printf("Num Tiles: %d\n", tileset->NumTiles());
		for (uint32_t i = 0; i < tileset->NumTiles(); i++)
		{
			uint32_t x = (i * 32) % 1024;
			uint32_t y = i - (i % 32);
			SDL_Rect srect;
			SDL_Rect drect;
			srect.x = 0;
			srect.y = 0;
			srect.w = 32;
			srect.h = 32;
			drect.x = x;
			drect.y = y;
			drect.w = 32;
			drect.h = 32;
			uint8_t *pixels = tileset->GetTile(i, false);
			SDL_Surface *tileSurface = SDL_CreateRGBSurfaceFrom(pixels, 32, 32, 32, 128, CHANNEL_RED, CHANNEL_GREEN, CHANNEL_BLUE, CHANNEL_ALPHA);
			SDL_BlitSurface(tileSurface, &srect, TileSheet, &drect);
			SDL_FreeSurface(tileSurface);
			delete[] pixels;
		}
		if (Tilesheet != 0)
			glDeleteTextures(1, &Tilesheet);
		glGenTextures(1, &Tilesheet);
		glBindTexture(GL_TEXTURE_2D, Tilesheet);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
		glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_ADD);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TileSheet->w, TileSheet->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, TileSheet->pixels);
		glGenerateMipmap(GL_TEXTURE_2D);
		SDL_FreeSurface(TileSheet);
		TileSheet = nullptr;
	}
}

void Game::CreateSpritesheets()
{
	printf("Creating Spritesheets\n");
	vector<SDL_Surface *>spritesheetSurfaces;
	SDL_Surface *spritesheet = SDL_CreateRGBSurface(0, SPRITESHEET_SIZE, SPRITESHEET_SIZE, 32, CHANNEL_RED, CHANNEL_GREEN, CHANNEL_BLUE, CHANNEL_ALPHA);
	spritesheetSurfaces.push_back(spritesheet);

	LoadSpriteCoords();

	for (uint32_t ai = 0; ai < anims->GetAnimSetCount(); ai++)
	{
		const AnimationSet *animSet = anims->GetAnimSet(ai);
		if (animSet)
		{
			for (int aj = 0; aj < animSet->GetAnimCount(); aj++)
			{
				const Animation *anim = animSet->GetAnim(aj);
				for (uint32_t k = 0; k < anim->GetFrameCount(); k++)
				{
					const AnimationFrame *frame = anim->GetFrame(k);
					SpriteCoords coord = Sprites[frame->getIndex()];
					uint32_t* pixels = frame->getImage();
					int width = frame->getWidth();
					int height = frame->getHeight();
					SDL_Surface *frameSurface = SDL_CreateRGBSurfaceFrom(pixels, width, height, 32, width * 4, CHANNEL_RED, CHANNEL_GREEN, CHANNEL_BLUE, CHANNEL_ALPHA);
					BlitSurface(frameSurface, spritesheet, (int)coord.x, (int)coord.y, false, false);
					SDL_FreeSurface(frameSurface);
					delete[] pixels;
				}
			}
		}
	}
	for (size_t i = 0; i < spritesheetSurfaces.size(); i++)
	{
		spritesheet = spritesheetSurfaces[i];
#ifdef SAVE_SPRITESHEETS
		char buffer[256];
		sprintf_s(buffer, 255, "spritesheet-%02lu.bmp", i);
		SDL_SaveBMP(spritesheet, Path::Combine(DumpPath, buffer).c_str());
#endif
		GLuint texture;
		glGenTextures(1, &texture);
		SpriteSheets.push_back(texture);
		glBindTexture(GL_TEXTURE_2D, SpriteSheets[i]);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
		glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_ADD);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, spritesheet->w, spritesheet->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, spritesheet->pixels);
		glGenerateMipmap(GL_TEXTURE_2D);
		SDL_FreeSurface(spritesheet);
	}
}

void Game::PrepareFramebuffer()
{
	printf("Preparing Framebuffer\n");
	glGenTextures(1, &framebufferTexture);
	glBindTexture(GL_TEXTURE_2D, framebufferTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, (int)(ASPECT_RATIO * 480), 480, 0, GL_BGRA, GL_UNSIGNED_BYTE, nullptr);
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER_EXT, framebuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, framebufferTexture, 0);

	glGenBuffers(1, &framebufferVBO);
	glBindBuffer(GL_ARRAY_BUFFER, framebufferVBO);

	vertex framebufferVerts[4];
	framebufferVerts[0].x = 0;
	framebufferVerts[0].y = 0;
	framebufferVerts[0].z = 0;
	framebufferVerts[0].s = 0;
	framebufferVerts[0].t = 1;
	framebufferVerts[1].x = 0;
	framebufferVerts[1].y = 1;
	framebufferVerts[1].z = 0;
	framebufferVerts[1].s = 0;
	framebufferVerts[1].t = 0;
	framebufferVerts[2].x = 1;
	framebufferVerts[2].y = 1;
	framebufferVerts[2].z = 0;
	framebufferVerts[2].s = 1;
	framebufferVerts[2].t = 0;
	framebufferVerts[3].x = 1;
	framebufferVerts[3].y = 0;
	framebufferVerts[3].z = 0;
	framebufferVerts[3].s = 1;
	framebufferVerts[3].t = 1;

	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(vertex), framebufferVerts, GL_STATIC_DRAW);
}

void LoadShaders()
{
	FILE *fi = openFile("TilesheetVertex.glsl", "rb");
	if (fi == nullptr)
	{
		printf("Error opening vertex shader\n");
		return;
	}

	fseek(fi, 0, SEEK_END);
	uint32_t length = ftell(fi);
	fseek(fi, 0, SEEK_SET);

	char *vertexShaderSource = new char[length + 1];
	vertexShaderSource[length] = '\0';
	fread(vertexShaderSource, 1, length, fi);
	fclose(fi);


	fi = openFile("TilesheetVertex.glsl", "rb");
	if (fi == nullptr)
	{
		printf("Error opening vertex shader\n");
		return;
	}

	fseek(fi, 0, SEEK_END);
	length = ftell(fi);
	fseek(fi, 0, SEEK_SET);

	char *fragmentShaderSource = new char[length + 1];
	fragmentShaderSource[length] = '\0';
	fread(fragmentShaderSource, 1, length, fi);
	fclose(fi);

	delete[] vertexShaderSource;
	delete[] fragmentShaderSource;
}

bool _startsWith(const char *a, const char *b)
{
	if (strlen(a) < strlen(b))
		return false;
	for (int i = 0; i < strlen(b); i++)
		if (a[i] != b[i])
			return false;
	return true;
}

void Game::LoadSettings()
{
	Episode = "Prince.j2e"; // Set a default

	FILE *fi = openFile("Settings.ini", "r");
	if (fi != nullptr)
	{
		while (!feof(fi))
		{
			char buffer[256];
			char sparam[256];
			int param = 0;
			memset(buffer, 0, 256);
			fgets(buffer, 256, fi);
			if (buffer[strlen(buffer) - 1 == '\n'])
				buffer[strlen(buffer) - 1] = '\0';
			memset(sparam, 0, 256);
			if (strlen(buffer))
			{
				if (sscanf_s(buffer, "ResolutionX=%d", &param))
					WindowWidth = param;
				else if (sscanf_s(buffer, "ResolutionY=%d", &param))
					WindowHeight = param;
				else if (sscanf_s(buffer, "Fullscreen=%d", &param))
					Fullscreen = (param != 0);
				else if (sscanf_s(buffer, "BorderlessFullscreen=%d", &param))
					BorderlessFullscreen = (param != 0);
				else if (_startsWith(buffer, "Jazz2Dir="))
					Path = &buffer[9];
				else if (_startsWith(buffer, "DumpDir="))
					DumpPath = &buffer[8];
				else if (_startsWith(buffer, "Episode="))
					Episode = &buffer[8];
			}
		}
		fclose(fi);
	}
}

void Game::BlitSurface(SDL_Surface *src, SDL_Surface *dst, float offsetX, float offsetY, bool tileX, bool tileY)
{
	SDL_Rect s, d;
	s.x = 0;
	s.y = 0;
	s.w = src->w;
	s.h = src->h;
	d.w = src->w;
	d.h = src->h;
	if (tileX && tileY)
	{
		int startx = 0;
		int starty = 0;
		while (startx + offsetX > 0)
			startx -= src->w;
		while (starty + offsetY > 0)
			starty -= src->h;
		for (int i = starty; (src->h * i) + offsetY < dst->h; i++)
		{
			for (int j = startx; (src->w * j) + offsetX < dst->h; j++)
			{
				s.w = src->w;
				s.h = src->h;
				d.w = src->w;
				d.h = src->h;
				d.x = (int)Math::Round((src->w * j) + offsetX);
				d.y = (int)Math::Round((src->h * i) + offsetY);
				SDL_BlitSurface(src, &s, dst, &d);
			}
		}
	}
	else if (tileX && !tileY)
	{
		int startx = 0;
		while (startx + offsetX > 0)
			startx -= src->w;
		for (int j = startx; (src->w * j) + offsetX < WindowWidth; j++)
		{
			s.w = src->w;
			s.h = src->h;
			d.w = src->w;
			d.h = src->h;
			d.x = (int)Math::Round((src->w * j) + offsetX);
			d.y = (int)Math::Round(offsetY);
			SDL_BlitSurface(src, &s, dst, &d);
		}
	}
	else if (!tileX && tileY)
	{
		int starty = 0;
		while (starty + offsetY > 0)
			starty -= src->h;
		for (int i = starty; (src->h * i) + offsetY < WindowHeight; i++)
		{
			s.w = src->w;
			s.h = src->h;
			d.w = src->w;
			d.h = src->h;
			d.x = offsetX;
			d.y = (src->h * i) + offsetY;
			SDL_BlitSurface(src, &s, dst, &d);
		}
	}
	else
	{
		d.x = offsetX;
		d.y = offsetY;
		SDL_BlitSurface(src, &s, dst, &d);
	}
}

Game::~Game()
{
	if (tileset != nullptr)
	{
		delete tileset;
	}
	if (level != nullptr)
	{
		delete level;
	}
	if (player != nullptr)
	{
		delete player;
	}
	if (anims != nullptr)
	{
		delete anims;
	}

	glDeleteBuffers(8, Layers);
	glDeleteBuffers(1, &playerVBO);
	glDeleteBuffers(1, &framebufferVBO);
	glDeleteTextures(1, &framebufferTexture);
	for (int i = 0; i < SpriteSheets.size(); i++)
	{
		glDeleteTextures(1, &SpriteSheets[i]);
	}
	glDeleteTextures(1, &Tilesheet);
	SDL_Quit();
}

struct Node
{
	SDL_Rect rc;
	int SpriteID;
};

bool Compare(const Node &a, const Node &b)
{
	return (a.rc.h < b.rc.h);
}

void SortDescending(vector<Node> &nodes)
{
	std::sort(nodes.rbegin(), nodes.rend(), Compare);
}

void Game::LoadSpriteCoords()
{
	std::vector<Node> nodes;
	for (int i = 0; i < anims->GetAnimSetCount(); i++)
	{
		const AnimationSet *animSet = anims->GetAnimSet(i);
		for (int j = 0; j < animSet->GetAnimCount(); j++)
		{
			const Animation *anim = animSet->GetAnim(j);
			for (int k = 0; k < anim->GetFrameCount(); k++)
			{
				const AnimationFrame *frame = anim->GetFrame(k);
				Node node;
				node.rc.w = frame->getWidth()+1;
				node.rc.h = frame->getHeight()+1;
				node.SpriteID = frame->getIndex();
				nodes.push_back(node);
			}
		}
	}
	SortDescending(nodes);
	int row = 0;
	int rowHeight = nodes[0].rc.h;
	int offsetX = 0;
	for (size_t i = 0; i < nodes.size(); i++)
	{
		if ((offsetX + nodes[i].rc.w) >= SPRITESHEET_SIZE)
		{
			offsetX = 0;
			row += rowHeight;
			rowHeight = nodes[i].rc.h;
		}
		nodes[i].rc.x = offsetX;
		nodes[i].rc.y = row;
		offsetX += nodes[i].rc.w;
		SpriteCoords coord;
		coord.x = nodes[i].rc.x;
		coord.y = nodes[i].rc.y;
		coord.width = nodes[i].rc.w - 1;
		coord.height = nodes[i].rc.h - 1;
		coord.TextureIndex = 0;
		Sprites[nodes[i].SpriteID] = coord;
	}
}