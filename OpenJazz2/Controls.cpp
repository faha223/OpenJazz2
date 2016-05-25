#include "Controls.h"
#include "FileIO.h"
#include <string>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_gamecontroller.h>
using namespace std;

uint32_t GetSDLK(const string &str);
uint32_t GetJoyButton(const string &str);

void LoadControls(const char *controlsIni, multimap<Control, uint32_t> &KeyBindings, multimap<Control, uint32_t> &JoyBindings)
{
	/*Bindings.insert(pair<Control, uint32_t>(LEFT, SDLK_LEFT));
	Bindings.insert(pair<Control, uint32_t>(RIGHT, SDLK_RIGHT));
	Bindings.insert(pair<Control, uint32_t>(UP, SDLK_UP));
	Bindings.insert(pair<Control, uint32_t>(DOWN, SDLK_DOWN));
	Bindings.insert(pair<Control, uint32_t>(JUMP, SDLK_SPACE));
	Bindings.insert(pair<Control, uint32_t>(RUN, SDLK_z));
	Bindings.insert(pair<Control, uint32_t>(SHOOT, SDLK_LCTRL));
	Bindings.insert(pair<Control, uint32_t>(SELECT, SDLK_RETURN));
	Bindings.insert(pair<Control, uint32_t>(MENU, SDLK_ESCAPE));*/

	//FILE *fo = openFile("C:\\Users\\fred.hallock\\Desktop\\GameControllerStrings.txt", "w");
	//fprintf(fo, "A: %s\n", SDL_GameControllerGetStringForButton(SDL_CONTROLLER_BUTTON_A));
	//fprintf(fo, "A: %s\n", SDL_GameControllerGetStringForButton(SDL_CONTROLLER_BUTTON_B));
	//fprintf(fo, "A: %s\n", SDL_GameControllerGetStringForButton(SDL_CONTROLLER_BUTTON_X));
	//fprintf(fo, "A: %s\n", SDL_GameControllerGetStringForButton(SDL_CONTROLLER_BUTTON_Y));
	//fprintf(fo, "A: %s\n", SDL_GameControllerGetStringForButton(SDL_CONTROLLER_BUTTON_START));
	//fprintf(fo, "A: %s\n", SDL_GameControllerGetStringForButton(SDL_CONTROLLER_BUTTON_BACK));
	//fprintf(fo, "A: %s\n", SDL_GameControllerGetStringForButton(SDL_CONTROLLER_BUTTON_LEFTSHOULDER));
	//fprintf(fo, "A: %s\n", SDL_GameControllerGetStringForButton(SDL_CONTROLLER_BUTTON_RIGHTSHOULDER));
	//fprintf(fo, "A: %s\n", SDL_GameControllerGetStringForButton(SDL_CONTROLLER_BUTTON_DPAD_LEFT));
	//fprintf(fo, "A: %s\n", SDL_GameControllerGetStringForButton(SDL_CONTROLLER_BUTTON_DPAD_RIGHT));
	//fprintf(fo, "A: %s\n", SDL_GameControllerGetStringForButton(SDL_CONTROLLER_BUTTON_DPAD_UP));
	//fprintf(fo, "A: %s\n", SDL_GameControllerGetStringForButton(SDL_CONTROLLER_BUTTON_DPAD_DOWN));
	//fclose(fo);

	FILE *fi = openFile("KeyBindings.ini", "r");
	if (fi != nullptr)
	{
		char buffer[256];
		while (!feof(fi))
		{
			memset(buffer, 0, 256);
			fgets(buffer, 255, fi);
			char param[256];
			if (sscanf_s(buffer, "Jump=JOY_%s", param, 255))
				JoyBindings.insert(pair<Control, uint32_t>(JUMP, GetJoyButton(string(param))));
			else if (sscanf_s(buffer, "Left=JOY_%s", param, 255))
				JoyBindings.insert(pair<Control, uint32_t>(LEFT, GetJoyButton(string(param))));
			else if (sscanf_s(buffer, "Right=JOY_%s", param, 255))
				JoyBindings.insert(pair<Control, uint32_t>(RIGHT, GetJoyButton(string(param))));
			else if (sscanf_s(buffer, "Up=JOY_%s", param, 255))
				JoyBindings.insert(pair<Control, uint32_t>(UP, GetJoyButton(string(param))));
			else if (sscanf_s(buffer, "Down=JOY_%s", param, 255))
				JoyBindings.insert(pair<Control, uint32_t>(DOWN, GetJoyButton(string(param))));
			else if (sscanf_s(buffer, "Run=JOY_%s", param, 255))
				JoyBindings.insert(pair<Control, uint32_t>(RUN, GetJoyButton(string(param))));
			else if (sscanf_s(buffer, "Shoot=JOY_%s", param, 255))
				JoyBindings.insert(pair<Control, uint32_t>(SHOOT, GetJoyButton(string(param))));
			else if (sscanf_s(buffer, "Select=JOY_%s", param, 255))
				JoyBindings.insert(pair<Control, uint32_t>(SELECT, GetJoyButton(string(param))));
			else if (sscanf_s(buffer, "Menu=JOY_%s", param, 255))
				JoyBindings.insert(pair<Control, uint32_t>(MENU, GetJoyButton(string(param))));
			else if (sscanf_s(buffer, "Jump=%s", param, 255))
				KeyBindings.insert(pair<Control, uint32_t>(JUMP, GetSDLK(string(param))));
			else if (sscanf_s(buffer, "Left=%s", param, 255))
				KeyBindings.insert(pair<Control, uint32_t>(LEFT, GetSDLK(string(param))));
			else if (sscanf_s(buffer, "Right=%s", param, 255))
				KeyBindings.insert(pair<Control, uint32_t>(RIGHT, GetSDLK(string(param))));
			else if (sscanf_s(buffer, "Up=%s", param, 255))
				KeyBindings.insert(pair<Control, uint32_t>(UP, GetSDLK(string(param))));
			else if (sscanf_s(buffer, "Down=%s", param, 255))
				KeyBindings.insert(pair<Control, uint32_t>(DOWN, GetSDLK(string(param))));
			else if (sscanf_s(buffer, "Run=%s", param, 255))
				KeyBindings.insert(pair<Control, uint32_t>(RUN, GetSDLK(string(param))));
			else if (sscanf_s(buffer, "Shoot=%s", param, 255))
				KeyBindings.insert(pair<Control, uint32_t>(SHOOT, GetSDLK(string(param))));
			else if (sscanf_s(buffer, "Select=%s", param, 255))
				KeyBindings.insert(pair<Control, uint32_t>(SELECT, GetSDLK(string(param))));
			else if (sscanf_s(buffer, "Menu=%s", param, 255))
				KeyBindings.insert(pair<Control, uint32_t>(MENU, GetSDLK(string(param))));
		}
		fclose(fi);
	}
}

string GetSDLKEntry(const uint32_t &key)
{
	switch (key)
	{
	case SDLK_BACKSPACE:
		return "BACKSPACE";
	case SDLK_TAB:
		return "TAB";
	case SDLK_CLEAR:
		return "CLEAR";
	case SDLK_RETURN:
		return "return ";
	case SDLK_PAUSE:
		return "PAUSE";
	case SDLK_ESCAPE:
		return "ESCAPE";
	case SDLK_SPACE:
		return "SPACE";
	case SDLK_EXCLAIM:
		return "EXCLAIM";
	case SDLK_QUOTEDBL:
		return "QUOTEDBL";
	case SDLK_HASH:
		return "HASH";
	case SDLK_DOLLAR:
		return "DOLLAR";
	case SDLK_AMPERSAND:
		return "AMPERSAND";
	case SDLK_QUOTE:
		return "QUOTE";
	case SDLK_LEFTPAREN:
		return "LEFTPAREN";
	case SDLK_RIGHTPAREN:
		return "RIGHTPAREN";
	case SDLK_ASTERISK:
		return "ASTERISK";
	case SDLK_PLUS:
		return "PLUS";
	case SDLK_COMMA:
		return "COMMA";
	case SDLK_MINUS:
		return "MINUS";
	case SDLK_PERIOD:
		return "PERIOD";
	case SDLK_SLASH:
		return "SLASH";
	case SDLK_0:
		return "0";
	case SDLK_1:
		return "1";
	case SDLK_2:
		return "2";
	case SDLK_3:
		return "3";
	case SDLK_4:
		return "4";
	case SDLK_5:
		return "5";
	case SDLK_6:
		return "6";
	case SDLK_7:
		return "7";
	case SDLK_8:
		return "8";
	case SDLK_9:
		return "9";
	case SDLK_COLON:
		return "COLON";
	case SDLK_SEMICOLON:
		return "SEMICOLON";
	case SDLK_LESS:
		return "LESS";
	case SDLK_EQUALS:
		return "EQUALS";
	case SDLK_GREATER:
		return "GREATER";
	case SDLK_QUESTION:
		return "QUESTION";
	case SDLK_AT:
		return "AT";
	case SDLK_LEFTBRACKET:
		return "LEFTBRACKET";
	case SDLK_BACKSLASH:
		return "BACKSLASH";
	case SDLK_RIGHTBRACKET:
		return "RIGHTBRACKET";
	case SDLK_CARET:
		return "CARET";
	case SDLK_UNDERSCORE:
		return "UNDERSCORE";
	case SDLK_BACKQUOTE:
		return "BACKQUOTE";
	case SDLK_a:
		return "a";
	case SDLK_b:
		return "b";
	case SDLK_c:
		return "c";
	case SDLK_d:
		return "d";
	case SDLK_e:
		return "e";
	case SDLK_f:
		return "f";
	case SDLK_g:
		return "g";
	case SDLK_h:
		return "h";
	case SDLK_i:
		return "i";
	case SDLK_j:
		return "j";
	case SDLK_k:
		return "k";
	case SDLK_l:
		return "l";
	case SDLK_m:
		return "m";
	case SDLK_n:
		return "n";
	case SDLK_o:
		return "o";
	case SDLK_p:
		return "p";
	case SDLK_q:
		return "q";
	case SDLK_r:
		return "r";
	case SDLK_s:
		return "s";
	case SDLK_t:
		return "t";
	case SDLK_u:
		return "u";
	case SDLK_v:
		return "v";
	case SDLK_w:
		return "w";
	case SDLK_x:
		return "x";
	case SDLK_y:
		return "y";
	case SDLK_z:
		return "z";
	case SDLK_DELETE:
		return "DELETE";
	case SDLK_KP_PERIOD:
		return "KP_PERIOD";
	case SDLK_KP_DIVIDE:
		return "KP_DIVIDE";
	case SDLK_KP_MULTIPLY:
		return "KP_MULTIPLY";
	case SDLK_KP_MINUS:
		return "KP_MINUS";
	case SDLK_KP_PLUS:
		return "KP_PLUS";
	case SDLK_KP_ENTER:
		return "KP_ENTER";
	case SDLK_KP_EQUALS:
		return "KP_EQUALS";
	case SDLK_UP:
		return "UP";
	case SDLK_DOWN:
		return "DOWN";
	case SDLK_RIGHT:
		return "RIGHT";
	case SDLK_LEFT:
		return "LEFT";
	case SDLK_INSERT:
		return "INSERT";
	case SDLK_HOME:
		return "HOME";
	case SDLK_END:
		return "END";
	case SDLK_PAGEUP:
		return "PAGEUP";
	case SDLK_PAGEDOWN:
		return "PAGEDOWN";
	case SDLK_F1:
		return "F1";
	case SDLK_F2:
		return "F2";
	case SDLK_F3:
		return "F3";
	case SDLK_F4:
		return "F4";
	case SDLK_F5:
		return "F5";
	case SDLK_F6:
		return "F6";
	case SDLK_F7:
		return "F7";
	case SDLK_F8:
		return "F8";
	case SDLK_F9:
		return "F9";
	case SDLK_F10:
		return "F10";
	case SDLK_F11:
		return "F11";
	case SDLK_F12:
		return "F12";
	case SDLK_F13:
		return "F13";
	case SDLK_F14:
		return "F14";
	case SDLK_F15:
		return "F15";
	case SDLK_CAPSLOCK:
		return "CAPSLOCK";
	case SDLK_RSHIFT:
		return "RSHIFT";
	case SDLK_LSHIFT:
		return "LSHIFT";
	case SDLK_RCTRL:
		return "RCTRL";
	case SDLK_LCTRL:
		return "LCTRL";
	case SDLK_RALT:
		return "RALT";
	case SDLK_LALT:
		return "LALT";
	case SDLK_MODE:
		return "MODE";
	case SDLK_HELP:
		return "HELP";
	case SDLK_SYSREQ:
		return "SYSREQ";
	case SDLK_MENU:
		return "MENU";
	case SDLK_POWER:
		return "POWER";
	default:
		return "";
	}
}

uint32_t GetSDLK(const string &str)
{
	if (str == "BACKSPACE") return SDLK_BACKSPACE;
	if (str == "TAB") return SDLK_TAB;
	if (str == "CLEAR") return SDLK_CLEAR;
	if (str == "RETURN") return SDLK_RETURN;
	if (str == "PAUSE") return SDLK_PAUSE;
	if (str == "ESCAPE") return SDLK_ESCAPE;
	if (str == "SPACE") return SDLK_SPACE;
	if (str == "EXCLAIM") return SDLK_EXCLAIM;
	if (str == "QUOTEDBL") return SDLK_QUOTEDBL;
	if (str == "HASH") return SDLK_HASH;
	if (str == "DOLLAR") return SDLK_DOLLAR;
	if (str == "AMPERSAND") return SDLK_AMPERSAND;
	if (str == "QUOTE") return SDLK_QUOTE;
	if (str == "LEFTPAREN") return SDLK_LEFTPAREN;
	if (str == "RIGHTPAREN") return SDLK_RIGHTPAREN;
	if (str == "ASTERISK") return SDLK_ASTERISK;
	if (str == "PLUS") return SDLK_PLUS;
	if (str == "COMMA") return SDLK_COMMA;
	if (str == "MINUS") return SDLK_MINUS;
	if (str == "PERIOD") return SDLK_PERIOD;
	if (str == "SLASH") return SDLK_SLASH;
	if (str == "0") return SDLK_0;
	if (str == "1") return SDLK_1;
	if (str == "2") return SDLK_2;
	if (str == "3") return SDLK_3;
	if (str == "4") return SDLK_4;
	if (str == "5") return SDLK_5;
	if (str == "6") return SDLK_6;
	if (str == "7") return SDLK_7;
	if (str == "8") return SDLK_8;
	if (str == "9") return SDLK_9;
	if (str == "COLON") return SDLK_COLON;
	if (str == "SEMICOLON") return SDLK_SEMICOLON;
	if (str == "LESS") return SDLK_LESS;
	if (str == "EQUALS") return SDLK_EQUALS;
	if (str == "GREATER") return SDLK_GREATER;
	if (str == "QUESTION") return SDLK_QUESTION;
	if (str == "AT") return SDLK_AT;
	if (str == "LEFTBRACKET") return SDLK_LEFTBRACKET;
	if (str == "BACKSLASH") return SDLK_BACKSLASH;
	if (str == "RIGHTBRACKET") return SDLK_RIGHTBRACKET;
	if (str == "CARET") return SDLK_CARET;
	if (str == "UNDERSCORE") return SDLK_UNDERSCORE;
	if (str == "BACKQUOTE") return SDLK_BACKQUOTE;
	if (str == "a") return SDLK_a;
	if (str == "b") return SDLK_b;
	if (str == "c") return SDLK_c;
	if (str == "d") return SDLK_d;
	if (str == "e") return SDLK_e;
	if (str == "f") return SDLK_f;
	if (str == "g") return SDLK_g;
	if (str == "h") return SDLK_h;
	if (str == "i") return SDLK_i;
	if (str == "j") return SDLK_j;
	if (str == "k") return SDLK_k;
	if (str == "l") return SDLK_l;
	if (str == "m") return SDLK_m;
	if (str == "n") return SDLK_n;
	if (str == "o") return SDLK_o;
	if (str == "p") return SDLK_p;
	if (str == "q") return SDLK_q;
	if (str == "r") return SDLK_r;
	if (str == "s") return SDLK_s;
	if (str == "t") return SDLK_t;
	if (str == "u") return SDLK_u;
	if (str == "v") return SDLK_v;
	if (str == "w") return SDLK_w;
	if (str == "x") return SDLK_x;
	if (str == "y") return SDLK_y;
	if (str == "z") return SDLK_z;
	if (str == "DELETE") return SDLK_DELETE;
	if (str == "KP_PERIOD") return SDLK_KP_PERIOD;
	if (str == "KP_DIVIDE") return SDLK_KP_DIVIDE;
	if (str == "KP_MULTIPLY") return SDLK_KP_MULTIPLY;
	if (str == "KP_MINUS") return SDLK_KP_MINUS;
	if (str == "KP_PLUS") return SDLK_KP_PLUS;
	if (str == "KP_ENTER") return SDLK_KP_ENTER;
	if (str == "KP_EQUALS") return SDLK_KP_EQUALS;
	if (str == "UP") return SDLK_UP;
	if (str == "DOWN") return SDLK_DOWN;
	if (str == "RIGHT") return SDLK_RIGHT;
	if (str == "LEFT") return SDLK_LEFT;
	if (str == "INSERT") return SDLK_INSERT;
	if (str == "HOME") return SDLK_HOME;
	if (str == "END") return SDLK_END;
	if (str == "PAGEUP") return SDLK_PAGEUP;
	if (str == "PAGEDOWN") return SDLK_PAGEDOWN;
	if (str == "F1") return SDLK_F1;
	if (str == "F2") return SDLK_F2;
	if (str == "F3") return SDLK_F3;
	if (str == "F4") return SDLK_F4;
	if (str == "F5") return SDLK_F5;
	if (str == "F6") return SDLK_F6;
	if (str == "F7") return SDLK_F7;
	if (str == "F8") return SDLK_F8;
	if (str == "F9") return SDLK_F9;
	if (str == "F10") return SDLK_F10;
	if (str == "F11") return SDLK_F11;
	if (str == "F12") return SDLK_F12;
	if (str == "F13") return SDLK_F13;
	if (str == "F14") return SDLK_F14;
	if (str == "F15") return SDLK_F15;
	if (str == "CAPSLOCK") return SDLK_CAPSLOCK;
	if (str == "RSHIFT") return SDLK_RSHIFT;
	if (str == "LSHIFT") return SDLK_LSHIFT;
	if (str == "RCTRL") return SDLK_RCTRL;
	if (str == "LCTRL") return SDLK_LCTRL;
	if (str == "RALT") return SDLK_RALT;
	if (str == "LALT") return SDLK_LALT;
	if (str == "MODE") return SDLK_MODE;
	if (str == "HELP") return SDLK_HELP;
	if (str == "SYSREQ") return SDLK_SYSREQ;
	if (str == "MENU") return SDLK_MENU;
	if (str == "POWER") return SDLK_POWER;
	
	return 0;
}

uint32_t GetJoyButton(const string &str)
{
	return SDL_GameControllerGetButtonFromString(str.c_str());
}

string GetJoyButton(const uint32_t &btn)
{
	switch (btn)
	{
	case SDL_CONTROLLER_BUTTON_A:
	case SDL_CONTROLLER_BUTTON_B:
	case SDL_CONTROLLER_BUTTON_X:
	case SDL_CONTROLLER_BUTTON_Y:
	case SDL_CONTROLLER_BUTTON_BACK:
	case SDL_CONTROLLER_BUTTON_GUIDE:
	case SDL_CONTROLLER_BUTTON_START:
	case SDL_CONTROLLER_BUTTON_LEFTSTICK:
	case SDL_CONTROLLER_BUTTON_RIGHTSTICK:
	case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
	case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
	case SDL_CONTROLLER_BUTTON_DPAD_UP:
	case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
	case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
	case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
	case SDL_CONTROLLER_BUTTON_MAX:
		break;
	default:
		return SDL_GameControllerGetStringForButton(SDL_CONTROLLER_BUTTON_INVALID);
	}
	return SDL_GameControllerGetStringForButton((SDL_GameControllerButton)btn);
}