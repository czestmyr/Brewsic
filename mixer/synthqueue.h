#ifndef _SYNTH_QUEUE_H_
#define _SYNTH_QUEUE_H_

#include <vector>
#include <string>
#include "synths/Isynth.h"
#include "synths/synthfactory.h"
#include "gui/Icontrol.h"
#include "gui/Iwithgui.h"
#include "common/pointers.h"
#include "common/Iobserver.h"
#include "common/property.h"
#include "common/signals.h"

// XXX: TEST
#include <iostream>

class SynthQueue: public IWithGui {
	public:
		SynthQueue(const std::string& name): _name(name), _newSynthSignal(this) {}
                ~SynthQueue() {
                  std::cout << "Synth queue " << _name << " destruction:" << std::endl;
                  for (int i = 0; i < _synths.size(); ++i) {
                    std::cout << "Synth " << i << " reffed " << _synths[i].getCount() << " times" << std::endl;
                  }
                  _gui_parent.clear(); _gui.clear();
                }

		size_t size() { return _synths.size(); }
		void pushSynth(SafePtr<ISynth> synth) { _synths.push_back(synth); }
                void dropSynth(SafePtr<ISynth> synth);

		SafePtr<ISynth> getSynth(int i) { return _synths[i]; }

                void setSynthFactory(SafePtr<SynthFactory> factory) { _factory = factory; }

                SIGNAL_DESTINATION(_newSynthSignal, SynthQueue, newSynth);

		void guiSignal();
                void newSynth();
	private:
		std::string _name;
                SafePtr<SynthFactory> _factory;
		std::vector<SafePtr<ISynth> > _synths;
};

#endif

