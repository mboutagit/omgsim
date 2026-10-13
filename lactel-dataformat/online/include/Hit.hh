#pragma once

#include "OM.hh"

using namespace lactel;

class Hit {
    private:
        long id = 0;
        float time;
        float charge;
        OM om;

    public:
        Hit(/* args */);
        ~Hit();
};
