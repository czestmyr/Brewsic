#include "gui/mixer/mixer.h"
#include "mixer/mainmixer.h"
#include "gui/slider.h"
#include "gui/label.h"
#include "gui/frame.h"
#include <cstring>

MixerGui::MixerGui(SafePtr<IControl> parent, MainMixer* mixer)
: Window(parent, 0, 0, 600, 300, "Main mixer"), _mixer(mixer) {
  SafePtr<IControl> hframe = safe_new(Frame(safePtr(), 0, 0, 600, 300, 0));

  // Main volume controls
  SafePtr<IControl> frame = safe_new(Frame(hframe, 0, 0, 20, 300, 0));
  SafePtr<Label> header  = safe_new(Label(frame, 0, 20, "M")).cast<Label>();
  SafePtr<Slider> slider = safe_new(Slider(frame, 0, 40, 280, 0.0, 1.5, &_mixer->getMasterVolume()->_vol, 50)).cast<Slider>();

  header->setPreferedSize(0, 14, 1);
  slider->setPreferedSize(0, 0, 1);

  frame->packVertically(5);
  frame->setPreferedSize(0, 0, 1);

  for (int i = 0; i < _mixer->getChannels(); ++i) {
    char buffer[16];
    sprintf(buffer, "Ch.%i", i+1);

    SafePtr<IControl> frame = safe_new(Frame(hframe, 0, 0, 20, 300, 0));
    SafePtr<Label> header  = safe_new(Label(frame, 0, 20, buffer)).cast<Label>();
    SafePtr<Slider> slider = safe_new(Slider(frame, 0, 40, 280, 0.0, 1.5, &_mixer->getVolume(i)->_vol, 50)).cast<Slider>();

    header->setPreferedSize(0, 14, 1);
    slider->setPreferedSize(0, 0, 1);

    frame->packVertically(5);
    frame->setPreferedSize(0, 0, 1);
  }

  hframe->packHorizontally(5);
}

