#include "SlackStream.h"
#include "SlackAPI_Auth.h"
#include "SlackAPI_Chat.h"

#include <utility>

using namespace ThorsAnvil::Slack;

SlackStream::SlackStream()
    : ClientStream("slack.com")
{}


using namespace std::string_literals;

const std::string API::Auth::Test::api         = "https://slack.com/api/auth.test"s;
const std::string API::Chat::PostMessage::api  = "https://slack.com/api/chat.postMessage"s;
