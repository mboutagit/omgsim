#include "BWaveCatcherConf.hh"

namespace lactel
{

        // Constructor with defaults
    BWaveCatcherConf::BWaveCatcherConf()
        : samplingFrequency(3.2), // 3.2 Gs/s 
          inputDynamicRange(4.70), // true WaveCatcher range / using attinuator
          inputOffsets{0.0, 0.0},
          bandwidth(500.0),
          resolution(12), // 12 bit resolution
          recordLength(1024),
          eventBufferDepth(7),
          triggerThresholds{-10, -10}, // mv threshold;
          triggerSource("self"),
          triggerEdges{"rising", "rising"},
          preTriggerTime(10.0),
          postTriggerTime(5.0),
          channelEnabled{true, true},
          triggerLogic("OR"),
          gateLength(10),
          applyBaselineCorrection(true),
          pedestalOffsets{0.0, 0.0},
          applyTimeINLCorrection(true),
          triggerDACOffsets{0, 0},
          enableHitRateMonitor(false),
          clockSource("internal"),
          syncWithOtherBoards(false),
          deadTime(125.0), // µs
          internalResistor(50) // Ohm

    {}

    BWaveCatcherConf::~BWaveCatcherConf(){}

    void BWaveCatcherConf::print(std::ostream& out) const {
            out     <<  "\tSampling frequency: "      << samplingFrequency << "\n"
                    <<  "\tinputDynamicRange: "     << inputDynamicRange << "\n"
                    <<  "\tpreTriggerTime: "        << preTriggerTime << "\n"
                    <<  "\tbandwidth: "             << bandwidth << "\n"
                    <<  "\tresolution: "            << resolution << "\n"
                    <<  "\trecordLength: "          << recordLength << "\n"
                    <<  "\teventBufferDepth: "      << eventBufferDepth << "\n"
                    <<  "\ttriggerSource: "         << triggerSource << "\n"
                    <<  "\tpreTriggerTime: "        << preTriggerTime << "\n"
                    <<  "\tpostTriggerTime: "       << postTriggerTime << "\n"
                    <<  "\ttriggerLogic: "          << triggerLogic << "\n"
                    <<  "\tgateLength: "            << gateLength << "\n"
                    <<  "\tenableHitRateMonitor: "  << enableHitRateMonitor << "\n"
                    <<  "\tOffset: \n";
                    for(int i=0; i < NumChannels; i++)
                            out << "\t\tCh" + to_string(i) << " : " << inputOffsets[i] << "\n";

                    out <<  "\ttriggerThresholds: \n";
                    for(int i=0; i < NumChannels; i++)
                            out << "\t\tCh" + to_string(i) << " : " << triggerThresholds[i] << "\n";

                    out <<  "\ttriggerEdges: \n";
                    for(int i=0; i < NumChannels; i++)
                            out << "\t\tCh" + to_string(i) << " : " << triggerEdges[i] << "\n";

                    out <<  "\tpedestalOffsets: \n";
                    for(int i=0; i < NumChannels; i++)
                            out << "\t\tCh" + to_string(i) << " : " << pedestalOffsets[i] << "\n";
                    
                    out <<  "\ttriggerDACOffsets: \n";
                    for(int i=0; i <NumChannels; i++)
                            out << "\t\tCh" + to_string(i) << " : " << triggerDACOffsets[i] << "\n";
                

    
    }

    void BWaveCatcherConf::setSimplingFrequenct(double f){
        samplingFrequency =f;
    }

    void BWaveCatcherConf::setChannelInputOffset(int c, double offset){
        inputOffsets[c] = offset;
    } 

    void BWaveCatcherConf::setInputDynamicRange(double dr){
        inputDynamicRange = dr;
    }

    void BWaveCatcherConf::setBandWidth(double bw){
        bandwidth = bw;
    }

    void BWaveCatcherConf::setRecordLength(int rl){
        recordLength = rl;
    }

    void BWaveCatcherConf::setBufferDepth (int bd){
        eventBufferDepth = bd;
    }

    void BWaveCatcherConf::setChannelTriggerThreshold(int c, int thr){
        triggerThresholds[c] = thr;
    }
    void BWaveCatcherConf::setAllChannelTriggerThreshold(int thr){
        triggerThresholds.fill(thr);
    }
    
    void BWaveCatcherConf::setTriggerSource(std::string src){
        triggerSource = src;
    }

    void BWaveCatcherConf::setChannelTriggerEdge(int c, std::string te){
        triggerEdges[c] = te;
    }

    void BWaveCatcherConf::setPreTriggerTime(double ptt){
        preTriggerTime = ptt;
    }

    void BWaveCatcherConf::setPostTriggerTime(double ptt){
        postTriggerTime = ptt;
    }

    bool BWaveCatcherConf::enableChannel(int c){
        channelEnabled[c] = true;
        return true;
    }

    bool BWaveCatcherConf::disableChannel(int c){
        channelEnabled[c] = false;
        return true;
    }

    bool BWaveCatcherConf::enableAllChannels(){
        channelEnabled.fill(true);
        return true;
    }

    bool BWaveCatcherConf::disableAllChannels(){
        channelEnabled.fill(false);
        return true;
    }

    void BWaveCatcherConf::setTriggerLogic(std::string triglog){
        triggerLogic = triglog;
    }

    void BWaveCatcherConf::setGateLEngth(int gl){
        gateLength = gl;
    }
    
} // namespace lactel

