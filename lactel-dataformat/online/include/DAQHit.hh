#pragma once 

#include "Hit.hh"

class DAQHit : public Hit
{
    private:
        float time = 0;
        float charge = 0;
    public:
        DAQHit(/* args */);
        ~DAQHit();
};

DAQHit::DAQHit(/* args */)
{
}

DAQHit::~DAQHit()
{
}
