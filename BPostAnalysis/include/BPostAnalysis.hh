#ifndef __BPOSTANALYSIS_HH__
#define __BPOSTANALYSIS_HH__

#include "BAnalysis.hh"

namespace km3net{

    typedef unsigned int uint;

    struct PhotonProduction
    {   
        uint run_id;
        uint evt_id;
        float VertexX=0;
        float VertexY=0;
        float VertexZ=0;
        float VertexT=0;
        float VertexP=0;
        float VertexDirX=0;
        float VertexDirY=0;
        float VertexDirZ=0;
        float VertexEnergy=0;
        float PhotoX=0;
        float PhotoY=0;
        float PhotoZ=0;
        float PhotoT=0;
        float PhotoP=0;
        short OMId=-1;
        short PMID=0;
    };

    struct Hit{

        float fTotalTime=0;
        float fEnergy=0;
        bool fPolarity =0;
        unsigned char fLimit=0;
    };

    struct PhotonArrival
    {
        float OMHitX=0;
        float OMHitY=0;
        float OMHitZ=0;
        float OMHitT=0;
        float OMHitP=0;
    };

} // namespace km3net
#endif