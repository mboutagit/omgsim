#ifndef BWaveCatcher_hh
#define BWaveCatcher_hh

#include "BWaveCatcherInterface.hh"
#include <random>
#include <chrono>
#include "TGraph.h"

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

    const BWaveCatcherConf getConfiguration();

private:
    BWaveCatcherConf config_;
    bool acquiring_;
    std::vector<Waveform> lastWaveforms_;
    std::default_random_engine rng_;
    std::uniform_int_distribution<uint16_t> noiseDist_;

    Waveform generateFakeWaveform(int channel);
};

#endif // BWaveCatcher_hh
