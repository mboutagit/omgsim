#pragma once

#include <iostream>
#include <string>
#include <array>
#include "OM.hh"

namespace lactel
{
    enum class TriggerLogic {
        OR = 0,
        AND = 1,
        MAJORITY = 2
    };

    class BWaveCatcherConf
    {
    private:
        /* data */
    public: 

        enum class TriggerEdge{Rising, Falling};

        BWaveCatcherConf(/* args */);
        ~BWaveCatcherConf();

        inline double getSamplingFrequency(){return samplingFrequency;}
        inline double getREcordLength(){return recordLength;}

        void setSimplingFrequenct(double f); // in GS/s
        void setChannelInputOffset(int c, double offset); // set DC Offset in volt
        void setInputDynamicRange(double dr);
        void setBandWidth(double bw);

        void setRecordLength(int rl); // number of slices
        void setBufferDepth (int); // number of event to be stored in the buffer

        void setChannelTriggerThreshold(int c, int thr);
        void setAllChannelTriggerThreshold(int thr);
        
        void setTriggerSource(std::string src);
        void setChannelTriggerEdge(int c, TriggerEdge te);

        void setPreTriggerTime(double);
        void setPostTriggerTime(double);

        bool enableChannel(int c);
        bool disableChannel(int c);
        bool enableAllChannels();
        bool disableAllChannels();

        void setTriggerLogic(TriggerLogic trig);
        void setGateLEngth(int);

        void print(std::ostream& out = std::cout) const;


         // Constants
        static constexpr int NumChannels = 2;

        // Sampling and Input Configuration
        double samplingFrequency;                        // in GS/s (e.g. 0.4, 0.8, 1.6, 3.2)
        double inputDynamicRange;                        // in Vpp (e.g. 2.5 V)
        std::array<double, NumChannels> inputOffsets;    // DC offset per channel (±1.25V)
        std::array<double, NumChannels> timeDelay;       // time Delay for each channel
        double bandwidth;                                // in MHz (e.g. 500 MHz)
        int resolution;                                  // resolution in bits

        // Acquisition Settings
        int recordLength;                                // Number of samples per event (up to 1024)
        int eventBufferDepth;                            // Number of events that can be stored in buffer

        // Trigger Configuration
        std::array<double, NumChannels> triggerThresholds;  // Discriminator thresholds (DAC values)
        std::string triggerSource;                       // "external", "self", "software", etc.
        std::array<TriggerEdge, NumChannels> triggerEdges; // "rising" or "falling"

        double preTriggerTime;                           // Pre-trigger window in microseconds
        double postTriggerTime;                          // Post-trigger window in microseconds

        std::array<bool, NumChannels> channelEnabled;    // Enable/disable channels for triggering
        TriggerLogic triggerLogic;                        // e.g. "OR", "AND", "MAJORITY"
        int gateLength;                                  // Gate width for trigger primitives (in ns or samples)

        // Calibration / Correction
        bool applyBaselineCorrection;
        std::array<double, NumChannels> pedestalOffsets; // Per-channel pedestal offsets
        bool applyTimeINLCorrection;
        std::array<int, NumChannels> triggerDACOffsets;  // Calibration DAC offsets

        // Monitoring
        bool enableHitRateMonitor;
        bool enableCoincidenceTrigger;

        // System & Clock
        std::string clockSource;                         // "internal", "external"
        bool syncWithOtherBoards;

        // Miscellaneous
        double deadTime;                                 // Readout dead time in microseconds

        double internalResistor;   // interanl resistor in Ohm
    };
 
} // namespace lactel
