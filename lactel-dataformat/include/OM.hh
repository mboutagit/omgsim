#ifndef OM_hh 
#define OM_hh 

#include <iostream>
#include <set>
#include <stdexcept>
#include "dbRoutines.hh"
#include "BLogger.hh"

namespace lactel
{

    class OM{
        
        public:
            struct OMsParameters
            {
                double photo_cathode_area = 500; // in cm²
                double TTS_mean  = 0.;
                double TTS_sigma = 0.;
                double TTS_amplitude = 0.;
            };

            struct OMProperties
            {
                double hv = 1800;
                double g  = 5e7;
                double rt = 4.;
                double ft = 11.; 
                double wt = 10.;
                double dc = 1000.;
                double pv = 3.5;

                bool operator<(const OMProperties& p) const { return hv < p.hv;}
                bool operator>(const OMProperties& p) const { return hv > p.hv;}
                bool operator==(const OMProperties& p) const { return hv == p.hv;}
            };

            int id;
            std::string om_id;
        
        public:
            OM();
            OM(std::string om_id);
            // OM(OM& om);
            ~OM();

            static int nInstance;
            static std::set<std::string> om_ids;
            static OMsParameters parameters;
            OMProperties properties; 
            std::string getOMID() const;
            int getOMNumber() const;
            void uploadOMValuesFormDB();
            bool isOK;

            bool operator<(const OM& om) const;
            bool operator>(const OM& om) const;
            bool operator==(const OM& om) const;
            bool operator!() const {return ! isOK;}

            void dump(){
                std::cout << "\r\033[10C\033[1,35m" 
                << " - hv - "<< " - g - "<< " - rt - "<< " - ft - "<< " - wt - "<< " - dc - "<< " - pv - "
                << std::endl << "\033[10C"
                <<  properties.hv << " - " <<  properties.g << " - " <<  properties.rt << " - " 
                <<  properties.ft << " - " <<  properties.wt << " - " <<  properties.dc << " - " <<  properties.pv
                << std::endl;
            }
        
        private:
            static ConnectionInfos con;
    };    
} // namespace lactel


#endif