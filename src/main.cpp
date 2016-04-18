#include <rltk.hpp>
#include "splash_screen.hpp"

using namespace rltk;
using namespace rltk::colors;

enum game_mode_t { SPLASH };
game_mode_t mode = SPLASH;

splash_screen splash;

void tick(double duration_ms) {
	switch (mode) {
		case SPLASH : splash.tick(duration_ms); break;
	}
}

int main()
{
	init(config_advanced("../assets", 800, 600, "Black Future"));
	splash.init();
    run(tick);
}
