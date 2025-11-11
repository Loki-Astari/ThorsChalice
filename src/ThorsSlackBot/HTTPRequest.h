#ifndef THORSANVIL_NISSE_NISSEHTTP_HTTPREQUEST_H
#define THORSANVIL_NISSE_NISSEHTTP_HTTPREQUEST_H

#include "ThorsSlackBotConfig.h"
#include "NisseHTTP/Util.h"
#include "NisseHTTP/URL.h"
// NOTE: Should move headers to UTIL
//       Then this include is not required.
#include "NisseHTTP/Response.h"

#include <ostream>
#include <string>

namespace ThorsAnvil::Nisse::HTTP
{

class HTTPRequest
{
    static std::string getHost(std::string const& url);
    static std::string_view getRequest(std::string const& url);

    protected:
        Method              method;
        Version             version;
        std::string         url;
        std::ostream&       baseStream;
        StreamOutput        stream;
        bool                headerSent;

    public:
        HTTPRequest(std::ostream& baseStream, std::string url, Method method = Method::GET, Version version = Version::HTTP1_1);
        ~HTTPRequest();

        void addHeaders(Header const& headers);
        std::ostream& body(BodyEncoding bodyEncoding);
};

}

#endif
