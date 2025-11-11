#ifndef THORSANVIL_NISSE_NISSEHTTP_HTTPRESPONSE_H
#define THORSANVIL_NISSE_NISSEHTTP_HTTPRESPONSE_H

#include "ThorsSlackBotConfig.h"
#include "NisseHTTP/Util.h"
// NOTE: Should move headers to UTIL
//       Then this include is not required.
#include "NisseHTTP/Response.h"

#include <istream>
#include <string>
#include <map>

namespace ThorsAnvil::Nisse::HTTP
{

struct StatusResponse
{
    int                 code;
    std::string         message;
};

class HTTPResponse
{
    protected:
        Version         version;
        StatusResponse  status;
        std::map<std::string, std::string>  headers;

    public:
        HTTPResponse(std::istream& baseStream);
        void print(std::ostream& stream) const;
        friend std::ostream& operator<<(std::ostream& stream, HTTPResponse const& data) {data.print(stream);return stream;}
};

}

#endif
