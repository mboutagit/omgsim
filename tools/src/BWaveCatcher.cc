#include "BWaveCatcher.hh"

namespace tools
{
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

        
        if (isRunning) { // avoid multiple starts
            std::cout << "\r\033[10C" << "Already acquiring ..." << std::endl;
            return true;
        }
        isRunning = true;

        acquisitionThread = std::thread([this]() {
             size_t sampleIndex = 0;
            double period = 1.0 / config_.samplingFrequency;
            while (true)
            {
                std::cout << "\r\033[10C" << "acquiring ...: sample Index: " << sampleIndex << std::endl;
                                // simulate one ADC sample

                // circular buffer
                sampleIndex = (sampleIndex + 1) % config_.recordLength;

                // wait for next sample
                std::this_thread::sleep_for(std::chrono::duration<double>(period));
            }   
        });
        return true;
    }

    bool BWaveCatcher::stopAcquisition() {
        acquiring_ = false;

        if (!isRunning) {
            std::cout << "\r\033[10C" << "Already Stopped ..." << std::endl;
            return true;
        }
        isRunning = false;
        if (acquisitionThread.joinable())
            acquisitionThread.join();

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
        if(config_.channelEnabled[channel])
            return generateFakeWaveform(channel);
        else{
            std::cout << "\r\033[10C" << "Channel: " << channel << " is desable." << std::endl;
        }
    }

    BWaveCatcherConf BWaveCatcher::getConfiguration(){
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

    // Waveform BWaveCatcher::digitizeEvent(int c, BAnalysis::DetectedEvent event){
    //     std::cout << "\r\033[10C" << "---->>>> Call Degitizer " << std::endl; 
    //     Waveform wf;

    //     if(!config_.channelEnabled[c] || ! event.hits.size()){
    //         std::cout << "\r\033[10C" << "Could not digitize: channel "
    //                                     << c 
    //                                     << " is disabled." 
    //                                     << " - status: "
    //                                     << config_.channelEnabled[c]
    //                                     << std::endl;
    //         return wf;
    //     }
    //     int i=0;
    //     double tmin =0, tmax=1/(config_.samplingFrequency*1e9); // sampling frequency in GS/s 3.2 GS/s

    //     int ihit=0;
    //     while(ihit < event.hits.size()){

            
    //         ihit++;
    //     }



        // while(){ 
        //     for(auto& hit : event.hits){
        //         if(hit.time < tmax && hit.time > tmin)
        //             std::cout << "\r\033[10C" << "event" << std::endl;
        //             // wf.samples[i] = hit.charge;
        //         // std::cout << "\r\033[55C" << "event dd: " << hit.time*1e9 << std::endl;
        //     }
        //     std::cout << "\r\033[10C" << "slice: " << i 
        //                                 << " - tmin: " << tmin
        //                                 << " - tmax: " << tmax 
        //                                 << std::endl; //wf.samples[i] << std::endl;
        //     tmin+= 1/(config_.samplingFrequency*1e9);
        //     tmax+= 1/(config_.samplingFrequency*1e9);
        // }
        
    //     std::cout << "\r\033[10C" << "----<<<< Call Degitizer " << std::endl;
    //     return wf;
    // }

} // namespace tools



