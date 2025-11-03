#ifndef BWaveCatcher_hh
#define BWaveCatcher_hh

#include <iostream>
#include "BWaveCatcherInterface.hh"
#include <random>
#include <chrono>
#include <thread>
#include <atomic>
#include "TCanvas.h"
#include "TGraph.h"


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
            TGraph drawWaveform(Waveform wf);

            BWaveCatcherConf getConfiguration();
            // Waveform digitizeEvent(int channel, lactel::BAnalysis::DetectedEvent event);


        private:
            BWaveCatcherConf config_;
            bool acquiring_;
            std::vector<Waveform> lastWaveforms_;
            std::default_random_engine rng_;
            std::uniform_int_distribution<uint16_t> noiseDist_;

            Waveform generateFakeWaveform(int channel);

            std::atomic<bool> isRunning;
            std::thread acquisitionThread;
    };
} // namespace tools
#endif // BWaveCatcher_hh
