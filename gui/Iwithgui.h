#ifndef _INTERFACE_WITH_GUI_H__
#define _INTERFACE_WITH_GUI_H__

#include "common/signals.h"
#include "common/pointers.h"
#include "gui/Icontrol.h"

class IWithGui {
  public:
    IWithGui() {}

    virtual void setGuiParent(SafePtr<IControl> par) { _gui_parent = par; }
    virtual void unsetGuiParent() { _gui_parent.clear(); }

    ACTION(IWithGui, showGui);
    virtual void showGui() = 0;

    SafePtr<IControl> getGui() { return _gui; }

  protected:
    SafePtr<IControl> _gui;
    SafePtr<IControl> _gui_parent;
};

#endif

