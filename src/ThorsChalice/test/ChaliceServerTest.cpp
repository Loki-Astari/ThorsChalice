#include "gtest/gtest.h"
#include <string>
#include <thread>
#include "ChaliceConfig.h"
#include "ChaliceServer.h"

#include "ThorSerialize/JsonThor.h"
#include "ThorsSocket/Socket.h"
#include "ThorsSocket/SocketStream.h"

#include "HTTPSend.h"
#include "HTTPResponse.h"

#include <sstream>

using namespace std::chrono_literals;

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
TEST(ChaliceServer, ServiceRunManuallyStopped)
{
    ThorsAnvil::ThorsChalice::ChaliceConfig     config;
    ThorsAnvil::ThorsChalice::ChaliceServer     server(config, ThorsAnvil::ThorsChalice::Active);

    auto work = [&]() {
        server.run();
    };

    std::jthread     serverThread(work);

    std::this_thread::sleep_for(250ms);
    server.stop();
}

TEST(ChaliceServer, ServiceRunDefaultConfigHitControl)
{
    ThorsAnvil::ThorsChalice::ChaliceConfig     config;
    ThorsAnvil::ThorsChalice::ChaliceServer     server(config, ThorsAnvil::ThorsChalice::Active);

    auto work = [&]() {
        server.run();
    };

    std::jthread     serverThread(work);

    // Touch the control point to shut down the server.
    std::this_thread::sleep_for(250ms);
    ThorsAnvil::ThorsSocket::Socket     socket({"localhost", 8079});
    socket.putMessageData("Stop", 4);
}

TEST(ChaliceServer, ServiceRunModifiedControl)
{
    using ThorsAnvil::ThorsChalice::ActionType;
    std::stringstream   configStream(R"(
        {
            "controlPort": 8078,
            "servers": []
        }
    )");
    ThorsAnvil::ThorsChalice::ChaliceConfig     config;

    if (!(configStream >> ThorsAnvil::Serialize::jsonImporter(config))) {
        ASSERT_TRUE(false);
    }


    ThorsAnvil::ThorsChalice::ChaliceServer     server(config, ThorsAnvil::ThorsChalice::Active);

    auto work = [&]() {
        server.run();
    };

    std::jthread     serverThread(work);

    // Touch the control point to shut down the server.
    std::this_thread::sleep_for(250ms);
    ThorsAnvil::ThorsSocket::Socket     socket({"localhost", 8078});
    socket.putMessageData("Stop", 4);
}

TEST(ChaliceServer, ServiceRunAddServer)
{
    using ThorsAnvil::ThorsChalice::ActionType;
    std::stringstream   configStream(R"(
        {
            "controlPort": 8079,
            "servers": [
                {
                    "port":     8070,
                    "actions": [
                    ]
                }
            ]
        }
    )");
    ThorsAnvil::ThorsChalice::ChaliceConfig     config;

    if (!(configStream >> ThorsAnvil::Serialize::jsonImporter(config))) {
        ASSERT_TRUE(false);
    }

    ThorsAnvil::ThorsChalice::ChaliceServer     server(config, ThorsAnvil::ThorsChalice::Active);

    auto work = [&]() {
        server.run();
    };

    std::jthread     serverThread(work);

    // Touch the control point to shut down the server.
    std::this_thread::sleep_for(250ms);
    ThorsAnvil::ThorsSocket::Socket     socket({"localhost", 8079});
    socket.putMessageData("Stop", 4);
}

TEST(ChaliceServer, ServiceRunAddServerWithFile)
{
    using ThorsAnvil::ThorsChalice::ActionType;
    std::stringstream   configStream(R"(
        {
            "controlPort": 8079,
            "servers": [
                {
                    "port":     8070,
                    "actions": [
                        {
                            "type":     "File",
                            "rootDir":  "./test/data/pages",
                            "path":     "page1"
                        }
                    ]
                }
            ]
        }
    )");
    ThorsAnvil::ThorsChalice::ChaliceConfig     config;

    if (!(configStream >> ThorsAnvil::Serialize::jsonImporter(config))) {
        ASSERT_TRUE(false);
    }


    ThorsAnvil::ThorsChalice::ChaliceServer     server(config, ThorsAnvil::ThorsChalice::Active);

    auto work = [&]() {
        server.run();
    };

    std::jthread     serverThread(work);

    // Touch the control point to shut down the server.
    std::this_thread::sleep_for(250ms);
    ThorsAnvil::ThorsSocket::Socket     socket({"localhost", 8079});
    socket.putMessageData("Stop", 4);
}

TEST(ChaliceServer, ServiceRunAddServerWithFileValidateWorks)
{
    using ThorsAnvil::ThorsChalice::ActionType;
    std::stringstream   configStream(R"(
        {
            "controlPort": 8079,
            "servers": [
                {
                    "port":     8070,
                    "actions": [
                        {
                            "type":     "File",
                            "rootDir":  "./test/data/pages",
                            "path":     "/files"
                        }
                    ]
                }
            ]
        }
    )");

    ThorsAnvil::ThorsChalice::ChaliceConfig     config;

    if (!(configStream >> ThorsAnvil::Serialize::jsonImporter(config))) {
        ASSERT_TRUE(false);
    }

    ThorsAnvil::ThorsChalice::ChaliceServer     server(config, ThorsAnvil::ThorsChalice::Active);

    auto work = [&]() {
        server.run();
    };

    std::jthread     serverThread(work);

    {
        // Talk to server.
        std::this_thread::sleep_for(20ms);
        ThorsAnvil::ThorsSocket::SocketStream socketData({"localhost", 8070});

        socketData << ThorsAnvil::ThorsSocket::HTTPSend(ThorsAnvil::ThorsSocket::SendType::GET, ThorsAnvil::ThorsSocket::SendVersion::HTTP1_1, "localhost", "/files/page1");

        ThorsAnvil::ThorsSocket::HTTPResponse   response;
        socketData >> response;

        ASSERT_EQ("Data for page 1\n", response.getBody());
    }

    // Touch the control point to shut down the server.
    std::this_thread::sleep_for(250ms);
    ThorsAnvil::ThorsSocket::Socket     socket({"localhost", 8079});
    socket.putMessageData("Stop", 4);
}

TEST(ChaliceServer, CallALoadedLib)
{
    using ThorsAnvil::ThorsChalice::ActionType;
    std::stringstream   configStream(R"(
        {
            "controlPort": 8079,
            "servers": [
                {
                    "port":     8070,
                    "actions": [
                        {
                            "type":     "Lib",
                            "rootDir":  "../L3/release/libL3.dylib",
                            "path":     "/page1"
                        }
                    ]
                }
            ]
        }
    )");

    ThorsAnvil::ThorsChalice::ChaliceConfig     config;

    if (!(configStream >> ThorsAnvil::Serialize::jsonImporter(config))) {
        ASSERT_TRUE(false);
    }

    ThorsAnvil::ThorsChalice::ChaliceServer     server(config, ThorsAnvil::ThorsChalice::Active);

    auto work = [&]() {
        server.run();
    };

    std::jthread     serverThread(work);

    {
        // Talk to server.
        std::this_thread::sleep_for(20ms);
        ThorsAnvil::ThorsSocket::SocketStream socketData({"localhost", 8070});

        socketData << ThorsAnvil::ThorsSocket::HTTPSend(ThorsAnvil::ThorsSocket::SendType::GET, ThorsAnvil::ThorsSocket::SendVersion::HTTP1_1, "localhost", "/page1");

        ThorsAnvil::ThorsSocket::HTTPResponse   response;
        socketData >> response;

        ASSERT_EQ(305, response.getCode());

    }

    // Touch the control point to shut down the server.
    std::this_thread::sleep_for(250ms);
    ThorsAnvil::ThorsSocket::Socket     socket({"localhost", 8079});
    socket.putMessageData("Stop", 4);
}

