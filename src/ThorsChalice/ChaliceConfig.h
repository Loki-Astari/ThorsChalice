#ifndef THORSANVIL_THORS_CHALICE_CHALICE_CONFIG_H
#define THORSANVIL_THORS_CHALICE_CHALICE_CONFIG_H

#include <string>
#include <vector>
#include <filesystem>

#include "ThorSerialize/Traits.h"
#include "ThorSerialize/SerUtil.h"

namespace FS = std::filesystem;

namespace ThorsAnvil::ThorsChalice
{

enum class ActionType {File, Lib};


struct Action
{
    std::string                 path;
    ActionType                  type;
    std::string                 value;
};

struct PortConfig
{
    int                         port;
    std::optional<std::string>  certPath;
    std::string                 rootDir;
    std::vector<Action>         actions;
};

struct ChaliceConfig
{
    int                         controlPort = 8079;
    std::vector<PortConfig>     servers;
};

}

ThorsAnvil_MakeTrait(ThorsAnvil::ThorsChalice::Action, path, type, value);
ThorsAnvil_MakeTrait(ThorsAnvil::ThorsChalice::PortConfig, port, certPath, rootDir, actions);
ThorsAnvil_MakeTrait(ThorsAnvil::ThorsChalice::ChaliceConfig, servers, controlPort);

#endif
