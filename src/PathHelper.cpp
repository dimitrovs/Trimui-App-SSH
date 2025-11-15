#include "PathHelper.h"
#include <unistd.h>
#include <limits.h>

#ifdef USE_SDL
#include <SDL2/SDL.h>
#endif

std::string PathHelper::appBaseDir() {
#ifdef USE_SDL
    char* base = SDL_GetBasePath();
    if (base) {
        std::string dir(base);
        SDL_free(base);
        return dir;            // SDL_GetBasePath() already ends with '/'
    }
#endif

    char exe[PATH_MAX] = {0};
    ssize_t n = readlink("/proc/self/exe", exe, sizeof(exe) - 1);
    if (n > 0) {
        exe[n] = '\0';
        std::string s(exe);
        auto pos = s.find_last_of('/');
        if (pos != std::string::npos) s.resize(pos + 1); // keep trailing '/'
        else s = "./";
        return s;
    }

    return std::string("./");
}

std::string PathHelper::bundledDropbearPath() {
    return appBaseDir() + "dropbear";
}

std::string PathHelper::bundledDropbearKeygenPath() {
    return appBaseDir() + "dropbearkey";
}

std::string PathHelper::hostKeyPath() {
    return appBaseDir() + "dropbear_rsa_host_key";
}
