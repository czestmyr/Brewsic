#include "song/eventqueue.h"

EventQueue::EventQueue(Pattern* pattern) {
  resetPlayback(0.0);
}

void EventQueue::resetPlayback(float time) {
  _begin_time = time;
  _playback = _events.begin();
}

bool EventQueue::play(float time) {
  int local_time = time - _begin_time;
  _last_local_time = local_time;

  SafePtr<ISynth> synth = _pattern->getSynth();
  if (!synth) return false;

  while (_playback != _events.end()) {
    // If a future event is encountered, stop
    if (_playback->first > local_time) break;

    NoteEvent& event = _playback->second;
    if (event._begin) {
      synth->startNote(event._note->_id, event._note->_frequency);
    } else {
      synth->stopNote(event._note->_id);
    }

    _playback++;
  }

  if (local_time > _pattern->getLength()) {
    return false;
  }

  return true;
}

void EventQueue::stop() {
  SafePtr<ISynth> synth = _pattern->getSynth();
  if (!synth) return;

  synth->stopAllNotes();
}

