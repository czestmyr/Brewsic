#include "triosc.h"

#include "gui/slider.h"
#include "gui/wheel.h"
#include "gui/pselect.h"
#include "synths/triosc.h"

TripleOscillatorGui::TripleOscillatorGui(SafePtr<IControl> parent, TripleOscillator* osc):
	_osc(osc),
	Window(parent, 0, 0, 80, 200, "3xOsc") {
	_base = new Slider(this, 5, 5, 190, 20.0, 20000.0, &_osc->_shift);
	_first = new Wheel(this, 30, 5, 20, 20, -100.0, 100.0, &_osc->_first);
	_second = new Wheel(this, 55, 5, 20, 20, -100.0, 100.0, &_osc->_second);
}
