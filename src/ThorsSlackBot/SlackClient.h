#ifndef THORSANVIL_SLACK_SLACKCLIENT_H
#define THORSANVIL_SLACK_SLACKCLIENT_H

#include "Environment.h"
#include "HTTPRequest.h"
#include "HTTPResponse.h"
#include "NisseHTTP/HeaderResponse.h"
#include "NisseHTTP/Util.h"
#include "SlackStream.h"
#include "ThorSerialize/JsonThor.h"
#include <thread>

namespace ThorsAnvil::Slack
{

using namespace std::literals::string_literals;
namespace Ser   = ThorsAnvil::Serialize;
namespace Nisse = ThorsAnvil::Nisse::HTTP;

class SlackClient
{
    Nisse::HeaderResponse   headers;
    public:
        SlackClient(std::string const& token)
        {
            headers.add("Connection", "close");
            headers.add("Content-Type", "application/json; charset=utf-8");
            headers.add("Authorization", "Bearer " + token);
        }

        template<typename T>
        T::Reply  sendMessage(T const& message, Nisse::Method method = Nisse::Method::POST)
        {
            SlackStream             stream;
            //Nisse::HTTPRequest      post(stream, "https://slack.com/api/chat.postMessage"s, method);
            Nisse::HTTPRequest      post(stream, T::api, method);

            post.addHeaders(headers);
            if constexpr (T::hasBody) {
                std::size_t size = Ser::jsonStreanSize(message);
                post.body(size) << Ser::jsonExporter(message, Ser::PrinterConfig{Ser::OutputType::Stream});
            }
            else {
                post.body(0);
            }

            Nisse::HTTPResponse     response(stream);
            typename T::Reply       reply;
            stream >> Ser::jsonImporter(reply);

            return reply;
        }
};

}

#endif
