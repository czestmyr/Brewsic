#ifndef _PATTERN_MANAGER_H_
#define _PATTERN_MANAGER_H_

#include <vector>
#include "gui/Iwithgui.h"
#include "common/signals.h"

class Pattern;

class PatternManager: public IWithGui {
  public:
    PatternManager(): _createPattern(this), _lastPatternNumber(0) {};
    ~PatternManager();

    SIGNAL_DESTINATION(_createPattern, PatternManager, createPattern);

    void createPattern();
    void guiSignal();

  private:
    std::vector<Pattern*> _patterns;

    int _lastPatternNumber;
};

#endif

