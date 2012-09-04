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
    PatternManager(): _createPattern(this), _lastPatternNumber(0) {};
    ~PatternManager();

    void setSynthFactory(SafePtr<SynthFactory> factory) { _synth_factory = factory; }

    SIGNAL_DESTINATION(_createPattern, PatternManager, createPattern);

    void createPattern();
    void guiSignal();

  private:
    std::vector<Pattern*> _patterns;
    SafePtr<SynthFactory> _synth_factory;

    int _lastPatternNumber;
};

#endif

