#ifndef BWaveCatcherConf_hh
#define BWaveCatcherConf_hh

#include <string>
#include <array>

namespace lactel
{
    class BWaveCatcherConf
    {
    private:
        /* data */
    public: 
        BWaveCatcherConf(/* args */);
        ~BWaveCatcherConf();

        inline double getSamplingFrequency(){return samplingFrequency;}
        inline double getREcordLength(){return recordLength;}

         // Constants
        static constexpr int NumChannels = 2;

        // Sampling and Input Configuration
        double samplingFrequency;                        // in GS/s (e.g. 0.4, 0.8, 1.6, 3.2)
        double inputDynamicRange;                        // in Vpp (e.g. 2.5 V)
        std::array<double, NumChannels> inputOffsets;    // DC offset per channel (±1.25V)
        double bandwidth;                                // in MHz (e.g. 500 MHz)

        // Acquisition Settings
        int recordLength;                                // Number of samples per event (up to 1024)
        int eventBufferDepth;                            // Number of events that can be stored in buffer

        // Trigger Configuration
        std::array<int, NumChannels> triggerThresholds;  // Discriminator thresholds (DAC values)
        std::string triggerSource;                       // "external", "self", "software", etc.
        std::array<std::string, NumChannels> triggerEdges; // "rising" or "falling"

        double preTriggerTime;                           // Pre-trigger window in microseconds
        double postTriggerTime;                          // Post-trigger window in microseconds

        std::array<bool, NumChannels> channelEnabled;    // Enable/disable channels for triggering
        std::string triggerLogic;                        // e.g. "OR", "AND", "MAJORITY"
        int gateLength;                                  // Gate width for trigger primitives (in ns or samples)

        // Calibration / Correction
        bool applyBaselineCorrection;
        std::array<double, NumChannels> pedestalOffsets; // Per-channel pedestal offsets
        bool applyTimeINLCorrection;
        std::array<int, NumChannels> triggerDACOffsets;  // Calibration DAC offsets

        // Monitoring
        bool enableHitRateMonitor;

        // System & Clock
        std::string clockSource;                         // "internal", "external"
        bool syncWithOtherBoards;

        // Miscellaneous
        double deadTime;                                 // Readout dead time in microseconds
    };
 
} // namespace lactel




#endif // BWaveCatcherConf_hh
