#ifndef _MAINTEST_H_
#define _MAINTEST_H_

#include <SDL.h>
#include "common/signals.h"
#include "common/pointers.h"
#include "mixer/mainmixer.h"
#include "gui/guimgr.h"
#include "synths/synthfactory.h"
#include "song/patternmanager.h"
#include "song/songcontrol.h"

#define _FREQ 22050
#define _SAMPLES 1024
#define _CHANNELS 16

#define WIDTH 800
#define HEIGHT 600

class MainMixer;
class GuiMgr;
class SynthFactory;
class PatternManager;

/// Main test class that contains all the initialization and deinitialization stuff.
/// It is here so that main.cpp does not contain any unnecesarry stuff and also
/// for debugging purposes - so that correct memory deallocation can be checked from
/// the main.cpp file.
/// This class might change to something useful in the future but for now it just
/// shows the way the program's classes might be organized.
class MainTest {
  public:
    MainTest() {}
    ~MainTest() {}

    int init();
    void deinit();

    void mainLoop();
    static void audioCallback(void *userdata, Uint8 *stream, int len);

    ACTION(MainTest, quit);
    void quit() { _do_quit = true; }
  private:
    static MainTest* _instance;

    bool _do_quit;

    SafePtr<MainMixer> _mixer;
    SafePtr<GuiMgr> _gui_mgr;
    SafePtr<SynthFactory> _synth_factory;
    SafePtr<PatternManager> _pattern_manager;
    SafePtr<SongControl> _song_control;

    SDL_AudioSpec* _obtained_audio_format;
    SDL_Surface* _screen;
};

#endif

