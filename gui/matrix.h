#ifndef _MATRIX_H_
#define _MATRIX_H_

#include "Icontrol.h"

class Pattern;

class Matrix: public IControl {
	public:
		Matrix(SafePtr<IControl> parent, int x, int y, int w, int h);

                const char* controlClassName() { return "Matrix"; }

		void draw(SDL_Surface* surf, int orig_x, int orig_y);

		void setShift(int shift) { _shift = shift; }
		int getShift() { return _shift; }

		bool leftPress(int x, int y);
		bool rightPress(int x, int y);

                void setPattern(Pattern* pattern) { _pattern = pattern; }
	private:
                Pattern* _pattern;

		int _shift;
		int _x_shift;
		int _zoom;

                int timeToXPos(float time);
                float xPosToTime(int pos);
                int freqToYPos(float freq);
                float yPosToFreq(int pos);
                float yPosToSnapFreq(int pos);
};

#endif

