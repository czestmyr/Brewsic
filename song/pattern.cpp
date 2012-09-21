#include "song/pattern.h"
#include "gui/song/patterngui.h"
#include <cmath>
#include <iostream>

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
  // Make the pattern longer if necessary
  if ((int)(ceil(end)) > _length) {
    _length = (int)(ceil(end));
  }

  //TODO: Copy older notes when splitting records

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

  // Create the new note
  _last_note_id++;
  SafePtr<Note> note = new Note(_last_note_id, begin, end, frequency);
  _notes.insert(note);

  // Add the note into all the records between begin and end times
  while (it_begin != it_end) {
    it_begin->second->push_back(note); // The notes probably don't have to be sorted due to polyphony restrictions
    ++it_begin;
  }

  // Add the note to the event queue
  _q.addNote(note);
}

SafePtr<Note> Pattern::getNote(float time, float frequency, int uTonality) {
  map<float, NoteRecord*>::iterator it = _data.upper_bound(time);
  if (it == _data.end()) return (Note*)NULL;
  it--;

  float minFreq = pow(2, -1.0/(12*uTonality)) * frequency;
  float maxFreq = pow(2,  1.0/(12*uTonality)) * frequency;

  std::cout << "MinFreq: " << minFreq << ", maxFreq: " << maxFreq << std::endl;

  NoteRecord& rec = *it->second;
  std::cout << "Rec time: " << it->first << std::endl;

  for (int i = 0; i < rec.size(); ++i) {
    if (minFreq <= rec[i]->_frequency && maxFreq >= rec[i]->_frequency) {
      return rec[i];
    }
  }

  return (Note*)NULL;
}

void Pattern::deleteNote(SafePtr<Note> note) {
  // Find the interval starting at begin time
  map<float, NoteRecord*>::iterator it_begin = _data.lower_bound(note->_begin);
  if (it_begin == _data.end() || it_begin->first != note->_begin) {
    std::cerr << "Something went wrong in note records..." << std::endl;
  }

  // Find the an interval starting at end time
  map<float, NoteRecord*>::iterator it_end = _data.lower_bound(note->_end);
  if (it_end == _data.end() || it_end->first != note->_end) {
    std::cerr << "Something went wrong in note records..." << std::endl;
  }

  it_end++;
  while (it_begin != it_end) {
    // Delete the note from the record
    NoteRecord& rec = *it_begin->second;
    NoteRecord::iterator rit = rec.begin();
    while (rit != rec.end()) {
      if (*rit == note) {
        rec.erase(rit);
        break;
      }
      ++rit;
    }

    // If the note record is no longer needed, remove it
    if (rec.size() == 0) {
      _data.erase(it_begin);
    }

    ++it_begin;
  }

  // Erase the note from the note set
  _notes.erase(note);

  // Erase the note from the event queue
  _q.removeNote(note);
}

void Pattern::setSynth(SafePtr<ISynth> synth) {
  _synth = synth;

  if (_gui) {
    _gui.cast<PatternGui>()->setSynth(synth);
  }
}

void Pattern::showGui() {
  if (_gui)
    _gui->deleteMe();

  _gui = safe_new(PatternGui(_gui_parent, this));
  _gui.cast<PatternGui>()->setSynth(_synth);
}

void Pattern::play() {
  _song_control->stop();
  _song_control->flushQueues();
  _song_control->addQueue(&_q);
  _song_control->play();
}

