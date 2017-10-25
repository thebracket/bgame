#include "design_mode.hpp"
#include "../keydamper.hpp"
#include "../../bengine/gl_include.hpp"
#include "../../global_assets/game_mode.hpp"

namespace systems {
	namespace design_mode {

		void run(const double &duration_ms) {
			if (game_master_mode == PLAY) {
				if (is_key_down(GLFW_KEY_D, false)) game_master_mode = DESIGN;
			}
			else if (game_master_mode == DESIGN) {
				if (is_key_down(GLFW_KEY_T, false)) game_design_mode = CHOPPING;
				if (is_key_down(GLFW_KEY_ESCAPE)) game_master_mode = PLAY;
			}
		}
	}
}