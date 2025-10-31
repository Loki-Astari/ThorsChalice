#include "ChaliceCLA.h"

using namespace ThorsAnvil::ThorsChalice;

const Paths             ChaliceCLA::searchPath = {"./chalice.cfg", "/etc/chalice.cfg", "/opt/homebrew/etc/chalice.cfg"};
const VerbosityMap      ChaliceCLA::verbosity  = {
                                                    {"All",     loguru::Verbosity_9},
                                                    {"Trace",   loguru::Verbosity_8},
                                                    {"Debug",   loguru::Verbosity_6},
                                                    {"Info",    loguru::Verbosity_INFO},
                                                    {"Warn",    loguru::Verbosity_WARNING},
                                                    {"Error",   loguru::Verbosity_ERROR},
                                                 };

SplitArg ChaliceCLA::splitArgument(std::string_view arg)
{
    std::size_t  find = arg.find('=');
    std::size_t  endF = find == std::string::npos ? arg.size() : find;
    std::size_t  argS = find == std::string::npos ? arg.size() : find + 1;
    bool         hasValue = find != std::string::npos;

    std::string_view        flag(std::begin(arg), std::begin(arg) + endF);
    std::string_view        param(std::begin(arg) + argS, std::end(arg));

    return {flag, param, hasValue};
}

void ChaliceCLA::parseArguments(std::vector<std::string_view> const& arguments)
{
    bool first = true;

    for (std::string_view const& arg: arguments)
    {
        if (first) {
            first = false;
            continue;
        }
        SplitArg  const argVal = splitArgument(arg);

        if (argVal.flag == "--help")
        {
            args.setHelp();
            continue;
        }
        if (argVal.flag == "--silent")
        {
            args.setSilent();
            continue;
        }
        if (argVal.flag == "--logFile")
        {
            args.logAddFile(argVal.value);
            continue;
        }
        if (argVal.flag == "--logSys")
        {
            args.logAddSys(!argVal.hasValue ? arguments[0] : argVal.value);
            continue;
        }
        if (argVal.flag == "--logLevel")
        {
            auto find = verbosity.find(argVal.value);
            if (find == std::end(verbosity)) {
                // Invalid Verbosity
                args.setHelp();
            }
            else {
                // Note: Default log level is loguru::Verbosity_3
                // See:  ThorsLogging/ThorsLogging.h
                args.logSetLevel(find->second);
            }
            continue;
        }
        if (argVal.flag == "--config")
        {
            setConfig = true;
            args.setConfigFile(argVal.value);
            continue;
        }

        // Invalid Flag;
        args.setHelp();
    }
}

void ChaliceCLA::displayHelp(std::string_view command, std::ostream& output)
{
    output << "Usage: " << command << " [--help] [--silent] [--logLevel=(All|Trace|Debug|Info|Warn|Error)] [--config=<configFile>]\n"
           << R"(
--help:     prints out the help.
--silent:   does not spint startup information.
--logLevel: sets the logging level on all information sent via ThorsLogging.
--logFile:  add a file that log messages will be sent to.
--logSys:   send messages to syslog.
--config:   specifies a specific config file.

If no config file is explicitly specified then the following files are checked in order to see If they exist:
)";
    for (auto const& file: searchPath) {
         output << "\t" << file << "\n";
    }

    output << R"(
Chalice config is loaded from the config file.
It is an error when:
    * the config file does not exist
    * has an invalid format.
    * contains zero config objects

The server will refuse to start when there is an error.

)";

}

ChaliceCLA::ChaliceCLA(std::vector<std::string_view> const& arguments, ChaliceCLAInterface& args)
    : args(args)
{
    parseArguments(arguments);

    if (!setConfig)
    {
        for (auto path: searchPath)
        {
            if (std::filesystem::exists(path))
            {
                args.setConfigFile(path);
                break;
            }
        }
    }
}
