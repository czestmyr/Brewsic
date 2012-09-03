#ifndef _PATTERN_H_
#define _PATTERN_H_

#include <map>
#include <vector>
#include <string>
#include "song/note.h"
#include "common/pointers.h"
#include "common/signals.h"
#include "gui/Iwithgui.h"

typedef std::vector<SafePtr<Note> > NoteRecord;

class Pattern: public IWithGui {
  public:
    Pattern(const std::string& name): _name(name) {}
    ~Pattern();

    // Begin and end times of a note are in milliseconds in the pattern.
    // Frequency of a note in Hz
    void newNote(float begin, float end, float frequency);

    // Microtonality defines frequency tolerance: 1 means conventional tone (i.e. 1/12 of an octave), 2 is 1/2 of a conventional tone, etc...
    SafePtr<Note> getNote(float time, float frequency, int uTonality = 1);
    void deleteNote(SafePtr<Note> note);

    void guiSignal();

    const std::string& getName() const { return _name; }

  private:
    std::map<float, NoteRecord*> _data;

    std::string _name;
};

#endif

