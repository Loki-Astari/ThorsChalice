#include "Environment.h"

#include <fstream>
#include "ThorSerialize/JsonThor.h"

Environment::Environment(std::string const& fileName)
{
    std::ifstream   file(fileName);
    file >> ThorsAnvil::Serialize::jsonImporter(*this);
}
