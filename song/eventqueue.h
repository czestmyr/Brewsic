#ifndef _SONG_EVENT_QUEUE_H_
#define _SONG_EVENT_QUEUE_H_

#include "song/noteevent.h"
#include <map>

class Pattern;

class EventQueue {
  public:
    EventQueue(Pattern* pattern);

    void resetPlayback(float time);
    // Returns whether it wants to play again next time
    bool play(float time);
    void stop();

    float getTime() { return _last_local_time; }

    float addNote(SafePtr<Note> note);
    float removeNote(SafePtr<Note> note);

  private:
    Pattern* _pattern;

    std::multimap<float, NoteEvent> _events;
    std::multimap<float, NoteEvent>::iterator _playback;
    float _begin_time; // Time is in beats
    float _last_local_time;
};

#endif

