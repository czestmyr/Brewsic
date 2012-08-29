#ifndef _SYNTH_QUEUE_H_
#define _SYNTH_QUEUE_H_

#include <vector>
#include <string>
#include "synths/Isynth.h"
#include "gui/Icontrol.h"
#include "common/pointers.h"
#include "common/Iobserver.h"
#include "common/property.h"
#include "common/signals.h"

class SynthQueue {
	public:
		SynthQueue(const std::string& name): _name(name), _guiSignal(this) {}
                ~SynthQueue() { _gui_parent.clear(); _gui.clear(); }

		size_t size() { return _synths.size(); }
		void pushSynth(SafePtr<ISynth> synth) { _synths.push_back(synth); }
                void dropSynth(SafePtr<ISynth> synth);

		SafePtr<ISynth> getSynth(int i) { return _synths[i]; }

		void setGuiParent(SafePtr<IControl> par) { _gui_parent = par; }
                void unsetGuiParent() { _gui_parent.clear(); }
		SIGNAL_DESTINATION(_guiSignal, SynthQueue, guiSignal);
		void guiSignal();
	private:
		std::string _name;
		SafePtr<IControl> _gui;
		SafePtr<IControl> _gui_parent;
		std::vector<SafePtr<ISynth> > _synths;
};

#endif

