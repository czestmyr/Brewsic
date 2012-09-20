#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <SDL.h>
#include <string>
#include "Icontrol.h"
#include "common/signals.h"

template <class T> class Property;

class Button: public IControl {
	public:
		Button(SafePtr<IControl> parent, int x, int y, const char* text = "A button", Action action = Action());
		~Button();

                const char* controlClassName() { return "Button"; }

		void draw(SDL_Surface* surf, int orig_x, int orig_y);
		bool leftPress(int x, int y);
		bool leftRelease(int x, int y);

                void addAction(Action action);
                void removeAction(Action action);

                bool getPressed() { return _pressed; }
	protected:
		Signal _sig;
		bool _pressed;

		std::string _text;
		SDL_Surface* _textSurf;
};

#endif
