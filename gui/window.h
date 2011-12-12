#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "Icontrol.h"

class Window: public IControl {
	public:
		Window(IControl*, int x, int y, int w, int h);
		void draw(SDL_Surface* surf, int orig_x, int orig_y);
};

#endif

