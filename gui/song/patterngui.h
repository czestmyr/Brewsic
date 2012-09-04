#ifndef _PATTERN_GUI_H_
#define _PATTERN_GUI_H_

#include "gui/window.h"
#include "common/signals.h"

class Pattern;

class PatternGui: public Window {
  public:
    PatternGui(SafePtr<IControl> parent, Pattern* pattern);

    const char* controlClassName() { return "Pattern GUI"; }

  private:
    SIGNAL_DESTINATION(_lowerSynth, PatternGui, lowerSynth);
    SIGNAL_DESTINATION(_upperSynth, PatternGui, upperSynth);
    void lowerSynth();
    void upperSynth();

    Pattern* _pattern;
};

#endif

