#include "Environment.h"
#include "SlackClient.h"
#include "SlackAPI_Chat.h"
#include "ThorSerialize/JsonThor.h"
#include "SlackBlockKit.h"
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
#if 0
    {
        PostMessage::Reply      reply = client.sendMessage(PostMessage{.channel = "C09RU2URYMS", .text = "I hope the tour went well, Mr. Wonka."});
        std::cout << ThorsAnvil::Serialize::jsonExporter(reply);
    }

    {
        PostMessage::Reply      reply = client.sendMessage(PostMessage{.channel = "C09RU2URYMS", .blocks = BK::Blocks{BK::Section{.text = BK::ElText{.type = BK::mrkdwn, .text = "Stuff to print"}}, BK::Divider{}, BK::Section{.text = BK::ElText{.type = BK::plain_text, .text = "Here we go"}}}});
        std::cout << ThorsAnvil::Serialize::jsonExporter(reply);
    }

    {
        PostMessage::Reply      reply = client.sendMessage(PostMessage{.channel = "C09RU2URYMS", .blocks = BK::Blocks{BK::RichText{.elements = {BK::RichTextSection{.elements = {BK::ElRtText{.text = "Hi there", .style = BK::InfoText{true,true,true,true}}}}}}}});
        std::cout << ThorsAnvil::Serialize::jsonExporter(reply);
    }

    {
        PostMessage::Reply      reply = client.sendMessage(PostMessage{.channel = "C09RU2URYMS", .blocks = BK::Blocks{BK::RichText{.elements = {BK::RichTextList{.style = BK::bullet, .elements = {BK::RichTextSection{.elements = {BK::ElRtText{.text = "Hi there", .style = BK::InfoText{true,true,true,true}}}}}}}}}});
        std::cout << ThorsAnvil::Serialize::jsonExporter(reply);
    }
#endif

    {
#if 0
        std::cerr << ThorsAnvil::Serialize::jsonExporter(PostMessage{.channel = "C09RU2URYMS", .blocks = BK::Blocks{BK::RichText{.elements = {BK::RichTextList{.style = BK::ordered, .elements = {BK::RichTextSection{.elements = {BK::ElRtText{.text = "Hi there", .style = BK::InfoText{true,true,true,true}}}}}}}}}}, ThorsAnvil::Serialize::PrinterConfig{ThorsAnvil::Serialize::OutputType::Stream}) << "\n";
        std::cerr << ThorsAnvil::Serialize::jsonStreanSize(PostMessage{.channel = "C09RU2URYMS", .blocks = BK::Blocks{BK::RichText{.elements = {BK::RichTextList{.style = BK::ordered, .elements = {BK::RichTextSection{.elements = {BK::ElRtText{.text = "Hi there", .style = BK::InfoText{true,true,true,true}}}}}}}}}}) << "\n";
        std::cerr << ThorsAnvil::Serialize::jsonExporter(BK::Blocks{BK::RichText{.elements = {BK::RichTextList{.style = BK::ordered, .elements = {BK::RichTextSection{.elements = {BK::ElRtText{.text = "Hi there", .style = BK::InfoText{true,true,true,true}}}}}}}}}, ThorsAnvil::Serialize::PrinterConfig{ThorsAnvil::Serialize::OutputType::Stream}) << "\n";
        std::cerr << ThorsAnvil::Serialize::jsonStreanSize(BK::Blocks{BK::RichText{.elements = {BK::RichTextList{.style = BK::ordered, .elements = {BK::RichTextSection{.elements = {BK::ElRtText{.text = "Hi there", .style = BK::InfoText{true,true,true,true}}}}}}}}}) << "\n";
        std::cerr << ThorsAnvil::Serialize::jsonExporter(BK::Block{BK::RichText{.elements = {BK::RichTextList{.style = BK::ordered, .elements = {BK::RichTextSection{.elements = {BK::ElRtText{.text = "Hi there", .style = BK::InfoText{true,true,true,true}}}}}}}}}, ThorsAnvil::Serialize::PrinterConfig{ThorsAnvil::Serialize::OutputType::Stream}) << "\n";
        std::cerr << ThorsAnvil::Serialize::jsonStreanSize(BK::Block{BK::RichText{.elements = {BK::RichTextList{.style = BK::ordered, .elements = {BK::RichTextSection{.elements = {BK::ElRtText{.text = "Hi there", .style = BK::InfoText{true,true,true,true}}}}}}}}}) << "\n";
        std::cerr << ThorsAnvil::Serialize::jsonExporter(BK::ElRichTextObj{BK::RichTextList{.style = BK::ordered, .elements = {BK::RichTextSection{.elements = {BK::ElRtText{.text = "Hi there", .style = BK::InfoText{true,true,true,true}}}}}}}, ThorsAnvil::Serialize::PrinterConfig{ThorsAnvil::Serialize::OutputType::Stream}) << "\n";
        std::cerr << ThorsAnvil::Serialize::jsonStreanSize(BK::ElRichTextObj{BK::RichTextList{.style = BK::ordered, .elements = {BK::RichTextSection{.elements = {BK::ElRtText{.text = "Hi there", .style = BK::InfoText{true,true,true,true}}}}}}}) << "\n";
        std::cerr << ThorsAnvil::Serialize::jsonExporter(BK::ElRichTextObj{BK::RichTextSection{.elements = {BK::ElRtText{.text = "Hi there", .style = BK::InfoText{true,true,true,true}}}}}, ThorsAnvil::Serialize::PrinterConfig{ThorsAnvil::Serialize::OutputType::Stream}) << "\n";
        std::cerr << ThorsAnvil::Serialize::jsonStreanSize(BK::ElRichTextObj{BK::RichTextSection{.elements = {BK::ElRtText{.text = "Hi there", .style = BK::InfoText{true,true,true,true}}}}}) << "\n";
        std::cerr << ThorsAnvil::Serialize::jsonExporter(BK::RtElement{BK::ElRtText{.text = "Hi there", .style = BK::InfoText{true,true,true,true}}}, ThorsAnvil::Serialize::PrinterConfig{ThorsAnvil::Serialize::OutputType::Stream}) << "\n";
        std::cerr << ThorsAnvil::Serialize::jsonStreanSize(BK::RtElement{BK::ElRtText{.text = "Hi there", .style = BK::InfoText{true,true,true,true}}}) << "\n";
        std::cerr << ThorsAnvil::Serialize::jsonExporter(BK::InfoText{true,true,true,true}, ThorsAnvil::Serialize::PrinterConfig{ThorsAnvil::Serialize::OutputType::Stream}) << "\n";
        std::cerr << ThorsAnvil::Serialize::jsonStreanSize(BK::InfoText{true,true,true,true}) << "\n";
#endif
        PostMessage::Reply      reply = client.sendMessage(PostMessage{.channel = "C09RU2URYMS", .blocks = BK::Blocks{BK::RichText{.elements = {BK::RichTextList{.style = BK::ordered, .elements = {BK::RichTextSection{.elements = {BK::ElRtText{.text = "Hi there", .style = BK::InfoText{true,true,true,true}}}}}}}}}});
        std::cout << ThorsAnvil::Serialize::jsonExporter(reply);
    }


#if 0
struct PostMessage
{
    std::string                 channel;
    OptString                   text;
    BlockKit::OptBlocks         blocks;
    OptString                   icon_emoji;
    OptString                   username;
    OptString                   thread_ts;
};
struct Section
{
    // https://docs.slack.dev/reference/block-kit/blocks/section-block/
    Element::OptText            text;           // The text for the block, in the form of a text object. Minimum length for the text in this field is 1 and maximum length is 3000 characters.
    Element::OptTexts           fields;         // Required if no text is provided. An array of text objects. Any text objects included with fields will be rendered in a compact format that allows for 2 columns of side-by-side text.
};
#endif

}
