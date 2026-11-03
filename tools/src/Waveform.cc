#include "../include/Waveform.hh"

ClassImp(Waveform);

Waveform::Waveform(){}
Waveform::~Waveform(){}

double Waveform::getAbsoluteMaxAmplitude() const{
    if(!samples.size())
        return 0.;
    double max = abs(samples[0]);
    for(auto& sample : samples)
        if(abs(sample) > max)
            max = abs(sample);
    return max;
}

double Waveform::getMaxAmplitude() const{
    if(!samples.size())
        return 0.;
    double max = samples[0];
    for(auto& sample : samples)
        if(sample > max)
            max = sample;
    return max;
}

double Waveform::getMinAmplitude() const {
    if(!samples.size())
        return 0.;
    double min = samples[0];
    for(auto& sample : samples)
        if(sample < min)
            min = sample;
    return min;
}

double Waveform::signalTimeF(float threshold, float timeStep) const{
    for(int i=0; i< samples.size(); i++)
        if(samples[i] <= threshold)
            return timeStep*i;
    return 0;
}

std::array<double, 2> Waveform::signalTime(float threshold, double timeStep) const{
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

double Waveform::integrate(double timeStep, float tmin, float tmax) {
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

double Waveform::integrate(double timeStep) {
    if(!samples.size())
        return 0;
    double integral = 0;
    for(int s = 0; s < samples.size()-1; s++){
            double avg_v = 0.5 * (samples[s+1] + samples[s]); // mV
            integral += abs(avg_v) * timeStep; // mV ns
    }
    return integral;
}

void Waveform::print(std::ostream& out){
    out     << "Peak       = " << peak        << " mV\n"
            << "Peak time  = " << peakTime    << " ns\n"
            << "Baseline   = " << baseline    << " mV\n"
            << "Noise RMS  = " << baselineRMS << " mV\n"
            << "tot        = " << tot         << " ns\n"
            << "Rise       = " << riseTime    << " ns\n"
            << "Fall       = " << fallTime    << " ns\n"
            << "Area       = " << area        << " mV ns\n"
            << "Charge     = " << charge      << " pC\n";
}


//////////////// Waveform fuctions //////////////////////////////////

void Waveform::extractBaseline(std::size_t baselineSamples){
    if (samples.empty())
        return;

    baselineSamples = std::min(baselineSamples, samples.size());

    if (baselineSamples == 0)
        return;

    double sum = 0.0;

    for (std::size_t i = 0; i < baselineSamples; ++i)
        sum += samples[i];

    baseline = sum / baselineSamples;

    double variance = 0.0;

    for (std::size_t i = 0; i < baselineSamples; ++i) {
        double d = samples[i] - baseline;
        variance += d * d;
    }

    baselineRMS =
        std::sqrt(variance / baselineSamples);
}

void Waveform::extractPeak(PulsePolarity polarity){
    if (samples.empty())
        return;

    double sign = static_cast<double>(polarity);

    double maxTransformed = -std::numeric_limits<double>::infinity();

    std::size_t peakIndex = 0;

    minimum = samples[0];
    maximum = samples[0];

    for (std::size_t i = 0; i < samples.size(); ++i) {

        double y = samples[i];

        minimum = std::min(minimum, y);
        maximum = std::max(maximum, y);

        // Positive-going representation internally
        double transformed = sign * (y - baseline);

        if (transformed > maxTransformed) {
            maxTransformed = transformed;
            peakIndex = i;
        }
    }

    peak = samples[peakIndex];
    peakAmplitude = std::abs(peak - baseline);
    peakTime = peakIndex * timeStep;
}

double Waveform::interpolateCrossingTime(double t1, double y1, double t2, double y2,double threshold){
    if (y2 == y1)
        return t1;

    double fraction = (threshold - y1) / (y2 - y1);
    return t1 + fraction * (t2 - t1);
}


void Waveform::extractEdges(double threshold, PulsePolarity polarity){
    if (samples.size() < 2)
        return;

    const double sign = static_cast<double>(polarity);

    // Transform everything so the pulse is positive-going
    const double transformedThreshold = sign * (threshold - baseline);

    bool foundLeading = false;
    bool foundTrailing = false;

    for (std::size_t i = 1; i < samples.size(); ++i){
        double y1 = sign * (samples[i - 1] - baseline);
        double y2 = sign * (samples[i] - baseline);

        // Leading edge: below -> above
        if (!foundLeading && y1 < transformedThreshold && y2 >= transformedThreshold) {
            leadingEdge = interpolateCrossingTime((i-1)*timeStep, y1, i*timeStep, y2,transformedThreshold);
            foundLeading = true;
        }

        // Trailing edge: above -> below
        if (foundLeading && !foundTrailing && y1 >= transformedThreshold && y2 < transformedThreshold){
            trailingEdge = interpolateCrossingTime((i-1)*timeStep, y1, i*timeStep, y2,transformedThreshold);
            foundTrailing = true;
        }
    }

    if (foundLeading && foundTrailing)
        tot = trailingEdge - leadingEdge;
    else
        tot = 0.0;
}

void Waveform::extractRiseFallTime(PulsePolarity polarity){
    if (samples.empty() || peakAmplitude <= 0.0)
        return;

    const double sign = static_cast<double>(polarity);

    double level10 = 0.10 * peakAmplitude;
    double level90 = 0.90 * peakAmplitude;

    bool found10 = false;
    bool found90 = false;

    double t10 = 0.0;
    double t90 = 0.0;

    // ----------------------------------------------------------
    // Rising edge: baseline -> peak
    // ----------------------------------------------------------

    for(std::size_t i = 1; i < samples.size(); ++i){
        double y1 = sign * (samples[i - 1] - baseline);
        double y2 = sign * (samples[i] - baseline);

        // 10%
        if (!found10 && y1 < level10 && y2 >= level10){
            t10 = interpolateCrossingTime((i-1)*timeStep, y1, i*timeStep, y2,level10);
            found10 = true;
        }

        // 90%
        if (!found90 && y1 < level90 && y2 >= level90) {
            t90 = interpolateCrossingTime((i-1)*timeStep, y1, i*timeStep, y2,level90);
            found90 = true;
        }

        if (found10 && found90)
            break;
    }

    if (found10 && found90)
        riseTime = t90 - t10;


    // ----------------------------------------------------------
    // Falling edge: peak -> baseline
    // ----------------------------------------------------------

    found10 = false;
    found90 = false;

    double fall10 = 0.0;
    double fall90 = 0.0;

    // Start at peak
    auto peakIt = std::min_element(samples.begin(),samples.end());
    std::size_t peakIndex;

    if (polarity == PulsePolarity::Negative) {
        peakIndex = std::distance(samples.begin(), peakIt);
    }
    else {
        auto maxIt = std::max_element(samples.begin(),samples.end());
        peakIndex = std::distance(samples.begin(), maxIt);
    }

    for (std::size_t i = peakIndex + 1; i < samples.size(); ++i){
        double y1 = sign * (samples[i - 1] - baseline);
        double y2 = sign * (samples[i] - baseline);

        // 90% -> below
        if (!found90 && y1 >= level90 && y2 < level90){
            fall90 = interpolateCrossingTime((i-1)*timeStep, y1,i*timeStep, y2,level90);
            found90 = true;
        }

        // 10% -> below
        if (!found10 && y1 >= level10 && y2 < level10){
            fall10 = interpolateCrossingTime((i-1)*timeStep, y1,i*timeStep, y2,level10);
            found10 = true;
        }

        if (found10 && found90)
            break;
    }

    if (found90 && found10)
        fallTime = fall10 - fall90;
}

void Waveform::extractAreaAndCharge(double impedance){
    if (samples.size() < 2)
        return;

    area = 0.0;

    for (std::size_t i = 1; i < samples.size(); ++i)
    {
        double y1 = samples[i - 1] - baseline;
        double y2 = samples[i] - baseline;
        double dt = i*timeStep - (i-1)*timeStep;

        area += 0.5 * (y1 + y2) * dt;
    }

    // mV * ns / Ohm = pC
    //
    // For a negative PMT pulse, make charge positive.
    charge = -area / impedance;
}

void Waveform::extractWaveformParameters(double threshold, PulsePolarity polarity, double impedance, std::size_t baselineSamples)
{
    if (samples.empty())
        return;

    nSamples = samples.size();

    // 1. Baseline and noise
    extractBaseline(baselineSamples);

    // 2. Peak / amplitude
    extractPeak(polarity);

    // 3. Threshold crossings + tot
    extractEdges(threshold, polarity);

    // 4. Rise / fall time
    extractRiseFallTime(polarity);

    // 5. Integral / charge
    extractAreaAndCharge(impedance);
}


void Waveform::setTimeStep(double ts){timeStep = ts;}
double Waveform::getTimeStep() const {return timeStep;};