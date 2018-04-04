#include "filesystem.hpp"
#include <boost/filesystem.hpp>

bool directory_exists(const char *path) {
	return boost::filesystem::exists(path);
}

bool exists(const std::string &filename) noexcept {
	return boost::filesystem::exists(filename);
}

std::string save_filename()
{
	return get_save_path() + std::string("/savegame") + std::to_string(SAVE_VERSION) + std::string(".dat");
}

#ifndef _WIN32
#include <pwd.h>
#include <sys/stat.h>
std::string get_save_path()
{
    struct passwd *pw = getpwuid(getuid());
    const std::string result = std::string(pw->pw_dir) + std::string("/nox futura");

    if (!directory_exists(result.c_str())) {
        // Create the directory
        mkdir(result.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    }

    return result;
}
#else

#include <Shlobj.h>
#include <windows.h>

std::string get_save_path()
{
    WCHAR path[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_PROFILE, NULL, 0, path))) {
    	const std::wstring p(path);
		const std::string ps = std::string(p.begin(), p.end()) + std::string("/noxfutura");
		//std::cout << ps << "\n";
		if (!directory_exists(ps.c_str())) {
			CreateDirectory(ps.c_str(), nullptr);
		}
        return ps;
	}
	else {
		throw std::runtime_error("Unable to locate save path");
	}
}

#endif

void remove_from_path(std::string &s, const std::string needle) {
    //std::cout << "Searching " << s << " for " << needle << "\n";
    const std::size_t location = s.find_last_of(needle);
    if (location == std::string::npos) {
        //std::cout << "Not found\n";
    } else {
        //std::cout << "Found at location " << location << "\n";
        s = s.substr(0, location);
    }
}
