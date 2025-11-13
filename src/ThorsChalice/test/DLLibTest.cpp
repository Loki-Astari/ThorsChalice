#include "gtest/gtest.h"

#include "DLLib.h"
#include "NisseHTTP/HTTPHandler.h"
#include "NisseHTTP/Request.h"
#include "NisseHTTP/Response.h"
#include "NisseHTTP/URL.h"
#include "NisseHTTP/Util.h"
#include <filesystem>
#include <sstream>
#include <thread>

namespace FS = std::filesystem;


TEST(DLLibTest, Create)
{
    ThorsAnvil::ThorsChalice::DLLib   dlLib;
}

TEST(DLLibTest, Move)
{
    ThorsAnvil::ThorsChalice::DLLib   dlLib1;
    ThorsAnvil::ThorsChalice::DLLib   dlLib2(std::move(dlLib1));
}

TEST(DLLibTest, MoveAssign)
{
    ThorsAnvil::ThorsChalice::DLLib   dlLib1;
    ThorsAnvil::ThorsChalice::DLLib   dlLib2;

    dlLib1 = std::move(dlLib1);
}

TEST(DLLibTest, LoadL3)
{
    std::error_code ec;
    ThorsAnvil::ThorsChalice::DLLib   dlLib1(FS::canonical(FS::path("../L3/release/libL3.dylib"), ec));
}
TEST(DLLibTest, LoadFailEmpty)
{
    std::error_code ec;
    auto action = [&](){ThorsAnvil::ThorsChalice::DLLib   dlLib1(FS::canonical(FS::path("../../build/lib/libBadLibName.dylib"), ec));};
    EXPECT_THROW(
                 action(),
                 std::runtime_error
                 );
}
TEST(DLLibTest, LoadFailWithPath)
{
    auto action = [&](){ThorsAnvil::ThorsChalice::DLLib   dlLib1(FS::path("../../build/lib/libBadLibName.dylib"));};
    EXPECT_THROW(
                 action(),
                 std::runtime_error
                 );
}
TEST(DLLibTest, LoadOKFunctionBad)
{
    std::error_code ec;
    auto action = [&](){ThorsAnvil::ThorsChalice::DLLib   dlLib1(FS::canonical(FS::path("../L5/release/libL5.dylib"), ec));};
    EXPECT_THROW(
                 action(),
                 std::runtime_error
                 );
}

TEST(DLLibTest, LoadL3Call)
{
    std::error_code ec;
    ThorsAnvil::ThorsChalice::DLLib     dlLib1(FS::canonical(FS::path("../L3/release/libL3.dylib"), ec));
    std::stringstream                   input{"GET /Plop/path/twist.gue?p=1&q=12#34 HTTP/1.1\r\n"
                                               "host: thorsanvil.dev:8070\r\n"
                                               "content-length: 0\r\n"
                                               "\r\n"
                                              };
    std::stringstream                   output;
    ThorsAnvil::Nisse::HTTP::Request    request("", input);
    ThorsAnvil::Nisse::HTTP::Response   response(output, ThorsAnvil::Nisse::HTTP::Version::HTTP1_1, 200);


    dlLib1.call(request, response);
    EXPECT_EQ(305, response.getCode().code);
}
TEST(DLLibTest, LoadL4Call)
{
    std::error_code ec;
    ThorsAnvil::ThorsChalice::DLLib     dlLib1(FS::canonical(FS::path("../L4/release/libL4.dylib"), ec));
    std::stringstream                   input{"GET /Plop/path/twist.gue?p=1&q=12#34 HTTP/1.1\r\n"
                                               "host: thorsanvil.dev:8070\r\n"
                                               "content-length: 0\r\n"
                                               "\r\n"
                                              };
    std::stringstream                   output;
    ThorsAnvil::Nisse::HTTP::Request    request("", input);
    ThorsAnvil::Nisse::HTTP::Response   response(output, ThorsAnvil::Nisse::HTTP::Version::HTTP1_1, 200);


    dlLib1.call(request, response);
    EXPECT_EQ(404, response.getCode().code);
}

TEST(DLLibTest, CallCheck)
{
    std::error_code ec;
    ThorsAnvil::ThorsChalice::DLLib     dlLib1(FS::canonical(FS::path("../L4/release/libL4.dylib"), ec));
    std::stringstream                   input{"GET /Plop/path/twist.gue?p=1&q=12#34 HTTP/1.1\r\n"
                                               "host: thorsanvil.dev:8070\r\n"
                                               "content-length: 0\r\n"
                                               "\r\n"
                                              };
    std::stringstream                   output;
    ThorsAnvil::Nisse::HTTP::Request    request("", input);
    ThorsAnvil::Nisse::HTTP::Response   response(output, ThorsAnvil::Nisse::HTTP::Version::HTTP1_1, 200);


    dlLib1.call(request, response);
    EXPECT_EQ(404, response.getCode().code);
    EXPECT_EQ(ThorsAnvil::ThorsChalice::NoChange, dlLib1.check());
    dlLib1.call(request, response);
    EXPECT_EQ(404, response.getCode().code);
}

TEST(DLLibTest, CallCheckAfterLibChange)
{
    std::filesystem::remove("./CallCheckAfterLibChange.dylib");
    std::filesystem::copy("../L3/release/libL3.dylib", "./CallCheckAfterLibChange.dylib");

    std::error_code                     ec;
    ThorsAnvil::ThorsChalice::DLLib     dlLib1(FS::canonical(FS::path("./CallCheckAfterLibChange.dylib"), ec));
    std::stringstream                   input{"GET /Plop/path/twist.gue?p=1&q=12#34 HTTP/1.1\r\n"
                                               "host: thorsanvil.dev:8070\r\n"
                                               "content-length: 0\r\n"
                                               "\r\n"
                                              };
    std::stringstream                   output;
    ThorsAnvil::Nisse::HTTP::Request    request("", input);
    ThorsAnvil::Nisse::HTTP::Response   response(output, ThorsAnvil::Nisse::HTTP::Version::HTTP1_1, 200);


    dlLib1.call(request, response);
    EXPECT_EQ(305, response.getCode().code);
    std::filesystem::remove("./CallCheckAfterLibChange.dylib");
    std::filesystem::copy("../L4/release/libL4.dylib", "./CallCheckAfterLibChange.dylib");
    EXPECT_EQ(ThorsAnvil::ThorsChalice::ChangeAndSwapped, dlLib1.check());
    dlLib1.call(request, response);
    EXPECT_EQ(404, response.getCode().code);
    std::filesystem::remove("./CallCheckAfterLibChange.dylib");
}

TEST(DLLibTest, CheckOnEmpty)
{
    ThorsAnvil::ThorsChalice::DLLib     dlLib;
    EXPECT_EQ(ThorsAnvil::ThorsChalice::NoChange, dlLib.check());
}

TEST(DLLibTest, ForceCheckOnEmpty)
{
    ThorsAnvil::ThorsChalice::DLLib     dlLib;
    dlLib.checkWithForce();
}

TEST(DLLibTest, CallOnEmpty)
{
    ThorsAnvil::ThorsChalice::DLLib     dlLib;
    std::stringstream                   input{"GET /Plop/path/twist.gue?p=1&q=12#34 HTTP/1.1\r\n"
                                               "host: thorsanvil.dev:8070\r\n"
                                               "content-length: 0\r\n"
                                               "\r\n"
                                              };
    std::stringstream                   output;
    ThorsAnvil::Nisse::HTTP::Request    request("", input);
    ThorsAnvil::Nisse::HTTP::Response   response(output, ThorsAnvil::Nisse::HTTP::Version::HTTP1_1, 200);
    auto action = [&](){dlLib.call(request, response);};

    EXPECT_THROW(action(),
                 std::runtime_error
                );
}

TEST(DLLibTest, Sleep)
{
    std::filesystem::remove("./CallCheckAfterLibChange.dylib");
    std::filesystem::copy("../L3/release/libL3.dylib", "./CallCheckAfterLibChange.dylib");

    std::error_code                     ec;
    ThorsAnvil::ThorsChalice::DLLib     dlLib(FS::canonical(FS::path("./CallCheckAfterLibChange.dylib"), ec));
    std::filesystem::remove("./CallCheckAfterLibChange.dylib");
    std::filesystem::copy("../L4/release/libL4.dylib", "./CallCheckAfterLibChange.dylib");


    std::stringstream                   input{"GET /Plop/path/twist.gue?p=1&sleep=3&q=12#34 HTTP/1.1\r\n"
                                               "host: thorsanvil.dev:8070\r\n"
                                               "content-length: 0\r\n"
                                               "\r\n"
                                             };
    std::stringstream                   output;
    ThorsAnvil::Nisse::HTTP::Request    request("", input);
    ThorsAnvil::Nisse::HTTP::Response   response(output, ThorsAnvil::Nisse::HTTP::Version::HTTP1_1, 200);


    ThorsAnvil::Nisse::HTTP::HTTPHandler    handler;
    handler.addPath(ThorsAnvil::Nisse::HTTP::Method::GET, "/Plop/{action}", [&dlLib](ThorsAnvil::Nisse::HTTP::Request& request, ThorsAnvil::Nisse::HTTP::Response& response) {
        dlLib.call(request, response);
    });

    auto action = [&](){handler.processRequest(request, response);};
    std::jthread    work(action);       // Note the sleep in the URL will cause Lib3 to actually sleep the calling thread.
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(20ms);
    auto result = dlLib.check();                      // This will try and unload the library but it can't because there is an active thread.
    EXPECT_EQ(ThorsAnvil::ThorsChalice::ChangedButLocked, result);

    work.join();
    std::filesystem::remove("./CallCheckAfterLibChange.dylib");
}


