#pragma once

#include "BDetectorConstruction.hh"
#include "BPrimaryGeneratorAction.hh"

namespace lactel
{
    class BHeader
    {
    private:
        /* data */
    public:
        BHeader(BDetectorConstruction* detInfo, BPrimaryGeneratorAction* generatorInfo);
        ~BHeader();
    };
    
} // namespace lactel
