#ifndef _GATE_FILTER_H_
#define _GATE_FILTER_H_

#include "Ifilter.h"

class GateFilter: public IFilter {
	public:
		GateFilter(float freq, float duty, float speed = 15) {
			_phase = 0;
			_active = false;
			_val = 0.0;
			_speed = speed;
			_freq = freq;
			_duty = duty;
			_phase_inc = _freq / _samp_freq;
		}
		~GateFilter() {};

		void filter(int bufsize, float* buffer);
	private:
		float _phase_inc;
		float _phase;
		float _freq;
		float _duty;

		bool _active;
		float _val;
		float _speed;
};

#endif

