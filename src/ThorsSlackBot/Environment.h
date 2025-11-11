#ifndef THORSANVIL_SLACKCLI_ENVIRONMENT_H
#define THORSANVIL_SLACKCLI_ENVIRONMENT_H

#include <string>
#include "ThorSerialize/Traits.h"

struct Environment
{
    std::string     slackToken;
    std::string     slackSecret;
    Environment(std::string const& fileName);
};
ThorsAnvil_MakeTrait(Environment, slackToken, slackSecret);

#endif
