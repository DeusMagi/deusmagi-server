## @file
## Script for the rocks puzzle in second level of Brynknot Sewers.

from Atrinik import *

def main():
    # The rocks.
    rocks = [
        ["world_0_71_-2", 13, 13, "brynknot_sewers_a_rock_1"],
        ["world_1_71_-2", 0, 4, "brynknot_sewers_a_rock_2"],
        ["world_2_71_-2", 1, 19, "brynknot_sewers_a_rock_3"],
        ["world_2_71_-2", 0, 9, "brynknot_sewers_a_rock_4"],
    ]

    for rock in rocks:
        m = ReadyMap(me.map.GetPath(rock[0]))
        m.Insert(m.LocateBeacon(rock[3]).env, rock[1], rock[2])

    key = me.FindObject(archname = "key2")

    if not activator.FindObject(INVENTORY_CONTAINERS, key.arch.name, key.name):
        key.Clone().InsertInto(activator)
        pl.DrawInfo("The rocks have moved back to their resting place and you have received a key.", COLOR_NAVY)
    else:
        pl.DrawInfo("The rocks have moved back to their resting place.", COLOR_NAVY)

main()
