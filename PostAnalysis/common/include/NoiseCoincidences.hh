#ifndef NoiseCoincidences_h
#define NoiseCoincidences_h

#include <K40TreeReader.hh>

class NoiseCoincidences : public K40TreeReader
{
public:
  NoiseCoincidences (int, char**);
  unsigned Loop ();
  void DoIt ();
};

#endif
