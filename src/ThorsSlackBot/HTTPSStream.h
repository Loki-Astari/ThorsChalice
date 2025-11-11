#ifndef THORSANVIL_NISSE_NISSEHTTP_HTTPSSTREAM_H
#define THORSANVIL_NISSE_NISSEHTTP_HTTPSSTREAM_H

#include "ThorsSlackBotConfig.h"
#include "ThorsSocket/SocketStream.h"

#include <string>
#include <ostream>
#include <istream>

namespace ThorsAnvil::Nisse::HTTP
{

class HTTPRequest;

class HTTPSStream
{
    ThorsAnvil::ThorsSocket::SSLctx            ctx;
    ThorsAnvil::ThorsSocket::SocketStream      stream;

    static std::string getHost(std::string const& url);
    static int getPort(std::string const& url);

    public:
        HTTPSStream(std::string const& url);

        friend std::ostream& operator<<(HTTPSStream& stream, HTTPRequest const& request);

        operator std::ostream&() {return stream;}
        operator std::istream&() {return stream;}
};

}

#endif
