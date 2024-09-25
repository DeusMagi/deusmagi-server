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
 * Implements the /take command.
 *
 * @author Alex Tokar
 */

#include <global.h>
#include <plugin.h>
#include <player.h>
#include <object.h>

/** @copydoc command_func */
void command_take(object *op, const char *command, char *params)
{
    object *tmp, *next;
    int did_one = 0, missed = 0, ival;
    uint32_t ground_total = 0;

    if (!params) {
        draw_info(COLOR_WHITE, op, "Take what?");
        return;
    }

    if (CONTR(op)->container) {
        tmp = CONTR(op)->container->inv;
    } else {
        tmp = GET_MAP_OB_LAST(op->map, op->x, op->y);
    }

    if (!tmp) {
        draw_info(COLOR_WHITE, op, "Nothing to take.");
        return;
    }

    if (op->map && op->map->events && trigger_map_event(MEVENT_CMD_TAKE, op->map, op, tmp, NULL, params, 0)) {
        return;
    }

    SET_FLAG(op, FLAG_NO_FIX_PLAYER);

    for (; tmp; tmp = next) {
        next = tmp->below;

        if ((tmp->layer != LAYER_ITEM && tmp->layer != LAYER_ITEM2) || QUERY_FLAG(tmp, FLAG_NO_PICK) || IS_INVISIBLE(tmp, op)) {
            continue;
        }

        ival = object_matches_string(tmp, op, params);

        if (ival > 0) {
            if (ival <= 2 && !object_can_pick(op, tmp)) {
                missed++;
            } else {
                pick_up(op, tmp, 1);
                did_one = 1;
            }
        }

        /* Keep track how many visible objects are left on the ground. */
        if (!CONTR(op)->container && !tmp->env) {
            if (CONTR(op)->tsi || !(tmp->layer <= LAYER_FMASK || IS_INVISIBLE(tmp, op))) {
                ground_total++;
            }
        }
    }

    CLEAR_FLAG(op, FLAG_NO_FIX_PLAYER);

    if (did_one) {
        living_update(op);

        FOR_MAP_LAYER_BEGIN(op->map, op->x, op->y, LAYER_LIVING, -1, tmp) {
            if (tmp->type != PLAYER) {
                continue;
            }

            if (CONTR(tmp)->cs->look_position > ground_total) {
                CONTR(tmp)->cs->look_position = ground_total;
            }
        } FOR_MAP_LAYER_END
    } else if (!missed) {
        draw_info(COLOR_WHITE, op, "Nothing to take.");
    }

    if (missed == 1) {
        draw_info(COLOR_WHITE, op, "You were unable to take one of the items.");
    } else if (missed > 1) {
        draw_info_format(COLOR_WHITE, op, "You were unable to take %d of the items.", missed);
    }

    CONTR(op)->count = 0;
}
