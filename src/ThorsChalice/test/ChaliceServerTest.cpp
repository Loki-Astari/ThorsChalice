#include "gtest/gtest.h"
#include "ChaliceConfig.h"
#include "ChaliceServer.h"

TEST(ChaliceServer, CreateHeadless)
{
    ThorsAnvil::ThorsChalice::ChaliceConfig     config;
    ThorsAnvil::ThorsChalice::ChaliceServerMode mode = ThorsAnvil::ThorsChalice::Headless;
    ThorsAnvil::ThorsChalice::ChaliceServer     server(config, mode);
}
TEST(ChaliceServer, CreateActive)
{
    ThorsAnvil::ThorsChalice::ChaliceConfig     config;
    ThorsAnvil::ThorsChalice::ChaliceServerMode mode = ThorsAnvil::ThorsChalice::Active;
    ThorsAnvil::ThorsChalice::ChaliceServer     server(config, mode);
}
TEST(ChaliceServer, ServiceRun)
{
    ThorsAnvil::ThorsChalice::ChaliceConfig     config;
    ThorsAnvil::ThorsChalice::ChaliceServerMode mode = ThorsAnvil::ThorsChalice::Headless;
    ThorsAnvil::ThorsChalice::ChaliceServer     server(config, mode);

    server.run();
}

