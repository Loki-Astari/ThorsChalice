#include "Environment.h"
#include "HTTPRequest.h"
#include "HTTPResponse.h"
#include "NisseHTTP/HeaderResponse.h"
#include "SlackStream.h"
#include "ThorSerialize/JsonThor.h"

using namespace std::literals::string_literals;

int main()
{
    loguru::g_stderr_verbosity = 9;
    Environment     environment(".slackenv");
    ThorsLogDebug("main", "main", "SlackCLI ", environment.slackToken);

    using ThorsAnvil::Nisse::HTTP::HTTPRequest;
    using ThorsAnvil::Nisse::HTTP::HTTPResponse;
    using ThorsAnvil::Nisse::HTTP::Method;
    using ThorsAnvil::Nisse::HTTP::HeaderResponse;
    using ThorsAnvil::Serialize::PrinterConfig;
    using ThorsAnvil::Serialize::OutputType;
    using ThorsAnvil::Slack::SlackStream;
    using ThorsAnvil::Slack::PostMessageData;
    using ThorsAnvil::Slack::Reply;

    HeaderResponse  headers;
    headers.add("Connection", "close");
    headers.add("Content-Type", "application/json; charset=utf-8");
    headers.add("Authorization", "Bearer " + environment.slackToken);

    PostMessageData data{"C09RU2URYMS", "I hope the tour went well, Mr. Wonka."};

    SlackStream     stream;
    {
        HTTPRequest     post(stream, "https://slack.com/api/chat.postMessage"s, Method::POST);
        post.addHeaders(headers);
        std::size_t size = ThorsAnvil::Serialize::jsonStreanSize(data);
        post.body(size) << ThorsAnvil::Serialize::jsonExporter(data, PrinterConfig{OutputType::Stream});
    }

    HTTPResponse    response(stream);
    Reply           reply;
    stream >> ThorsAnvil::Serialize::jsonImporter(reply);

    std::cout << response << "\n\n";
    std::cout << ThorsAnvil::Serialize::jsonExporter(reply);
}
