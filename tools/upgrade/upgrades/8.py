def upgrade_func(arch):
    if arch["archname"] == "quest_container":
        i = Upgrader.arch_get_attr_num(arch, "name", None)

        if i != -1:
            if arch["attrs"][i][1] in ("Shipment of Charob Beer", "Ogres near Brynknot", "Evil Trees at Brynknot", "Forgotten Graveyard at Strakewood Island", "Giant Wasps at Strakewood Island", "Wyverns at Strakewood Island", "Ice Golems in Old Outpost", "Lava Golems in Old Outpost", "Manard's Prayerbook", "Cashin's Cap", "Tutorial Island Well", "Slimes at Tutorial Island", "Frah'ak and Kobolds", "Jahrlen's Heavy Rod", "Hill Giants Stronghold", "Dark Cave Elder Wyverns", "Enemies beneath Brynknot", "Investigation for Maplevale", "Aris Undead Infestation", "Ants under Fort Ghzal", "Battle of Fort Ghzal", "Knowledge of Meteor Swarm", "Knowledge of Meteor", "Vielumin Ruins Altar", "Rirain's Nightmares", "Hungry Wolves", "Quidton's Mana Crystal", "Improved Asteroid", "Gandyld's Quest", "Gandyld's Quest II"):
                arch["archname"] = None

        i = Upgrader.arch_get_attr_num(arch, "underground_city_lake_portal", None)

        if i != -1:
            arch["attrs"].pop(i)
    elif arch["archname"] == "barrel2.101":
        i = Upgrader.arch_get_attr_num(arch, "name", None)

        if i != -1:
            if arch["attrs"][i][1] == "shipment of Charob Beer":
                arch["archname"] = None
    elif arch["archname"] == "key2":
        i = Upgrader.arch_get_attr_num(arch, "name", None)

        if i != -1:
            if arch["attrs"][i][1] in ("Maplevale's amulet", "Brynknot Maze Key"):
                arch["archname"] = None
    elif arch["archname"] == "note":
        i = Upgrader.arch_get_attr_num(arch, "name", None)

        if i != -1:
            if arch["attrs"][i][1] == "Letter from Nyhelobo to oty captain":
                arch["archname"] = None
    elif arch["archname"] == "pillar_blue10":
        i = Upgrader.arch_get_attr_num(arch, "name", None)

        if i != -1:
            if arch["attrs"][i][1] == "blue crystal fragment":
                arch["archname"] = None
    elif arch["archname"] == "empty_archetype":
        i = Upgrader.arch_get_attr_num(arch, "name", None)

        if i != -1:
            if arch["attrs"][i][1] == "Silmedsen's potion bottle":
                i = Upgrader.arch_get_attr_num(arch, "face", None)

                if i != -1:
                    if arch["attrs"][i][1] == "potion_cyan.101":
                        arch["archname"] = "silmedsen_potion_bottle_filled"
                        arch["attrs"] = []
                    else:
                        arch["archname"] = "silmedsen_potion_bottle"
                        arch["attrs"] = []
    elif arch["archname"] == "event_obj":
        i = Upgrader.arch_get_attr_num(arch, "race", None)

        if i != -1:
            if arch["attrs"][i][1] == "/shattered_islands/scripts/lyondale_archipelago/morliana/telescope_quest.py":
                arch["attrs"][i][1] = "/python/items/silmedsen_potion_bottle.py"
    elif arch["archname"] == "tree_branch1":
        i = Upgrader.arch_get_attr_num(arch, "name", None)

        if i != -1:
            if arch["attrs"][i][1] == "Silmedsen's branches":
                arch["archname"] = "silmedsen_branches"
                arch["attrs"] = []
    elif arch["archname"] == "power_crystal":
        i = Upgrader.arch_get_attr_num(arch, "name", None)

        if i != -1:
            if arch["attrs"][i][1] == "Gandyld's Mana Crystal":
                arch["archname"] = None

    return arch

upgrader = Upgrader.ObjectUpgrader(files, upgrade_func)
upgrader.upgrade()
