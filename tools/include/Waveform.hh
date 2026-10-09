#pragma once

#include "TROOT.h"
#include <Rtypes.h>
#include <iostream>
#include <vector>
#include <array>
#include <cmath>
#include <limits>
#include <algorithm>

using namespace std;

enum class PulsePolarity {
    Positive = +1,
    Negative = -1
};

struct ADC_record {
    int channel;
    std::vector<signed int> samples; // from 0 to bit resolution
    double timestamp;
    double tStart;

    bool operator<(const ADC_record& adc) const {return timestamp < adc.timestamp;}
    bool operator>(const ADC_record& adc) const {return timestamp > adc.timestamp;}
    bool operator==(const ADC_record& adc) const {return timestamp == adc.timestamp & channel == adc.channel;}
};

class Waveform
{
private:

public:
    int channel = -1;

    // Reconstructed waveform
    std::vector<float> samples;   // mV
    double timeStep = 1;      // ns

    // Timing
    double timestamp = 0.0;       // waveform/event timestamp
    double peakTime = 0.0;        // ns
    double leadingEdge = 0.0;     // ns
    double trailingEdge = 0.0;    // ns
    double tot = 0.0;             // ns

    // Baseline
    double baseline = 0.0;        // mV
    double baselineRMS = 0.0;     // mV

    // Amplitude
    double peak = 0.0;            // signed mV
    double peakAmplitude = 0.0;   // positive magnitude, mV
    double minimum = 0.0;         // mV
    double maximum = 0.0;         // mV

    // Shape
    double riseTime = 0.0;        // ns, 10 -> 90 %
    double fallTime = 0.0;        // ns, 90 -> 10 %

    // Integral
    double area = 0.0;            // mV*ns
    double charge = 0.0;          // pC
    bool triggered = false;

    std::size_t nSamples = 0;

public:
    Waveform(/* args */);
    ~Waveform();

    bool operator<(const Waveform& wf) const {return timestamp < wf.timestamp;}
    bool operator>(const Waveform& wf) const {return timestamp > wf.timestamp;}
    bool operator==(const Waveform& wf) const {return timestamp == wf.timestamp & channel == wf.channel;}

    void extractBaseline(std::size_t baselineSamples);
    void extractPeak(PulsePolarity polarity);
    double interpolateCrossingTime(double t1, double y1, double t2, double y2,double threshold);
    void extractEdges(double threshold, PulsePolarity polarity);
    void extractRiseFallTime(PulsePolarity polarity);
    void extractAreaAndCharge(double impedance);
    void extractWaveformParameters(double threshold, PulsePolarity polarity, double impedance = 50.0, std::size_t baselineSamples = 100);


    double getAbsoluteMaxAmplitude() const; 
    double getMaxAmplitude() const;
    double getMinAmplitude() const;
    double signalTimeF(float threshold, float timeStep) const;
    std::array<double, 2> signalTime(float threshold, double timeStep) const;
    double integrate(double timeStep, float tmin, float tmax);
    double integrate(double timeStep);
    void print(std::ostream& out = std::cout);

    void setTimeStep(double ts);
    double getTimeStep() const;

    ClassDef(Waveform, 2);
};

