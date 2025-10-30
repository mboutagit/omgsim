#include "BWaveCatcher.hh"

BWaveCatcher::BWaveCatcher()
    : acquiring_(false), rng_(std::random_device{}()), noiseDist_(0, 4095) {}

BWaveCatcher::~BWaveCatcher() {}

bool BWaveCatcher::initialize() {
    // Simulate board presence
    return true;
}

bool BWaveCatcher::applyConfig(const BWaveCatcherConf& config) {
    config_ = config;
    return true;
}

bool BWaveCatcher::startAcquisition() {
    acquiring_ = true;
    return true;
}

bool BWaveCatcher::stopAcquisition() {
    acquiring_ = false;
    return true;
}

bool BWaveCatcher::isAcquiring() const {
    return acquiring_;
}

bool BWaveCatcher::readEventData() {
    if (!acquiring_) return false;

    lastWaveforms_.clear();
    for (int ch = 0; ch < BWaveCatcherConf::NumChannels; ++ch) {
        if (config_.channelEnabled[ch])
            lastWaveforms_.push_back(generateFakeWaveform(ch));
    }
    return true;
}

std::vector<Waveform> BWaveCatcher::getWaveforms() const {
    return lastWaveforms_;
}

bool BWaveCatcher::forceSoftwareTrigger() {
    return readEventData();
}

std::string BWaveCatcher::getStatus() const {
    return acquiring_ ? "Acquiring (simulated)" : "Idle (simulated)";
}

void BWaveCatcher::reset() {
    stopAcquisition();
    lastWaveforms_.clear();
}

Waveform BWaveCatcher::generateFakeWaveform(int channel) {
    Waveform wf;
    wf.channel = channel;
    wf.timestamp = static_cast<double>(
        std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::steady_clock::now().time_since_epoch()
        ).count()) / 1e6;

    int numSamples = config_.recordLength;
    wf.samples.resize(numSamples);

    for (int i = 0; i < numSamples; ++i) {
        // Simulate a signal + noise
        double signal = 2048 + 1000 * std::cos(2.* 3.14159 * i / numSamples);
        double noise = noiseDist_(rng_) % 100;
        wf.samples[i] = static_cast<uint16_t>(std::max(0.0, std::min(4095.0, signal + noise)));
    }

    return wf;
}

Waveform BWaveCatcher::getFakeWaveform(int channel){
    return generateFakeWaveform(channel);
}

const BWaveCatcherConf BWaveCatcher::getConfiguration(){
    return config_;
}

TGraph BWaveCatcher::drawWaveform(Waveform wf){
    TGraph gr;
    gr.SetName("waveform");
    gr.SetTitle("Waveform; Time (ns); Amplitude (mV)");

    int sample=0;
    while (sample < config_.recordLength){
        gr.SetPoint(sample, 
                    config_.samplingFrequency*(1 + sample), 
                    wf.samples[sample]*config_.inputDynamicRange*1e3/std::pow(2, 12)
                );
        sample++;
    }
    return gr;
}



