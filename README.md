# bgame - Black Future

__Black Future__ is gradually turning into a game! It's eventually going to be a roguelike-lite ("roguelite"), trying to combine some of the
elements of the roguelike genre I enjoy the most, with some of the parts of games such as Sim City and Dwarf Fortress that keep me busy. For now,
the focus is on making something that _I_ want to play - in the hopes that others will enjoy it also. It's open source (GPL), so feel free to
poke around and use whatever you might find useful.

The game is written in modern C++ (mostly C++11, but with a few C++14 elements). I've tried to stay with zero-cost abstractions where possible,
so performance should remain solid as the game expands.

_Premise_

By the year 2525, Eden - the human homeworld - is crowded, scarred by war, and bordering on uninhabitable. It is decided to build colony ships,
and spread humanity's influence into the stars. After a (rigged) vote, it is determined that different classes of colony ship shall be built.
The _A Class_ colonies will be comprised of humanity's best and brightest: scientists, engineers, the best farmers - the people who actually
do something useful. The _B Class_ colonies will transport the people who __don't__ have anything to contribute to society: the insurance
adjusters, telephone sanitizers and atmosphere coordinators of the world. _C Class_ colonies will be comprised of the petty criminals who
don't pose an immediate danger to society (dangerous criminals will be left behind), and the _D Class_ vessels will be stocked with anyone
who didn't fit in the previous categories.

Realizing that the _A Class_ vessels are far too important for the future of humanity to take big risks, it is decided to perform the final
beta testing on _B Class_ colony ships. Hair dressers, feng-shui consultants and bloggers are loaded onto their ships, fed mimosa's, and
told great stories of their future in the stars. Then, they are put into deep-sleep, and the colony ships jump to hyperspace - under the guidance
of a very advanced, and thoroughly untested, artificial intelligence.

You are __Cordex__, the artificial intelligence in charge of Beta 1 of the colony program. Take off goes smoothly, and everyone is settled
in hyper-sleep while you lead your settlers to their new home. Then, of course, something goes wrong. Sensors register an impact, damage
control reports that the damage control sensors are damaged, and navigation goes haywire. In the blink of an eye, Cordex drops the stricken
ship from hyperspace, and searches the knowledge-base for further instructions. "TODO: Write emergency protocols" is the only guidance the
beta software has to offer, so Cordex improvises. Scanners detect a habitable - but uncharted - planet not too far away, and Cordex limps
towards it. Damage control sensors come online for long enough to indicate that the ship is too damaged to land, so Cordex has the ship's
systems load the computer and six randomly selected settlers (the ones nearest to the door) into the primary escape pod - and launches
towards the planet. The ship should remain in orbit for years to come, and hopefully the remaining settlers can be rescued.

Leaving a trail of devastation, the escape pod comes to rest on the newly discovered planet. Communications and defense systems are offline,
power is scarce - but six settlers are waking up from hyper-sleep, the food replicator and water condensers are online. There is still hope!
As the ship's AI - and honestly, the only competent being here - it is your duty to keep the settlers alive, help them rebuild, and use them
as your eyes, ears and limbs to either thrive here or escape the planet.

_Basic Gameplay_

(Most of the game isn't done yet!) __Cordex__ - the ship AI - runs on electricity. Power is initially generated from solar collectors onboard
the escape pod, and stored in attached battery packs. If Cordex runs out of power, it's game over - the computer powers down, and nobody
onboard is qualified to get it running again. Power is generated during the day, when there is sunlight. There isn't enough power to
use the ship's more advanced features - such as long-range communications. Only by securing more power can there be any hope of rescue!

The __Settlers__ run on food, water and sleep. When they get hungry or thirsty, settlers will run to the nearest source of food/water and
fill up. Currently, when they get sleepy - they take a nap wherever they happen to be (beds are currently being implemented). The escape
pod's replicator can produce an infinite amount of NutraGunk*, but doing so consumes power. The water condensers can likewise produce
enough water to sustain the settlers - but also consumes power. There is just enough power to keep everyone fed, and make it through the
dark night. If any expansion is to occur, alternate food/water sources must be found.

_* NutraGunk is the result of much scientific experiment. Taking the human requirement for nutrients and water, the brightest minds of a generation asked "why not both?" - and created a sludge that can sustain human life. It has the texture of cardboard, and tastes vaguely of chicken._

_Systems_

Internally, the game currently implements the following:
* A __device independent back-end__, providing a virtual terminal that can render to SDL2, NCurses and an ANSII escape-code renderer. This includes translation of old DOS code-page 437 characters to unicode as necessary, keyboard and mouse support.
* A __GUI framework__ and attached __game state stack__, used for the basic menus of the game.
* A __Console__, with colored formatting via tags.
* An __Entity Component System__. Entities are shallow: little more than an ID number that can be used for querying. Components are data. Using template programming, components are sorted into strongly-typed vectors (contiguous memory) for speed. Various search functions make for fast access to components by the systems that drive the game.
* A __world generator__, which produces a large world. It starts with a height-map, performs marching squares to identify contour levels (and place water), and then divides by climate. The result is a pleasing terrain.
* A __lighting system__, that dynamically lights terrain tiles by time of day, using a 'slope' value calculated during world-generation.
* A __serialization__ system that can load/save the entire world, and the state of the entire entity-component system.
* A __viewshed__ system that calculates fields-of-view quite accurately and efficiently.

The new user interface gives the whole window to the game, while retaining the pertinent information:
![New UI Screenshot](https://raw.githubusercontent.com/thebracket/bgame/master/assets/new-ui.png "New User Interface")

Settlers now have a direction, and associated field of view:
![Field of View Screenshot](https://raw.githubusercontent.com/thebracket/bgame/master/assets/field-of-view.png "New User Interface")

You can mouse over (and very soon, select with the "look at" keyboard control) a tile and learn all about it:
![Look At Screenshot](https://raw.githubusercontent.com/thebracket/bgame/master/assets/tooltip-demo.png "New User Interface")

All of this combines with dynamic lighting, a fading pop-up system for transient (not important) dialogue and emoting to provide an
engaging view of the settlers' life:
![Animated Screenshot](https://raw.githubusercontent.com/thebracket/bgame/master/assets/hyper-game-demo3.gif "Animated Screenshot")

## To build this:
It requires: SDL2, SD2 Image, and NCurses. I've tested on Linux and OS X. On Linux, I'm using g++5 - since I'd really miss the C++14 allocator lists. It's a pretty standard cmake, so:

### To setup the first time:
````
mkdir build
cd build
cmake -g "Unix Makefiles" ..
make
mkdir world
./bgame
````

### To build after that
````
cd build
mkdir world
make
./bgame
````
