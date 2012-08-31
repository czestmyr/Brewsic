#include "maintest.h"
#include "mixer/mainmixer.h"
#include "gui/guimgr.h"
#include "gui/background.h"
#include "gui/style.h"
#include "synths/synthfactory.h"

#include <iostream>

using namespace std;

MainTest* MainTest::_instance = NULL;

int MainTest::init() {
  // Set this instance as the main one that should respond to the sound processing
  _instance = this;

  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
    cerr << "Could not initialize SDL!" << endl;
    return -1;
  }

  // Audio initialization
  SDL_AudioSpec* desired = (SDL_AudioSpec*)malloc(sizeof(SDL_AudioSpec));
  _obtained_audio_format = (SDL_AudioSpec*)malloc(sizeof(SDL_AudioSpec));

  desired->freq = _FREQ;
  desired->format = AUDIO_S16SYS;
  desired->channels = 1;
  desired->samples = _SAMPLES*2;
  desired->callback = audioCallback;
  desired->userdata = NULL;

  if (SDL_OpenAudio(desired, _obtained_audio_format)) {
          cerr << "Could not open audio device!" << endl;
          SDL_Quit();
          return -1;
  }

  cout << "Frequency : " << _obtained_audio_format->freq << endl;
  cout << "Channels  : " << _obtained_audio_format->channels << endl;
  cout << "Samples   : " << _obtained_audio_format->samples << endl;

  free(desired);

  // SDL surface creation
  _screen = SDL_SetVideoMode(WIDTH, HEIGHT, 32, 0);

  // Gui creation
  _gui_mgr = new GuiMgr();
  SafePtr<IControl> gui_bg = safe_new(Background((IControl*)NULL, 4, 4, WIDTH-8, HEIGHT-8));
  _gui_mgr->adoptControl(gui_bg);

  // Main classes creation
  _mixer = new MainMixer(_SAMPLES, _CHANNELS);
  _synth_factory = new SynthFactory(_SAMPLES);

  return 0;
}

void MainTest::deinit() {
  delete _synth_factory;
  delete _mixer;
  delete _gui_mgr;

  SDL_CloseAudio();
  free(_obtained_audio_format);
  SDL_Quit();
}

void MainTest::mainLoop() {
  // Timing
  Uint32 time = SDL_GetTicks();
  Uint32 dt = 0;

  // Start sound TODO: Give control of this to user (and connect it to song controls)
  SDL_PauseAudio(0);

  // Main event loop
  SDL_Event e;
  bool do_quit = false;
  while (!do_quit) {
    SDL_Delay(50);

    while (SDL_PollEvent(&e)) {
      switch (e.type) {
        case SDL_MOUSEBUTTONDOWN:
          if (e.button.button == SDL_BUTTON_LEFT) {
            _gui_mgr->leftPress(e.button.x, e.button.y);
          } else if (e.button.button == SDL_BUTTON_RIGHT) {
            _gui_mgr->rightPress(e.button.x, e.button.y);
          }
        break;
        case SDL_MOUSEBUTTONUP:
          if (e.button.button == SDL_BUTTON_LEFT) {
            _gui_mgr->leftRelease(e.button.x, e.button.y);
          } else if (e.button.button == SDL_BUTTON_RIGHT) {
            _gui_mgr->rightRelease(e.button.x, e.button.y);
          }
        break;
        case SDL_MOUSEMOTION:
          _gui_mgr->mouseMove(e.motion.x, e.motion.y, e.motion.xrel, e.motion.yrel);
        break;
        case SDL_KEYDOWN:
          _gui_mgr->keyPress(e.key.keysym.sym);
        break;
        case SDL_KEYUP:
          _gui_mgr->keyRelease(e.key.keysym.sym);
        break;
        case SDL_QUIT:
          do_quit = true;
        break;
      }
    }

    // Timing
    dt = SDL_GetTicks() - time;
    time += dt;

    // Screen background
    SDL_Color shadecol = Style::inst()->getShadeColor();
    Uint32 shade = SDL_MapRGB(_screen->format, shadecol.r, shadecol.g, shadecol.b);
    SDL_FillRect(_screen, NULL, shade);

    // Gui drawing
    _gui_mgr->cleanup();
    _gui_mgr->draw(_screen);

    SDL_Flip(_screen);
  }
}

void MainTest::audioCallback(void *userdata, Uint8 *stream, int len) {
  if (_instance)
    _instance->_mixer->mixInto((Sint16*)stream);
}

