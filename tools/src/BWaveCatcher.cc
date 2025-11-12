#include "BWaveCatcher.hh"

namespace tools
{
    BWaveCatcher::BWaveCatcher()
    : acquiring_(false), rng_(std::random_device{}()), noiseDist_(-2, 2) {

        scope.resize(config_.NumChannels);
        signalSource.resize(config_.NumChannels);

        canvas = new TCanvas("scope", "BWaveCatcher scope");
        canvas->Divide(config_.NumChannels);
        canvas->SetTitle("Oscil");
        for(int c = 0; c < config_.NumChannels; c++){
            canvas->cd(c);
            scope[c] = new TGraph();
            scope[c]->SetTitle(("Channel " + std::to_string(c) + "; Time (ns); Amplitude (mV)").c_str());
            scope[c]->Draw("APL");
        }
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

    BWaveCatcherConf BWaveCatcher::getConfiguration(){
        return config_;
    }

    bool BWaveCatcher::plugOM(int c, const OM& om){
        if(c > config_.NumChannels -1){
            std::cout << "\033[1;35mChannel with index " << c << " does not exists ..\033[0m" << std::endl;
            return false;
        }

        signalSource[c] = om;
        return true;
    }


    void BWaveCatcher::signalHandler(int signum) {
        std::cout << "\nCtrl-C pressed. Stopping...\n";
        isRunning = false;
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
            double period = 1.0 / (config_.samplingFrequency*1e9);
            
            while (true)
            {
                std::cout << "\r\033[10C" << "acquiring ...: sample Index: " << sampleIndex << std::endl;
                                // simulate one ADC sample

                // circular buffer

                for(int c = 0; c < config_.NumChannels; c++){
                    sampleIndex = (sampleIndex + 1) % config_.recordLength;
                    double signal = 2048 + 1000 * std::cos(2.* 3.14159 * sampleIndex / config_.recordLength);
                    double noise = noiseDist_(rng_) % 100;

                    
                    scope[c]->SetPoint(sampleIndex, sampleIndex*config_.samplingFrequency, signal+noise);
                    if(sampleIndex == config_.recordLength -1){
                        std::cout << "\r\033[50C" << "Update" << std::endl;
                        scope[c]->Draw("APL");
                        canvas->Draw();
                        canvas->Modified();
                        canvas->Update();
                    }
                }      
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

    std::vector<ADC_record> BWaveCatcher::digitizeEvent(int channel, BAnalysisManager::DetectedEvent event){
        std::vector<ADC_record> adcs;
        if(! event.hits.size())
            return adcs;
        // for(int i = 0; i < config_.NumChannels; i++)
        //     std::cout << "\r\033[10C" << "Channel " << i << ": OM Connected: " << signalSource[i].getOMID() << std::endl;
            
        double dt = 1. / (config_.samplingFrequency); // ns per sample
        double tau_r = 4; // rise time  ns
        double tau_f = 11; // fall time ns
        double pmt_gain = 5e7;
        double electron_charge = 1.602e-19; // in Columb
        double resistor = 50; // in ohm
        double one_pe_charge = pmt_gain * electron_charge;
        double amplitude = one_pe_charge/((tau_f - tau_r)*1e-9); // charge in C and time in s
        double vStep = (config_.inputDynamicRange*1e3/pow(2, config_.resolution));

        for(auto& hit : event.hits){
            if(hit.om_id != channel+1) // om_id starts from 1 while channel starts from 0
                continue;

            double global_time = 0;
            ADC_record adc;
            adc.samples.resize(config_.recordLength);
            adc.channel = channel; // set channel
            double t=0; // time in ns
            for (size_t i = 0; i < config_.recordLength; ++i) {
                t = i * dt;
                if (t < hit.time ){
                    adc.samples[i] = pow(2, config_.resolution)/2. + noiseDist_(rng_); // set to 0 and always add basline noise;
                   continue; 
                } 
                double pulse = - (1 - exp(-(t - hit.time)/tau_r)) * exp(-(t - hit.time) / tau_f); // model hit pulse shape // current
                double voltage = amplitude * pulse * resistor * 1e3; // U=RI in mV 
                double signal = pow(2, config_.resolution)/2. + voltage * vStep + noiseDist_(rng_); // mV

                // std::cout << "\r\033[10C" << "Voltage: " << voltage 
                //                             << " // one_pe_charge: " << one_pe_charge 
                //                             << " // amplitude: " << amplitude 
                //                             << " // pulse: " << pulse 
                //                             << " // step: " << i 
                //                             << " // time: " << t
                //                             << " // comp: " << signal 
                //                             << " // v*vstep: " << voltage * vStep 
                //                             << " // v*vstep int: " << (int)(std::round(signal)) 
                //                             << std::endl;

                if(signal > pow(2, config_.resolution))
                    signal = pow(2, config_.resolution); // positive saturation 
                if(signal < 0)
                    signal = 0; // negative saturation 

                adc.samples[i] = std::round(signal);    
                adc.timestamp=hit.time;            
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
        double vStep=(config_.inputDynamicRange*1e3/pow(2, config_.resolution));
        double dt = 1/config_.samplingFrequency; // in ns
        int shift = 0;
        bool useShift=false;
        bool trigger = false;
        double inputOffset=config_.inputOffsets[record.channel];
        for(int i=0; i<record.samples.size(); i++){
            if(i*dt < record.timestamp || 
                i-shift > record.samples.size() || 
                i+inputOffset > record.samples.size()
            ){    // check range
                // wf.samples[i] = noiseDist_(rng_) % 2; // always add basline noise;
                continue; 
            }
            if(!useShift){
                shift=i;
                useShift=true;
            }
                
            wf.samples[i+inputOffset] =(record.samples[(i-shift)] - pow(2, config_.resolution)/2.) * vStep; // +  noiseDist_(rng_) % 2;  // positive region
            // std::cout << "\r\033[15C" << "wf.samples[i+inputOffset]: " << wf.samples[i+inputOffset] 
            //                             << "config_.triggerThresholds[record.channel]: " << config_.triggerThresholds[record.channel]                           
            //                             << std::endl;
            if(abs(wf.samples[i+inputOffset]) > config_.triggerThresholds[record.channel]){
                trigger=true;
            }
                

        }
        wf.timestamp=record.timestamp;
        if(trigger){ // if descriminator OK
            // std::cout << "\r\033[15C" << "Trigger OK" << std::endl;
            return wf; 
        }
        else{
            // std::cout << "\r\033[15C" << "Trigger Not OK" << std::endl;
            return Waveform();
        }   
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

    std::vector<Waveform> BWaveCatcher::waveformFromADC(std::vector<ADC_record> records){
        std::vector<Waveform> wfs;
        if( ! records.size())
            return wfs;
        for(auto& adc : records){
            wfs.push_back(waveformFromADC(adc));
        }
        return wfs;
    }

    Waveform BWaveCatcher::joinWavefroms(std::vector<Waveform> wfs){
        if(wfs.size() == 0)
            return Waveform();
        // std::cout << "\r\033[10C" << "SSSSSSSSize: " << wfs.size() << std::endl;
        std::sort(wfs.begin(), wfs.end());
       
        double length = config_.eventBufferDepth*config_.recordLength;
        double dt = 1./config_.samplingFrequency;

        // (*std::max_element(wfs.begin(), wfs.end())).timestamp > length ? 
        //         cout << "Wavefrom is longuer than the scope" << endl :
        //         cout << "Wavefrom is within the scope" << endl;
            

        // std::cout << "\r\033[10C" << "length: " << length << " - dt = " << dt << " - eventBufferDepfth: " << config_.eventBufferDepth << std::endl;
        Waveform waveform;
        waveform.channel = wfs[0].channel;
        waveform.samples.resize(length);
        
        for(auto& wf : wfs){
            int sample=0; // sample 
            int ref_sample=0;
            bool flag = true;
            while(sample < length){
                if(sample > wf.samples.size() -1 )
                    break;

                if(sample*dt < wf.timestamp ){
                    sample++;
                    continue;
                }
                else{
                    if(flag){
                        ref_sample = sample;
                        flag=false;
                    }   
                }

                waveform.samples[sample] += wf.samples[sample-ref_sample];
                if(waveform.samples[sample] > config_.inputDynamicRange*1e3/2)
                    waveform.samples[sample] = config_.inputDynamicRange*1e3/2; // positive saturation  np noise
                if(waveform.samples[sample] < - config_.inputDynamicRange*1e3/2)
                    waveform.samples[sample] = - config_.inputDynamicRange*1e3/2; // negative saturation  no noise
                
                // std::cout << "\r\033[10C" << "time is: " << sample*dt << " - sample is: " << sample 
                //                                                         << " - sample non cum: " << waveform.samples.at(sample) 
                //                                                         << " - sample cumm: " << wf.samples.at(sample-ref_sample) 
                //                                                         << std::endl;
                sample++;
            }
        }
        return waveform;
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



