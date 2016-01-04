# To-Do List

**Tiles**

* I don't like the way flags are stored in tiles currently; it's cache-inefficient and clunky to access a whole tile when all you need is a yes/no. Clean it up.

**Visibility**

* Efficient ray-cast routine; cast to box rather than by angle, and cache the resulting shape per-size.

**Lighting**

* Create "lit tile" array of float RGB.
* Lighting pass: set ambient by time-of-day for outside, dark for inside.
* Sunlight: ray-cast surface tiles to the sun, and light if not occluded (will give perfect shadowing)
* Light sources: ray-cast from each light-source until range/occlusion occurs.
* Integrate into the render loop.

**Render becomes a system**

* Move the render code out of being a special case, and into an ECS system to benefit from the timing.

**GUI**

* Create a flexible GUI system, rather than the piecemeal approach we have right now.

**World-gen**

* Store the height-map, biomes and levels on world-gen.
* Don't create regions that we aren't using yet; instead, a flexible system to create them as needed. Since we already have the height/water levels this should be straightforward and will still tile perfectly.
* Rivers and lakes!
* Trees!
* Rock types for mining
* Refactor settler generation into its own module.

**Structure**

* Retire the 'world' module, globals belong in their place in the universe.
* Provide helper functions for commonly accessed component access.

**Overall goals**

* Get building working again. The structure is there, it needs GUI work.
* Mining
* Deconstruction
* Smart wall builder
* Once it's possible to build a fort, we have the basics of a game. Tweak.
* Single-settler control mode; turn-based roguelike play for a single settler.
* Add in animals.
* Add in hunting.
* Add in butchering.
* Add in tanning.
* Add in farming.
* Add in weaving.
* Add in clothes-making.
* Add in smelting.
* Add in forging.
* Add in oil and plastics.
* Start work on other civilizations
* Fighting and conversation...

**Engine**

* Logging system
* Input handler needs re-working
* Support different graphics modes

**Raws**

* Move to a standard file-format for easier reading.
* Optimize the storage, so data-structures don't require a big recursive search to find things like "buildables".

**Graphics**

* Tile-engine again in the future.

