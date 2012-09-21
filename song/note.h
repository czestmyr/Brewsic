#ifndef _NOTE_H_
#define _NOTE_H_

class Note {
  public:
    Note(int id, float begin, float end, float frequency):
      _id(id), _begin(begin), _end(end), _frequency(frequency) {}

    int _id;
    float _begin; // Time is in beats
    float _end;   // Time is in beats
    float _frequency;
};

#endif

