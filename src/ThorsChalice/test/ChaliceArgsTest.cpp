#include "gtest/gtest.h"
#include "ChaliceArgs.h"

TEST(ChaliceArgsTest, HelpFlag)
{
    ThorsAnvil::ThorsChalice::ChaliceArgs   args;

    EXPECT_FALSE(args.help);
    args.setHelp();
    EXPECT_TRUE(args.help);
}
TEST(ChaliceArgsTest, SilentFlag)
{
    ThorsAnvil::ThorsChalice::ChaliceArgs   args;
    EXPECT_FALSE(args.silent);
    args.setSilent();
    EXPECT_TRUE(args.silent);
}
TEST(ChaliceArgsTest, ConfigFlag)
{
    ThorsAnvil::ThorsChalice::ChaliceArgs   args;

    EXPECT_EQ("", args.configPath);
    args.setConfigFile("MyFile");
    EXPECT_EQ("MyFile", args.configPath);
}
TEST(ChaliceArgsTest, addLogFile)
{
    ThorsAnvil::ThorsChalice::ChaliceArgs   args;
    args.logAddFile("LogFile");
}
TEST(ChaliceArgsTest, addSysLog)
{
    ThorsAnvil::ThorsChalice::ChaliceArgs   args;
    args.logAddSys("Application");
}
TEST(ChaliceArgsTest, setLogLevelAll)
{
    ThorsAnvil::ThorsChalice::ChaliceArgs   args;

    args.logSetLevel(loguru::Verbosity_5);
    EXPECT_EQ(loguru::g_stderr_verbosity, loguru::Verbosity_5);
}
