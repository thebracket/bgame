#include "world_gen_noise.hpp"
#include "../../../engine/perlin_noise.hpp"
#include "../world_defs.hpp"

void make_noisy_heightmap ( heightmap_t* base_map, engine::random_number_generator * rng )
{
     constexpr int width = WORLD_WIDTH * REGION_WIDTH;
     constexpr int height = WORLD_HEIGHT * REGION_HEIGHT;
     const int random_seed = rng->roll_dice ( 1,65000 );
     perlin_noise mountains ( 0.1, 0.1, 1.0, 8, random_seed );
     perlin_noise mixer ( 0.1, 0.1, 1.0, 8, random_seed );

     for ( int y=0; y<height; ++y ) {
          for ( int x=0; x<width; ++x ) {
               const double X = x;
               const double Y = y;
               const double mtn_noise_pixel = ( mountains.get_height ( X, Y ) + 1.0 ) * 300.0;
               double mixer_noise_pixel = ( mixer.get_height ( X*0.25, Y*0.25 ) + 0.5 );
	       if (mixer_noise_pixel < 0.0) mixer_noise_pixel = 0.0;
	       if (mixer_noise_pixel > 1.0) mixer_noise_pixel = 1.0;

               const double height = mixer_noise_pixel * mtn_noise_pixel;

               base_map->operator[] ( height_map_idx ( x,y ) ) = height;
          }
     }

}
