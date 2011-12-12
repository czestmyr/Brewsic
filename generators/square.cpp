#include "square.h"
#include <cmath>

void SquareGenerator::generate(int bufsize, float* buffer) {
	double dummy;
	for (int i = 0; i < bufsize; ++i) {
		buffer[i] = (floor(modf(_phase/3.14, &dummy) - 0.5) + 0.5) * 0.125; //SQUARE
		_phase += _phase_inc;
	}

	while (_phase > 2*3.1415926535) _phase -= 2*3.1415926535;
}

IGenerator* SquareGenerator::clone() {
	return new SquareGenerator(_freq, _phase, _duty);
}
