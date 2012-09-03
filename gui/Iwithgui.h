#ifndef _INTERFACE_WITH_GUI_H__
#define _INTERFACE_WITH_GUI_H__

#include "common/signals.h"
#include "common/pointers.h"
#include "gui/Icontrol.h"

class IWithGui {
  public:
    virtual void setGuiParent(SafePtr<IControl> par) { _gui_parent = par; }
    virtual void unsetGuiParent() { _gui_parent.clear(); }
    Signal getGuiSignal() { return _guiSignal.getSignal(); }

    SIGNAL_DESTINATION(_guiSignal, IWithGui, guiSignal);

    virtual void guiSignal() = 0;

  protected:
    SafePtr<IControl> _gui;
    SafePtr<IControl> _gui_parent;
}

#endif

