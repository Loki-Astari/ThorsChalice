#ifndef THORSANVIL_SLACK_WELCOME_MESSAGE_H
#define THORSANVIL_SLACK_WELCOME_MESSAGE_H

#include <ctime>
#include <string>
#include "SlackAPI_Chat.h"

namespace ThorsAnvil::Slack
{

class WelcomeMessage
{
    static API::Chat::Block const startText;
    static API::Chat::Block const divider;

    std::string     channel;
    std::string     user;
    std::string     iconEmoji;
    public:
    std::time_t     timestamp;
    private:
    bool            completed;

    public:
        WelcomeMessage(std::string channel, std::string user)
            : channel(std::move(channel))
            , user(std::move(user))
            , iconEmoji(":robot_face:")
            , timestamp(std::time(nullptr))
            , completed(false)
        {}

        API::Chat::PostMessage getMessage()
        {
            return {    .channel = channel,
                        .text = "Plop",
                        .blocks = API::Chat::Blocks{startText, divider, getReactionTask()},
                        //.blocks = API::Chat::Blocks{{"section", {"mrkdwn", R"(Welcome to this awesome channel! -- *Get started by completing the tasks!*)"}}},
                        //.blocks = API::Chat::Blocks{{"section", {"plain_text", "Hello world"}}},
                        .icon_emoji = iconEmoji,
                        .username = "Welcome Robot!",
                   };
        }
    private:
        API::Chat::Block getReactionTask()
        {
            std::string checkmark = ":white_check_mark:";
            if (not completed) {
                checkmark = ":white_large_square:";
            }
            std::string text = checkmark + " *React to this message!*";

            return {.type = "section", .text = API::Chat::Text{"mrkdwn", text}};
        }
};

}

#endif
