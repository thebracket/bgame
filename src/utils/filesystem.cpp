#include "filesystem.hpp"
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

bool directory_exists(const char *path)
{
    struct stat info;

    if(stat( path, &info ) != 0)
        return false;
    else if(info.st_mode & S_IFDIR)
        return true;
    else
        return false;
}

std::string get_save_path()
{
    struct passwd *pw = getpwuid(getuid());
    const std::string result = std::string(pw->pw_dir) + std::string("/black future savegame");

    if (!directory_exists(result.c_str())) {
        // Create the directory
        mkdir(result.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    }

    return result;
}