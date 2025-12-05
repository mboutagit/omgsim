#ifndef BWaveCatcherInterface_H
#define BWaveCatcherInterface_H
#include "BWaveCatcherConf.hh"
#include <vector>

using namespace lactel;

struct Waveform {
    int channel;
    std::vector<float> samples; // float for negative & floating voltage values
    double timestamp;

    bool operator<(const Waveform& wf) const {return timestamp < wf.timestamp;}
    bool operator>(const Waveform& wf) const {return timestamp > wf.timestamp;}
    bool operator==(const Waveform& wf) const {return timestamp == wf.timestamp & channel == wf.channel;}

    double getAbsoluteMaxAmplitude(){
        if(!samples.size())
            return 0.;
        double max = abs(samples[0]);
        for(auto& sample : samples)
            if(abs(sample) > max)
                max = sample;
        return max;
    }

    double getMaxAmplitude(){
        if(!samples.size())
            return 0.;
        double max = samples[0];
        for(auto& sample : samples)
            if(sample > max)
                max = sample;
        return max;
    }

    double getMinAmplitude(){
        if(!samples.size())
            return 0.;
        double min = samples[0];
        for(auto& sample : samples)
            if(sample < min)
                min = sample;
        return min;
    }
};

struct ADC_record {
    int channel;
    std::vector<signed int> samples; // from 0 to bit resolution
    double timestamp;

    bool operator<(const ADC_record& adc) const {return timestamp < adc.timestamp;}
    bool operator>(const ADC_record& adc) const {return timestamp > adc.timestamp;}
    bool operator==(const ADC_record& adc) const {return timestamp == adc.timestamp & channel == adc.channel;}
};




class BWaveCatcherInterface {
    public:
        virtual ~BWaveCatcherInterface() = default;

        virtual bool initialize() = 0;
        virtual bool applyConfig(const BWaveCatcherConf& config) = 0;

        virtual bool startAcquisition() = 0;
        virtual bool stopAcquisition() = 0;
        virtual bool isAcquiring() const = 0;

        virtual bool readEventData() = 0;
        virtual std::vector<Waveform> getWaveforms() const = 0;

        virtual bool forceSoftwareTrigger() = 0;
        virtual std::string getStatus() const = 0;
        virtual void reset() = 0;
};

#endif // BWaveCatcherInterface_H