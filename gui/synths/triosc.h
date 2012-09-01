#ifndef _TRIOSC_GUI_H_
#define _TRIOSC_GUI_H_

#include "gui/window.h"

class TripleOscillator;
class Slider;
class Wheel;

class TripleOscillatorGui: public Window {
	public:
		TripleOscillatorGui(SafePtr<IControl> parent, TripleOscillator* osc);

                const char* controlClassName() { return "Triosc GUI"; }
	private:
		TripleOscillator* _osc;
};

#endif

