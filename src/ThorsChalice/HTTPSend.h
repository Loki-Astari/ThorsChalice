#ifndef THORSANVIL_THORSSOCKET_HTTP_SEND_H
#define THORSANVIL_THORSSOCKET_HTTP_SEND_H

#include <ostream>
#include <string_view>

namespace ThorsAnvil::ThorsSocket
{

enum SendType       {GET, POST};
enum SendVersion    {HTTP1_1};

inline
std::ostream& operator<<(std::ostream& stream, SendType type)
{
    switch (type)
    {
        case GET:   return stream << "GET";
        case POST:  return stream << "POST";
    }
}

inline
std::ostream& operator<<(std::ostream& stream, SendVersion version)
{
    switch (version)
    {
        case HTTP1_1:   return stream << "HTTP/1.1";
    }
}


class HTTPSend
{
    SendType            type;
    SendVersion         version;
    std::string_view    path;
    std::string_view    mimeType;
    std::string_view    host;
    std::string_view    body;

    public:
        HTTPSend(SendType type, SendVersion version, std::string_view host, std::string_view path, std::string_view mimeType = "application/json", std::string_view body = "")
            : type(type)
            , version(version)
            , path(path)
            , mimeType(mimeType)
            , host(host)
            , body(body)
        {}

        void send(std::ostream& stream) const
        {
            stream << type << " " << path << " " << version << "\r\n"
                   << "Host: " << host << "\r\n"
                   << "Accept: " << mimeType << "\r\n";
            if (body.size() != 0) {
                stream << "Content-Length: " << body.size() << "\r\n";
            }
            stream << "\r\n" << body << std::flush;
        }

        friend std::ostream& operator<<(std::ostream& stream, HTTPSend const& data) {data.send(stream);return stream;}
};

}


#endif // !THORSANVIL_THORSSOCKET_HTTP_SEND
