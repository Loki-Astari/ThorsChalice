#ifndef THORSANVIL_THORS_CHALICE_CHALICE_SERVER_H
#define THORSANVIL_THORS_CHALICE_CHALICE_SERVER_H

#include "ChaliceArgs.h"

namespace ThorsAnvil::ThorsChalice
{

class ChaliceServer
{
    public:
        ChaliceServer(ChaliceArgs const& arguments);
        void run();
};

}

#endif
