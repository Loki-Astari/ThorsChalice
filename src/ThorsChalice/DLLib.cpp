#include "DLLib.h"

#include "ThorsLogging/ThorsLogging.h"
#include <dlfcn.h>
#include <filesystem>

using namespace ThorsAnvil::ThorsChalice;


DLLib::DLLib()
    : lib(nullptr)
{}

DLLib::DLLib(FS::path const& path)
    : path(path)
{
    if (path.empty()) {
        ThorsLogAndThrowError(std::runtime_error, "DLLib", "DLLib", "Empty path used for library");
    }

    reload();
}

DLLib::DLLib(DLLib&& move) noexcept
{
    swap(move);
}

DLLib& DLLib::operator=(DLLib&& move) noexcept
{
    swap(move);
    return *this;
}

void DLLib::swap(DLLib& other) noexcept
{
    using std::swap;
    swap(lib,           other.lib);
    swap(chaliceHandle, other.chaliceHandle);
}

char const* DLLib::safeDLerror()
{
    char const* message = dlerror();
    return message == nullptr ? "" : message;
}

DLLib::~DLLib()
{
    if (lib) {
        int status = dlclose(lib);
        if (status != 0) {
            ThorsLogError("DLLib", "~DLLib", "dlclose() failed: ", safeDLerror());
        }
    }
}

void DLLib::inc()
{
    std::unique_lock<std::mutex>    lock(mutex);
    if (reloadInProgress) {
        cond.wait(lock, [&reloadInProgress = this->reloadInProgress](){return !reloadInProgress;});
    }
    ++activeCalls;
}
void DLLib::dec()
{
    std::unique_lock<std::mutex>    lock(mutex);
    --activeCalls;
    if (reloadInProgress && activeCalls == 0) {
        reload();
    }
}

void DLLib::call(ThorsAnvil::Nisse::HTTP::Request& request, ThorsAnvil::Nisse::HTTP::Response& response)
{
    if (chaliceHandle == nullptr) {
        ThorsLogAndThrowError(std::runtime_error, "DLLib", "call", "Calling unloaded fuction");
    }
    inc();
    (*chaliceHandle)(request, response);
    dec();
}

void DLLib::checkWithForce()
{
    loadFailed = false;
    check();
}

CheckState DLLib::check()
{
    if (loadFailed || path.empty() || lastModified == FS::last_write_time(path)) {
        return NoChange;
    }

    std::unique_lock<std::mutex>    lock(mutex);
    if (lastModified == FS::last_write_time(path)) {
        return NoChange;
    }
    reloadInProgress = true;
    if (activeCalls != 0) {
        return ChangedButLocked;
    }
    reload();
    return ChangeAndSwapped;
}

void DLLib::reload()
{
    if (lib != nullptr) {
        dlclose(lib);
        lib = nullptr;
        chaliceHandle = nullptr;
    }
    ThorsLogDebug("DLLib", "reload", "Reload DLL: ", path);
    std::error_code ec;
    lib = dlopen(FS::canonical(path, ec).c_str(), RTLD_NOW | RTLD_LOCAL);
    if (lib == nullptr) {
        loadFailed = true;
        ThorsLogAndThrowError(std::runtime_error, "DLLib", "reload", "dlopen() failed: ", safeDLerror());
    }

    void*           chaliceFuncSym = dlsym(lib, "chaliceFunction");
    if (chaliceFuncSym == nullptr) {
        dlclose(lib);
        lib = nullptr;
        loadFailed = true;
        ThorsLogAndThrowError(std::runtime_error, "DLLib", "reload", "dlsym() failed: ", safeDLerror());
    }
    lastModified = FS::last_write_time(path);


    GenericFuncPtr  chaliceFuncGen  = reinterpret_cast<GenericFuncPtr>(chaliceFuncSym);
    ChaliceFunc     chaliceFunc     = reinterpret_cast<ChaliceFunc>(chaliceFuncGen);
    GenericFuncPtr  chaliceResult   = (*chaliceFunc)();

    chaliceHandle = reinterpret_cast<ChaliceHanlde>(chaliceResult);
    reloadInProgress = false;
    cond.notify_all();
}

std::size_t DLLibMap::load(std::string const& path)
{
    std::error_code ec;
    FS::path    libPath = FS::canonical(path, ec);
    if (libPath.empty()) {
        ThorsLogAndThrowError(std::runtime_error, "DLLibMap", "load", "Invalid path to shared library: ", path);
    }
    auto find = libNameMap.find(libPath.string());
    if (find != std::end(libNameMap)) {
        return find->second;
    }
    std::size_t result = loadedLibs.size();
    libNameMap[libPath.string()] = result;
    loadedLibs.emplace_back(libPath);
    return result;
}
