#ifndef _MATRIX_H_
#define _MATRIX_H_

#include "Icontrol.h"

class Matrix: public IControl {
	public:
		Matrix(SafePtr<IControl> parent, int x, int y, int w, int h);

                const char* controlClassName() { return "Matrix"; }

		void draw(SDL_Surface* surf, int orig_x, int orig_y);

		void setShift(int shift) { _shift = shift; }
		int getShift() { return _shift; }
	private:
		int _shift;
		int _x_shift;
		int _zoom;
};

#endif

