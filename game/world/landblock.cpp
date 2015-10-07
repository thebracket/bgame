#include "landblock.h"
#include "../../engine/rng.h"

land_block::land_block()
{
  tiles.resize(tiles_per_landblock);
  for (tile &t : tiles) {
    int symbol = engine::roll_dice(1,6);
    switch (symbol) {
      case 1 : t.display = '.'; break;
      case 2 : t.display = '#'; break;
      case 3 : t.display = '`'; break;
      case 4 : t.display = '\''; break;
      case 5 : t.display = '"'; break;
      case 6 : t.display = '%'; break;
    }
  }
}
