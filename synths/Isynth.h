#ifndef _INTERFACE_SYNTH_H__
#define _INTERFACE_SYNTH_H__

#include "gui/Icontrol.h"
#include "gui/Iwithgui.h"
#include "common/signals.h"
#include "common/property.h"

#include <string>

class ISynth: public IWithGui {
  public:
    ISynth() {}
    virtual ~ISynth() {}

    virtual void startNote(int noteId, float frequency) = 0;
    virtual void stopNote(int noteId) = 0;
    virtual void stopAllNotes() = 0;

    virtual void generateOutput() = 0;
    virtual float* getBuffer() = 0;

    virtual const char* getClassName() = 0;
    const std::string& getSynthName() { return _synth_name; }

  protected:
    void constructSynthName();

    Property<std::string> _synth_name;
    static int _lastSerialNumber;
};

#endif

