#include "mixer/synthqueue.h"
#include "gui/window.h"
#include "gui/button.h"

void SynthQueue::dropSynth(SafePtr<ISynth> synth) {
  std::vector<SafePtr<ISynth> >::iterator it = _synths.begin();
  while (it != _synths.end()) {
    if (*it == synth) {
      it = _synths.erase(it);
    } else {
      ++it;
    }
  }
}

void SynthQueue::guiSignal() {
	if (_gui)
		_gui->deleteMe();

	int _size = _synths.size();
	int window_size = 60 + _size * 20 + (_size-1) * 5;  // Title, buttons, margins
	_gui = safe_new(Window(_gui_parent, 0, 0, 200, window_size, _name.c_str()));

	for (int i = 0; i < _size; ++i) {
		safe_new(Button(_gui, 0, 0, _synths[i]->getClassName(), _synths[i]->_createGui.getSignal()));
	}

	_gui->packVertically(5);
}

