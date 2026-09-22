#ifndef OM_hh 
#define OM_hh 

#include <iostream>
#include <set>
#include <stdexcept>
#include "dbRoutines.hh"
#include "BLogger.hh"

#include "G4ThreeVector.hh"
#include "G4SystemOfUnits.hh"
#include <random>
#include <numeric>



namespace lactel
{

    struct VolumeProperties
    {
        typedef std::string str;

        str motherVolume;
        str borderSurafce;
        str borderSurfaceMaterial;

        VolumeProperties() : motherVolume("LacWater"), borderSurafce(""), borderSurfaceMaterial("") {}
        VolumeProperties(str mother) : VolumeProperties(){motherVolume = mother;}
        VolumeProperties(str mother, str borderSurf) : VolumeProperties(mother){borderSurafce = borderSurf;}

        inline void setMotherVolName(std::string name){motherVolume = name;}
        inline void setBorderSurfName(std::string name){borderSurafce = name;}
        inline void setBorderSurfMatName(std::string name){borderSurfaceMaterial = name;}

        inline const std::string getMotherVolName(){return motherVolume;}
        inline const std::string getBorderSurfName(){return borderSurafce;}
        inline const std::string getBorderSurfMatName(){return borderSurfaceMaterial;}
    };

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

            struct Geometery {
                G4ThreeVector position = G4ThreeVector();
                G4ThreeVector origin = G4ThreeVector();
                G4ThreeVector rotation = G4ThreeVector();
                G4ThreeVector dimension = G4ThreeVector();

                Geometery() : position(G4ThreeVector()), origin(G4ThreeVector()),
                             rotation(G4ThreeVector()), dimension(G4ThreeVector()){}
                Geometery(G4ThreeVector pos) : Geometery() {position = pos;}
                Geometery(G4ThreeVector pos, G4ThreeVector dim) : Geometery(pos) {dimension = dim;}

                inline void setPosition(G4ThreeVector pos){position = pos;}
                inline void setOrigin(G4ThreeVector org){origin = org;}
                inline void setDimension(G4ThreeVector dim){dimension = dim;}
                inline void setRotation(G4ThreeVector rot){rotation = rot;}

                inline G4ThreeVector getPosition(){return position;}
                inline G4ThreeVector getOrigin(){return origin;}
                inline G4ThreeVector getDimension(){return dimension;}
                inline G4ThreeVector getRotation(){return rotation;}
            };

            int id;
            std::string om_id;
            G4ThreeVector position = G4ThreeVector(0, 0, 0);
            std::string motherVolume="defaultVolume";
        
        public:
            OM();
            OM(std::string om_id);
            // OM(OM& om);
            ~OM();

            friend std::ostream& operator<<(std::ostream& os, const OM& obj);

            static int nInstance;
            static std::set<OM*> instances;
            static std::set<std::string> om_ids;
            static OMsParameters parameters;
            static std::mt19937 rng_;
            static std::uniform_int_distribution<int> noiseDist_;
            static std::normal_distribution<> TTSDist_; // mean=0, sigma=1
            static std::normal_distribution<> AmpDist_; // mean=0, sigma=1
            OMProperties properties; 
            Geometery geometry;
            VolumeProperties volumeProperties;
            std::string name="AntaresOM";


            inline VolumeProperties getVolumeProperties(){return volumeProperties;}
            inline Geometery& getGeometry(){return geometry;}
            std::string getOMID() const;
            int getID() const;
            int getOMNumber() const;
            inline const std::string getName() const {return name;}
            inline void setName(std::string na){name = na;}
            void uploadOMValuesFormDB();
            bool isOK = false;

            inline void setID(std::string id){om_id = id;}
            inline void setID(int uid){id = uid;}

            bool operator<(const OM& om) const;
            bool operator>(const OM& om) const;
            bool operator==(const OM& om) const;
            bool operator!() const {return ! isOK;}
            static inline int howMany(){return nInstance;};

            G4ThreeVector getPosition(){
                return position;
            }

            void setPoition(G4ThreeVector pos){
                position = pos;
            }


            const void dump(std::ostream& out = std::cout) const {
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