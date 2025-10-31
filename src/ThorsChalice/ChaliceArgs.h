#ifndef THORSANVIL_THORS_CHALICE_CHALICE_ARGS_H
#define THORSANVIL_THORS_CHALICE_CHALICE_ARGS_H

#include <string_view>
#include <filesystem>

#include "ThorsLogging/ThorsLogging.h"


namespace FSys = std::filesystem;

namespace ThorsAnvil::ThorsChalice
{

struct ChaliceCLAInterface
{
    virtual void setHelp()                              = 0;
    virtual void setSilent()                            = 0;
    virtual void setConfigFile(FSys::path file)         = 0;
    virtual void logAddFile(FSys::path file)            = 0;
    virtual void logAddSys(std::string_view app)        = 0;
    virtual void logSetLevel(loguru::Verbosity level)   = 0;
};

struct ChaliceArgs: public ChaliceCLAInterface
{
    bool        help    = false;
    bool        silent  = false;
    FSys::path  configPath;

    virtual void setHelp()                              override;
    virtual void setSilent()                            override;
    virtual void setConfigFile(FSys::path file)         override;
    virtual void logAddFile(FSys::path file)            override;
    virtual void logAddSys(std::string_view app)        override;
    virtual void logSetLevel(loguru::Verbosity level)   override;
};

}

#endif
