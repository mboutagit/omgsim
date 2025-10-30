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
    
} // namespace lactel

