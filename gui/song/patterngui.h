#ifndef _PATTERN_GUI_H_
#define _PATTERN_GUI_H_

#include "gui/window.h"
#include "common/signals.h"
#include "common/pointers.h"
#include "gui/keyboard.h"

class Pattern;

class PatternGui: public Window {
  public:
    PatternGui(SafePtr<IControl> parent, Pattern* pattern);

    const char* controlClassName() { return "Pattern GUI"; }

    void setSynth(SafePtr<ISynth> synth);
  private:
    SIGNAL_DESTINATION(_lowerSynth, PatternGui, lowerSynth);
    SIGNAL_DESTINATION(_upperSynth, PatternGui, upperSynth);
    void lowerSynth();
    void upperSynth();

    Pattern* _pattern;
    SafePtr<Keyboard> keyboard;
};

#endif

