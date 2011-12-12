#ifndef _SAW_GENERATOR_H_
#define _SAW_GENERATOR_H_

#include "Igenerator.h"

class SawGenerator: public IGenerator {
	public:
		SawGenerator(float frequency, float phase = 0.0):
			IGenerator(frequency, phase) {};
		~SawGenerator() {};

		void generate(int bufsize, float* buffer);
};

#endif

