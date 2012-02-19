#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <SDL.h>
#include <string>
#include "Icontrol.h"
#include "common/Iobserver.h"
#include "common/signals.h"

template <class T> class Property;

class Button: public IControl {
	public:
		Button(SafePtr<IControl> parent, int x, int y, const char* text = "A button", Signal sig = Signal());
		~Button();

		void draw(SDL_Surface* surf, int orig_x, int orig_y);
		bool leftPress(int x, int y);
		bool leftRelease(int x, int y);
	protected:
		Signal _sig;
		bool _pressed;

		std::string _text;
		SDL_Surface* _textSurf;
};

#endif
