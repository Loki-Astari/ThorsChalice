#ifndef THORSANVIL_THORS_CHALICE_CHALICE_SERVER_H
#define THORSANVIL_THORS_CHALICE_CHALICE_SERVER_H

#include "ChaliceConfig.h"

namespace ThorsAnvil::ThorsChalice
{

enum ChaliceServerMode {Headless, Active};
class ChaliceServer
{
    public:
        ChaliceServer(ChaliceConfig const& config, ChaliceServerMode mode);
        void run();
};

}

#endif
