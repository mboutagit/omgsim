#include "BWaveCatcher.hh"
#include <numeric>

#include "CLHEP/Units/SystemOfUnits.h"


namespace tools
{

    int BWaveCatcher::id = 0;
    BWaveCatcherConf BWaveCatcher::config_ = BWaveCatcherConf();

    BWaveCatcher::BWaveCatcher(std::string nm) : name(nm), acquiring_(false), rng_(std::random_device{}()), noiseDist_(-2, 2), TTSDist_(3.314, 0.1531), AmpDist_(0, 0.1156) { // 0.1156 is to have 90% of amplitudes inside 30% fluctuation
        
    }

    BWaveCatcher::~BWaveCatcher() {}

    bool BWaveCatcher::initialize() {
        // Simulate board presence
        return true;
    }

    bool BWaveCatcher::applyConfig(const BWaveCatcherConf& config) {
        config_ = config;
        return true;
    }

    BWaveCatcherConf& BWaveCatcher::getConfiguration() const {
        return config_;
    }

    bool BWaveCatcher::plugOM(int channel, OM& om){
        if(channel > config_.NumChannels -1){
            std::cout << "\033[1;35mChannel with index " << channel << " does not exists ..\033[0m" << std::endl;
            return false;
        }

        if(inputOMs[channel].isOK){
            std::cout << "\033[1;35mInfo: Channel " << channel 
                                << " of wavecatcher: " << name 
                                << " is already in use ..\033[0m" 
                                << std::endl;
            return false;
        }

        if(om.isOK)
            connectedOMs.insert({channel, om});
            
        inputOMs[channel] = om;
        return true;
    }

    void BWaveCatcher::signalHandler(int signum) {
        std::cout << "\nCtrl-C pressed. Stopping...\n";
    }

    bool BWaveCatcher::startAcquisition() {}

    void BWaveCatcher::print(std::ostream& out) const {
        out << string(10, '-') << " WaveCatcher: " << name << " " << string(10, '-');
        for(int i =0; i < BWaveCatcherConf::NumChannels; i++){
            out << "\n\tchannel: " << i 
                << " - OM connected: " << connectedOMs.at(i).getOMID()
                << " - ID: " << connectedOMs.at(i).getID();
        }
        out << "\n\n" << string(10, '+') << " Configuration " << string(10, '+') << "\n";
        getConfiguration().print();
        out << string(30, '+');

        out << "\n" <<string(30, '-') << "\n";
    }

    bool BWaveCatcher::stopAcquisition() {}

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

    void BWaveCatcher::setChannelTriggerEdge(int channel, BWaveCatcherConf::TriggerEdge te){
        config_.triggerEdges[channel] = te;
    }

    void BWaveCatcher::setTriggerEdge(BWaveCatcherConf::TriggerEdge te){
        for(int channel = 0; channel < config_.NumChannels; channel ++)
            setChannelTriggerEdge(channel, te);
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

    TGraph BWaveCatcher::drawWaveform(Waveform wf, int buffer_size){
        TGraph gr;
        gr.SetTitle(("Waveform - channel "+ std::to_string(wf.channel) +"; Time (ns); Amplitude (mV)").c_str());

        int sample=0;
        while (sample < config_.recordLength*buffer_size && sample < wf.samples.size()){
            gr.SetPoint(sample, 
                        (1./config_.samplingFrequency)*(1 + sample), 
                        wf.samples.at(sample) //*config_.inputDynamicRange*1e3/std::pow(2, 12)
                    );
            sample++;
        }
        return gr;
    }

     TGraph BWaveCatcher::drawADCRecord(ADC_record record){
        TGraph gr;
        gr.SetTitle(("ADC counts - channel "+ std::to_string(record.channel) +"; sample; Count").c_str());

        int sample=0;
        while (sample < record.samples.size()){
            gr.SetPoint(sample, 
                        sample, 
                        record.samples[sample] //*config_.inputDynamicRange*1e3/std::pow(2, 12)
                    );
            sample++;
        }
        return gr;
     }

    void BWaveCatcher::digitizeEvent(BAnalysisManager::DetectedEvent& event){

        if(! isActive()){
            BLOG_WARNING(Form("WaveCatcher %s is not active!", name.c_str()));
            return;
        }
            
        Waveform channelWaveform[getConfiguration().NumChannels];
        for(int i=0; i < getConfiguration().NumChannels; i++){
            ADCs[i] = digitizeEvent(i, event);
            if(! ADCs[i].size())
                return;
            std::vector<Waveform> wfs = waveformFromADC(ADCs[i]);
            Waveform fullWF = joinWavefroms(i, wfs);
            fullWF.extractWaveformParameters(config_.triggerThresholds[i], 
                                                PulsePolarity::Negative,
                                                config_.internalResistor,
                                                15
                                            );
            channelWaveform[i] = fullWF;
            BAnalysisManager::DAQEvent daqEvent;
            // BAnalysisManager::DAQEvent daqEvent;
            daqEvent.waveform = fullWF;
            daqEvent.channel = i;
            daqEvent.amp = fullWF.getMinAmplitude(); // to mV;
            daqEvent.c = fullWF.integrate(1/getConfiguration().samplingFrequency) * 
                            1e-3 * 1e-9 / getConfiguration().internalResistor; // convert to Columb
            daqEvent.om_id = connectedOMs[i].getID();
            daqEvent.t = fullWF.signalTimeF(getConfiguration().triggerThresholds[i], 1/getConfiguration().samplingFrequency);
            event.daqEvents.push_back(daqEvent);
        }

        // for(int i = 0; i < getConfiguration().NumChannels -1 ; i++){
        //     if( abs(channelWaveform[i]. - channelWaveform[i+1]) )
        // }


        std::vector<double> times;
        for(auto& wf : channelWaveform)
            times.push_back(wf.leadingEdge);
        
        event.tmin = *std::min_element(times.begin(), times.end());
        event.tmax = *std::max_element(times.begin(), times.end());
        event.time = event.tmin;
        event.dt = event.tmax - event.tmin;
        event.coincidence = event.isACoincidence(15);
        // event.wCoincidence = event.checkCoincidence(15);
    }

    void BWaveCatcher::setTriggerThreshold(int channel, double threshold){
        config_.triggerThresholds[channel] = threshold; // mV
    }

    void BWaveCatcher::setTriggerThreshold(double threshold){
        config_.triggerThresholds.fill(threshold);
    }

    void BWaveCatcher::setInputOffset(int channel, double offset){
        config_.inputOffsets[channel] = offset;
    }

    void BWaveCatcher::setInputOffset(double offset){
        config_.inputOffsets.fill(offset);
    }


    std::vector<ADC_record> BWaveCatcher::digitizeEvent(int channel, BAnalysisManager::DetectedEvent& event){
        std::vector<ADC_record> adcs;

        if(! isActive())
            BLOG_WARNING(Form("WaveCatcher %s is not active!", name.c_str()));

        if(! event.hits.size())
            return adcs;

        OM om = inputOMs[channel];
        // om.dump();
        OM::OMProperties omp = om.properties; 
        if(! om){
            BLOG_ERROR(Form("Invalid channel input %s", om.getOMID().c_str()));
            return adcs;
        }
        double dt = 1. / (config_.samplingFrequency); // ns per sample
        double one_pe_charge = omp.g * (-1) * e_SI; // in C
        double I_0 = (one_pe_charge/((omp.ft/2.2 - omp.rt/2.2)*1e-9));
        double I_peak = (one_pe_charge/(omp.ft/2.2*1e-9)) * pow((omp.rt/2.2/omp.ft/2.2), (omp.rt/2.2/(omp.ft/2.2-omp.rt/2.2))); // in A
        double lsb = config_.inputDynamicRange / (pow(2, config_.resolution) - 1); // in Volts
        std::normal_distribution<> pedDist_(0, 0.30*I_peak/lsb); 
        double t_jutter = 0.;
        // std::cout << "\r\033[10C" << "info: " << one_pe_charge << " - " << I_0 << " - " << I_peak << " - " << lsb << " - " << -e_SI << std::endl;

        for(auto& hit : event.hits){
            // if(hit.om_id != channel+1) // om id comes from copynumber in geant4 and channel is 0 or 1 while copy number starts from 1
            //     continue;
            // std::cout << "\r\033[10C" << "OM: " << connectedOMs[channel].getID() << "  -- OM id = " << hit.om_id << std::endl;
            if(connectedOMs[channel].getID() != hit.om_id)
                continue;

            ADC_record adc;
            adc.samples.resize(config_.recordLength);
            adc.channel = channel; // set channel

            double I_peak_stat;
            // if(amplitudeCorrection){
                I_peak_stat = I_peak*(1 + AmpDist_(rng_)) ;
            // }
            // else{
            //     I_peak_stat = I_peak;
            // }
            
            hit.a = I_peak * config_.internalResistor * 1e3; // mV
            hit.a_f = I_peak_stat * config_.internalResistor * 1e3; // mV

            double sigma_tts = TTSDist_(rng_)/2.355; // FWHM /2*sqrt(2*ln(2))
            std::normal_distribution<> TTSFDist_(t_jutter, sigma_tts);  // sample from TTS FWHM dist. with time jutter
            hit.t_tts = std::max(0.0, hit.time + TTSFDist_(rng_)); // hit time + tts in ns

            double t=0; // time in ns
            for (size_t i = 0; i < config_.recordLength; ++i) {
                t = i * dt;
                double ped=pedDist_(rng_);
                
                if (t < hit.t_tts){
                    //adc.samples[i] = ped; // + noiseDist_(rng_); // set to 0 and always add basline noise;
                   continue; 
                } 
                
                double i_t = I_peak_stat* (1 - exp(-(t - hit.t_tts)/omp.rt/2.2)) * exp(-(t - hit.t_tts) / omp.ft/2.2); // i(t) = I*(1 - exp(..))*exp(..) model hit i pulse shape // current
                double v_t = config_.internalResistor * i_t; // + analog noise // u(t)=Ri(t) in V 
                double pre_count = v_t/lsb; // + ped; // + ped;
                double count = std::clamp(std::round(pre_count),  - pow(2, config_.resolution-1), pow(2, config_.resolution-1) -1); // + noiseDist_(rng_); // mV

                adc.samples[i] = count;  
                // if(timeCorrection){
                    adc.timestamp=hit.t_tts; 
                // }
                // else{
                //     adc.timestamp=hit.time; 
                // }
                           
            }
            adcs.push_back(adc);
        }
        return adcs;
    }

    Waveform BWaveCatcher::waveformFromADC(ADC_record record){
        if(!record.samples.size())
            return Waveform();
        Waveform wf;
        wf.channel = record.channel;
        wf.samples.resize(record.samples.size());
        double dt = 1/config_.samplingFrequency; // in ns
        int shift = 0;
        double inputOffset=config_.inputOffsets[record.channel];
        double lsb = config_.inputDynamicRange / (pow(2, config_.resolution) - 1); // in Volts
        std::normal_distribution<> pedDist_(0, 500*lsb); 
        for(int i=0; i<record.samples.size(); i++){
            double ped=pedDist_(rng_);  
            if(i*dt < record.timestamp)
                continue; 
            wf.samples[i] = (record.samples[i]) * lsb *1e3 ; // +  noiseDist_(rng_) % 2;  mV   
        }
        wf.timestamp=record.timestamp;
        return wf;  
    }
    
    std::vector<Waveform> BWaveCatcher::waveformFromADC(std::vector<ADC_record> records){
        std::vector<Waveform> wfs;
        if( ! records.size())
            return wfs;
        for(auto& adc : records){
            wfs.push_back(waveformFromADC(adc));
        }
        return wfs;
    }

    Waveform BWaveCatcher::joinWavefroms(int channel, std::vector<Waveform> wfs){
        if(wfs.size() == 0)
            return Waveform();
        std::sort(wfs.begin(), wfs.end());
       
        double length = config_.eventBufferDepth*config_.recordLength;
        double dt = 1./config_.samplingFrequency;

        Waveform waveform;
        waveform.channel = wfs[0].channel;
        waveform.samples.resize(length);
        waveform.setTimeStep(dt);
        bool triggered = false;
        double lsb = config_.inputDynamicRange / (pow(2, config_.resolution) - 1); // in Volts
        std::normal_distribution<> pedDist_(0, 10*lsb); 


        for(auto& wf : wfs){
            if(! wf.samples.size() || wf.channel != channel)
                continue; // if this specific wavefrom is empty
            int sample=0; // sample 
            int ref_sample=0;
            bool flag = true;
            while(sample < length){
                if(sample > wf.samples.size() -1 ) // sample beyond the wf samples
                    break;

                if(sample*dt < wf.timestamp ){ // time before actual waveform time
                    sample++;
                    continue;
                }
                else{
                    if(flag){ // keep reference of actual wavefrom sample
                        ref_sample = sample;
                        flag=false;
                    }   
                }

                waveform.samples[sample] += wf.samples[sample-ref_sample];
                // saturation
                if(waveform.samples[sample] > config_.inputDynamicRange*1e3/2)
                    waveform.samples[sample] = config_.inputDynamicRange*1e3/2; // positive saturation  np noise
                if(waveform.samples[sample] < - config_.inputDynamicRange*1e3/2)
                    waveform.samples[sample] = - config_.inputDynamicRange*1e3/2; // negative saturation  no noise

                sample++;
            }    
        }

        if(checkTrigger(waveform, channel))
            return waveform;
        else 
            return Waveform();
    }

    bool BWaveCatcher::checkTrigger(Waveform waveform, int channel){
        for (std::size_t i = 1; i < waveform.samples.size(); i++) {
            if (config_.triggerEdges[channel] == BWaveCatcherConf::TriggerEdge::Rising) {
                if (waveform.samples[i - 1] < config_.triggerThresholds[channel] &&
                    waveform.samples[i] >= config_.triggerThresholds[channel]) {
                    waveform.timestamp = i * 1./config_.samplingFrequency;
                    waveform.triggered = true;
                    return true;
                }
            }
            else if(config_.triggerEdges[channel] == BWaveCatcherConf::TriggerEdge::Falling) {
                if (waveform.samples[i - 1] > config_.triggerThresholds[channel] &&
                    waveform.samples[i] <= config_.triggerThresholds[channel]) {
                    return true;
                }
            }
            else 
                BLOG_ERROR("Error in trigger Edge");
        }
        return false;
    }
} // namespace tools



