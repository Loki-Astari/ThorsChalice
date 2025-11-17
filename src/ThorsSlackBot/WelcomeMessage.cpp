#include "WelcomeMessage.h"


using namespace ThorsAnvil::Slack;

API::Chat::Block const WelcomeMessage::startText =
{
    "section",
    API::Chat::Text
    {
        "mrkdwn",
        R"(Welcome to this awesome channel! -- *Get started by completing the tasks!*)"
    }
};

API::Chat::Block const WelcomeMessage::divider =
{
    .type = "divider"
};
