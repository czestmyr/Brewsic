#include "triosc.h"

#include <cmath>

using namespace std;

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

void TripleOscillator::generateOutput(float* buffer) {
	_mixer.clear();
	for (int i = 0; i < POLYPHONY; ++i) {
		for (int j = 0; j < 3; ++j) {
			_generators[i][j]->generate(_bufsize, _buffers[i]);
			_adsr[i]->filter(_bufsize, _buffers[i]);
			_mixer.mixIn(_buffers[i]);
		}
	}

	_mixer.copyBufferFloat(buffer);
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

