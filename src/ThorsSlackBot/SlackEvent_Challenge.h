#ifndef THORSANVIL_SLACK_EVENT_CHALLENGE_H
#define THORSANVIL_SLACK_EVENT_CHALLENGE_H

#include "ThorSerialize/Traits.h"
#include <string>

namespace ThorsAnvil::Slack::Event::Challenge
{

struct Response
{
    std::string                 challenge;
};

struct Event
{
    std::string                 token;
    std::string                 challenge;
    std::string                 type;
};

}

ThorsAnvil_MakeTrait(ThorsAnvil::Slack::Event::Challenge::Response, challenge);
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::Event::Challenge::Event, token, challenge, type);

#endif
