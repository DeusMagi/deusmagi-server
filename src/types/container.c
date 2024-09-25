/*************************************************************************
 *           Atrinik, a Multiplayer Online Role Playing Game             *
 *                                                                       *
 *   Copyright (C) 2009-2014 Alex Tokar and Atrinik Development Team     *
 *                                                                       *
 * Fork from Crossfire (Multiplayer game for X-windows).                 *
 *                                                                       *
 * This program is free software; you can redistribute it and/or modify  *
 * it under the terms of the GNU General Public License as published by  *
 * the Free Software Foundation; either version 2 of the License, or     *
 * (at your option) any later version.                                   *
 *                                                                       *
 * This program is distributed in the hope that it will be useful,       *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 * GNU General Public License for more details.                          *
 *                                                                       *
 * You should have received a copy of the GNU General Public License     *
 * along with this program; if not, write to the Free Software           *
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.             *
 *                                                                       *
 * The author can be reached at admin@atrinik.org                        *
 ************************************************************************/

/**
 * @file
 * Handles code for handling @ref CONTAINER "containers".
 *
 * @author Alex Tokar
 */

#include <global.h>
#include <plugin.h>
#include <arch.h>
#include <player.h>
#include <object.h>
#include <object_methods.h>
#include <container.h>
#include <rune.h>
#include <key.h>

/**
 * Check if both objects are magical containers.
 *
 * @param op
 * Object being put into the container.
 * @param container
 * The container.
 * @return
 * True if both op and container are magical containers, false otherwise.
 */
bool
container_check_magical (object *op, object *container)
{
    if (op->type == CONTAINER && container->type == CONTAINER &&
        !DBL_EQUAL(op->weapon_speed, 1.0) &&
        !DBL_EQUAL(container->weapon_speed, 1.0)) {
        return true;
    }

    return false;
}

/**
 * Actually open a container, springing traps/monsters, and doing the
 * linked list linking.
 *
 * @param applier
 * Player that is opening the container.
 * @param op
 * The container.
 */
static void
container_open (object *applier, object *op)
{
    HARD_ASSERT(applier != NULL);
    HARD_ASSERT(op != NULL);

    /* Safety. */
    if (applier->type != PLAYER) {
        return;
    }

    /* Safety. */
    if (op->attacked_by && op->attacked_by->type != PLAYER) {
        op->attacked_by = NULL;
    }

    /* Check for quest containers. */
    if (HAS_EVENT(op, EVENT_QUEST)) {
        FOR_INV_PREPARE(op, inv) {
            if (inv->type == QUEST_CONTAINER) {
                quest_handle(applier, inv);
            }
        } FOR_INV_FINISH();
    }

    player *pl = CONTR(applier);
    pl->container = op;
    pl->container_count = op->count;
    pl->container_above = op->attacked_by;

    /* Already opened. */
    if (op->attacked_by != NULL) {
        CONTR(op->attacked_by)->container_below = applier;
    } else {
        /* Not open yet. */
        SET_FLAG(op, FLAG_APPLIED);

        if (op->other_arch != NULL) {
            op->face = op->other_arch->clone.face;
            op->animation_id = op->other_arch->clone.animation_id;
            SET_ANIMATION_STATE(op);
            esrv_update_item(UPD_FACE | UPD_ANIM | UPD_FLAGS, op);
        } else {
            esrv_update_item(UPD_FLAGS, op);
        }

        object_update(op, UP_OBJ_FACE);

        FOR_INV_PREPARE(op, tmp) {
            if (tmp->type == RUNE) {
                rune_spring(tmp, applier);
            } else if (tmp->type == MONSTER) {
                object_remove(tmp, 0);
                tmp->x = op->x;
                tmp->y = op->y;

                int i = map_free_spot(op->map,
                                       tmp->x,
                                       tmp->y,
                                       0,
                                       SIZEOFFREE1, tmp->arch,
                                       tmp);
                if (i != -1) {
                    tmp->x += freearr_x[i];
                    tmp->y += freearr_y[i];
                }

                tmp = object_insert_map(tmp, op->map, tmp, 0);
                if (tmp != NULL) {
                    living_update_monster(tmp);
                    char *name = object_get_base_name_s(op, applier);
                    char *monster_name = object_get_base_name_s(tmp, applier);
                    draw_info_format(COLOR_WHITE, applier,
                                     "A %s jumps out of the %s!",
                                     monster_name,
                                     name);
                    efree(name);
                    efree(monster_name);
                }
            }
        } FOR_INV_FINISH();
    }

    esrv_send_inventory(applier, op);
    pl->container_below = NULL;
    op->attacked_by = applier;
    op->attacked_by_count = applier->count;
}

/**
 * Close a container and remove player from the container's linked list.
 *
 * @param applier
 * The player. If NULL, we will unlink all players from
 * the container 'op'.
 * @param op
 * The container object. If NULL, unlink the applier's current
 * container.
 * @return
 * True if the container was closed and has no players left looking
 * into the container, false otherwise.
 */
bool
container_close (object *applier, object *op)
{
    HARD_ASSERT(applier != NULL || op != NULL);

    if (applier != NULL && applier->type == PLAYER) {
        player *pl = CONTR(applier);

        /* No container, nothing to do. */
        if (pl->container == 0) {
            return false;
        }

        /* Make sure the object is valid. */
        if (!OBJECT_VALID(pl->container, pl->container_count)) {
            pl->container = NULL;
            pl->container_count = 0;
            return false;
        }

        /* Only applier left, go ahead and close the container. */
        if (pl->container_below == NULL && !pl->container_above) {
            return container_close(NULL, pl->container);
        }

        /* The applier is at the beginning of the linked list. */
        if (pl->container_below == NULL) {
            pl->container->attacked_by = pl->container_above;
            pl->container->attacked_by_count = pl->container_above->count;
            CONTR(pl->container_above)->container_below = NULL;
        } else {
            /* Elsewhere in the list. */
            CONTR(pl->container_below)->container_above = pl->container_above;

            if (pl->container_above != NULL) {
                CONTR(pl->container_above)->container_below =
                    pl->container_below;
            }
        }

        esrv_close_container(applier, pl->container);
        pl->container_above = NULL;
        pl->container_below = NULL;
        pl->container = NULL;
        pl->container_count = 0;
    } else if (op != NULL) {
        if (op->attacked_by == NULL) {
            return false;
        }

        CLEAR_FLAG(op, FLAG_APPLIED);

        if (op->other_arch != NULL) {
            op->face = op->arch->clone.face;
            op->animation_id = op->arch->clone.animation_id;
            SET_ANIMATION_STATE(op);
            esrv_update_item(UPD_FACE | UPD_ANIM | UPD_FLAGS, op);
        } else {
            esrv_update_item(UPD_FLAGS, op);
        }

        object_update(op, UP_OBJ_FACE);

        for (object *tmp = op->attacked_by, *next; tmp != NULL; tmp = next) {
            next = CONTR(tmp)->container_above;

            CONTR(tmp)->container = NULL;
            CONTR(tmp)->container_count = 0;
            CONTR(tmp)->container_below = NULL;
            CONTR(tmp)->container_above = NULL;
            esrv_close_container(tmp, op);
        }

        op->attacked_by = NULL;
        op->attacked_by_count = 0;

        if (op->env != NULL && op->env->type == PLAYER && OBJECT_IS_AMMO(op)) {
            living_update(op->env);
        }

        return true;
    }

    return false;
}

/** @copydoc object_methods_t::apply_func */
static int
apply_func (object *op, object *applier, int aflags)
{
    HARD_ASSERT(op != NULL);
    HARD_ASSERT(applier != NULL);

    if (applier->type != PLAYER) {
        return OBJECT_METHOD_UNHANDLED;
    }

    object *container = CONTR(applier)->container;
    if (op == NULL || op->type != CONTAINER ||
        (container != NULL && container->type != CONTAINER)) {
        return OBJECT_METHOD_UNHANDLED;
    }

    /* Already opened container, so close it, even if the player wants to
     * open another container. */
    if (container != NULL) {
        /* Trigger the CLOSE event. */
        if (trigger_event(EVENT_CLOSE,
                          applier,
                          container,
                          NULL,
                          NULL,
                          0,
                          0,
                          0,
                          0) != 0) {
            return OBJECT_METHOD_OK;
        }

        char *name = object_get_base_name_s(container, applier);
        if (container_close(applier, container)) {
            draw_info_format(COLOR_WHITE, applier, "You close %s.", name);
        } else {
            draw_info_format(COLOR_WHITE, applier, "You leave %s.", name);
        }
        efree(name);

        /* Applied the one we just closed, no need to go on. */
        if (container == op) {
            return OBJECT_METHOD_OK;
        }
    }

    int basic_aflag = aflags & APPLY_BASIC_FLAGS;

    if (basic_aflag == APPLY_ALWAYS_UNAPPLY) {
        if (QUERY_FLAG(op, FLAG_APPLIED)) {
            if (OBJECT_IS_AMMO(op)) {
                object_apply_item(op, applier, aflags);
            } else {
                char *name = object_get_base_name_s(op, applier);
                draw_info_format(COLOR_WHITE, applier, "You unready %s.", name);
                efree(name);
                CLEAR_FLAG(op, FLAG_APPLIED);
            }
        }

        return OBJECT_METHOD_OK;
    }

    /* If the player is trying to open it (which he must be doing if we
     * got here), and it is locked, check to see if player has the means
     * to open it. */
    if (op->slaying != NULL || op->stats.maxhp != 0) {
        if (op->sub_type == ST1_CONTAINER_NORMAL) {
            /* Locked container, try to open it with a key. */
            char *name = object_get_base_name_s(op, applier);
            object *tmp = key_match(applier, op);
            if (tmp != NULL) {
                if (tmp->type == KEY) {
                    char *key_name = object_get_base_name_s(tmp, applier);
                    draw_info_format(COLOR_WHITE, applier,
                            "You unlock %s with %s.", name, key_name);
                    efree(key_name);
                } else if (tmp->type == FORCE) {
                    draw_info_format(COLOR_WHITE, applier,
                            "The %s is unlocked for you.", name);
                }

                efree(name);
            } else {
                draw_info_format(COLOR_WHITE, applier,
                        "You don't have the key to unlock %s.", name);
                efree(name);
                return OBJECT_METHOD_OK;
            }
        } else {
            if (op->sub_type == ST1_CONTAINER_CORPSE_party &&
                !party_can_open_corpse(applier, op)) {
                /* Party corpse. */
                return OBJECT_METHOD_OK;
            } else if (op->sub_type == ST1_CONTAINER_CORPSE_player &&
                       op->slaying != applier->name) {
                /* Normal player-only corpse. */
                draw_info(COLOR_WHITE, applier, "It's not your bounty.");
                return OBJECT_METHOD_OK;
            }
        }
    }

    char *name = object_get_base_name_s(op, applier);

    if (op->env != applier) {
        /* If in inventory of some other object other than the applier,
         * can't open it. */
        if (op->env != NULL) {
            draw_info_format(COLOR_WHITE, applier, "You can't open %s.", name);
            efree(name);
            return OBJECT_METHOD_OK;
        }

        draw_info_format(COLOR_WHITE, applier, "You open %s.", name);
        container_open(applier, op);

        /* Handle party corpses. */
        if (op->slaying != NULL && op->sub_type == ST1_CONTAINER_CORPSE_party) {
            party_handle_corpse(applier, op);
        }
    } else {
        /* If it's readied, open it, otherwise ready it. */
        if (QUERY_FLAG(op, FLAG_APPLIED)) {
            draw_info_format(COLOR_WHITE, applier, "You open %s.", name);
            container_open(applier, op);
        } else {
            if (OBJECT_IS_AMMO(op)) {
                object_apply_item(op, applier, aflags);
            } else {
                draw_info_format(COLOR_WHITE, applier, "You ready %s.", name);
                SET_FLAG(op, FLAG_APPLIED);

                object_update(op, UP_OBJ_FACE);
                esrv_update_item(UPD_FLAGS, op);
            }
        }
    }

    efree(name);

    /* If it's a corpse and it has not been searched before, add to
     * player's statistics. */
    if ((op->sub_type == ST1_CONTAINER_CORPSE_party ||
         op->sub_type == ST1_CONTAINER_CORPSE_player) &&
        !QUERY_FLAG(op, FLAG_BEEN_APPLIED)) {
        CONTR(applier)->stat_corpses_searched++;
    }

    /* Only after actually readying/opening the container we know more
     * about it. */
    SET_FLAG(op, FLAG_BEEN_APPLIED);

    return OBJECT_METHOD_OK;
}

/** @copydoc object_methods_t::remove_inv_func */
static void
remove_inv_func (object *op)
{
    container_close(NULL, op);
}

/** @copydoc object_methods_t::remove_map_func */
static void
remove_map_func (object *op)
{
    remove_inv_func(op);
}

/**
 * Initialize the container type object methods.
 */
OBJECT_TYPE_INIT_DEFINE(container)
{
    OBJECT_METHODS(CONTAINER)->apply_func = apply_func;
    OBJECT_METHODS(CONTAINER)->remove_inv_func = remove_inv_func;
    OBJECT_METHODS(CONTAINER)->remove_map_func = remove_map_func;
}
