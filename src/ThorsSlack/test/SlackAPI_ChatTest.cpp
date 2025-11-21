#include "gtest/gtest.h"

#include "Environment.h"
#include "SlackClient.h"
#include "SlackAPI_Chat.h"
#include "ThorSerialize/JsonThor.h"
#include "SlackBlockKit.h"

using namespace std::literals::string_literals;
namespace BK = ThorsAnvil::Slack::BlockKit;

using ThorsAnvil::Slack::SlackClient;
using ThorsAnvil::Slack::API::Chat::PostMessage;

Environment             environment("test/data/environment.json");
SlackClient             client(environment.slackToken);

TEST(SlackAPI_ChatTEST, SimpleText)
{
    PostMessage::Reply      reply = client.sendMessage(PostMessage{.channel = "C09RU2URYMS", .text = "I hope the tour went well, Mr. Wonka."});
    std::cout << ThorsAnvil::Serialize::jsonExporter(reply);
}

TEST(SlackAPI_ChatTEST, Block_Section_ElText)
{
    PostMessage::Reply      reply = client.sendMessage(PostMessage{.channel = "C09RU2URYMS", .blocks = BK::Blocks{BK::Section{.text = BK::ElText{.type = BK::mrkdwn, .text = "Stuff to print"}}, BK::Divider{}, BK::Section{.text = BK::ElText{.type = BK::plain_text, .text = "Here we go"}}}});
    std::cout << ThorsAnvil::Serialize::jsonExporter(reply);
}

TEST(SlackAPI_ChatTEST, Block_RichText_Section_Text)
{
    PostMessage::Reply      reply = client.sendMessage(PostMessage{.channel = "C09RU2URYMS", .blocks = BK::Blocks{BK::RichText{.elements = {BK::RichTextSection{.elements = {BK::ElRtText{.text = "Hi there", .style = BK::InfoText{true,true,true,true}}}}}}}});
    std::cout << ThorsAnvil::Serialize::jsonExporter(reply);
}

TEST(SlackAPI_ChatTEST, Block_RichText_ListBullet_Section_Text)
{
    PostMessage::Reply      reply = client.sendMessage(PostMessage{.channel = "C09RU2URYMS", .blocks = BK::Blocks{BK::RichText{.elements = {BK::RichTextList{.style = BK::bullet, .elements = {BK::RichTextSection{.elements = {BK::ElRtText{.text = "Hi there", .style = BK::InfoText{true,true,true,true}}}}}}}}}});
    std::cout << ThorsAnvil::Serialize::jsonExporter(reply);
}

TEST(SlackAPI_ChatTEST, Block_RichText_ListOrder_Section_Text)
{
    PostMessage::Reply      reply = client.sendMessage(PostMessage{.channel = "C09RU2URYMS", .blocks = BK::Blocks{BK::RichText{.elements = {BK::RichTextList{.style = BK::ordered, .elements = {BK::RichTextSection{.elements = {BK::ElRtText{.text = "Hi there", .style = BK::InfoText{true,true,true,true}}}}}}}}}});
    std::cout << ThorsAnvil::Serialize::jsonExporter(reply);
}

