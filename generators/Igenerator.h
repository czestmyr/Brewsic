#ifndef _I_GENERATOR_H_
#define _I_GENERATOR_H_

#include "generatorfactory.h"

#include <SDL.h>

class IGenerator {
	public:
		IGenerator(float frequency, float phase) {
			_samp_freq = 22050.0;
			_phase = phase;
			setFreq(frequency);
		}

		virtual ~IGenerator() {}

		virtual void generate(int bufsize, float* buffer) = 0;
		virtual IGenerator* clone() = 0;
		virtual GeneratorType type() = 0;

		float getFreq() { return _freq; }

		void setFreq(float newFreq) {
			SDL_LockAudio();
			_freq = newFreq;
			_phase_inc = (_freq*2*3.1415926535)/_samp_freq;
			SDL_UnlockAudio();
		}

	protected:
		float _samp_freq;
		float _freq;
		float _phase_inc;
		float _phase;
};

#endif

