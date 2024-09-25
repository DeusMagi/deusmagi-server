## @file
## Implements geysers in Dark Cave on Strakewood Island.

from random import randint

from Atrinik import *
from Common import diagonal_distance


## Number of geysers to create at once.
geysers = randint(1, 3)

# Now create the geysers.
for _ in range(0, geysers):
    # Randomly choose where to place the geyser.
    x = randint(0, me.map.width - 1)
    y = randint(0, me.map.height - 1)

    floor = None
    for obj in me.map.ObjectsReversed(x, y):
        if obj.type == Type.FLOOR:
            floor = obj
            break

    # Must have at least one object, and it must be 'fire'.
    if floor and floor.name == "fire":
        # Play firestorm-like sound.
        me.map.PlaySound("magic_fire.ogg", x, y)

        # Now create the geyser.
        for i in range(0, SIZEOFFREE):
            new_x = x + freearr_x[i]
            new_y = y + freearr_y[i]

            # Don't allow geysers to go out of map.
            if new_x < 0 or new_x >= me.map.width or new_y < 0 or new_y >= me.map.height:
                continue

            # Now create the geyser.
            fire = me.map.CreateObject("firebreath", new_x, new_y)
            # And calculate how long it should stay there.
            fire.speed_left = -diagonal_distance(x, y, new_x, new_y)
