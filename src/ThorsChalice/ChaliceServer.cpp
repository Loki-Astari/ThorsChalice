#include "ChaliceServer.h"

#include "NisseHTTP/Request.h"
#include "NisseHTTP/Response.h"
#include "NisseServer/NisseServer.h"
#include <ratio>
#include <string>

using namespace ThorsAnvil::ThorsChalice;


TASock::ServerInit ChaliceServer::getServerInit(std::optional<FS::path> certPath, int port)
{
    if (!certPath.has_value()) {
        return TASock::ServerInfo{port};
    }

    TASock::CertificateInfo     certificate{FS::canonical(FS::path(*certPath) /= "fullchain.pem"),
                                            FS::canonical(FS::path(*certPath) /= "privkey.pem")
                                           };
    TASock::SSLctx              ctx{TASock::SSLMethodType::Server, certificate};
    return TASock::SServerInfo{port, std::move(ctx)};
}

void ChaliceServer::handleRequestLib(NisHttp::Request& request, NisHttp::Response& response, std::size_t libIndex)
{
    ThorsLogTrace("ChaliceServer", "handleRequestLib", "Send request to shared lib");
    // Make the loaded library handle the request.
    libraries.call(libIndex, request, response);
}

void ChaliceServer::handleRequestPath(NisHttp::Request& request, NisHttp::Response& response, FS::path const& contentDir)
{
    ThorsLogTrace("ChaliceServer", "handleRequestLib", "Handle file extract");
    // Get the path from the HTTP request object.
    // Remove the leading slash if it exists.
    std::string_view    path = request.getUrl().pathname();
    ThorsLogTrace("ChaliceServer", "handleRequestPath", "Input Path:   ", path);
    while (!path.empty() && path[0] == '/') {
        path.remove_prefix(1);
    }

    // Check that the path is valid
    // i.e. Some basic checks that the user is not trying to break into the filesystem.
    FS::path            requestPath = FS::path(path).lexically_normal();
    ThorsLogTrace("ChaliceServer", "handleRequestPath", "Request Path: ", requestPath.string());
    std::cerr << "RP:   >" << requestPath.string() << "<\n";
    if (requestPath.empty() || (*requestPath.begin()) == "..") {
        ThorsLogTrace("ChaliceServer", "handleRequestPath", "400 Invalid Path");
        response.error(400, "Invalid Request Path");
        return;
    }

    // Add the path to the root contentDir and check if the file exists.
    // Note if the user picked a directory we look for index.html
    std::error_code ec;
    FS::path        filePath = FS::canonical(FS::path{contentDir} /= requestPath, ec);
    ThorsLogTrace("ChaliceServer", "handleRequestPath", "File Path:    ", filePath.string());
    if (!ec && FS::is_directory(filePath)) {
        filePath = FS::canonical(filePath /= "index.html", ec);
    }
    if (ec || !FS::is_regular_file(filePath)) {
        ThorsLogTrace("ChaliceServer", "handleRequestPath", "404 No File Found");
        response.error(404, "No File Found At Path");
        return;
    }

    // Create an async File stream for the file we found.
    TASock::SocketStream    file{TASock::Socket{TASock::FileInfo{filePath.string(), TASock::FileMode::Read}, TASock::Blocking::No}};
    NisServer::AsyncStream  async(file, request.getContext(), NisServer::EventType::Read);

    // Stream the file to the output stream (this is an async operation).
    // So if the streaming blocks the thread will be released to do other work.
    // But return here to complete the task when either the file or the socket become available.
    response.body(NisHttp::Encoding::Chunked) << file.rdbuf();
}

ChaliceServer::ChaliceServer(ChaliceConfig const& config, ChaliceServerMode /*mode*/)
    : NisseServer(workerCount)
    , control(*this)
    , libraryChecker(*this)
{
    ThorsLogTrace("ChaliceServer", "ChaliceServer", "Create Server");
    servers.reserve(config.servers.size());

    for (auto const& server: config.servers) {
        ThorsLogTrace("ChaliceServer", "ChaliceServer", "Adding Server: ", server.port);
        servers.emplace_back();
        for (auto const& action: server.actions) {
            ThorsLogTrace("ChaliceServer", "ChaliceServer", "  Adding Action: ", action.path);
            switch (action.type)
            {
                case ActionType::File:
                {
                    std::cerr << "Adding Listener: FILE: >" << action.path << "<\n";
                    ThorsLogTrace("ChaliceServer", "ChaliceServer", "    File Listener: ");
                    servers.back().addPath(NisHttp::Method::GET,
                                           action.path,
                                           [&, rootDir = action.rootDir](NisHttp::Request& request, NisHttp::Response& response)
                                           {handleRequestPath(request, response, rootDir);}
                                          );
                    break;
                }
                case ActionType::Lib:
                {
                    ThorsLogTrace("ChaliceServer", "ChaliceServer", "    Lib  Listener: ");
                    std::size_t libIndex = libraries.load(action.rootDir);
                    servers.back().addPath(NisHttp::Method::GET,
                                           action.path,
                                           [&, libIndex](NisHttp::Request& request, NisHttp::Response& response)
                                           {handleRequestLib(request, response, libIndex);}
                                          );
                    break;
                }
            }
        }
        listen(getServerInit(server.certPath, server.port), servers.back());
    }
    ThorsLogTrace("ChaliceServer", "ChaliceServer", "  Adding Control Port: ", config.controlPort);
    listen(TASock::ServerInfo{config.controlPort}, control);
    using namespace std::chrono_literals;
    std::chrono::milliseconds   libraryCheckTime(config.libraryCheckTime);
    if (config.libraryCheckTime == 0) {
        libraryCheckTime = 1s;
    }
    addTimer(libraryCheckTime, libraryChecker);
}

void ChaliceServer::checkLibrary()
{
    libraries.checkAll();
}
