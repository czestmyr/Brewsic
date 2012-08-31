#include <iostream>
#include <cstring>
#include "mixer/mainmixer.h"
#include "mixer/filterqueue.h"
#include "mixer/synthqueue.h"
#include "synths/Isynth.h"
#include "gui/mixer/mixer.h"

MainMixer::MainMixer(int bufsize, int channels): _main_mixer(bufsize), _guiSignal(this), _master_volume(1.0) {
	_channels = channels;
	_bufsize = bufsize;

	_mixers = new Mixer*[channels];
	_filters = new FilterQueue*[channels];
        _volumes = new Volume*[channels];
	_synths = new SynthQueue*[channels];

	for (int i = 0; i < _channels; ++i) {
		_mixers[i] = new Mixer(_bufsize);
		_filters[i] = new FilterQueue();
                _volumes[i] = new Volume(1.0);

                char buffer[16];
                sprintf(buffer, "Synth Queue %i", i+1);
		_synths[i] = new SynthQueue(buffer);
	}
}

MainMixer::~MainMixer() {
	for (int i = 0; i < _channels; ++i) {
		delete _mixers[i];
		delete _filters[i];
                delete _volumes[i];
		delete _synths[i];
	}
	delete[] _mixers;
	delete[] _filters;
        delete[] _volumes;
	delete[] _synths;

        _gui_parent.clear();
        _gui.clear();
}

void MainMixer::mixInto(Sint16* buffer) {
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

                _volumes[ch]->filter(_mixers[ch]->getBufferSize(), _mixers[ch]->getBuffer());
		_main_mixer.mixIn(_mixers[ch]->getBuffer());
	}

        _master_volume.filter(_main_mixer.getBufferSize(), _main_mixer.getBuffer());
	_main_mixer.copyBuffer(buffer);
}

void MainMixer::setGuiParent(SafePtr<IControl> guiParent) {
  _gui_parent = guiParent;
  for (int i = 0; i < _channels; ++i) {
    _synths[i]->setGuiParent(guiParent);
  }
}

void MainMixer::unsetGuiParent() {
  _gui_parent.clear();
  for (int i = 0; i < _channels; ++i) {
    _synths[i]->unsetGuiParent();
  }
}

void MainMixer::guiSignal() {
	if (_gui) _gui->deleteMe();

	_gui = safe_new(MixerGui(_gui_parent, this));
}

