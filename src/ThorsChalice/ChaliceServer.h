#ifndef THORSANVIL_THORS_CHALICE_CHALICE_SERVER_H
#define THORSANVIL_THORS_CHALICE_CHALICE_SERVER_H

#include "ChaliceCLA.h"

namespace ThorsAnvil::ThorsChalice
{

class ChaliceServer
{
    public:
        ChaliceServer(ChaliceCLA const& arguments);
        void run();
};

}

#endif
