#ifndef THORSANVIL_THORS_CHALICE_CHALICE_CLA_H
#define THORSANVIL_THORS_CHALICE_CHALICE_CLA_H

#include "ChaliceArgs.h"

#include <string_view>
#include <vector>
#include <map>
#include <filesystem>
#include <iostream>

#include "ThorsLogging/ThorsLogging.h"

namespace ThorsAnvil::ThorsChalice
{

struct SplitArg
{
    std::string_view    flag;
    std::string_view    value;
    bool                hasValue;
};

using Paths         = std::vector<FSys::path>;
using VerbosityMap  = std::map<std::string_view, loguru::Verbosity>;

class ChaliceCLA
{
    static const Paths          searchPath;
    static const VerbosityMap   verbosity;

    ChaliceCLAInterface&        args;
    bool                        setConfig = false;

    SplitArg    splitArgument(std::string_view arg);
    void        parseArguments(std::vector<std::string_view> const& arguments);
    public:
        ChaliceCLA(std::vector<std::string_view> const& arguments, ChaliceCLAInterface& args);

        void displayHelp(std::string_view command, std::ostream& output = std::cout);
};

}

#endif
