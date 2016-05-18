# OpenJazz2
An Open Source Jazz Jackrabbit 2 engine project.

This project uses the art assets from the game engine directly to play the game. It doesn't need to convert the art assets into another format to be playable. I wrote this using C++ and OpenGL and SDL for windowing. This is in no way anywhere close to functional, but it makes a good proof of concept for how you would load the art assets and render the levels, and it has a pretty basic collision detection and resolution system that would be a good starting point for making a serious attempt at building a new engine for Jazz Jackrabbit 2.

# What Works
- Level Rendering, Mostly
- Walking
- Running
- Coins
- Score UI
- Spritefonts (not animated)
- Ropes, Poles (mostly)
- Ammo
- Springs, mostly

# What Doesn't Work
- Sound
- Weapons
- Most Pickups
- Enemies
- Crates
- Destructible Environment Pieces
- Bridges

# Dependencies
- OpenGL (GLEW, GLU)
- SDL2
- On Windows:
    - GLEW
- On Linux:
    - GLEW
    - GLU
    - Boost