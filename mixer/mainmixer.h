#ifndef _MAIN_MIXER_H_
#define _MAIN_MIXER_H_

#include "mixer/mixer.h"
#include "common/signals.h"
#include "common/pointers.h"
#include "gui/Icontrol.h"

class FilterQueue;
class SynthQueue;

class MainMixer {
	public:
		MainMixer(int bufsize, int channels);
		~MainMixer();

		void mixInto(Uint16* buffer);

		FilterQueue* getFilterQueue(int i) { return _filters[i]; }
		SynthQueue* getSynthQueue(int i) { return _synths[i]; }

		void setGuiParent(SafePtr<IControl> guiParent) { _gui_parent = guiParent; }
		SIGNAL_DESTINATION(_guiSignal, MainMixer, guiSignal);
		void guiSignal();
	private:
		SafePtr<IControl> _gui_parent;
		SafePtr<IControl> _gui;

		int _channels;
		int _bufsize;

		Mixer _main_mixer;

		Mixer** _mixers;
		FilterQueue** _filters;
		SynthQueue** _synths;
};

#endif

