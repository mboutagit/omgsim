#include "BWaveCatcherConf.hh"

namespace lactel
{

        // Constructor with defaults
    BWaveCatcherConf::BWaveCatcherConf()
        : samplingFrequency(3.2),
          inputDynamicRange(2.5),
          inputOffsets{0.0, 0.0},
          bandwidth(500.0),
          recordLength(1024),
          eventBufferDepth(7),
          triggerThresholds{1000, 1000},
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
          deadTime(125.0) // µs
    {}

    BWaveCatcherConf::~BWaveCatcherConf(){}

    void BWaveCatcherConf::print(){
        std::cout <<  "Sampling frequency: " << samplingFrequency << "\n"
                    <<  "inputDynamicRange: " << inputDynamicRange << "\n"
                    <<  "preTriggerTime: " << preTriggerTime << "\n"
                    <<  "channelEnabled: " 
                    << std::endl;

    
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

