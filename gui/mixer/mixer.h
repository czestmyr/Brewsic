#ifndef _MIXER_GUI_H_
#define _MIXER_GUI_H_

#include "gui/window.h"

class MainMixer;

class MixerGui: public Window {
	public:
		MixerGui(SafePtr<IControl> parent, MainMixer* mixer);

	private:
		MainMixer* _mixer;
};

#endif

