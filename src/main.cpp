#include "stdafx.h"
#include "bengine/main_window.hpp"
#include "main_loops/splash_screen.hpp"
#ifdef __APPLE__
#include <libproc.h>
#include <unistd.h>
#endif
#include "bengine/filesystem.hpp"
#include "global_assets/game_config.hpp"
#include "main_loops/first_run_screen.hpp"
#include "utils/system_log.hpp"

using namespace bengine;

int main() {
#ifdef __APPLE__
    // We're probably running inside a bundle, which means that we need to find the executable's path
    // and have that as our working directory

    int ret;
    pid_t pid;
    char pathbuf[PROC_PIDPATHINFO_MAXSIZE];

    std::string executable_path;

    pid = getpid();
    ret = proc_pidpath (pid, pathbuf, sizeof(pathbuf));
    if ( ret <= 0 ) {
        //fprintf(stderr, "PID %d: proc_pidpath ();\n", pid);
        //fprintf(stderr, "    %s\n", strerror(errno));
    } else {
        //printf("proc %d: %s\n", pid, pathbuf);
        executable_path = pathbuf;
    }

    remove_from_path(executable_path, std::string("/"));

    //std::cout << executable_path << "\n";
    chdir(executable_path.c_str());

#endif // apple

	bool first_run_checked = false;
	bool first_run = false;

    config::read_config();
    init(config::game_config.fullscreen, config::game_config.window_width, config::game_config.window_height, config::game_config.gui_ttf, config::game_config.gui_ttf_size);
	if (!first_run_checked) {
		first_run_checked = true;
		first_run = first_run_screen::is_first_run();
	}
	if (first_run) {
		main_func = first_run_screen::tick;
		if (first_run_screen::first_run_done) first_run = false;
		gamelog(log_target::LOADER, log_severity::info, "Launching first run screen");
	}
	else {
		splash_screen::init();
		main_func = splash_screen::tick;
	}
    run();

    return 0;
}
