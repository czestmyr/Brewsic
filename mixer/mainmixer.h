#ifndef _MAIN_MIXER_H_
#define _MAIN_MIXER_H_

#include "mixer/mixer.h"

class FilterQueue;
class SynthQueue;

class MainMixer {
	public:
		MainMixer(int bufsize, int channels);
		~MainMixer();

		void mixInto(Uint16* buffer);

		FilterQueue* getFilterQueue(int i) { return _filters[i]; }
		SynthQueue* getSynthQueue(int i) { return _synths[i]; }
	private:
		int _channels;
		int _bufsize;

		Mixer _main_mixer;

		Mixer** _mixers;
		FilterQueue** _filters;
		SynthQueue** _synths;
};

#endif

