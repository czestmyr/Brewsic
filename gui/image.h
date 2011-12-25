#ifndef _IMAGE_H_
#define _IMAGE_H_

#include "Icontrol.h"
#include <SDL.h>

class Image: public IControl {
	public:
		Image(SafePtr<IControl> parent, int x, int y, int w, int h, const char* filename);
		~Image();

		void draw(SDL_Surface* surf, int orig_x, int orig_y);
	private:
		SDL_Surface* _img;

		SDL_Rect _size;
};

#endif

