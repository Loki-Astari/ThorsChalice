#include "ChaliceServer.h"

#include "NisseHTTP/Request.h"
#include "NisseHTTP/Response.h"
#include "NisseServer/NisseServer.h"
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

void ChaliceServer::handleRequest(NisHttp::Request& request, NisHttp::Response& response, FS::path const& contentDir)
{
    std::string_view    path = request.getUrl().pathname();
    while (!path.empty() && path[0] == '/') {
        path.remove_prefix(1);
    }
    FS::path            requestPath = path;
    if (requestPath.empty() || (*requestPath.begin()) == "..") {
        return response.error(400, "Invalid Request Path");
    }

    std::error_code ec;
    FS::path        filePath = FS::canonical(FS::path{contentDir} /= requestPath, ec);
    if (!ec && FS::is_directory(filePath)) {
        filePath = FS::canonical(filePath /= "index.html", ec);
    }
    if (ec || !FS::is_regular_file(filePath)) {
        return response.error(404, "No File Found At Path");
    }

    TASock::SocketStream    file{TASock::Socket{TASock::FileInfo{filePath.string(), TASock::FileMode::Read}, TASock::Blocking::No}};
    NisServer::AsyncStream  async(file, request.getContext(), NisServer::EventType::Read);

    response.body(NisHttp::Encoding::Chunked) << file.rdbuf();
}

ChaliceServer::ChaliceServer(ChaliceConfig const& config, ChaliceServerMode /*mode*/)
    : NisseServer(workerCount)
    , control(*this)
{
    //std::cerr << "Server Create\n";
    servers.reserve(config.servers.size());
    //std::cerr << "    Init: " << servers.size() << "\n";

    for (auto const& server: config.servers) {
        //std::cerr << "    Server: " << server.port << "\n";
        servers.emplace_back();
        for (auto const& action: server.actions) {
            if (action.type == ActionType::File) {
                //std::cerr << "        Path: " << action.path << "\n";
                servers.back().addPath(NisHttp::Method::GET,
                                       action.path,
                                       [&, rootDir = server.rootDir](NisHttp::Request& request, NisHttp::Response& response)
                                       {handleRequest(request, response, rootDir);}
                                      );
            }
        }
        listen(getServerInit(server.certPath, server.port), servers.back());
    }
    //std::cerr << "    Control\n";
    listen(TASock::ServerInfo{config.controlPort}, control);
}
