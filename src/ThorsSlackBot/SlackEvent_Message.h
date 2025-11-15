#ifndef THORSANVIL_SLACK_EVENT_MESSAGE_H
#define THORSANVIL_SLACK_EVENT_MESSAGE_H

// Documentation: https://docs.slack.dev/reference/events/message

#include "ThorSerialize/Traits.h"
#include "ThorSerialize/SerUtil.h"
#include <string>
#include <vector>

namespace ThorsAnvil::Slack::Event::Message
{

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
struct EventData
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
    Enterprise*                 enterprise_id   = nullptr;
    std::string                 team_id;
    std::string                 user_id;
    bool                        is_bot;
    bool                        is_enterprise_install;
};
struct Event
{
    std::string                 token;
    std::string                 challenge;
    std::string                 url_verification;
    std::string                 team_id;
    std::string                 context_team_id;
    Context*                    context_enterprise_id = nullptr;
    std::string                 api_app_id;
    EventData                       event;
    std::string                 type;
    std::string                 event_id;
    std::size_t                 event_time;
    std::vector<Authorization>  authorizations;
    bool                        is_ext_shared_channel;
    std::string                 event_context;
};

}

ThorsAnvil_MakeTrait(ThorsAnvil::Slack::Event::Message::ElementPart, type, text);
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::Event::Message::Element, type, elements);
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::Event::Message::Block, type, block_id, elements);
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::Event::Message::EventData, type, user, ts, client_msg_id, text, team, blocks, channel, event_ts, channel_type);
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::Event::Message::Enterprise);
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::Event::Message::Context);
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::Event::Message::Authorization, enterprise_id, team_id, user_id, is_bot, is_enterprise_install);
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::Event::Message::Event, token, challenge, url_verification, team_id, context_team_id, context_enterprise_id, api_app_id, event, type, event_id, event_time, authorizations, is_ext_shared_channel, event_context);

#endif
