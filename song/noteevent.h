#ifndef _NOTE_EVENT_H_
#define _NOTE_EVENT_H_

#include "common/pointers.h"
#include "song/note.h"

class NoteEvent {
  public:
    NoteEvent(SafePtr<Note> note, bool begin): _note(note) {}

    SafePtr<Note> _note;
    bool _begin; 
};

#endif

