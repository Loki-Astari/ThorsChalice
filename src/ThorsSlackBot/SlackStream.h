#ifndef THORSANVIL_SLACK_SLACKSTREAM_H
#define THORSANVIL_SLACK_SLACKSTREAM_H

#include "ThorsSlackBotConfig.h"
#include "HTTPSStream.h"
#include "ThorSerialize/Traits.h"
#include "ThorSerialize/SerUtil.h"
#include "ThorsLogging/ThorsLogging.h"

#include <cstddef>
#include <string>
#include <ostream>

namespace ThorsAnvil::Slack
{

class SlackStream: public ThorsAnvil::Nisse::HTTP::HTTPSStream
{
    public:
        SlackStream();
};

struct PostMessageData
{
    std::string             channel;
    std::string             text;
};

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

struct Reply
{
    bool                    ok;
    std::string             channel;
    std::string             ts;
    Message                 message;
};


}

ThorsAnvil_MakeTrait(ThorsAnvil::Slack::PostMessageData, channel, text);
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::BotIcon, image_36, image_48, image_72);
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::BotProfile, id, app_id, user_id, name, icons, deleted, updated, team_id);
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::Item, type, text);
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::Element, type, elements);
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::Block, type, block_id, elements);
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::Message, user, type, ts, bot_id, app_id, text, team, bot_profile, blocks);
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::Reply, ok, channel, ts, message);

#endif
