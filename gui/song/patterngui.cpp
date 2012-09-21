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
  Window(parent, 0, 0, 300, 400, pattern->getName().c_str()), _pattern(pattern){
  
  SafePtr<IControl> frame1 = safe_new(Frame(safePtr(), 0, 0, 0, 0, 0));
  safe_new(Button(frame1, 0, 0, "<", lowerSynthAction()))->setPreferedSize(20, 0, 1);
  safe_new(Label(frame1, 0, 0, "--- no synth ---"))->setPreferedSize(0, 0, 1);
  safe_new(Button(frame1, 0, 0, ">", upperSynthAction()))->setPreferedSize(20, 0, 1);
  frame1->setPreferedSize(0, 20, 1);

  frame1->packHorizontally(2);

  SafePtr<IControl> frame2 = safe_new(Frame(safePtr(), 0, 0, 0, 0, 0));
  frame2->setPreferedSize(0, 0, 1);
  _keyboard = safe_new(Keyboard(frame2, 0, 0, 360)).cast<Keyboard>();
  _keyboard->setPreferedSize(80, 0, 1);
  _matrix = safe_new(Matrix(frame2, 0, 0, 0, 0)).cast<Matrix>();
  _matrix->setPattern(pattern);
  SafePtr<IControl> frame3 = safe_new(Frame(frame2, 0, 0, 0, 0, 0));
  frame3->setPreferedSize(20, 0, 1);

  frame2->packHorizontally(2);

    SafePtr<Button> button1 = safe_new(Button(frame3, 0, 0, "^")).cast<Button>();
    button1->setPreferedSize(0, 20, 1);
    SafePtr<Slider> slider = safe_new(Slider(frame3, 0, 0, 0, 0, 1, &_shift)).cast<Slider>();
    slider->setInverted();
    slider->setPreferedSize(0, 0, 1);
    SafePtr<Button> button2 = safe_new(Button(frame3, 0, 0, "v")).cast<Button>();
    button2->setPreferedSize(0, 20, 1);

    button1->addAction(slider->upAction());
    button2->addAction(slider->downAction());

    frame3->packVertically(2);

  packVertically(0);

  // Bind change of _shift control variable to the real shift changes
  _shift.addActions(shiftChangedAction());

  _shift = 0.5;
}

void PatternGui::setSynth(SafePtr<ISynth> synth) {
  _keyboard->setSynth(synth);
}

void PatternGui::lowerSynth() {

}

void PatternGui::upperSynth() {

}

#define MAX_SHIFT 1800
#define MIN_SHIFT 0
void PatternGui::shiftChanged() {
  _real_shift = (int)((MAX_SHIFT - MIN_SHIFT) * _shift);
  _keyboard->setShift(_real_shift);
  _matrix->setShift(_real_shift);
}

