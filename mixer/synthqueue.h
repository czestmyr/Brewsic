#ifndef _SYNTH_QUEUE_H_
#define _SYNTH_QUEUE_H_

#include <vector>
#include "synths/Isynth.h"
#include "common/pointers.h"

class SynthQueue {
	public:
		size_t size() { return _synths.size(); }
		void pushSynth(SafePtr<ISynth> synth) { _synths.push_back(synth); }

		SafePtr<ISynth> getSynth(int i) { return _synths[i]; }

	private:
		std::vector<SafePtr<ISynth> > _synths;
};

#endif

