#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <SDL.h>
#include <string>
#include "Icontrol.h"
#include "common/Iobserver.h"

template <class T> class Property;

class Button: public IControl, IObserver {
	public:
		Button(SafePtr<IControl> parent, int x, int y, const char* text = "A button", Property<int>* prop = NULL);
		~Button();

		void draw(SDL_Surface* surf, int orig_x, int orig_y);
		bool leftPress(int x, int y);
		bool leftRelease(int x, int y);

		void signal();
		void disconnect();
	protected:
		Property<int>* _prop;
		bool _pressed;

		std::string _text;
		SDL_Surface* _textSurf;
};

#endif
