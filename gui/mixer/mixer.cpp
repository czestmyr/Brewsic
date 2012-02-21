#include "gui/mixer/mixer.h"
#include "mixer/mixer.h"
#include "gui/slider.h"
#include "gui/label.h"

MixerGui::MixerGui(SafePtr<IControl> parent, MainMixer* mixer)
: Window(parent, 0, 0, 600, 300, "Main mixer"), _mixer(mixer) {
	safe_new(Label(safePtr(), 0, 20, "Master"));
	safe_new(Slider(safePtr(), 0, 40, 280, 0, 100, NULL, 50));
}

