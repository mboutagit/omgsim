#include <iostream>
#include <stdlib.h>

#include "CLHEP/Units/PhysicalConstants.h"

using namespace std;

namespace analyse
{

class signalAnaysis
{
    private:
        /* data */
    public:
        signalAnaysis();
        ~signalAnaysis();
        double energyToPE(const double energy){
            return CLHEP::h_Planck;
        }
};
   
} // namespace analyse.


