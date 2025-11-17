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
#if 0
// Example Reply
{
    "ok": true,
    "channel": "C09RU2URYMS",
    "ts": "1763354325.527129",
    "message":
    {
        "user": "U09S3D8R00Z",
        "type": "message",
        "ts": "1763354325.527129",
        "bot_id": "B09RJ4A000K",
        "app_id": "A09RQFXSEKC",
        "text": "I see: Time",
        "team": "T095XJHH589",
        "bot_profile":
        {
            "id": "B09RJ4A000K",
            "app_id": "A09RQFXSEKC",
            "user_id": "U09S3D8R00Z",
            "name": "ThorsSlackBotOne",
            "icons":
            {
                "image_36": "https://a.slack-edge.com/80588/img/plugins/app/bot_36.png",
                "image_48": "https://a.slack-edge.com/80588/img/plugins/app/bot_48.png",
                "image_72": "https://a.slack-edge.com/80588/img/plugins/app/service_72.png"
            },
            "deleted": false,
            "updated": 1762644664,
            "team_id": "T095XJHH589"
        },
        "blocks":
        [
            {
                "type": "rich_text",
                "block_id": "aRB4i",
                "elements":
                [
                    {
                        "type": "rich_text_section",
                        "elements":
                        [
                            {
                                "type": "text",
                                "text": "I see: Time"
                            }
                        ]
                    }
                ]
            }
        ]
    }
}
#endif

#if 0
struct Attachment
{
    std::string     pretext;
    std::string     text;
};
struct Text
{
    std::string     type;           // plain_text
    std::string     text;
};
struct Block
{
    std::string     type;
    Text            text;
};
    // Required
    std::string     token;                  // Authentication token bearing required scopes. Tokens should be passed as an HTTP Authorization header or alternatively, as a POST parameter.
    std::string     channel;                // An encoded ID or channel name that represents a channel, private group, or IM channel to send the message to. See below for more details.

    // Optional arguments
    bool            as_user;                // (Legacy) Pass true to post the message as the authed user instead of as a bot. Defaults to false. Can only be used by classic apps. See legacy as_user parameter below.
    std::vector<Attachment> attachments;    // A JSON-based array of structured attachments, presented as a URL-encoded string.
    std::vector<Block>      blocks;         // A JSON-based array of structured blocks, presented as a URL-encoded string.
                                            // Example: [{"type": "section", "text": {"type": "plain_text", "text": "Hello world"}}]
    std::string     current_draft_last_updated_ts;  // This field represents the timestamp of the draft's last update at the time this API is called. If the current message is a draft, this field can be provided to ensure synchronization with the server.
    std::string     icon_emoji;             // Emoji to use as the icon for this message. Overrides icon_url.
                                            // Example: :chart_with_upwards_trend:
    URL             icon_url;               // URL to an image to use as the icon for this message.
                                            // Example: http://lorempixel.com/48/48
    bool            link_names;             // Find and link user groups. No longer supports linking individual users; use syntax shown in Mentioning Users instead.
    std::string     markdown_text;          // Accepts message text formatted in markdown. This argument should not be used in conjunction with blocks or text. Limit this field to 12,000 characters.
                                            // Example: **This is bold text**
    JSON            metadata;               // JSON object with event_type and event_payload fields, presented as a URL-encoded string. Metadata you post to Slack is accessible to any app or user who is a member of that workspace.
                                            // Example: {"event_type": "task_created", "event_payload": { "id": "11223", "title": "Redesign Homepage"}}
    bool            mrkdwn;                 // Disable Slack markup parsing by setting to false. Enabled by default.
                                            // Default: true
    Parse           parse;                  // Change how messages are treated. See below.
                                            // Example: full
    bool            reply_broadcast;        // Used in conjunction with thread_ts and indicates whether reply should be made visible to everyone in the channel or conversation. Defaults to false.
    std::string     text;                   // How this field works and whether it is required depends on other fields you use in your API call. See below for more detail.
    std::string     thread_ts;              // Provide another message's ts value to make this message a reply. Avoid using a reply's ts value; use its parent instead.
    bool            unfurl_links;           // Pass true to enable unfurling of primarily text-based content.
    bool            unfurl_media;           // Pass false to disable unfurling of media content.
    std::string     username;               // Set your bot's user name.
#endif

using OptString         = std::optional<std::string>;
struct BotIcon
{
    std::string                 image_36;
    std::string                 image_48;
    std::string                 image_72;
};
struct BotProfile
{
    std::string                 id;
    std::string                 app_id;
    std::string                 user_id;
    std::string                 name;
    BotIcon                     icons;
    bool                        deleted;
    std::time_t                 updated;
    std::string                 team_id;
};
struct Text
{
    std::string                 type;   // plain_text or mrkdwn.
    OptString                   text;
};
struct Element
{
    std::string                 type;
    std::vector<Text>           elements;
};

struct MessageBlock
{
    std::string                 type;
    std::string                 block_id;
    std::vector<Element>        elements;
};
struct Message
{
    std::string                 user;
    std::string                 type;
    std::string                 ts;
    std::string                 bot_id;
    std::string                 app_id;
    std::string                 text;
    std::string                 team;
    BotProfile                  bot_profile;
    std::vector<MessageBlock>   blocks;
};

struct PostMessageReply
{
    bool                        ok;
    std::string                 error;
    std::string                 channel;
    std::string                 ts;
    Message                     message;
};

using OptText       = std::optional<Text>;

struct Block
{
    std::string                 type;
    OptText                     text;
};

using Blocks            = std::vector<Block>;
using OptBlocks         = std::optional<Blocks>;


struct PostMessage
{
    static const std::string api;
    static constexpr bool hasBody = true;
    using Reply = PostMessageReply;
    std::string                 channel;
    OptString                   text;
    OptBlocks                   blocks;
    OptString                   icon_emoji;
    OptString                   username;
    OptString                   thread_ts;
};


}

ThorsAnvil_MakeTrait(ThorsAnvil::Slack::API::Chat::BotIcon, image_36, image_48, image_72);
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::API::Chat::BotProfile, id, app_id, user_id, name, icons, deleted, updated, team_id);
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::API::Chat::Text, type, text);
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::API::Chat::Element, type, elements);
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::API::Chat::MessageBlock, type, elements);
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::API::Chat::Message, user, type, ts, bot_id, app_id, text, team, bot_profile, blocks);
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::API::Chat::PostMessageReply, ok, error, channel, ts, message);
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::API::Chat::Block, type, text);
ThorsAnvil_MakeTrait(ThorsAnvil::Slack::API::Chat::PostMessage, channel, text, blocks, icon_emoji, username, thread_ts);

#endif
