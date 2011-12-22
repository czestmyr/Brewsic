#ifndef _TRIPLE_OSCILLATOR_
#define _TRIPLE_OSCILLATOR_

#include "Isynth.h"
#include "mixer.h"
#include "generators/Igenerator.h"
#include "generators/saw.h"
#include "generators/square.h"
#include "generators/sine.h"
#include "filters/adsr.h"
#include "common/property.h"
#include "trioscobserver.h"

#include <queue>

#define POLYPHONY 4

enum {
	PROP_FIRST,
	PROP_SECOND,
	PROP_SHIFT
};

class TripleOscillator: public ISynth{
	public:
		TripleOscillator(int bufsize):
			_bufsize(bufsize),
			_mixer(bufsize),
			_adsr(),
			_first(-1205.0),
			_first_obs(this, PROP_FIRST),
			_second(-1195.0),
			_second_obs(this, PROP_SECOND),
			_shift(0),
			_shift_obs(this, PROP_SHIFT) {
			for (int i = 0; i < POLYPHONY; ++i) {
				for (int j = 0; j < 3; ++j) {
					_generators[i][j] = new SineGenerator(440.0);
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

		void setGenerator(int oscId, IGenerator* gen);

		void startNote(int noteId, float frequency);
		void stopNote(int noteId);
		void generateOutput(float* buffer);

		Property<float> _shift;
		Property<float> _first;
		Property<float> _second;
	private:
		friend class TriOscObserver;
		TriOscObserver _shift_obs;
		TriOscObserver _first_obs;
		TriOscObserver _second_obs;

		Mixer _mixer;

		Adsr* _adsr[POLYPHONY];
		IGenerator* _generators[POLYPHONY][3];
		float* _buffers[POLYPHONY];
		int _notes[POLYPHONY];

		std::queue<int> _channelQueue;

		int _bufsize;
};

#endif

