#include <SDL.h>

#include "maintest.h"
#include "gui/Icontrol.h"

using namespace std;

MainTest test;

/*bool do_quit(false);
SafePtr<Keyboard> kbd;
SafePtr<Matrix> mtrx;

class MainSignals {
	public:
		MainSignals(): _quit(this), _kbdUp(this), _kbdDown(this) {}

		SIGNAL_DESTINATION(_quit, MainSignals, quit);
		SIGNAL_DESTINATION(_kbdUp, MainSignals, kbdUp);
		SIGNAL_DESTINATION(_kbdDown, MainSignals, kbdDown);
	
		void quit() { do_quit = true; }
		void kbdUp() {
			int shift = kbd->getShift() - 20;
			if (shift < 0) shift = 0;
			kbd->setShift(shift);
			mtrx->setShift(shift);
		}
		void kbdDown() {
			int shift = kbd->getShift() + 20;
			kbd->setShift(shift);
			mtrx->setShift(shift);
		}
};
MainSignals msig;*/

int main(int argc, char* argv[]) {
  int initResult = test.init();
  if (initResult) return initResult;
  test.mainLoop();
  test.deinit();

  /*
        SafePtr<TripleOscillator> osc;

	// Gui setup
	GuiMgr* gui = new GuiMgr();
	SafePtr<IControl> gui_bg = safe_new(Background((IControl*)NULL, 4, 4, WIDTH-8, HEIGHT-8));
	gui->adoptControl(gui_bg);

        // Synth factory test
	osc = synthFactory.createNewSynth("TripleOscillator").cast<TripleOscillator>();
	osc->setGuiParent(gui_bg);

	// Mixer
	mmix.setGuiParent(gui_bg);
	SynthQueue* sq = mmix.getSynthQueue(0);
	sq->setGuiParent(gui_bg);
	sq->pushSynth(osc.cast<ISynth>());

	safe_new(Button(gui_bg, 600, 500, "Synth Queue 1 Gui", sq->_guiSignal.getSignal()));

        // TEST WINDOWS:
        safe_new(Window(gui_bg, 50, 50, 400, 200, "Test window"));

	// Keyboard and matrix test
	kbd = safe_new(Keyboard(gui_bg, 50, 350, 200)).cast<Keyboard>();
	kbd->setSynth(osc.cast<ISynth>());
	mtrx = safe_new(Matrix(gui_bg, 130, 350, 400, 200)).cast<Matrix>();

	safe_new(Button(gui_bg, WIDTH - 250, 50, "Up", msig._kbdUp.getSignal()));
	safe_new(Button(gui_bg, WIDTH - 250, 75, "Down", msig._kbdDown.getSignal()));

	// Sequencer
        int seq_size = 8;
	int seq_length = 500;
	int seq_ind = 0;
	int seq_dur = 0;
	Property<bool> sequencer[seq_size];
	Property<float> seq_freq[seq_size];
	for (int i = 0; i < seq_size; ++i) {
		safe_new(Checkbox(gui_bg, 50 + i*30, 50, 20, 20, &sequencer[i]));
		safe_new(Slider(gui_bg, 50 + i*30, 75, 200, 100.0, 2000.0, &seq_freq[i]));
	}
	// Packing test
	SafePtr<IControl> frame = safe_new(Frame(gui_bg, 500, 100, 100, 600));
	SafePtr<IControl> b1 = safe_new(Button(frame, 0, 0, "1"));
	SafePtr<IControl> b2 = safe_new(Button(frame, 0, 0, "2"));
	SafePtr<IControl> b3 = safe_new(Button(frame, 0, 0, "3"));
	SafePtr<IControl> b4 = safe_new(Button(frame, 0, 0, "4"));
	SafePtr<IControl> b5 = safe_new(Button(frame, 0, 0, "5"));
	SafePtr<IControl> b6 = safe_new(Button(frame, 0, 0, "6"));

	b1->setPreferedSize(0, 10, 4);
	b2->setPreferedSize(0, 20, 1);
	b3->setPreferedSize(15, 30, 1);
	b4->setPreferedSize(0, 40, 4);
	b5->setPreferedSize(0, 50, 1);
	b6->setPreferedSize(15, 0, 1);

	frame->packVertically(5);
*/
        IControl::dumpRegisteredControls();

	return 0;
}

