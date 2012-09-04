#ifndef _PATTERN_GUI_H_
#define _PATTERN_GUI_H_

#include "gui/window.h"

class Pattern;

class PatternGui: public Window {
  public:
    PatternGui(SafePtr<IControl> parent, Pattern* pattern);

    const char* controlClassName() { return "Pattern GUI"; }
  private:
    Pattern* _pattern;
};

#endif

