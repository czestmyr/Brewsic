#ifndef _PATTERN_GUI_H_
#define _PATTERN_GUI_H_

#include "gui/window.h"
#include "common/signals.h"
#include "common/pointers.h"
#include "gui/keyboard.h"
#include "gui/matrix.h"

class Pattern;

class PatternGui: public Window {
  public:
    PatternGui(SafePtr<IControl> parent, Pattern* pattern);

    const char* controlClassName() { return "Pattern GUI"; }

    void setSynth(SafePtr<ISynth> synth);

  private:
    ACTION(PatternGui, lowerSynth);
    ACTION(PatternGui, upperSynth);
    ACTION(PatternGui, shiftChanged);
    void lowerSynth();
    void upperSynth();
    void shiftChanged();

    Property<float> _shift;
    int _real_shift;

    Pattern* _pattern;
    SafePtr<Keyboard> _keyboard;
    SafePtr<Matrix> _matrix;
};

#endif

