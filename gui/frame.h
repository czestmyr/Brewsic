#ifndef _FRAME_H_
#define _FRAME_H_

#include "gui/Icontrol.h"

class Frame: public IControl {
	public:
		Frame(SafePtr<IControl> parent, int x, int y, int w, int h, int t = 1): IControl(parent), _thickness(t) {
			redim(x, y, w, h);
		}

		int getXMin() { return _thickness; }
		int getYMin() { return _thickness; }
		int getXMax() { return _w-_thickness; }
		int getYMax() { return _h-_thickness; }

		void draw(SDL_Surface* surf, int orig_x, int orig_y);
	protected:
		int _thickness;
};

#endif

