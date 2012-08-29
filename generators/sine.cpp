#include "sine.h"
#include <cmath>

void SineGenerator::generate(int bufsize, float* buffer) {
	for (int i = 0; i < bufsize; ++i) {
		buffer[i] = sin(_phase) / 8.0;
		_phase += _phase_inc;
	}

	while (_phase > 2*3.1415926535) _phase -= 2*3.1415926535;
}

IGenerator* SineGenerator::clone() {
	return new SineGenerator(_freq, _phase);
}
