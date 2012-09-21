#include "song/songcontrol.h"
#include "song/eventqueue.h"
#include "gui/frame.h"
#include "gui/button.h"

void SongControl::play() {
  _playing = true;
}

void SongControl::stop() {
  _playing = false;

  // Stop all the contained event queues
  std::list<EventQueue*>::iterator it = eventQueues.begin();
  while (it != eventQueues.end()) {
    (*it)->stop();
    ++it;
  }
}

void SongControl::processEvents(float timeMs) {
  float time = timeMs * _bps / 1000.0;
}

void SongControl::increaseTempo() {
  _bpm++;
  if (_bpm > 333) _bpm = 333;
  recalculateBps();
}

void SongControl::decreaseTempo() {
  _bpm--;
  if (_bpm < 20) _bpm = 20;
  recalculateBps();
}

void SongControl::showGui() {
  if (_gui)
    _gui->deleteMe();

  _gui = safe_new(Frame(_gui_parent, 0, 0, 0, 0, 1));
  safe_new(Button(_gui, 0, 0, "Play", playAction()));
  safe_new(Button(_gui, 0, 0, "Stop", stopAction()));
  safe_new(Button(_gui, 0, 0, "Plus", increaseTempoAction()));
  safe_new(Button(_gui, 0, 0, "Minus", decreaseTempoAction()));
  _gui->packHorizontally(5);
}

void SongControl::recalculateBps() {
  _bps = _bpm / 60.0;
}

