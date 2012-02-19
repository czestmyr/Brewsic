#include "mixer/synthqueue.h"
#include "gui/window.h"
#include "gui/button.h"

void SynthQueue::guiSignal() {
	if (_gui)
		_gui->deleteMe();

	int _size = _synths.size();
	int window_size = 60 + _size * 20 + (_size-1) * 5;  // Title, buttons, margins
	_gui = safe_new(Window(_gui_parent, 0, 0, 200, window_size, _name.c_str()));

	for (int i = 0; i < _size; ++i) {
		safe_new(Button(_gui, 0, 0, _synths[i]->getClassName()));
	}

	_gui->packVertically(5);
}

