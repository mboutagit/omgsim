#pragma once

#include "Waveform.hh"

namespace lactel
{
    class DAQEvt
    {
    private:
        /* data */
    public:
        DAQEvt(/* args */);
        ~DAQEvt();
    };
    
  
    uint channel;
    int om_id;
    // std::string om_id;
    float amp; // amplitude in mV
    float c; // charge in pC
    float t; // in ns
    Waveform waveform; // Event WaveForm

    DAQEvt(): channel(9999), amp(-9999), t(-9999), c(-9999), om_id(-1){}
    void reset(){
        channel = 9999; amp = -9999; t = -9999; c = -9999, om_id = -1;
    }

    DAQEvt(uint c, float _a, float _t){
        channel = c; amp = _a; t = _t;
    }
    void addEvent(uint c, float _a, float _t){
        channel = c; amp = _a; t = _t;
    }

} // namespace lactel
