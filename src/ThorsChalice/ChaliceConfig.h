#ifndef THORSANVIL_THORS_CHALICE_CHALICE_CONFIG_H
#define THORSANVIL_THORS_CHALICE_CHALICE_CONFIG_H

#include <string>
#include <vector>

#include "ThorSerialize/Traits.h"
#include "ThorSerialize/SerUtil.h"


namespace ThorsAnvil::ThorsChalice
{

enum class ActionType {File, Lib};


struct Action
{
    std::string         path;
    ActionType          type;
    std::string         value;
};

struct Config
{
    int                 port;
    std::vector<Action> actions;
};

}

ThorsAnvil_MakeTrait(ThorsAnvil::ThorsChalice::Action, path, type, value);
ThorsAnvil_MakeTrait(ThorsAnvil::ThorsChalice::Config, port, actions);

#endif
