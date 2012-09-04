#include "patterngui.h"
#include "song/pattern.h"
#include "common/pointers.h"
#include "gui/frame.h"
#include "gui/button.h"
#include "gui/slider.h"
#include "gui/matrix.h"
#include "gui/keyboard.h"
#include "gui/label.h"

PatternGui::PatternGui(SafePtr<IControl> parent, Pattern* pattern):
  _lowerSynth(this),
  _upperSynth(this),
  Window(parent, 0, 0, 300, 400, pattern->getName().c_str()), _pattern(pattern){
  
  SafePtr<IControl> frame1 = safe_new(Frame(safePtr(), 0, 0, 0, 0, 0));
  safe_new(Button(frame1, 0, 0, "<", _lowerSynth.getSignal()))->setPreferedSize(20, 0, 1);
  safe_new(Label(frame1, 0, 0, "Synth"))->setPreferedSize(0, 0, 1);
  safe_new(Button(frame1, 0, 0, ">", _upperSynth.getSignal()))->setPreferedSize(20, 0, 1);
  frame1->setPreferedSize(0, 20, 1);

  frame1->packHorizontally(2);

  SafePtr<IControl> frame2 = safe_new(Frame(safePtr(), 0, 0, 0, 0, 0));
  frame2->setPreferedSize(0, 0, 1);
  safe_new(Keyboard(frame2, 0, 0, 360))->setPreferedSize(80, 0, 1);
  safe_new(Matrix(frame2, 0, 0, 0, 0)).cast<Matrix>()->setPattern(pattern);
  SafePtr<IControl> frame3 = safe_new(Frame(frame2, 0, 0, 0, 0, 0));
  frame3->setPreferedSize(20, 0, 1);

  frame2->packHorizontally(2);

    safe_new(Button(frame3, 0, 0, "^"))->setPreferedSize(0, 20, 1);
    safe_new(Slider(frame3, 0, 0, 0, 0, 100))->setPreferedSize(0, 0, 1);
    safe_new(Button(frame3, 0, 0, "v"))->setPreferedSize(0, 20, 1);

    frame3->packVertically(2);

  packVertically(0);
}

void PatternGui::lowerSynth() {

}

void PatternGui::upperSynth() {

}

