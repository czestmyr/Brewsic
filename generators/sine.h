#ifndef _SINE_GENERATOR_H_
#define _SINE_GENERATOR_H_

#include "Igenerator.h"

class SineGenerator: public IGenerator {
	public:
		SineGenerator(float frequency, float phase = 0.0):
			IGenerator(frequency, phase) {};
		~SineGenerator() {};

		void generate(int bufsize, float* buffer);

		IGenerator* clone();
};

#endif

