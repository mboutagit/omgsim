#ifndef BWaveCatcher_hh
#define BWaveCatcher_hh

#include <iostream>
#include "BWaveCatcherInterface.hh"
#include <random>
#include <chrono>
#include <thread>
#include <atomic>
#include <csignal>

#include "TCanvas.h"
#include "TGraph.h"

#include "BAnalysisManager.hh"
#include "OM.hh"

using namespace lactel;

namespace tools{
    class BWaveCatcher : public BWaveCatcherInterface {
        public:
            BWaveCatcher();
            virtual ~BWaveCatcher();

            bool initialize() override;
            bool applyConfig(const BWaveCatcherConf& config) override;

            bool startAcquisition() override;
            bool stopAcquisition() override;
            bool isAcquiring() const override;

            bool readEventData() override;
            std::vector<Waveform> getWaveforms() const override;

            bool forceSoftwareTrigger() override;
            std::string getStatus() const override;
            void reset() override;

            Waveform getFakeWaveform(int channel);
            TGraph drawWaveform(Waveform wf, int buffer_size=1);

            BWaveCatcherConf getConfiguration();

            std::vector<ADC_record> digitizeEvent(int channel, BAnalysisManager::DetectedEvent event);
            Waveform waveformFromADC(ADC_record record);
            TGraph drawADCRecord(ADC_record record);
            std::vector<Waveform> waveformFromADC(std::vector<ADC_record> records);
            TCanvas* getCanvas(){return canvas;}

            bool plugOM(int channel, const OM& om);
            Waveform joinWavefroms(std::vector<Waveform> wfs);

            void setTriggerThreshold(int channel, double threshold); // in mV
            void setTriggerThreshold(double threshold); // for all channels;

            void setInputOffset(int channel, double offset);
            void setInputOffset(double offset); // for all channels;


            // Waveform digitizeEvent(int channel, lactel::BAnalysis::DetectedEvent event);


        private:
            BWaveCatcherConf config_;
            bool acquiring_;
            std::vector<Waveform> lastWaveforms_;
            std::default_random_engine rng_;
            std::uniform_int_distribution<int> noiseDist_;
            std::vector<OM> signalSource;

            Waveform generateFakeWaveform(int channel);

            void signalHandler(int signum);

            std::atomic<bool> isRunning=false;
            std::thread acquisitionThread;

            TCanvas* canvas;
            std::vector<TGraph*> scope;
    };
} // namespace tools
#endif // BWaveCatcher_hh
