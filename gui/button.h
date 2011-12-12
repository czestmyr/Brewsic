#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <SDL.h>
#include <string>
#include "Icontrol.h"

class Button: public IControl {
	public:
		Button(IControl* parent, int x, int y, const char* text = "A button", void* data = NULL);
		~Button();

		void draw(SDL_Surface* surf, int orig_x, int orig_y);
		bool leftPress(int x, int y);
		bool leftRelease(int x, int y);
		void setCallback(void (*callback)(void* data)) { _callback = callback; }
	protected:
		bool _pressed;

		std::string _text;
		SDL_Surface* _textSurf;

		void* _data;
		void (*_callback)(void* data);
};

#endif
