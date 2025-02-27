#ifndef ParametrizedGenerator_hh
#define ParametrizedGenerator_hh

#include <stdlib.h>
#include <iostream>

#include <G4PrimaryParticle.hh>
#include <G4VSolid.hh>

#include <KM3PrimaryGeneratorAction.hh>

using namespace std;
//using namespace km3net;

namespace km3net
{
   
    typedef unsigned int uint;

   // const std::string generator[] = {"GEANT4", "CORSIKA", "PYTHIA", "OTHER"};

    class ParametrizedGenerator
    {    
        
    public:
        enum class gen_mode {GEANT4, CORSIKA, PYTHIA, FILE, OTHER};
        ~ParametrizedGenerator();

        void init(const gen_mode generator);
        bool read();
        void setParticleName(std::string p_name){
            particle_name = p_name;
        }

        void setGenerator(gen_mode gen){
            generator = gen;
        }

        gen_mode getGenerator(){
            return generator;
        }

        std::string getGeneratorName();

        KM3PrimaryGeneratorAction* getPGA(){return pga;}

        void print(std::ostream& out);
        void writeGenData(bool);
        bool isWrite();
        void setEventID(uint ev_id){event_id = ev_id;}

        static ParametrizedGenerator* getInstance();

        ParametrizedGenerator(const ParametrizedGenerator&) = delete;
        ParametrizedGenerator& operator=(const ParametrizedGenerator&) = delete;

        void printHeader(ostream&);

        G4ThreeVector GenerateMuonDirection(std::string);
        G4double GenerateMuonEnergy(double, double, float);
        G4ThreeVector GetRandomPointOnBoxFace(G4VPhysicalVolume*, G4String);


    private:
        gen_mode generator;
        std::string particle_name = "";
        uint event_id;
        int particle_PDG_id;
        G4ThreeVector particle_position;
        G4ThreeVector particle_direction;
        G4double particle_energy;
        KM3PrimaryGeneratorAction* pga;
        bool write_generation_data = false;
        ofstream gen_data;

        ParametrizedGenerator();
        ParametrizedGenerator(const gen_mode generator, KM3PrimaryGeneratorAction* pga);

        static ParametrizedGenerator* instance;

    };

} // namespace genPars

#endif