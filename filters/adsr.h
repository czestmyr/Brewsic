#ifndef _ADSR_H_
#define _ADSR_H_

#include "Ifilter.h"
#include "common/property.h"
#include "gui/Icontrol.h"

class Adsr: public IFilter {
	public:
		Adsr(float att_ms = 200, float dec_ms = 300, float sus_ms = 500, float rel_ms = 1000) {
			_attack = att_ms;
			_decay = dec_ms;
			_sustain = sus_ms;
			_release = rel_ms;
			_phase_inc = 1000.0/_samp_freq;
			_phase = 1000000.0;
			_curr = 0.0;

			prepareGui();
		}

		~Adsr() {};

		void filter(int bufsize, float* buffer);

		void trigger();
		void release();

		SafePtr<IControl> getGui() { return _gui; }

		Property<float> _attack;
		Property<float> _decay;
		Property<float> _sustain;
		Property<float> _release;

	protected:
		void prepareGui();
		SafePtr<IControl> _gui;

		float _a, _d, _s, _r;
		float _phase;
		float _phase_inc;
		bool _hold;
		float _curr;
};

#endif

