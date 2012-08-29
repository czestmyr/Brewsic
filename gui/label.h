#ifndef _LABEL_H_
#define _LABEL_H_

#include <SDL.h>
#include <string>
#include "Icontrol.h"
#include "common/Iobserver.h"

template <class T> class Property;

class Label: public IControl, IObserver {
	public:
		Label(SafePtr<IControl> parent, int x, int y, Property<std::string>* prop = NULL);
		Label(SafePtr<IControl> parent, int x, int y, const char* text);
		~Label();

		void draw(SDL_Surface* surf, int orig_x, int orig_y);

		void signal();
		void disconnect();
	private:
		Property<std::string>* _prop;
		SDL_Surface* _textSurf;
		const char* _text;
};

#endif
