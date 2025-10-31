#include "ThorsChalice.h"
#include "ChaliceArgs.h"
#include "ChaliceCLA.h"
#include "ChaliceServer.h"
#include "ChaliceConfig.h"

#include "ThorSerialize/JsonThor.h"
#include "ThorsLogging/ThorsLogging.h"

#include <iostream>
#include <fstream>

int main(int argc, char* argv[])
{
    using ThorsAnvil::ThorsChalice::ChaliceCLA;
    using ThorsAnvil::ThorsChalice::ChaliceArgs;
    using ThorsAnvil::ThorsChalice::ChaliceConfig;
    using ThorsAnvil::ThorsChalice::ChaliceServer;
    using ThorsAnvil::ThorsChalice::ChaliceServerMode;

    std::cout << "ThorsChalice\n";
    try
    {
        ChaliceArgs     arguments;
        ChaliceCLA      parser(std::vector<std::string_view>(argv + 1, argv + argc), arguments);

        if (arguments.help) { /// TODO
            parser.displayHelp(argv[0]);
            return 0;
        }

        if (arguments.configPath.empty()) {
            ThorsLogAndThrowCritical("ThorsChalice", "main", "No config file set. Not explicitly set and default ones don't exist");
        }
        if (!FSys::exists(arguments.configPath)) {
            ThorsLogAndThrowCritical("ThorsChalice", "main", "Specified config file does not exist. Config File: ", arguments.configPath);
        }

        using ThorsAnvil::Serialize::jsonImporter;
        using ThorsAnvil::Serialize::ParserConfig;
        using ThorsAnvil::Serialize::ParseType;
        std::ifstream   configStream(arguments.configPath);
        ChaliceConfig   config;
        if (!(configStream >> jsonImporter(config, ParserConfig{ParseType::Exact}))) {
            ThorsLogAndThrowCritical("ThorsChalice", "main", "Failed to load config file: ", arguments.configPath);
        }

        ChaliceServer       server(config, arguments.silent ? ChaliceServerMode::Headless : ChaliceServerMode::Active);
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
