#ifndef _BACKGROUND_H_
#define _BACKGROUND_H_

#include "Icontrol.h"

class Background: public IControl {
	public:
		Background(IControl* parent, int x, int y, int w, int h): IControl(parent) {
			redim(x, y, w, h);
			_rect.x = _x;
			_rect.y = _y;
			_rect.w = _w;
			_rect.h = _h;
		}

		int getXMin() { return 10; }
		int getYMin() { return 10; }
		int getXMax() { return _w-10; }
		int getYMax() { return _h-10; }

		void draw(SDL_Surface* surf, int orig_x, int orig_y);
	protected:
		SDL_Rect _rect;
};

#endif

