#include "synths/synthfactory.h"
#include "synths/triosc.h"

// XXX: TEST
#include <iostream>

SynthFactory::~SynthFactory() {
        std::vector<SafePtr<ISynth> >::iterator it = _synths.begin();
        std::cout << "Destroying synth factory" << std::endl;
        int i = 0;
        while (it != _synths.end()) {
                std::cout << "Synth " << i << " reffed " << (*it).getCount() << " times" << std::endl;
                ++it; ++i;
        }
}

int SynthFactory::getClassNumber() {
	return 1;
}

std::string SynthFactory::getClassName(int index) {
	switch (index) {
		case 1: return "TripleOscillator";
	}
}

SafePtr<ISynth> SynthFactory::createNewSynth(const std::string& className) {
	ISynth* ret = NULL;
	if (!className.compare("TripleOscillator")) {
		ret = new TripleOscillator(_bufsize);
	};

	SafePtr<ISynth> safePtr(ret);
	_synths.push_back(safePtr);
	return safePtr;
}

void SynthFactory::dropSynth(SafePtr<ISynth> synth) {
        std::vector<SafePtr<ISynth> >::iterator it = _synths.begin();
        while (it != _synths.end()) {
                if (*it == synth) {
                          it =_synths.erase(it);
                } else {
                  ++it;
                }
        }
}

int SynthFactory::getSynthPosition(SafePtr<ISynth> synth) {
        std::vector<SafePtr<ISynth> >::iterator it = _synths.begin();
        int pos = 0;
        while (it != _synths.end()) {
                if (*it == synth) {
                          return pos;
                }
                ++it;
                ++pos;
        }
        return 0;
}

