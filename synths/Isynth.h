#ifndef _INTERFACE_SYNTH_H__
#define _INTERFACE_SYNTH_H__

#include "gui/Icontrol.h"
#include "common/signals.h"

class ISynth {
  public:
    ISynth(): _createGui(this) {}
    virtual ~ISynth() {}

    virtual void startNote(int noteId, float frequency) = 0;
    virtual void stopNote(int noteId) = 0;

    virtual void generateOutput() = 0;
    virtual float* getBuffer() = 0;

    virtual const char* getClassName() = 0;

    void setGuiParent(SafePtr<IControl> parent) { _gui_parent = parent; }
    SIGNAL_DESTINATION(_createGui, ISynth, createGui);
    virtual void createGui() = 0;
  protected:
    SafePtr<IControl> _gui_parent;
};

#endif

