#ifndef _TRIPLE_OSCILLATOR_
#define _TRIPLE_OSCILLATOR_

#include "Isynth.h"
#include "../mixer.h"
#include "../generators/Igenerator.h"
#include "../generators/saw.h"
#include "../generators/square.h"
#include "../generators/sine.h"
#include "../filters/adsr.h"

#include <queue>

#define POLYPHONY 4

class TripleOscillator: public ISynth{
	public:
		TripleOscillator(int bufsize): _bufsize(bufsize), _mixer(bufsize), _adsr(), _first(-1205.0), _second(-1195.0), _shift(0) {
			for (int i = 0; i < POLYPHONY; ++i) {
				for (int j = 0; j < 1; ++j) {
					_generators[i][j] = new SquareGenerator(440.0);
				}
				for (int j = 1; j < 3; ++j) {
					_generators[i][j] = new SawGenerator(440.0);
				}
				_adsr[i] = new Adsr(20, 100, 101, 2000);
				_buffers[i] = new float[bufsize];
			}

			for (int i = 0; i < POLYPHONY; ++i) {
				_channelQueue.push(i);
			}
		}

		~TripleOscillator() {
			for (int i = 0; i < POLYPHONY; ++i) {
				for (int j = 0; j < 3; ++j) {
					delete _generators[i][j];
				}
				delete _adsr[i];
				delete[] _buffers[i];
			}
		}

		void setBase(float freq) { _shift = freq - 440; }
		void setFirst(float cents) { _first = cents; }
		void setSecond(float cents) { _second = cents; }

		void startNote(int noteId, float frequency);
		void stopNote(int noteId);
		void generateOutput(float* buffer);
	private:
		Mixer _mixer;

		Adsr* _adsr[POLYPHONY];
		IGenerator* _generators[POLYPHONY][3];
		float* _buffers[POLYPHONY];
		int _notes[POLYPHONY];

		std::queue<int> _channelQueue;

		int _bufsize;

		float _shift;
		float _first;
		float _second;
};

#endif

