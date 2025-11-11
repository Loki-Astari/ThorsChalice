#include "NisseServer/EventHandlerLibEvent.h"
#include "ThorSerialize/PrinterConfig.h"
#include "ThorsSlackBotConfig.h"
#include "NisseHTTP/Request.h"
#include "NisseHTTP/Response.h"
#include "ThorSerialize/Traits.h"
#include "ThorSerialize/JsonThor.h"

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


/*

Your Response:
"code":
"error": "challenge_failed"
"body": {
}
*/

namespace Ser = ThorsAnvil::Serialize;

void handle(ThorsAnvil::Nisse::HTTP::Request& request, ThorsAnvil::Nisse::HTTP::Response& response)
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
