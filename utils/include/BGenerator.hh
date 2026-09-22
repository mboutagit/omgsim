#pragma once
#include <stdio.h>
#include <string>

#include "TH1.h"
#include "TF1.h"
#include "TF2.h"

namespace utils
{
    using namespace std;

    class BGenerator
    {
        private:
            /* data */
        public:
            BGenerator();
            ~BGenerator();

            static double ModifiedGaisser(double *x, double *par);
            static double FluxRate(double *x, double *par);
            static double RateIntegrand(double *x, double *par);
    };
} // namespace utils
