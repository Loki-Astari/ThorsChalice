#include "ThorsChalice.h"
#include "ChaliceArgs.h"
#include "ChaliceCLA.h"
#include "ChaliceServer.h"

#include <iostream>

int main(int argc, char* argv[])
{
    using ThorsAnvil::ThorsChalice::ChaliceArgs;
    using ThorsAnvil::ThorsChalice::ChaliceCLA;
    using ThorsAnvil::ThorsChalice::ChaliceServer;

    std::cout << "ThorsChalice\n";
    try
    {
        ChaliceArgs     arguments;
        ChaliceCLA      parser(std::vector<std::string_view>(argv + 1, argv + argc), arguments);
{
    if (arguments.configPath.empty()) {
        ThorsLogAndThrowCritical("ChaliceArgs", "loadConfig", "No config file set. Not explicitly set and default ones don't exist");
    }
    if (!FSys::exists(arguments.configPath)) {
        ThorsLogAndThrowCritical("ChaliceArgs", "loadConfig", "Specified config file does not exist. Config File: ", configPath);
    }
    using ThorsAnvil::Serialize::jsonImporter;
    using ThorsAnvil::Serialize::ParserConfig;
    using ThorsAnvil::Serialize::ParseType;
    std::ifstream   configStream(configPath);
    configStream >> jsonImporter(config, ParserConfig{ParseType::Exact});
}
        if (arguments.help) {
            parser.displayHelp(argv[0]);
            return 0;
        }
        ChaliceServer       server(arguments);
        server.run();
    }
    catch (std::exception const& e)
    {
        std::cout << "Exception: " << e.what() << "\n";
        throw;
    }
    catch (...)
    {
        std::cout << "Exception: Uknowne\n";
        throw;
    }
}
