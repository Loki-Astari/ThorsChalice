#include "NisseServer/EventHandlerLibEvent.h"
#include "ThorSerialize/PrinterConfig.h"
#include "ThorsSlackBotConfig.h"
#include "NisseHTTP/Request.h"
#include "NisseHTTP/Response.h"
#include "ThorSerialize/Traits.h"
#include "ThorSerialize/JsonThor.h"
#include "SlackClient.h"
#include "SlackStream.h"
#include "Environment.h"

#include <cstddef>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

// std::string const SLACK_TOKEN = "<TOKEN>";
// REST:
//      https://slack.com/api/METHOD_FAMILY.method.
//      HTTPS, SSL, and TLS v1.2
//
//      GET querystring parameters,
//      POST parameters presented as application/x-www-form-urlencode
//

struct ChallengeResponse
{
    std::string                 challenge;
};

struct ElementPart
{
    std::string                 type;
    std::string                 text;
};
struct Element
{
    std::string                 type;
    std::vector<ElementPart>    elements;
};
struct Block
{
    std::string                 type;
    std::string                 block_id;
    std::vector<Element>        elements;
};
struct Event
{
    std::string                 type;
    std::string                 user;
    std::string                 ts;
    std::string                 client_msg_id;
    std::string                 text;
    std::string                 team;
    std::vector<Block>          blocks;
    std::string                 channel;
    std::string                 event_ts;
    std::string                 channel_type;
};
struct Enterprise
{};
struct Context
{};
struct Authorization
{
    Enterprise*                 enterprise_id;
    std::string                 team_id;
    std::string                 user_id;
    bool                        is_bot;
    bool                        is_enterprise_install;
};
struct Message
{
    std::string                 token;
    std::string                 challenge;
    std::string                 url_verification;
    std::string                 team_id;
    std::string                 context_team_id;
    Context*                    context_enterprise_id;
    std::string                 api_app_id;
    Event                       event;
    std::string                 type;
    std::string                 event_id;
    std::size_t                 event_time;
    std::vector<Authorization>  authorizations;
    bool                        is_ext_shared_channel;
    std::string                 event_context;
};

ThorsAnvil_MakeTrait(ChallengeResponse, challenge);
ThorsAnvil_MakeTrait(ElementPart, type, text);
ThorsAnvil_MakeTrait(Element, type, elements);
ThorsAnvil_MakeTrait(Block, type, block_id, elements);
ThorsAnvil_MakeTrait(Event, type, user, ts, client_msg_id, text, team, blocks, channel, event_ts, channel_type);
ThorsAnvil_MakeTrait(Enterprise);
ThorsAnvil_MakeTrait(Context);
ThorsAnvil_MakeTrait(Authorization, enterprise_id, team_id, user_id, is_bot, is_enterprise_install);
ThorsAnvil_MakeTrait(Message, token, challenge, url_verification, team_id, context_team_id, context_enterprise_id, api_app_id, event, type, event_id, event_time, authorizations, is_ext_shared_channel, event_context);


namespace Ser = ThorsAnvil::Serialize;

const Environment                   environment("/Users/martinyork/Repo/ThorsChalice/src/ThorsSlackBot/.slackenv");
ThorsAnvil::Slack::SlackClient      client(environment.slackToken);
std::string                         botId = client.sendMessage(ThorsAnvil::Slack::AuthTest{}).user_id;
std::map<std::string, int>          messageCount;

void handleEvent(ThorsAnvil::Nisse::HTTP::Request& request, ThorsAnvil::Nisse::HTTP::Response& response)
{
    Message    message;
    request.body() >> Ser::jsonImporter(message);

    std::cout << "Request:      " << request << "\n\n";
    std::cout << "Verification: " << Ser::jsonExporter(message) << "\n\n";

    using ThorsAnvil::Nisse::HTTP::HeaderResponse;
    using namespace std::string_literals;

    /*
     * TODO:
     * =====
     * Validate that the request is timely.
     * and that it is correctly signed.
     * See: https://docs.slack.dev/authentication/verifying-requests-from-slack
     */

    if (message.challenge != "") {
        HeaderResponse  headers;
        headers.add("Content-Type", "application/json; charset=utf-8");

        ChallengeResponse   challangeBack{message.challenge};

        std::size_t         challangeBackSize = Ser::jsonStreanSize(challangeBack);

        response.addHeaders(headers);
        response.body(challangeBackSize) << Ser::jsonExporter(challangeBack, Ser::PrinterConfig{Ser::OutputType::Stream});
        return;
    }

    std::cout << "Message: " << Ser::jsonExporter(message) << "\n\n";

    std::string const& userId = message.event.user;
    if (userId != botId) {
        ++messageCount[userId];
        std::string const&  channel = message.event.channel;
        std::string         text = "I see: " + message.event.text;

        client.sendMessage(ThorsAnvil::Slack::PostMessageData{channel, text}, ThorsAnvil::Nisse::HTTP::Method::POST);
    }
}
void handleCommand(ThorsAnvil::Nisse::HTTP::Request& request, ThorsAnvil::Nisse::HTTP::Response& response)
{
    std::string const& userId = request.variables()["user_id"];
    std::string const& channel = request.variables()["channel_id"];

    client.sendMessage(ThorsAnvil::Slack::PostMessageData{channel, "I have seen " + std::to_string(messageCount[userId])}, ThorsAnvil::Nisse::HTTP::Method::POST);
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
