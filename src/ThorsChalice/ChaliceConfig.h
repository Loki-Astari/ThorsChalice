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
    ActionType                  type;
    std::string                 rootDir;
    std::string                 path;
    // For File:
    //      rootDir:        The directory on the file system where files will be loaded from.
    //      path:           registered with Nisse to match against input request.
    //          The file loaded from the server will be "<rootDir>/<path>" (after normalization)
    //          Note: If path tries to dip below root (using ../../ etc) it will log an error and return a 400
    // For Lib:
    //      rootDir:        The path to the shared library.
    //      path:           registered with Nisse to match against input request.
};

struct PortConfig
{
    int                         port;
    std::optional<std::string>  certPath;
    std::vector<Action>         actions;
};

struct ChaliceConfig
{
    int                         controlPort         = 8079;
    int                         libraryCheckTime    = 0;
    std::vector<PortConfig>     servers;
};

}

ThorsAnvil_MakeTrait(ThorsAnvil::ThorsChalice::Action, type, rootDir, path);
ThorsAnvil_MakeTrait(ThorsAnvil::ThorsChalice::PortConfig, port, certPath, actions);
ThorsAnvil_MakeTrait(ThorsAnvil::ThorsChalice::ChaliceConfig, servers, controlPort, libraryCheckTime);

#endif
