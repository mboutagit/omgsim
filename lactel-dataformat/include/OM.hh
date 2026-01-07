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
        
        private:
            struct OMsParameters
            {
                double photo_cathode_area = 500; // in cm²
                double TTS_mean  = 0.;
                double TTS_sigma = 0.;
                double TTS_amplitude = 0.;
            };

            struct OMProperties
            {
                double high_voltage   = 0.;
                double gain           = 0.;
                double rise_time      = 0.;
                double fall_time      = 0.; 
                double width_time     = 0.;
                double dark_count     = 0.;
                double peak_to_valley = 0.;

                bool operator<(const OMProperties& p) const { return high_voltage < p.high_voltage;}
                bool operator>(const OMProperties& p) const { return high_voltage > p.high_voltage;}
                bool operator==(const OMProperties& p) const { return high_voltage == p.high_voltage;}
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
            std::set<OMProperties>  properties; 
            std::string getOMID() const;
            int getOMNumber() const;
            void uploadOMValuesFormDB();
            bool isOK;

            bool operator<(const OM& om) const;
            bool operator>(const OM& om) const;
            bool operator==(const OM& om) const;
            bool operator!() const {return ! isOK;}
        
        private:
            static ConnectionInfos con;
    };    
} // namespace lactel


#endif