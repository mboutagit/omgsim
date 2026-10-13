#pragma once
#include <iostream>
#include <vector>
#include <array>


namespace lactel
{
    struct Waveform {
        int channel;
        std::vector<float> samples; // float for negative & floating voltage values
        std::vector<float> time; // float for negative & floating voltage values
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
} // namespace lactel
