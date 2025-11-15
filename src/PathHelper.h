#pragma once

#include <string>

class PathHelper {
public:
    // Resolve base directory where our executable + dropbear binaries live
    static std::string appBaseDir();
    
    static std::string bundledDropbearPath();
    static std::string bundledDropbearKeygenPath();
    static std::string hostKeyPath();
};
