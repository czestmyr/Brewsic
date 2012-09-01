#ifndef _MAIN_MIXER_H_
#define _MAIN_MIXER_H_

#include "mixer/mixer.h"
#include "common/signals.h"
#include "common/pointers.h"
#include "gui/Icontrol.h"
#include "filters/volume.h"

class FilterQueue;
class SynthQueue;
class SynthFactory;

class MainMixer {
	public:
		MainMixer(int bufsize, int channels, SafePtr<SynthFactory> factory);
		~MainMixer();

		void mixInto(Sint16* buffer);

		FilterQueue* getFilterQueue(int i) { return _filters[i]; }
		SynthQueue* getSynthQueue(int i) { return _synths[i]; }
                Volume* getVolume(int i) { return _volumes[i]; }
                Volume* getMasterVolume() { return &_master_volume; }

                int getChannels() const { return _channels; }

		void setGuiParent(SafePtr<IControl> guiParent);
                void unsetGuiParent();

		SIGNAL_DESTINATION(_guiSignal, MainMixer, guiSignal);
		void guiSignal();
	private:
		SafePtr<IControl> _gui_parent;
		SafePtr<IControl> _gui;

		int _channels;
		int _bufsize;

		Mixer _main_mixer;
                SafePtr<SynthFactory> _factory;

		Mixer** _mixers;
		FilterQueue** _filters;
                Volume** _volumes;
                Volume _master_volume;
		SynthQueue** _synths;
};

#endif

