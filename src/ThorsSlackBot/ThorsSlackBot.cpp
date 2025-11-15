#include "Environment.h"
#include "NisseHTTP/Request.h"
#include "NisseHTTP/Response.h"
#include "ThorSerialize/JsonThor.h"
#include "ThorSerialize/Traits.h"
#include "ThorsCrypto/hash.h"
#include "ThorsCrypto/hmac.h"
#include "SlackClient.h"
#include "SlackAPI_Auth.h"
#include "SlackAPI_Chat.h"
#include "SlackEvent_Message.h"
#include "SlackEvent_Challenge.h"
#include <string>
#include <string_view>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>
#include <ctime>

namespace Ser = ThorsAnvil::Serialize;

const Environment                   environment("/Users/martinyork/Repo/ThorsChalice/src/ThorsSlackBot/.slackenv");
ThorsAnvil::Slack::SlackClient      client(environment.slackToken);
std::string                         botId = client.sendMessage(ThorsAnvil::Slack::API::Auth::Test{}).user_id;
std::map<std::string, int>          messageCount;

bool validateRequest(ThorsAnvil::Nisse::HTTP::Request& request)
{
    std::string const&  key = environment.slackSecret;
    std::string const&  sig = request.variables()["x-slack-signature"];
    std::string const&  timestampStr = request.variables()["x-slack-request-timestamp"];
    auto                timestamp = std::stoll(timestampStr);
    auto                versionEnd = std::min(std::size(sig), sig.find('='));

    if (std::abs(std::time(nullptr) - timestamp) > (60 * 5)) {
        return false;
    }

    using namespace std::literals::string_literals;
    using HMac = ThorsAnvil::Crypto::HMacBuilder<ThorsAnvil::Crypto::Sha256>;

    ThorsAnvil::Crypto::Digest<ThorsAnvil::Crypto::Sha256>      digest;
    {
        HMac hmac(key, digest);
        hmac.appendData(std::string_view{std::begin(sig), std::begin(sig) + versionEnd});
        hmac.appendData(":"s);
        hmac.appendData(timestampStr);
        hmac.appendData(":"s);

        std::string_view    body = request.preloadStreamIntoBuffer();
        hmac.appendData(body);
    }
    std::size_t versionNext = versionEnd + (versionEnd == std::size(sig) ? 0 : 1);
    return ThorsAnvil::Crypto::hexdigest<ThorsAnvil::Crypto::Sha256>(digest) == std::string_view{std::begin(sig) + versionNext, std::end(sig)};
}

void handleUrlVerification(ThorsAnvil::Nisse::HTTP::Request& /*request*/, ThorsAnvil::Slack::Event::Challenge::Event const& event, ThorsAnvil::Nisse::HTTP::Response& response)
{
    ThorsAnvil::Nisse::HTTP::HeaderResponse  headers;
    headers.add("Content-Type", "application/json; charset=utf-8");

    ThorsAnvil::Slack::Event::Challenge::Response   reply{event.challenge};

    std::size_t         challangeBackSize = Ser::jsonStreanSize(reply);

    response.addHeaders(headers);
    response.body(challangeBackSize) << Ser::jsonExporter(reply, Ser::PrinterConfig{Ser::OutputType::Stream});
    return;
}

void handleEventCallback(ThorsAnvil::Nisse::HTTP::Request& /*request*/, ThorsAnvil::Slack::Event::Message::Event const& event, ThorsAnvil::Nisse::HTTP::Response& /*response*/)
{
    std::string const& userId = event.event.user;
    if (userId != botId) {
        ++messageCount[userId];
        std::string const&  channel = event.event.channel;
        std::string         text = "I see: " + event.event.text;

        client.sendMessage(ThorsAnvil::Slack::API::Chat::PostMessage{channel, text}, ThorsAnvil::Nisse::HTTP::Method::POST);
    }
}

void handleEvent(ThorsAnvil::Nisse::HTTP::Request& request, ThorsAnvil::Nisse::HTTP::Response& response)
{
    /*
     * TODO:
     * =====
     * Validate that the request is timely.
     * and that it is correctly signed.
     * See: https://docs.slack.dev/authentication/verifying-requests-from-slack
     */
    if (!validateRequest(request)) {
        response.setStatus(400);
        return;
    }

    using ThorsAnvil::Nisse::HTTP::HeaderResponse;
    using namespace std::string_literals;
    //std::cerr << "Recieved: Message\n";
    //std::cerr << request << "\n";

    std::string_view    body = request.preloadStreamIntoBuffer();
    if (body.find(R"("type":"url_verification")") != std::string_view::npos) {
        ThorsAnvil::Slack::Event::Challenge::Event   event;
        request.body() >> ThorsAnvil::Serialize::jsonImporter(event);
        handleUrlVerification(request, event, response);
        return;
    }
    if (body.find(R"("type":"event_callback")") != std::string_view::npos) {
        ThorsAnvil::Slack::Event::Message::Event    event;
        request.body() >> ThorsAnvil::Serialize::jsonImporter(event);
        handleEventCallback(request, event, response);
        return;
    }
    response.setStatus(404);
}

void handleCommand(ThorsAnvil::Nisse::HTTP::Request& request, ThorsAnvil::Nisse::HTTP::Response& response)
{
    std::string const& userId = request.variables()["user_id"];
    std::string const& channel = request.variables()["channel_id"];

    client.sendMessage(ThorsAnvil::Slack::API::Chat::PostMessage{channel, "I have seen " + std::to_string(messageCount[userId])}, ThorsAnvil::Nisse::HTTP::Method::POST);
    response.setStatus(200);
}

void handle(ThorsAnvil::Nisse::HTTP::Request& request, ThorsAnvil::Nisse::HTTP::Response& response)
{
    if (request.variables()["Command"] == "event") {
        handleEvent(request, response);
        return;
    }
    if (request.variables()["Command"] == "command/speak") {
        handleCommand(request, response);
        return;
    }
    response.setStatus(404);
}

extern "C"
{
    typedef void(*GenericFuncPtr)();
}

extern "C" GenericFuncPtr chaliceFunction()
{
    GenericFuncPtr  result = (GenericFuncPtr)&handle;
    return result;
}
