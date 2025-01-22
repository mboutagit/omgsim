#include "NoiseCoincidences.hh"
#include "RunInfo.hh"

int main (int argc, char** argv)
{
  NoiseCoincidences nc(argc, argv);
  nc.DoIt ();
}
