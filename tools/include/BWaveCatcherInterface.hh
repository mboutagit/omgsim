#pragma once

#include "BWaveCatcherConf.hh"
#include <vector>
#include "Waveform.hh"

using namespace lactel;
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
