#ifndef _NOTE_H_
#define _NOTE_H_

class Note {
  public:
    Note(float begin, float end, float frequency):
      _begin(begin), _end(end), _frequency(frequency) {}

    float _begin;
    float _end;
    float _frequency;
};

#endif

