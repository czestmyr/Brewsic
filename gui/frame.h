#ifndef _FRAME_H_
#define _FRAME_H_

#include "gui/Icontrol.h"

class Frame: public IControl {
	public:
		Frame(SafePtr<IControl> parent, int x, int y, int w, int h): IControl(parent) {
			redim(x, y, w, h);
		}

		int getXMin() { return 3; }
		int getYMin() { return 3; }
		int getXMax() { return _w-3; }
		int getYMax() { return _h-3; }

		void draw(SDL_Surface* surf, int orig_x, int orig_y);
};

#endif

