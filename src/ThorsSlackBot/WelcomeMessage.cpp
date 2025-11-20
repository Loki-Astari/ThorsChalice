#include "WelcomeMessage.h"


using namespace ThorsAnvil::Slack;

BlockKit::Section const WelcomeMessage::startText =
{
    .text = BlockKit::ElText
    {
        .type = BlockKit::mrkdwn,
        .text = R"(Welcome to this awesome channel! -- *Get started by completing the tasks!*)"
    }
};

BlockKit::Divider const WelcomeMessage::divider;
