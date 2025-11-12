#ifndef BHardwareManager_hh 
#define BHardwareManager_hh 

#include <iostream>
#include <set>
#include "OM.hh"
#include "BWaveCatcher.hh"

using namespace tools;
namespace lactel
{
    class BHardwareManager : public OM {

        private:
            static BHardwareManager* instance;
            BHardwareManager();
            std::set<OM> oms;
            std::set<BWaveCatcher> waveCatchers;


        public:
            BHardwareManager* getInstance();
            void addOM(OM& om);
            void addWaveCatcher(BWaveCatcher& wc);
            void ConnectWaveCatcherToOM(BWaveCatcher& wc, OM& om);
            


    };
} // namespace lactel

#endif