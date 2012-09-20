#include "mixer/synthqueue.h"
#include "gui/window.h"
#include "gui/button.h"
#include "gui/frame.h"

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

void SynthQueue::newSynth() {
  // TODO: Make the user choose the synth
  SafePtr<ISynth> synth = _factory->createNewSynth("TripleOscillator");
  synth->setGuiParent(_gui_parent);
  _synths.push_back(synth);

  // Recreate gui
  showGui();
}

// TODO: Might create a special class for this?
void SynthQueue::showGui() {
  if (_gui)
    _gui->deleteMe();

  int _size = _synths.size();
  int window_size = 60 + (_size+1) * 20 + _size * 5;  // Title, buttons, margins
  _gui = safe_new(Window(_gui_parent, 0, 0, 200, window_size, _name.c_str()));
  
  safe_new(Button(_gui, 0, 0, "New Synth", newSynthAction()))->setPreferedSize(0, 20, 1);
  safe_new(Frame(_gui, 0, 0, 0, 0))->setPreferedSize(0, 2, 1);

  for (int i = 0; i < _size; ++i) {
    safe_new(Button(_gui, 0, 0, _synths[i]->getSynthName().c_str(), _synths[i]->showGuiAction()));
  }

  _gui->packVertically(5);
}

