#include "triosc.h"

#include <cmath>
#include <iostream>

#include "gui/Icontrol.h"
#include "gui/synths/triosc.h"

using namespace std;

TripleOscillator::TripleOscillator(int bufsize):
	_bufsize(bufsize),
	_mixer(bufsize),
	_adsr(),
	_first(0.0),
	_second(0.0),
	_shift(0),
	_first_gen(GEN_SINE, checkGeneratorsAction()),
	_second_gen(GEN_SINE, checkGeneratorsAction()),
	_third_gen(GEN_SINE, checkGeneratorsAction()) {
	for (int i = 0; i < POLYPHONY; ++i) {
		_adsr[i] = new Adsr(200, 100, 101, 20000);
                _a_linker.addProperty(&_adsr[i]->_attack);
                _d_linker.addProperty(&_adsr[i]->_decay);
                _s_linker.addProperty(&_adsr[i]->_sustain);
                _r_linker.addProperty(&_adsr[i]->_release);
		_buffers[i] = new float[bufsize];
		for (int j = 0; j < 3; ++j) {
			_generators[i][j] = GeneratorFactory::inst()->createGenerator(GEN_SINE, 440.0, 0);
		}
	}

	for (int i = 0; i < POLYPHONY; ++i) {
		_channelQueue.push(i);
	}

        constructSynthName();
}

void TripleOscillator::setGenerator(int oscId, IGenerator* gen) {
	if (oscId < 0 || oscId >= 3) return;
	for (int i = 0; i < POLYPHONY; ++i) {
		delete _generators[i][oscId];
		if (i == 0)
			_generators[i][oscId] = gen;
		else
			_generators[i][oscId] = gen->clone();
	}
}

void TripleOscillator::startNote(int noteId, float frequency) {
	//Prevent retriggering
	for (int i = 0; i < POLYPHONY; ++i) {
		if (_notes[i] == noteId) return;
	}

	//Get an oscillator channel (either free or the first one)
	int channel;
	if (!_channelQueue.empty()) {
		channel = _channelQueue.front();
		_channelQueue.pop();
	} else channel = 0;

	frequency += _shift;

	_generators[channel][0]->setFreq(frequency);
	_generators[channel][1]->setFreq(frequency*pow(2, _first/1200.0));
	_generators[channel][2]->setFreq(frequency*pow(2, _second/1200.0));
	_adsr[channel]->trigger();
	_notes[channel] = noteId;
}

void TripleOscillator::stopNote(int noteId) {
	for (int i = 0; i < POLYPHONY; ++i) {
		if (_notes[i] == noteId) {
			_notes[i] = 0;
			_channelQueue.push(i);
			_adsr[i]->release();
			break;
		}
	}
}

void TripleOscillator::stopAllNotes() {
	for (int i = 0; i < POLYPHONY; ++i) {
	  _notes[i] = 0;
	  _channelQueue.push(i);
	  _adsr[i]->release();
	}
}

void TripleOscillator::generateOutput() {
	_mixer.clear();
	for (int i = 0; i < POLYPHONY; ++i) {
		for (int j = 0; j < 3; ++j) {
			_generators[i][j]->generate(_bufsize, _buffers[i]);
			_adsr[i]->filter(_bufsize, _buffers[i]);
			_mixer.mixIn(_buffers[i]);
		}
	}
}

float* TripleOscillator::getBuffer() {
	return _mixer.getBuffer();
}

void TripleOscillator::showGui() {
	if (_gui) _gui->deleteMe();

	_gui = safe_new(TripleOscillatorGui(_gui_parent, this));
}

void TripleOscillator::checkGenerators() {
	for (int j = 0; j < 3; ++j) {
		int type;
		if (j == 0) type = _first_gen;
		if (j == 1) type = _second_gen;
		if (j == 2) type = _third_gen;
		if (_generators[0][j]->type() == type) continue;
		for (int i = 0; i < POLYPHONY; ++i) {
			delete _generators[i][j];
			_generators[i][j] = GeneratorFactory::inst()->createGenerator((GeneratorType)type, 440.0, 0);
		}
	}
}

