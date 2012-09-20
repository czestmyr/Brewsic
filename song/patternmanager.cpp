#include "patternmanager.h"

#include <string>
#include <sstream>
#include "song/pattern.h"
#include "gui/button.h"
#include "gui/frame.h"
#include "gui/window.h"

using namespace std;

PatternManager::~PatternManager() {
  for (int i = 0; i < _patterns.size(); ++i) {
    delete _patterns[i];
  }
}

void PatternManager::createPattern() {
  ++_lastPatternNumber;
  stringstream patternName;
  patternName << "Pattern " << _lastPatternNumber;

  Pattern* newPattern = new Pattern(patternName.str());
  newPattern->setGuiParent(_gui_parent);
  newPattern->setSynth(_synth_factory->getSynth(0));
  _patterns.push_back(newPattern);
  showGui();
}

void PatternManager::showGui() {
  if (_gui)
    _gui->deleteMe();

  int windowHeight = 73 + 25*_patterns.size();

  _gui = safe_new(Window(_gui_parent, 0, 0, 200, windowHeight, "Patterns"));

  safe_new(Button(_gui, 0, 0, "New pattern", createPatternAction()))->setPreferedSize(0, 20, 1);
  safe_new(Frame(_gui, 0, 0, 0, 0))->setPreferedSize(0, 2, 1);

  for (int i = 0; i < _patterns.size(); ++i) {
    safe_new(Button(_gui, 0, 0, _patterns[i]->getName().c_str(), _patterns[i]->showGuiAction()))->setPreferedSize(0, 0, 1);
  }

  _gui->packVertically(5);
}

