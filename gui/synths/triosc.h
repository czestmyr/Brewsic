#ifndef _TRIOSC_GUI_H_
#define _TRIOSC_GUI_H_

#include "gui/window.h"

class TripleOscillator;
class Slider;
class Wheel;

class TripleOscillatorGui: public Window {
	public:
		TripleOscillatorGui(IControl* parent, TripleOscillator* osc);

	private:
		TripleOscillator* _osc;

		Slider* _base;
		Wheel* _first;
		Wheel* _second;
};

#endif

