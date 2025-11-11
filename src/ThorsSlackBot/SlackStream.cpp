#include "SlackStream.h"

#include <utility>

using namespace ThorsAnvil::Slack;

SlackStream::SlackStream()
    : HTTPSStream("slack.com")
{}
