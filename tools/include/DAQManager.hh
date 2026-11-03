#ifndef DAQManager_hh
#define DAQManager_hh

#include "BWaveCatcher.hh"
#include "BAnalysisManager.hh"
#include "TFile.h"
#include "TTree.h"
#include "OM.hh"




namespace tools{
    class BWaveCatcherMessenger;
}

using namespace tools;
namespace DAQ {

    /**
     * @brief DAQ Manager class to manage LAC-TEL Digitizer.
     * @param vector of detectedEvent
     * @param vector of wavecatcher assigned
     */

    // class DAQEvent
    // {
    // private:
    //     /* data */
    //     int channel;
    //     int om_id;
    //     int event_id;
    //     float amplitude;
    //     float tot;
    //     float time;

    // public:
    //     DAQEvent(/* args */){};
    //     ~DAQEvent(){};
    // };

    class DAQManager
    {            
        public:                 
            static DAQManager* getIt();
            ~DAQManager(){std::cout << "\r\033[20C" << "DAQManager stopped..." << std::endl;};

            enum class DAQState{OFF, ON};
            void getDetectedEvents(TFile* file, std::string tree_name);
            void getDetectedEvents(TTree* tree);
            void setStatus(bool s);
            bool getStatus() const;
            bool isActive() const;
            void activate();
            void deactivate();
            void dumpEvents(std::ostream& out);
            void dumpEvents(std::vector<BAnalysisManager::DetectedEvent*>& events, std::ostream& out);
            void addAWaveCatcher(BWaveCatcher* wc);
            void addAWaveCatchers(std::vector<BWaveCatcher*> wcs);
            void init();

            void addEvent(BAnalysisManager::DetectedEvent* event);
            void addAWaveCatcher(std::vector<BWaveCatcher*> wcs);
            std::vector<BAnalysisManager::DetectedEvent*> getEvents();


            BWaveCatcher* getWavaCatcherByName(std::string name);
            BWaveCatcher* getWavaCatcherByIndex(int index);

            DAQManager(const DAQManager&) = delete;
            void dump(std::ostream& out = std::cout);
            DAQManager& operator=(const DAQManager&) = delete;

            std::vector<BAnalysisManager::Hit*> getHitsByChannelID(BWaveCatcher* wc, int channel); 

        private:
            /* data */
            static DAQManager* instance;
            DAQManager();
            std::vector<BWaveCatcher*> wavecatchers;
            std::vector<BAnalysisManager::DetectedEvent*> Events;
            BWaveCatcherMessenger* waveCatcherMessenger;
            bool state;
    };
    
} // namespace DAQ

#endif