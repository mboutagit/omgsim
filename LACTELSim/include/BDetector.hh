#ifndef BDETECTOR_HH
#define BDETECTOR_HH

#include <iostream>
#include "GeometryObject.hh"
#include "GeometryGrid.hh"

using namespace std;


class BDetector
{
private:
    /* data */
public:
    BDetector(/* args */);
    ~BDetector();

    virtual void construct() = 0;

    std::string name = "default_detector";
    std::string description = "Default detector description";

    ClassDef(BDetector, 0);
};

#endif