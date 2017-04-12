#include "filesystem.hpp"

#ifndef WIN32
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#endif

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

#ifndef WIN32
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
#else

#include <Shlobj.h>
#include <windows.h>
#include <iostream>

std::string get_save_path()
{
    WCHAR path[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_PROFILE, NULL, 0, path))) {
    	const std::wstring p(path);
	const std::string ps = std::string(p.begin(), p.end()) + std::string("/bfsave");
	std::cout << ps << "\n";
	if (!directory_exists(ps.c_str())) {
	    CreateDirectory(ps.c_str(), nullptr);
	}
        return ps;
    }
}

#endif


