#include "DAQManager.hh"
#include "BWaveCatcherMessenger.hh"

namespace DAQ{
    DAQManager* DAQManager::instance = nullptr;
    DAQManager* DAQManager::getIt(){
        if(!instance)
            instance = new DAQManager();
        return (DAQManager*) instance;
    }

    DAQManager::DAQManager() : state(false) {
        waveCatcherMessenger = new BWaveCatcherMessenger(this);
        std::cout << "Starting DAQ: with state: " << getStatus() << std::endl;
    }

    void DAQManager::getDetectedEvents(TFile* file, std::string tree_name){
        TTree* tree = (TTree*)file->Get(tree_name.data());
        getDetectedEvents(tree);
    }

    void DAQManager::getDetectedEvents(TTree* tree){
        BAnalysisManager::DetectedEvent* event = nullptr;
        tree->SetBranchAddress("DetectedEvent", &event);

        Long64_t nEntries = tree->GetEntries();
        for (Long64_t i = 0; i < nEntries; ++i) {
            tree->GetEntry(i);
            Events.push_back(event);
        }
        delete event;
    }

    void DAQManager::setStatus(bool s){
        std::cout << "\r\033[80C" << "here: I get: " << s << std::endl;
        state = s;
    }

    bool DAQManager::getStatus() const {
        return state;
    }

    bool DAQManager::isActive() const {
        return state;
    }
    void DAQManager::activate() {
        state = true;
    }
    void DAQManager::deactivate() {
        state = false;
    }

    void DAQManager::addEvent(BAnalysisManager::DetectedEvent* event){
        Events.push_back(event);
    }

    void DAQManager::addAWaveCatcher(BWaveCatcher* wc){
        wavecatchers.push_back(wc);
    }
    void DAQManager::addAWaveCatcher(std::vector<BWaveCatcher*> wcs){
        for(auto& wc : wcs)
            addAWaveCatcher(wc);
    }


    std::vector<BAnalysisManager::DetectedEvent*> DAQManager::getEvents(){
        return Events;
    }

    void DAQManager::dumpEvents(std::ostream& out){
        for(auto& ev : Events){
            ev->dumpEvent();
        }
    }

    void DAQManager::dumpEvents(std::vector<BAnalysisManager::DetectedEvent*>& events, std::ostream& out){
        for(auto& ev : events){
            ev->dumpEvent();
        }
    }

    void DAQManager::init(){
        Events.clear();
        wavecatchers.clear();
    }

    BWaveCatcher* DAQManager::getWavaCatcherByName(std::string name){
        for(auto& wc : wavecatchers)
            if(wc->getName() == name)    
                return wc;
        return new BWaveCatcher("default");
    }

    BWaveCatcher* DAQManager::getWavaCatcherByIndex(int index){
        if(wavecatchers[index]->getName() != "default")
            return wavecatchers[index];
        return nullptr;
    }

    std::vector<BAnalysisManager::Hit*> DAQManager::getHitsByChannelID(BWaveCatcher* wc, int channel){
        std::vector<BAnalysisManager::Hit*> hits;
        if(wc->getName() == "default")
            return hits;
        if(wc->getOMByChannelID(channel).isOK){
            for(auto& evt : Events){
                std::cout << "\r\033[44C" << "Event size: " << Events.size() << std::endl;
                for(auto& hit : evt->hits){
                    if(hit.om_id == channel + 1){
                        hits.push_back(&hit);
                    } 
                }
            }
        return hits;
        }
        return hits;
    }   

    void DAQManager::dump(std::ostream& out){
        out << string(10, '-') <<  " WaveCatchers in use " << string(10, '-') << "\n\n";
        for(auto& wc : wavecatchers){
            out <<"\nWaveCatcher " << wc->getID() << " - Name: " << wc->getName() << "\n";
            out << "OMs Conneceted: \n\t(";
            for(int i=0; i < wc->getConfiguration().NumChannels; i++){
                out << "ch" + std::to_string(i) << " => " << wc->getOMByChannelID(i).getOMID();
                if(i == wc->getConfiguration().NumChannels -1){
                    out << ")"; 
                }  
                else{
                    out << ", ";
                }  
            }
            out << std::endl;
        }
    }


} // namespace DAQ

