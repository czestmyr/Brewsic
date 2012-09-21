#ifndef _SONG_EVENT_QUEUE_H_
#define _SONG_EVENT_QUEUE_H_

#include "song/pattern.h"
#include "song/noteevent.h"
#include <map>

class EventQueue {
  public:
    EventQueue(Pattern* pattern);

    void resetPlayback(float time);
    // Returns whether it wants to play again next time
    bool play(float time);
    void stop();

    float getTime() { return _last_local_time; }

  private:
    Pattern* _pattern;

    std::map<float, NoteEvent> _events;
    std::map<float, NoteEvent>::iterator _playback;
    float _begin_time; // Time is in beats
    float _last_local_time;
};

#endif

