#ifndef _SONG_CONTROL_H_
#define _SONG_CONTROL_H_

#include "gui/Iwithgui.h"
#include "common/signals.h"
#include <list>

class EventQueue;

class SongControl: public IWithGui {
  public:
    SongControl(): _bpm(120), _playing(false) { recalculateBps(); }
    ~SongControl() {}

    ACTION(SongControl, play);
    ACTION(SongControl, stop);
    void play();
    void stop();

    bool isPlaying() { return _playing; }
    void processEvents(float timeMs);

    ACTION(SongControl, increaseTempo);
    ACTION(SongControl, decreaseTempo);
    void increaseTempo();
    void decreaseTempo();

    void showGui();

  private:
    void recalculateBps();

    bool _playing;

    int _bpm;
    float _bps; // Beats per second. Should be a bit better that BPM for calculations

    std::list<EventQueue*> eventQueues;
};

#endif

