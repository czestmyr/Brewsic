#include "song/pattern.h"
#include "gui/window.h"
#include "gui/matrix.h"
#include "gui/keyboard.h"
#include "gui/frame.h"
#include "gui/slider.h"

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

  _gui = safe_new(Window(_gui_parent, 0, 0, 400, 400, _name.c_str()));

  SafePtr<IControl> frame1 = safe_new(Frame(_gui, 0, 0, 0, 0, 0));
  frame1->setPreferedSize(0, 20, 1);
  SafePtr<IControl> frame2 = safe_new(Frame(_gui, 0, 0, 0, 0, 0));
  frame2->setPreferedSize(0, 0, 1);

  safe_new(Keyboard(frame2, 0, 0, 360))->setPreferedSize(80, 0, 1);
  safe_new(Matrix(frame2, 0, 0, 0, 0)).cast<Matrix>()->setPattern(this);

  SafePtr<IControl> frame3 = safe_new(Frame(frame2, 0, 0, 0, 0, 0));
  frame3->setPreferedSize(20, 0, 1);

  safe_new(Button(frame3, 0, 0, "^"))->setPreferedSize(0, 20, 1);
  safe_new(Slider(frame3, 0, 0, 0, 0, 100))->setPreferedSize(0, 0, 1);
  safe_new(Button(frame3, 0, 0, "v"))->setPreferedSize(0, 20, 1);

  frame1->packHorizontally(2);
  frame2->packHorizontally(2);
  frame3->packVertically(2);
  _gui->packVertically(0);
}

