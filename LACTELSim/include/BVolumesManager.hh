#ifndef BVolumesManager_hh 
#define BVolumesManager_hh 

#include "KM3ParametrizedVolume.hh"
#include "KM3PassiveVolumeMgr.hh"

using namespace std;
using namespace km3net;

namespace lactel
{

    class BVolumesManager
    {
        private:
            BVolumesManager();
            static BVolumesManager* Instance;
            KM3PassiveVolumeMgr& volumes;

        public:

            static BVolumesManager* GetInstance();
            ~BVolumesManager();

            void getListOfVolumeNames(std::ostream& out = std::cout);
    };

} // namespace lactel

    

#endif