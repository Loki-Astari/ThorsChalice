#ifndef THORSANVIL_SLACK_API_CHAT_H
#define THORSANVIL_SLACK_API_CHAT_H

#include "ThorsSlackBotConfig.h"
#include "ThorSerialize/Traits.h"
#include "ThorSerialize/SerUtil.h"

#include <string>
#include <vector>

namespace ThorsAnvil::Slack::API::Chat
{

// Documentation: https://docs.slack.dev/reference/methods/chat.postmessage

struct BotIcon
{
    std::string             image_36;
    std::string             image_48;
    std::string             image_72;
};
struct BotProfile
{
    std::string             id;
    std::string             app_id;
    std::string             user_id;
    std::string             name;
    BotIcon                 icons;
    bool                    deleted;
    std::size_t             updated;
    std::string             team_id;
};
struct Item
{
    std::string             type;
    std::string             text;
};
struct Element
{
    std::string             type;
    std::vector<Item>       elements;
};
struct Block
{
    std::string             type;
    std::string             block_id;
    std::vector<Element>    elements;
};
struct Message
{
    std::string             user;
    std::string             type;
    std::string             ts;
    std::string             bot_id;
    std::string             app_id;
    std::string             text;
    std::string             team;
    BotProfile              bot_profile;
    std::vector<Block>      blocks;
};

struct PostMessageReply
{
    bool                    ok;
    std::string             channel;
    std::string             ts;
    Message                 message;
};

struct PostMessage
{
    static const std::string api;
    static constexpr bool hasBody = true;
    using Reply = PostMessageReply;
    std::string             channel;
    std::string             text;
};


}

ThorsAnvil_MakeTrait(ThorsAnvil::Slack::API::Chat::BotIcon, image_36, image_48, image_72);
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::API::Chat::BotProfile, id, app_id, user_id, name, icons, deleted, updated, team_id);
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::API::Chat::Item, type, text);
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::API::Chat::Element, type, elements);
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::API::Chat::Block, type, block_id, elements);
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::API::Chat::Message, user, type, ts, bot_id, app_id, text, team, bot_profile, blocks);
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::API::Chat::PostMessageReply, ok, channel, ts, message);
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::API::Chat::PostMessage, channel, text);

#endif
