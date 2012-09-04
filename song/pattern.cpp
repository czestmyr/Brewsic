#include "song/pattern.h"
#include "gui/song/patterngui.h"

/*typedef std::vector<SafePtr<Note> > NoteRecord;

class Pattern {
  public:
    Pattern() {}
    ~Pattern() {}

  private:
    std::map<float, NoteRecord> _data;
};*/

using namespace std;

Pattern::~Pattern() {
  map<float, NoteRecord*>::iterator it = _data.begin();
  while (it != _data.end()) {
    delete it->second;
    ++it;
  }
}

void Pattern::newNote(float begin, float end, float frequency) {
  // Find (or create) an interval starting at begin time
  map<float, NoteRecord*>::iterator it_begin = _data.lower_bound(begin);
  if (it_begin == _data.end() || it_begin->first != begin) {
    it_begin = _data.insert(make_pair(begin, new NoteRecord())).first;
  }

  // Find (or create) an interval starting at end time
  map<float, NoteRecord*>::iterator it_end = _data.lower_bound(end);
  if (it_end == _data.end() || it_end->first != end) {
    it_end = _data.insert(make_pair(end, new NoteRecord())).first;
  }

  SafePtr<Note> note = new Note(begin, end, frequency);
  _notes.insert(note);

  // Add the note into all the records between begin and end times
  while (it_begin != it_end) {
    it_begin->second->push_back(note); // The notes probably don't have to be sorted due to polyphony restrictions
    ++it_begin;
  }

  // TODO: Update event queue?
}

SafePtr<Note> Pattern::getNote(float time, float frequency, int uTonality) {

}

void Pattern::deleteNote(SafePtr<Note> note) {

}

void Pattern::guiSignal() {
  if (_gui)
    _gui->deleteMe();

  _gui = safe_new(PatternGui(_gui_parent, this));
}

