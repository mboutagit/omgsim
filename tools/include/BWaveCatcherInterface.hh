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

    double getAbsoluteMaxAmplitude() const{
        if(!samples.size())
            return 0.;
        double max = abs(samples[0]);
        for(auto& sample : samples)
            if(abs(sample) > max)
                max = abs(sample);
        return max;
    }

    double getMaxAmplitude() const{
        if(!samples.size())
            return 0.;
        double max = samples[0];
        for(auto& sample : samples)
            if(sample > max)
                max = sample;
        return max;
    }

    double getMinAmplitude() const {
        if(!samples.size())
            return 0.;
        double min = samples[0];
        for(auto& sample : samples)
            if(sample < min)
                min = sample;
        return min;
    }

    double signalTimeF(float threshold, float timeStep) const{
        for(int i=0; i< samples.size(); i++)
            if(samples[i] <= threshold)
                return timeStep*i;
        return 0;
    }

    std::array<double, 2> signalTime(float threshold, double timeStep) const{
        bool first = false;
        bool second = false;
        double tmin = 0, tmax = 0;
        for(int i=0; i< samples.size(); i++){
            if(samples[i] <= threshold){
                if(! first){
                    tmin = timeStep*i;
                    first = true;
                }
            }
            if(samples[i] >= threshold && first){
                if(! second){
                    tmax = timeStep*i;
                    second = true;
                }
            }
                // std::cout << "\r\033[55C" << "samples: " << samples[i] << " - timeStep: " << timeStep << " - threshold: " << threshold << std::endl;
        }
        return {tmin, tmax};
    }

    double integrate(double timeStep, float tmin, float tmax) {
        if(!samples.size())
            return 0;
        double integral = 0;
        for(int s = 0; s < samples.size()-1; s++){
            if(s*timeStep >= tmin && s*timeStep <= tmax){
                double avg_v = 0.5 * (samples[s+1] + samples[s]); 
                integral += abs(avg_v) * timeStep;
            }
        }
        return integral;
    }

    double integrate(double timeStep) {
        if(!samples.size())
            return 0;
        double integral = 0;
        for(int s = 0; s < samples.size()-1; s++){
                double avg_v = 0.5 * (samples[s+1] + samples[s]); // mV
                integral += abs(avg_v) * timeStep; // mV ns
        }
        return integral;
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