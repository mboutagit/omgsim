#define NoiseCoincidences_cxx
#include "NoiseCoincidences.hh"
#include "RunInfo.hh"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <bitset>
#include <iostream>
#include <vector>
#include <TVector3.h>

#include <TH1F.h>
#include <TH2F.h>
#include <TGraph.h>
#include <TString.h>

using namespace std;

TH2F* hpairlist;
TH1F* hcoincidences;
TH1F* hPhotonsInPM;
TGraph* gAngularCoincidences=0;

//new version
float PMTPositions[31][2] = {
  {0.,0.}
  ,{0.56,0.524}, {0.56,1.571},{0.56,2.62},{0.56,3.665},{0.56,4.712},{0.56,5.76}
  ,{0.98,0.},{0.98,1.047},{0.98,2.094},{0.98,3.142}, {0.98,4.1889},{0.98,5.236}
  ,{1.27,0.524}, {1.27,1.571},{1.27,2.62},{1.27,3.665},{1.27,4.712},{1.27,5.76}
  ,{1.872,5.236},{1.872,0.},{1.872,1.047},{1.872,2.094},{1.872,3.142},{1.872,4.1889}
  ,{2.162,5.76},{2.162,0.524},{2.162,1.571},{2.162,2.62},{2.162,3.665},{2.162,4.712}
};
                            //*/
  /*  //old version
  {{0.,0.}
   ,{0.56,0.}, {0.56,1.047},{0.56,2.094},{0.56,3.142},{0.56,4.1889},{0.56,5.236}
   ,{0.98,0.524},{0.98,1.571},{0.98,2.62},{0.98,3.665}, {0.98,4.712},{0.98,5.76}
   ,{1.27,0.}, {1.27,1.047},{1.27,2.094},{1.27,3.142},{1.27,4.1889},{1.27,5.236}
   ,{1.872,0.524},{1.872,1.571},{1.872,2.62},{1.872,3.665},{1.872,4.712},{1.872,5.76}
   ,{2.162,0.},{2.162,1.047},{2.162,2.094},{2.162,3.142},{2.162,4.1889},{2.162,5.236}
  };//*/

unsigned myPow(unsigned x, unsigned p) {
  if (p == 0) return 1;
  if (p == 1) return x;
  return x * myPow(x, p-1);
}

unsigned ConvertPMMask (unsigned PM)
{
  if (PM == 0)
    return 0;
  return myPow (2,PM-1);
}

unsigned GetPairId (unsigned PM1, unsigned PM2)
{
  if (PM1 > PM2)
    swap (PM1,PM2);

  // if (PM2 == 21)
  //   swap (PM1,PM2);


  //       unsigned j=PM2-PM1;

  //       unsigned tmp=0;
  //       for (int it=0;it<j-1;it++)
  //         tmp+=it;

  //       return (j-1)*31-tmp+PM1;


  //return (j-1)*31-tmp+PM1+1;
  unsigned PMR=(31-PM2);
  return 31*PMR-(PMR+1)*PMR/2+(PM2-PM1);

}

#include <random>
std::default_random_engine generator;
std::normal_distribution<double> distribution(0,1.8);

void FillPairHistogramm (const float* Times)
{
  for (int first=1; first<31;first++)
    for (int second=first+1; second<32;second++)
      {
        if (Times[first] < 0. || Times[second] < 0)
          continue;
        double dTTS = distribution(generator)-distribution(generator);

        hpairlist->Fill(dTTS+Times[first]-Times[second], GetPairId (first, second));
        //hpairlist->Fill(Times[second]-Times[first], GetPairId (second, first));
      }


}

TH1D* hpairlistProj;

void GetAngularCoincidences ()
{
  gAngularCoincidences=new TGraph;
  gAngularCoincidences->SetTitle("Angular coincidences;ns");
  gAngularCoincidences->SetName("AngularCoincidences");
  int pointcounter=0;

  hpairlistProj=hpairlist->ProjectionY("hpairlistProj");

  for (int PM1=1; PM1<31;PM1++)
    for (int PM2=PM1+1; PM2<32;PM2++)
      {
        TVector3 tvPM1(0,0,1),tvPM2(0,0,1);
        tvPM1.SetMagThetaPhi(1,PMTPositions[PM1-1][0],PMTPositions[PM1-1][1]);
        tvPM2.SetMagThetaPhi(1,PMTPositions[PM2-1][0],PMTPositions[PM2-1][1]);
        float angle=tvPM2.Angle(tvPM1);
        int bin=hpairlistProj->FindBin (GetPairId(PM1,PM2));
        gAngularCoincidences->SetPoint(pointcounter++, angle, hpairlistProj->GetBinContent (bin));
        if (angle < 1)
          cout << "for PMs " << PM1 << ' ' << PM2<< " the angle is " << angle<< " the id is "<< GetPairId (PM1,PM2) << " and the value is " << hpairlistProj->GetBinContent(bin)<<endl;
      }
}

NoiseCoincidences::NoiseCoincidences (int argc, char** argv)
{
  SetSourceFileName (argv[1]);
  if (argc >2)
    SetOutPutFileName(argv[2]);
}

unsigned NoiseCoincidences::Loop()
{
   if (fChain == 0) return 0;

   Long64_t nentries = fChain->GetEntriesFast();

   float previousTime;
   unsigned previousPMID;

   unsigned PMMask=0;
   unsigned CurrentPMMask=0;

   unsigned tphotons=0;
   unsigned HitCounter=0;

   float HitTimePerPM[32];
   memset (HitTimePerPM,0xaa,sizeof(HitTimePerPM)); //put everything to less than 0

   ULong64_t CurrentHitNumber=0xffffffff;
   unsigned short coincidences=0;

   hcoincidences=new TH1F("hcoincidences","Folds",30,0,30);
   hPhotonsInPM=new TH1F("hPhotonsInPM","Folds",2000,0,2000);
   hpairlist=new TH2F("hpairlist","coincidences time per pair id",90,-15,15,500,0,500);

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries
          ;jentry++) {
     if (jentry%100 == 0)
       {
         cout << "\r" <<jentry;
         cout.flush ();
       }
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      if (Hit_Limit == 4) continue;
      if (HitNumber != CurrentHitNumber)
        {
          hPhotonsInPM->Fill(tphotons);
          FillPairHistogramm (HitTimePerPM);
          tphotons=0;
          HitCounter++;
          //cout << coincidences << endl;
          hcoincidences->Fill (coincidences);
          coincidences =0;
          CurrentHitNumber=HitNumber;
          PMMask=0;
          memset (HitTimePerPM,0xaa,sizeof(HitTimePerPM)); //put everything to less than 0
          //previousTime=0;
          //previousPMID=Vertex_PMID;
        }
      else
        {
          tphotons++;
          CurrentPMMask=ConvertPMMask(Vertex_PMID);
          //cout << "CurrentPMMask "<< hex << CurrentPMMask << " " <<dec<< Vertex_PMID << endl;
          if (! (PMMask & CurrentPMMask))
            {
              /*if (previousTime!=0)
                {
                  hpairlist->Fill(previousTime-Hit_TotalTime,GetPairId (previousPMID, Vertex_PMID));
                  }*/
              HitTimePerPM[Vertex_PMID]=Hit_TotalTime;
              //previousTime=Hit_TotalTime;
              //previousPMID=Vertex_PMID;

              coincidences++;
              PMMask = CurrentPMMask | PMMask;
            }
        }

   }
   return HitCounter;

}

void NoiseCoincidences::DoIt ()
{
  unsigned NbHit=Loop ();

  RunInfo ri (fFileName);
  unsigned NBTrigg=ri.Loop ();

  cout << NbHit << endl;

  unsigned NbNoCoin=NBTrigg-NbHit;
  hcoincidences->Fill (0., NbNoCoin);

  hcoincidences->Scale (1./NBTrigg);

  GetAngularCoincidences ();

  TFile output(fOutputName.data (),"recreate");
  output.Add(hcoincidences);
  output.Add(hpairlist);
  output.Add(hPhotonsInPM);
  output.Add(gAngularCoincidences);
  output.Add(hpairlistProj);

  output.Write ();
  output.Close ();

}
