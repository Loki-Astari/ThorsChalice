#include "ThorsChalice.h"
#include "ChaliceCLA.h"
#include "ChaliceServer.h"

#include <iostream>

int main(int argc, char* argv[])
{
    using ThorsAnvil::ThorsChalice::ChaliceCLA;
    using ThorsAnvil::ThorsChalice::ChaliceServer;

    std::cout << "ThorsChalice\n";
    try
    {
        ChaliceCLA          arguments(std::vector<std::string>(argv + 1, argv + argc));
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
