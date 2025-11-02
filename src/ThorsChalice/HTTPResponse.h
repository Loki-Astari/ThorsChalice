#ifndef THORSANVIL_THORSSOCKET_HTTP_RESPONSE_H
#define THORSANVIL_THORSSOCKET_HTTP_RESPONSE_H

#include <cctype>
#include <cstddef>
#include <map>
#include <sstream>
#include <istream>
#include <string>
#include <string_view>

#include <iostream>

namespace ThorsAnvil::ThorsSocket
{

class HTTPResponse
{
    using Headers = std::map<std::string, std::string>;

    int         code;
    std::string message;
    Headers     headers;
    std::string body;

    public:

        int                 getCode()       const {return code;}
        std::string_view    getMessage()    const {return message;}
        std::string_view    getBody()       const {return body;}
        std::string_view    operator[](std::string_view header) const
        {
            auto find = headers.find(std::string(header));
            if (find == std::end(headers)) {
                return {""};
            }
            return find->second;
        }

        void recv(std::istream& stream)
        {
            std::string     version;
            if (!(stream >> version >> code && std::getline(stream, message) && version == "HTTP/1.1")) {
                std::cerr << "Bad Input\n";
                return;
            }
            // Remove the '\r'
            message.resize(message.size() - 1);

            std::string     line;
            std::string     header;
            std::string     value;
            while (std::getline(stream, line)) {
                if (line == "\r") {
                    break;
                }
                std::stringstream   lineStream(std::move(line));


                while (std::getline(lineStream, header, ':') && (lineStream >> std::ws) && std::getline(lineStream, value)) {
                    // Remove the '\r'
                    value.resize(value.size() - 1);
                    std::transform(std::begin(header), std::end(header), std::begin(header), []( char x){return std::tolower(x);});
                    headers[header] = value;
                    std::cout << "HEADER: >" << header << "< => >" << value << "<\n";
                }
            }
            std::string    length   = std::string((*this)["content-length"]);
            if (length.size() != 0) {
                readData(stream, std::stoul(length));
            }
            std::string_view    encoding = (*this)["transfer-encoding"];
            if (encoding == "chunked") {
                readChunked(stream);
            }
        }
    private:
        void readData(std::istream& stream, std::size_t size)
        {
            body.resize(size);
            stream.read(&body[0], size);
        }
        void readChunked(std::istream& stream)
        {
            std::string     chunkSizeStr;
            while (std::getline(stream, chunkSizeStr) && chunkSizeStr.back() == '\r') {
                std::size_t     chunkSize = std::stoul(chunkSizeStr, nullptr, 16);
                std::size_t     bodySize = body.size();
                body.resize(bodySize + chunkSize);
                stream.read(&body[bodySize], chunkSize);
                stream.ignore(2);
                if (chunkSize == 0) {
                    break;
                }
            }
        }


        friend std::istream& operator>>(std::istream& stream, HTTPResponse& data) {data.recv(stream);return stream;}
};

}


#endif // !THORSANVIL_THORSSOCKET_HTTP_SEND
