#include "triosc.h"

#include "generators/sine.h"
#include "generators/saw.h"
#include "generators/square.h"
#include "gui/slider.h"
#include "gui/wheel.h"
#include "gui/pselect.h"
#include "gui/label.h"
#include "gui/frame.h"
#include "synths/triosc.h"

TripleOscillatorGui::TripleOscillatorGui(SafePtr<IControl> parent, TripleOscillator* osc):
	_osc(osc),
	Window(parent, 0, 0, 240, 260, osc->getSynthName().c_str()) {

	SafePtr<IControl> slider = safe_new(Slider(safePtr(), 0, 0, 0, -400, 600, &osc->_shift));
	SafePtr<IControl> frame = safe_new(Frame(safePtr(), 0, 0, 0, 0, 0));

        frame->adopt(osc->_adsr[0]->getGui());

	SafePtr<IControl> label1 = safe_new(Label(frame, 0, 0, "First oscillator"));

	SafePtr<IControl> frame3 = safe_new(Frame(frame, 0, 0, 0, 0, 0));
	SafePtr<PictureSelector> psel1 = safe_new(PictureSelector(frame3, 0, 0, 32, 32, &osc->_first_gen)).cast<PictureSelector>();
	SafePtr<IControl> exp = safe_new(IControl(frame3));  // expander to make pselector go left

	frame3->setPreferedSize(0, 34, 1);
	frame3->packHorizontally(5);

	SafePtr<IControl> label2 = safe_new(Label(frame, 0, 0, "Second oscillator"));

	SafePtr<IControl> frame1 = safe_new(Frame(frame, 0, 0, 0, 0, 0));
	SafePtr<PictureSelector> psel2 = safe_new(PictureSelector(frame1, 0, 0, 32, 32, &osc->_second_gen)).cast<PictureSelector>();
	SafePtr<IControl> wheel1_label = safe_new(Label(frame1, 0, 0, "Detune osc2"));
	SafePtr<IControl> wheel1 = safe_new(Wheel(frame1, 30, 5, 20, 20, -100.0, 100.0, &_osc->_first));
	wheel1_label->setPreferedSize(100, 20, 1);

	frame1->setPreferedSize(0, 34, 1);
	frame1->packHorizontally(5);

	SafePtr<IControl> label3 = safe_new(Label(frame, 0, 0, "Third oscillator"));

	SafePtr<IControl> frame2 = safe_new(Frame(frame, 0, 0, 0, 0, 0));
	SafePtr<PictureSelector> psel3 = safe_new(PictureSelector(frame2, 0, 0, 32, 32, &osc->_third_gen)).cast<PictureSelector>();
	SafePtr<IControl> wheel2_label = safe_new(Label(frame2, 0, 0, "Detune osc3"));
	SafePtr<IControl> wheel2 = safe_new(Wheel(frame2, 30, 5, 20, 20, -100.0, 100.0, &_osc->_second));
	wheel2_label->setPreferedSize(100, 20, 1);

	frame2->setPreferedSize(0, 34, 1);
	frame2->packHorizontally(5);

	frame->packVertically(5);

	packHorizontally(5);

	// Add generator images to picture selectors
	for (int i = 0; i < (int)GEN_NUMBER; ++i) {
		const char* filename = GeneratorFactory::inst()->getGeneratorPictureFilename((GeneratorType)i);
		psel1->addPicture(filename);
		psel2->addPicture(filename);
		psel3->addPicture(filename);
	}
}
