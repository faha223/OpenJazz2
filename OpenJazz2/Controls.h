#ifndef _CONTROLS_H_
#define _CONTROLS_H_

#include <map>
#include <cstdint>

enum Control
{
	LEFT,
	RIGHT,
	UP,
	DOWN,
	JUMP,
	SHOOT,
	RUN,
	SELECT,
	MENU
};

void LoadControls(const char *controlsIni, std::multimap<Control, uint32_t> &KeyBindings, std::multimap<Control, uint32_t> &JoyBindings);

#endif