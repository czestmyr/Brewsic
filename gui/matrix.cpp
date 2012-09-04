#include "matrix.h"
#include "style.h"
#include <cmath>
#include "song/pattern.h"

#include <iostream>

Matrix::Matrix(SafePtr<IControl> parent, int x, int y, int w, int h)
: IControl(parent) {
	_shift = 720;
	_x_shift = 0;
	_zoom = 1;
	redim(x, y, w, h);
}

void Matrix::draw(SDL_Surface* surf, int orig_x, int orig_y) {
	SDL_Color sh = Style::inst()->getShadeColor();
	SDL_Color med = Style::inst()->getMediumColor();
	SDL_Color fg = Style::inst()->getFgColor();
	Uint32 sh32 = SDL_MapRGB(surf->format, sh.r, sh.g, sh.b);
	Uint32 med32 = SDL_MapRGB(surf->format, med.r, med.g, med.b);
	Uint32 fg32 = SDL_MapRGB(surf->format, fg.r, fg.g, fg.b);

	Uint32 note32 = SDL_MapRGB(surf->format, 192, 64, 32);

	int yoff = orig_y + _y;
	int xoff = orig_x + _x;
	Draw_FillRect(surf, xoff, yoff, _w, _h, sh32);

	// Draw horizontal matrix lines	
	#define TAB_HEIGHT 16
	int n = (_shift / TAB_HEIGHT + 1) % 12;
	for (int y = TAB_HEIGHT-(_shift%TAB_HEIGHT); y < getH(); y += TAB_HEIGHT) {
		if (n == 0)
			Draw_HLine(surf, xoff, yoff + y, xoff + _w - 1, med32);
		else
			Draw_HLine(surf, xoff, yoff + y, xoff + _w - 1, fg32);
		n = (n+1)%12;
	}

	// Draw vertical matrix lines	
	int step_width = 64/_zoom;      // How wide in pixels should be each step
	int step_skip = 16/step_width;  // We should render every step_skip-th step
	if (step_skip < 1) step_skip = 1;

	int highlights = 4/step_skip;   // How often we should draw a lighter vertical line

	int h = 0;
	for (int x = 0; x < getW(); x += step_width*step_skip) {
		if (h == 0)
			Draw_VLine(surf, xoff + x, yoff, yoff + _h - 1, med32);
		else
			Draw_VLine(surf, xoff + x, yoff, yoff + _h - 1, fg32);
		h = (h+1)%highlights;
	}

        // Draw notes
        std::set<SafePtr<Note> >::iterator it = _pattern->_notes.begin();
        while (it != _pattern->_notes.end()) {
          int x_begin = timeToXPos((*it)->_begin);
          int x_end = timeToXPos((*it)->_end);
          int y_begin = freqToYPos((*it)->_frequency);
          //std::cout << "Note coords: " << x_begin << "," << x_end << "," << y_begin << std::endl;
	  Draw_FillRect(surf, x_begin + xoff, y_begin + yoff, x_end - x_begin, 10, note32);
          ++it;
        }

//	Style::inst()->drawInset(surf, orig_x + _x, orig_y + _y, _w, _h, 2);
}

// TODO: use tempo information
int Matrix::timeToXPos(float time) {
  #define STEPS_PER_SECOND 1.0
  float steps = time / 1000.0 * STEPS_PER_SECOND;
  int step_width = 64/_zoom;
  return (int)(steps * step_width);
}

int Matrix::freqToYPos(float freq) {
  //TODO: SO far 1:1 correspondency (just a test)
  return (int)freq;
}

