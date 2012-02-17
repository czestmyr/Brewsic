#include "synths/synthfactory.h"
#include "synths/triosc.h"

int getClassNumber() {
	return 1;
}

std::string getClassName(int index) {
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

