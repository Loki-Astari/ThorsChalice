#include "ThorsSlackBot.h"
#include "NisseHTTP/Util.h"
#include "ThorSerialize/JsonThor.h"
#include "ThorSerialize/Traits.h"
#include "ThorsCrypto/hash.h"
#include "ThorsCrypto/hmac.h"
#include "ThorsSlack/SlackAPI_Auth.h"
#include "ThorsSlack/SlackAPI_Chat.h"
#include "WelcomeMessage.h"
#include <string_view>
#include <vector>
#include <algorithm>
#include <cmath>
#include <ctime>


SlackBot    slackBot;

extern "C" void* chaliceFunction()
{
    return dynamic_cast<ThorsAnvil::ThorsChalice::ChalicePlugin*>(&slackBot);
}

namespace Ser       = ThorsAnvil::Serialize;
namespace NisHTTP   = ThorsAnvil::Nisse::HTTP;

SlackBot::SlackBot()
    : environment("/Users/martinyork/Repo/ThorsChalice/src/ThorsSlackBot/.slackenv")
    , client(environment.slackToken)
    , botId{client.sendMessage(ThorsAnvil::Slack::API::Auth::Test{}).user_id}
{}

void SlackBot::registerHandlers(NisHttp::HTTPHandler& handler, std::string const& /*name*/)
{
    handler.addPath(NisHTTP::Method::POST, "/event",           [&](NisHTTP::Request& request, NisHTTP::Response& response){handleEvent(request, response);});
    handler.addPath(NisHTTP::Method::POST, "/command/speak",   [&](NisHTTP::Request& request, NisHTTP::Response& response){handleCommand(request, response);});
}

bool SlackBot::validateRequest(NisHTTP::Request& request)
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

void SlackBot::handleUrlVerification(NisHTTP::Request& /*request*/, ThorsAnvil::Slack::Event::Challenge::Event const& event, NisHTTP::Response& response)
{
    NisHTTP::HeaderResponse  headers;
    headers.add("Content-Type", "application/json; charset=utf-8");

    ThorsAnvil::Slack::Event::Challenge::Response   reply{event.challenge};

    std::size_t         challangeBackSize = Ser::jsonStreanSize(reply);

    response.addHeaders(headers);
    response.body(challangeBackSize) << Ser::jsonExporter(reply, Ser::PrinterConfig{Ser::OutputType::Stream});
    return;
}

void SlackBot::sendWelcomeMessage(std::string const& channel, std::string const& user)
{
    auto find = welcomeMessages.find(std::make_pair(channel, user));
    if (find == std::end(welcomeMessages)) {
        auto result = welcomeMessages.emplace(std::make_pair(channel, user), ThorsAnvil::Slack::WelcomeMessage{channel, user});
        find = result.first;
    }

    auto message = find->second.getMessage();
    std::cerr << "Message: " << ThorsAnvil::Serialize::jsonExporter(message) << "\n=====\n";
    auto response = client.sendMessage(message);
    std::cerr << "Response: " << ThorsAnvil::Serialize::jsonExporter(response) << "\n====\n";
    if (response.ts.has_value()) {
        find->second.timestamp = std::stoi(response.ts.value());
    }
}

void SlackBot::handleEventCallback(NisHTTP::Request& /*request*/, ThorsAnvil::Slack::Event::Message::Event const& event, NisHTTP::Response& /*response*/)
{
    std::string const& userId = event.event.user;
    if ((userId != "") && (userId != botId)) {
        ++messageCount[userId];
        //std::string const&  channel = event.event.channel;
        std::string channel = "@" + userId;
        std::string         text = "I see: " + event.event.text;

        client.sendMessage(ThorsAnvil::Slack::API::Chat::PostMessage{.channel = channel, .text = text}, NisHTTP::Method::POST);
        if (event.event.text == "start") {
            sendWelcomeMessage(event.event.channel, userId);
        }
    }
}

void SlackBot::handleEvent(NisHTTP::Request& request, NisHTTP::Response& response)
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

    using NisHTTP::HeaderResponse;
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

void SlackBot::handleCommand(NisHTTP::Request& request, NisHTTP::Response& response)
{
    std::string const& userId = request.variables()["user_id"];
    std::string const& channel = request.variables()["channel_id"];

    client.sendMessage(ThorsAnvil::Slack::API::Chat::PostMessage{.channel = channel, .text = "I have seen " + std::to_string(messageCount[userId])}, NisHTTP::Method::POST);
    response.setStatus(200);
}
