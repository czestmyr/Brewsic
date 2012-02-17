#ifndef _SYNTH_FACTORY_H_
#define _SYNTH_FACTORY_H_

#include <string>
#include <vector>
#include "common/pointers.h"
#include "synths/Isynth.h"

class SynthFactory {
	public:
		SynthFactory(int bufsize): _bufsize(bufsize) {}

		int getClassNumber();
		std::string getClassName(int index);

		SafePtr<ISynth> createNewSynth(const std::string& className);

		int getSynthNumber() { return _synths.size(); }
		SafePtr<ISynth> getSynth(int index) {
			if (index >= 0 && index < _synths.size())
				return _synths[index];
			else
				return SafePtr<ISynth>();
		}
	private:
		int _bufsize;

		std::vector<SafePtr<ISynth> > _synths;
};

#endif

