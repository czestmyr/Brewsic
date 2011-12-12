#ifndef _ADSR_H_
#define _ADSR_H_

#include "Ifilter.h"

class Adsr: public IFilter {
	public:
		Adsr(float att_ms = 50, float dec_ms = 100, float sus_ms = 200, float rel_ms = 1000) {
			_a = att_ms;
			_d = dec_ms;
			_s = sus_ms;
			_r = rel_ms;
			_phase_inc = 1000.0/_samp_freq;
			_phase = 1000000.0;
			_curr = 0.0;
		}

		~Adsr() {};

		void filter(int bufsize, float* buffer);

		void trigger();
		void release();
	private:
		float _a, _d, _s, _r;
		float _phase;
		float _phase_inc;
		bool _hold;
		float _curr;
};

#endif

