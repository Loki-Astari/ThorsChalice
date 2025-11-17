#ifndef THORSANVIL_SLACK_THORSSLACKBOT_H
#define THORSANVIL_SLACK_THORSSLACKBOT_H

#include "../ThorsChalice/DLLib.h"
#include "Environment.h"
#include "WelcomeMessage.h"
#include "NisseHTTP/Request.h"
#include "NisseHTTP/Response.h"
#include "SlackClient.h"
#include "SlackEvent_Message.h"
#include "SlackEvent_Challenge.h"
#include <string>
#include <map>

class SlackBot: public ThorsAnvil::ThorsChalice::ChalicePlugin
{
        using WelcomeMessage = ThorsAnvil::Slack::WelcomeMessage;

        const Environment                       environment;
        ThorsAnvil::Slack::SlackClient          client;
        std::string                             botId;
        std::map<std::string, int>              messageCount;
        std::map<std::pair<std::string, std::string>, WelcomeMessage>   welcomeMessages;

        bool validateRequest(ThorsAnvil::Nisse::HTTP::Request& request);
        void handleUrlVerification(ThorsAnvil::Nisse::HTTP::Request& request, ThorsAnvil::Slack::Event::Challenge::Event const& event, ThorsAnvil::Nisse::HTTP::Response& response);
        void handleEventCallback(ThorsAnvil::Nisse::HTTP::Request& request, ThorsAnvil::Slack::Event::Message::Event const& event, ThorsAnvil::Nisse::HTTP::Response& response);
        void handleEvent(ThorsAnvil::Nisse::HTTP::Request& request, ThorsAnvil::Nisse::HTTP::Response& response);
        void handleCommand(ThorsAnvil::Nisse::HTTP::Request& request, ThorsAnvil::Nisse::HTTP::Response& response);

        void sendWelcomeMessage(std::string const& channel, std::string const& user);
    public:
        SlackBot();

        virtual void registerHandlers(NisHttp::HTTPHandler& handler, std::string const& name) override;

};

#endif
