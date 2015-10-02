#pragma once

namespace engine {

/* Initializes the random number generator. This should be called during
   program start-up.
   TODO: Future versions will support seed-passing.
*/
void init_rng();

/* Rolls a series of dice, returning the total. 'n' dice of type 'd', each
   with a distribution from 1..d
*/
int roll_dice(int n, int d);

}

