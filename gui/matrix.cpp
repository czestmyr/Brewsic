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
          x_begin = x_begin < 0 ? 0 : x_begin;
          int x_end = timeToXPos((*it)->_end);
          x_end = x_end > _w ? _w : x_end;
          int y_begin = freqToYPos((*it)->_frequency);
          y_begin = y_begin < 0 ? 0 : y_begin;
          int height = (y_begin + TAB_HEIGHT) > _h ? _h - y_begin : TAB_HEIGHT;
          if (height > 0 && x_end > x_begin) {
            //std::cout << "Note coords: " << x_begin << "," << x_end << "," << y_begin << std::endl;
	    Draw_FillRect(surf, x_begin + xoff, y_begin + yoff, x_end - x_begin, height, note32);
            Style::inst()->drawInset(surf, x_begin + xoff, y_begin + yoff, x_end - x_begin, height, 2);
          }
          ++it;
        }
}

bool Matrix::leftPress(int x, int y) {
  SafePtr<Note> note = _pattern->getNote(xPosToTime(x - _x), yPosToFreq(y - _y));
  if (!note) {
    float time = xPosToTime(x - _x);
    _pattern->newNote(floor(time), floor(time) + 0.25, yPosToSnapFreq(y - _y));
  }
}

bool Matrix::rightPress(int x, int y) {
  float time = xPosToTime(x - _x);
  float freq = yPosToFreq(y - _y);
  std::cout << "Right press, time: " << time << ", freq: " << freq << std::endl;
  SafePtr<Note> note = _pattern->getNote(time, freq);
  std::cout << note.get() << std::endl;
  if (note) {
    _pattern->deleteNote(note);
  }
}

int Matrix::timeToXPos(float time) {
  int step_width = 64/_zoom;
  return (int)(time * step_width);
}

float Matrix::xPosToTime(int pos) {
  int step_width = 64/_zoom;
  return ((float)pos) / step_width;
}

int Matrix::freqToYPos(float freq) {
  // === A short explanation of the math here ===
  // tone = 57 + 12 * ld(f/440.0)    -> This is MIDI-like definition, only mine starts at C0. 440.0 is frequency of A4
  // One tone is 16 px, thus:
  // y = tone * 16 = 1248 + 16*12/ln(2) * ln(f/440.0)
  int y = 913 + 276.997447851 * log(freq/440.0);
  return y - _shift;
}

float Matrix::yPosToFreq(int pos) {
  // Inverse of the aforementioned function
  return exp((pos + _shift - 913) / 276.997447851) * 440.0;
}

//TODO: snapping
float Matrix::yPosToSnapFreq(int pos) {
  // Inverse of the aforementioned function
  return exp((floor((pos + _shift)/16.0)*16.0 - 913) / 276.997447851) * 440.0;
}

