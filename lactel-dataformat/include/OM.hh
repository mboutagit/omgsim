#ifndef OM_hh 
#define OM_hh 

#include <iostream>
#include <set>
#include <stdexcept>

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
                double gain           = 0.;
                double rise_time      = 0.;
                double fall_time      = 0.; 
                double width_time     = 0.;
                double dark_count     = 0.;
                double peak_to_valley = 0.;
            };

            int id;
            std::string om_id;
        
        public:
            OM();
            OM(std::string om_id);
            ~OM();

            static int nInstance;
            static std::set<std::string> om_ids;
            static OMsParameters parameters;
            OMProperties properties; 
            std::string getOMID(){return om_id;}
            int getOMNumber() const;

            bool operator<(const OM& om) const;
            bool operator>(const OM& om) const;
            bool operator==(const OM& om) const;
    };    
} // namespace lactel


#endif