#ifndef THORSANVIL_SLACK_API_AUTH_H
#define THORSANVIL_SLACK_API_AUTH_H

#include "ThorsSlackBotConfig.h"
#include "ThorSerialize/Traits.h"
#include "ThorSerialize/SerUtil.h"

#include <string>

namespace ThorsAnvil::Slack::API::Auth
{

// Documentation: https://docs.slack.dev/reference/methods/auth.test/

struct TestReply
{
    bool                    ok;
    std::string             url;
    std::string             team;
    std::string             user;
    std::string             team_id;
    std::string             user_id;
    std::string             bot_id;
    std::string             error;
};
struct Test
{
    static const std::string api;
    static constexpr bool hasBody = false;
    using Reply = TestReply;
};

}
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::API::Auth::TestReply, ok, url, team, user, team_id, user_id, bot_id, error);


#endif
