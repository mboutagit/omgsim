#ifdef __ROOTCLING__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class std::string+;

#pragma link C++ namespace lactel;
#pragma link C++ class lactel::BAnalysisManager+;
#pragma link C++ class lactel::BAnalysisManager::MeasureParameters+;
#pragma link C++ class lactel::BAnalysisManager::Measure+;
#pragma link C++ class lactel::BAnalysisManager::Hit+;

#pragma link C++ class Waveform+;
#pragma link C++ class lactel::BAnalysisManager::DAQEvent+;
#pragma link C++ class std::vector<lactel::BAnalysisManager::DAQEvent>+;


#pragma link C++ class GeometryGrid+;
#pragma link C++ class GeometryObject+;
#pragma link C++ class BDetector+;
#pragma link C++ class lactel::BLactelDetector+;
#pragma link C++ class lactel::Module+;
#pragma link C++ class lactel::ElementaryCell+;
#pragma link C++ class GeometryGrid<lactel::ElementaryCell>+;
#pragma link C++ class GeometryGrid<lactel::Module>+;
#pragma link C++ class GeometryGrid<lactel::BLactelDetector>+;
#pragma link C++ class CLHEP::Hep3Vector+;

#endif