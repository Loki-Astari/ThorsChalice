#include "Environment.h"
#include "ThorsSlack/SlackClient.h"
#include "ThorsSlack/SlackAPI_Chat.h"
#include "ThorsSlack/SlackBlockKit.h"
#include "ThorSerialize/JsonThor.h"
#include "ThorSerialize/PrinterConfig.h"

using namespace std::literals::string_literals;
namespace BK = ThorsAnvil::Slack::BlockKit;

int main()
{
    loguru::g_stderr_verbosity = 9;
    const Environment     environment(".slackenv");
    ThorsLogDebug("main", "main", "SlackCLI ", environment.slackToken);

    using ThorsAnvil::Slack::SlackClient;
    using ThorsAnvil::Slack::API::Chat::PostMessage;

    SlackClient             client(environment.slackToken);
    PostMessage::Reply      reply = client.sendMessage(PostMessage{.channel = "C09RU2URYMS", .text = "I hope the tour went well, Mr. Wonka."});
    std::cout << ThorsAnvil::Serialize::jsonExporter(reply);
}
