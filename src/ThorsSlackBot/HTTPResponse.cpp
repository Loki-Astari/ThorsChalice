#include "HTTPResponse.h"
#include <string>

using namespace ThorsAnvil::Nisse::HTTP;

    /*
    protected:
        Version         version;
        StatusCode      status;
        std::map<std::string, std::string>  headers;
    */
namespace ThorsAnvil::Nisse::HTTP
{
    std::istream& operator>>(std::istream& stream, StatusResponse& data)
    {
        stream >> data.code;
        std::getline(stream, data.message);
        return stream;
    }
    std::ostream& operator<<(std::ostream& stream, StatusResponse const& data)
    {
        return stream << data.code << " " << data.message;
    }
}

HTTPResponse::HTTPResponse(std::istream& stream)
{
    std::string     line;
    if (stream >> version >> status)
    {
        std::string     line;
        while (std::getline(stream, line)) {
            if (line == "\r") {
                break;
            }
            auto colon = std::min(std::size(line), line.find(':'));
            auto value = colon == std::size(line) ? colon : line.find_first_not_of(" ", colon + 1);

            headers.insert_or_assign(line.substr(0, colon), line.substr(value));
        }
    }
}

void HTTPResponse::print(std::ostream& stream) const
{
    stream << version << " " << status << "\r\n";
    for (auto const& head: headers) {
        stream << head.first << ": " << head.second << "\r\n";
    }
    stream << "\r\n";
}
