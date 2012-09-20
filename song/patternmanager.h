#ifndef _PATTERN_MANAGER_H_
#define _PATTERN_MANAGER_H_

#include <vector>
#include "gui/Iwithgui.h"
#include "common/signals.h"
#include "common/pointers.h"
#include "synths/synthfactory.h"

class Pattern;

class PatternManager: public IWithGui {
  public:
    PatternManager(): _lastPatternNumber(0) {};
    ~PatternManager();

    void setSynthFactory(SafePtr<SynthFactory> factory) { _synth_factory = factory; }

    ACTION(PatternManager, createPattern);
    void createPattern();
    void showGui();

  private:
    std::vector<Pattern*> _patterns;
    SafePtr<SynthFactory> _synth_factory;

    int _lastPatternNumber;
};

#endif

