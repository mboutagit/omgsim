
#include <iostream>
#include <vector>
#include "TGraph.h"
#include <TApplication.h>


using namespace std;

int  main(int argc, char** argv)
{

    TApplication app("ROOTApp", &argc, argv);

std::vector<std::pair<double, double>> AngularEfficiency =
{
    { .012217304, 0.7755460},
    { .024434609, 0.8219338},
    { .036651914, 0.8683680},
    { .048869219, 0.9164099},
    { .061086523, 0.9611203},
    { .073303828, 0.9638412},
    { .085521133, 0.9765568},
    { .097738438, 0.9919360},
    { .109955742, 1},
};

int size=AngularEfficiency.size();
std::vector<double> x(size), y(size);
for (int i=0; i < size; i++){
    x[i] = AngularEfficiency[i].first * 180 / 3.145;
    y[i] = AngularEfficiency[i].second;
}

TGraph* graph = new TGraph(size, x.data(), y.data());
graph->SetTitle("Antares OM Angular Efficiency; Angle [Deg]; Efficiency");
graph->Draw();

    app.Run();


return 0;
}
