#ifndef _TRIPLE_OSCILLATOR_
#define _TRIPLE_OSCILLATOR_

#include "Isynth.h"
#include "mixer/mixer.h"
#include "generators/Igenerator.h"
#include "generators/saw.h"
#include "generators/square.h"
#include "generators/sine.h"
#include "filters/adsr.h"
#include "common/property.h"
#include "common/propertylinker.h"
#include "gui/synths/triosc.h"

#include <queue>
#include <string>

//XXX: TEST
#include <iostream>

#define POLYPHONY 4

class TripleOscillator: public ISynth {
	public:
		TripleOscillator(int bufsize);
		~TripleOscillator() {
                        std::cout << "Deleting triosc" << std::endl;
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
                void stopAllNotes();
		void generateOutput();
		float* getBuffer();

		const char* getClassName() { return "TripleOscillator"; }
		void showGui();

		Property<float> _shift;
		Property<float> _first;
		Property<float> _second;

		Property<int> _first_gen;
		Property<int> _second_gen;
		Property<int> _third_gen;

	protected:
		ACTION(TripleOscillator, checkGenerators);
                void checkGenerators();

		friend class TripleOscillatorGui;

		Mixer _mixer;

		Adsr* _adsr[POLYPHONY];
                PropertyLinker<float> _a_linker;
                PropertyLinker<float> _d_linker;
                PropertyLinker<float> _s_linker;
                PropertyLinker<float> _r_linker;

		IGenerator* _generators[POLYPHONY][3];
		float* _buffers[POLYPHONY];
		int _notes[POLYPHONY];

		std::queue<int> _channelQueue;

		int _bufsize;
};

#endif

