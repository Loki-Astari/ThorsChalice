#include "ChaliceCLA.h"

#include <iostream>


using namespace ThorsAnvil::ThorsChalice;

ChaliceCLA::ChaliceCLA(std::vector<std::string> const& arguments)
{
    for (auto const& arg: arguments) {
        std::cout << "Arg: " << arg << "\n";
    }
}
