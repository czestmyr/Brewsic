#ifndef _SQUARE_GENERATOR_H_
#define _SQUARE_GENERATOR_H_

#include "Igenerator.h"

class SquareGenerator: public IGenerator {
	public:
		SquareGenerator(float frequency, float phase = 0.0, float duty = 0.5):
			IGenerator(frequency, phase), _duty(duty) {};
		~SquareGenerator() {};

		void generate(int bufsize, float* buffer);

	private:
		float _duty;
};

#endif

