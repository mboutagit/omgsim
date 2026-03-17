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
            static std::set<OM*> instances;
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
            static inline int howMany(){return nInstance;};

            void dump(std::ostream& out = std::cout){
                out << "\033[1;35m" 
                << left << setw(15) << "id" 
                << left << setw(15) << "hv" 
                << left << setw(15) << "g "
                << left << setw(15) << "rt" 
                << left << setw(15) << "ft" 
                << left << setw(15) << "wt" 
                << left << setw(15) << "dc" 
                << left << setw(15) << "pv"
                << "\033[0m"
                << std::endl;

                out << string(120, '-')
                << std::endl;

                out << left << setw(15) << om_id 
                << left << setw(15) << properties.hv 
                << left << setw(15) << properties.g 
                << left << setw(15) << properties.rt
                << left << setw(15) << properties.ft
                << left << setw(15) << properties.wt
                << left << setw(15) << properties.dc
                << left << setw(15) << properties.pv
                << std::endl;
            }
    
    static void dumpOMs(std::ostream& out = std::cout){
        out << string(100, '=') << endl;
        out << "nOMs = " << nInstance << endl;
        for(auto& om : instances)
            om->dump();
        out << string(100, '=') << endl;
    }
        
        private:
            static ConnectionInfos con;
    };    
} // namespace lactel


#endif