#include <iostream>
#include "mixer/mainmixer.h"
#include "mixer/filterqueue.h"
#include "mixer/synthqueue.h"
#include "synths/Isynth.h"

MainMixer::MainMixer(int bufsize, int channels): _main_mixer(bufsize) {
	_channels = channels;
	_bufsize = bufsize;

	_mixers = new Mixer*[channels];
	_filters = new FilterQueue*[channels];
	_synths = new SynthQueue*[channels];

	for (int i = 0; i < _channels; ++i) {
		_mixers[i] = new Mixer(_bufsize);
		_filters[i] = new FilterQueue();
		_synths[i] = new SynthQueue("Synth Queue");
	}
}

MainMixer::~MainMixer() {
	for (int i = 0; i < _channels; ++i) {
		delete _mixers[i];
	}
	delete[] _mixers;
	delete[] _filters;
	delete[] _synths;
}

void MainMixer::mixInto(Uint16* buffer) {
	_main_mixer.clear();
	for (int ch = 0; ch < _channels; ch++) {
		if (_synths[ch]->size() == 0) continue;

		_mixers[ch]->clear();
		for (int s = 0; s < _synths[ch]->size(); ++s) {
			SafePtr<ISynth> synth = _synths[ch]->getSynth(s);
			synth->generateOutput();
			if (s == 0) {
				_mixers[ch]->copyFromBufferFloat(synth->getBuffer());
			} else {
				_mixers[ch]->mixIn(synth->getBuffer());
			}
		}

		_main_mixer.mixIn(_mixers[ch]->getBuffer());
	}

	_main_mixer.copyBuffer(buffer);
}

