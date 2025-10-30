#include "gtest/gtest.h"
#include <cstddef>
#include <filesystem>
#include <string>
#include "ChaliceCLA.h"
#include "ThorsLogging/ThorsLogging.h"

struct MockArguments: public ThorsAnvil::ThorsChalice::ChaliceCLAInterface
{
    std::size_t                 classCount      = 0;
    std::array<std::size_t, 6>  methodCallCount = {0, 0, 0, 0, 0, 0};
    FSys::path                  config;
    FSys::path                  logFile;
    std::string                 appName;
    loguru::Verbosity           verbosity;

    virtual void logAddFile(FSys::path file)            override
    {
        ++classCount;
        ++methodCallCount[0];
        logFile = file;
    }
    virtual void logAddSys(std::string_view app)        override
    {
        ++classCount;
        ++methodCallCount[1];
        appName = app;
    }
    virtual void logSetLevel(loguru::Verbosity level)   override
    {
        ++classCount;
        ++methodCallCount[2];
        verbosity = level;
    }
    virtual void setHelp()                              override
    {
        ++classCount;
        ++methodCallCount[3];
    }
    virtual void setSilent()                            override
    {
        ++classCount;
        ++methodCallCount[4];
    }
    virtual void setConfigFile(FSys::path file)         override
    {
        ++classCount;
        ++methodCallCount[5];
        config = file;
    }
};

TEST(ChaliceCLATest, NoArgumentsOnlyApplicationName)
{
    MockArguments                           mockArgs;
    ThorsAnvil::ThorsChalice::ChaliceCLA    cla({"Test"}, mockArgs);

    // Call to load
    ASSERT_EQ(0, mockArgs.classCount);
}
TEST(ChaliceCLATest, InvalidArgument)
{
    MockArguments                           mockArgs;
    ThorsAnvil::ThorsChalice::ChaliceCLA    cla({"Test", "--invalid"}, mockArgs);

    // Call to load
    ASSERT_EQ(1, mockArgs.classCount);
    ASSERT_EQ(1, mockArgs.methodCallCount[3]);
}
TEST(ChaliceCLATest, HelpFlag)
{
    MockArguments                           mockArgs;
    ThorsAnvil::ThorsChalice::ChaliceCLA    cla({"Test", "--help"}, mockArgs);

    // Call to load
    ASSERT_EQ(1, mockArgs.classCount);
    ASSERT_EQ(1, mockArgs.methodCallCount[3]);
}
TEST(ChaliceCLATest, SilentFlag)
{
    MockArguments                           mockArgs;
    ThorsAnvil::ThorsChalice::ChaliceCLA    cla({"Test", "--silent"}, mockArgs);

    // Call to load
    ASSERT_EQ(1, mockArgs.classCount);
    ASSERT_EQ(1, mockArgs.methodCallCount[4]);
}
TEST(ChaliceCLATest, ConfigFlag)
{
    MockArguments                           mockArgs;
    ThorsAnvil::ThorsChalice::ChaliceCLA    cla({"Test", "--config=LocalFile"}, mockArgs);

    // Call to load
    ASSERT_EQ(1, mockArgs.classCount);
    ASSERT_EQ(1, mockArgs.methodCallCount[5]);
    ASSERT_EQ("LocalFile", mockArgs.config);
}
TEST(ChaliceCLATest, addLogFile)
{
    MockArguments                           mockArgs;
    ThorsAnvil::ThorsChalice::ChaliceCLA    cla({"Test", "--logFile=LogFile"}, mockArgs);

    // Call to load
    ASSERT_EQ(1, mockArgs.classCount);
    ASSERT_EQ(1, mockArgs.methodCallCount[0]);
    ASSERT_EQ("LogFile", mockArgs.logFile);
}
TEST(ChaliceCLATest, addSysLog)
{
    MockArguments                           mockArgs;
    ThorsAnvil::ThorsChalice::ChaliceCLA    cla({"Test", "--logSys"}, mockArgs);

    // Call to load
    ASSERT_EQ(1, mockArgs.classCount);
    ASSERT_EQ(1, mockArgs.methodCallCount[1]);
    ASSERT_EQ("Test", mockArgs.appName);
}
TEST(ChaliceCLATest, addSysLogExplicit)
{
    MockArguments                           mockArgs;
    ThorsAnvil::ThorsChalice::ChaliceCLA    cla({"Test", "--logSys=AppName"}, mockArgs);

    // Call to load
    ASSERT_EQ(1, mockArgs.classCount);
    ASSERT_EQ(1, mockArgs.methodCallCount[1]);
    ASSERT_EQ("AppName", mockArgs.appName);
}
TEST(ChaliceCLATest, setLogLevelAll)
{
    MockArguments                           mockArgs;
    ThorsAnvil::ThorsChalice::ChaliceCLA    cla({"Test", "--logLevel=All"}, mockArgs);

    // Call to load
    ASSERT_EQ(1, mockArgs.classCount);
    ASSERT_EQ(1, mockArgs.methodCallCount[2]);
    ASSERT_EQ(loguru::Verbosity_9, mockArgs.verbosity);
}
TEST(ChaliceCLATest, setLogLevelTrace)
{
    MockArguments                           mockArgs;
    ThorsAnvil::ThorsChalice::ChaliceCLA    cla({"Test", "--logLevel=Trace"}, mockArgs);

    // Call to load
    ASSERT_EQ(1, mockArgs.classCount);
    ASSERT_EQ(1, mockArgs.methodCallCount[2]);
    ASSERT_EQ(loguru::Verbosity_8, mockArgs.verbosity);
}
TEST(ChaliceCLATest, setLogLevelDebug)
{
    MockArguments                           mockArgs;
    ThorsAnvil::ThorsChalice::ChaliceCLA    cla({"Test", "--logLevel=Debug"}, mockArgs);

    // Call to load
    ASSERT_EQ(1, mockArgs.classCount);
    ASSERT_EQ(1, mockArgs.methodCallCount[2]);
    ASSERT_EQ(loguru::Verbosity_6, mockArgs.verbosity);
}
TEST(ChaliceCLATest, setLogLevelInfo)
{
    MockArguments                           mockArgs;
    ThorsAnvil::ThorsChalice::ChaliceCLA    cla({"Test", "--logLevel=Info"}, mockArgs);

    // Call to load
    ASSERT_EQ(1, mockArgs.classCount);
    ASSERT_EQ(1, mockArgs.methodCallCount[2]);
    ASSERT_EQ(loguru::Verbosity_INFO, mockArgs.verbosity);
}
TEST(ChaliceCLATest, setLogLevelWarn)
{
    MockArguments                           mockArgs;
    ThorsAnvil::ThorsChalice::ChaliceCLA    cla({"Test", "--logLevel=Warn"}, mockArgs);

    // Call to load
    ASSERT_EQ(1, mockArgs.classCount);
    ASSERT_EQ(1, mockArgs.methodCallCount[2]);
    ASSERT_EQ(loguru::Verbosity_WARNING, mockArgs.verbosity);
}
TEST(ChaliceCLATest, setLogLevelError)
{
    MockArguments                           mockArgs;
    ThorsAnvil::ThorsChalice::ChaliceCLA    cla({"Test", "--logLevel=Error"}, mockArgs);

    // Call to load
    ASSERT_EQ(1, mockArgs.classCount);
    ASSERT_EQ(1, mockArgs.methodCallCount[2]);
    ASSERT_EQ(loguru::Verbosity_ERROR, mockArgs.verbosity);
}
TEST(ChaliceCLATest, allFlags)
{
    MockArguments                           mockArgs;
    ThorsAnvil::ThorsChalice::ChaliceCLA    cla({"Test", "--logFile=log1", "--logSys=Sys", "--logLevel=Warn", "--config=Configer", "--help", "--silent"}, mockArgs);

    // Call to load
    ASSERT_EQ(6, mockArgs.classCount);
    ASSERT_EQ(1, mockArgs.methodCallCount[0]);
    ASSERT_EQ(1, mockArgs.methodCallCount[1]);
    ASSERT_EQ(1, mockArgs.methodCallCount[2]);
    ASSERT_EQ(1, mockArgs.methodCallCount[3]);
    ASSERT_EQ(1, mockArgs.methodCallCount[4]);
    ASSERT_EQ(1, mockArgs.methodCallCount[5]);
    ASSERT_EQ("Sys", mockArgs.appName);
    ASSERT_EQ("log1", mockArgs.logFile);
    ASSERT_EQ("Configer", mockArgs.config);
    ASSERT_EQ(loguru::Verbosity_WARNING, mockArgs.verbosity);
}

