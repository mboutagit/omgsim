#include "BGenerator.hh"


namespace utils
{
    
    using namespace std;

    BGenerator::BGenerator(){};
    BGenerator::~BGenerator(){};
    double BGenerator::ModifiedGaisser(double *x, double *par){
        double E     = x[0];   // GeV
        double theta = x[1];   // rad

        double c = cos(theta);

        const double P1 = 0.102573;
        const double P2 = -0.068287;
        const double P3 = 0.958633;
        const double P4 = 0.0407253;
        const double P5 = 0.817285;

        double cstar = sqrt(
            (c*c +
            P1*P1 +
            P2*pow(c,P3) +
            P4*pow(c,P5))
            /
            (1.0 + P1*P1 + P2 + P4)
        );

        double Eeff = E/(1.0 + 3.64/(E*pow(cstar,1.29)));

        double flux = 0.14*pow(Eeff,-2.7)*
            (
                1.0/(1.0 + 1.1*E*cstar/115.0)
                +
                0.054/(1.0 + 1.1*E*cstar/850.0)
            );

        return flux;
    }

    double BGenerator::FluxRate(double *x, double *par)
    {
        return ModifiedGaisser(x,par)*cos(x[1]);
    }

    double BGenerator::RateIntegrand(double *x, double *par)
    {
        return 2.0*TMath::Pi()
            *sin(x[1])
            *cos(x[1])
            *ModifiedGaisser(x,par);
    }
} // namespace utils
