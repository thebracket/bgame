#include <ctime>
#include "run_systems.hpp"
#include "io/camera_system.hpp"
#include "gui/hud_system.hpp"
#include <string>
#include <boost/container/flat_map.hpp>
#include <vector>

namespace systems {
    boost::container::flat_map<int, std::pair<int, std::vector<double>>> run_time;
    boost::container::flat_map<int, std::string> system_names;

    template <typename F>
    inline void run_system(F &run_func, const double &duration_ms, const int SYSTEM) {
        clock_t start_time = clock();
        run_func(duration_ms);
        const double system_running_time = ((clock() - start_time) * 1000.0) / CLOCKS_PER_SEC;
        auto finder = run_time.find(SYSTEM);
        if (finder == run_time.end()) {
            std::vector<double> timer;
            timer.resize(100);
            timer[0] = system_running_time;
            run_time.insert(std::make_pair(SYSTEM, std::make_pair(1, timer)));
        } else {
            finder->second.second[finder->second.first] = system_running_time;
            ++finder->second.first;
            if (finder->second.first > 100) finder->second.first = 0;
        }
    }

    void init() {
        system_names[CAMERA_SYSTEM] = "Camera System";
        system_names[HUD_SYSTEM] = "HUD System";
    }

    void run(const double &duration_ms) {
        run_system(camerasys::run, duration_ms, CAMERA_SYSTEM);
        run_system(hud::run, duration_ms, HUD_SYSTEM);
    }
}