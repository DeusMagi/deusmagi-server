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
 * Handles code related to @ref LIGHT_REFILL "light refills".
 *
 * @author Alex Tokar
 */

#include <global.h>
#include <object.h>
#include <player.h>
#include <object_methods.h>

/** @copydoc object_methods_t::apply_func */
static int
apply_func (object *op, object *applier, int aflags)
{
    HARD_ASSERT(op != NULL);
    HARD_ASSERT(applier != NULL);

    if (applier->type != PLAYER) {
        return OBJECT_METHOD_UNHANDLED;
    }

    object *light = find_marked_object(applier);
    if (light == NULL) {
        draw_info_format(COLOR_WHITE, applier,
                         "You need to mark a light source that you "
                         "want to refill.");
        return OBJECT_METHOD_OK;
    }

    char *light_name = object_get_name_s(light, applier);

    if (light->type != LIGHT_APPLY ||
        light->race == NULL ||
        light->race != op->race) {
        char *name = object_get_name_s(op, applier);
        draw_info_format(COLOR_WHITE, applier,
                         "You can't refill the %s with the %s.",
                         light_name,
                         name);
        efree(name);
        goto out;
    }

    int capacity_missing = light->stats.maxhp - light->stats.food;
    if (capacity_missing == 0) {
        draw_info_format(COLOR_WHITE, applier,
                         "The %s is full and can't be refilled.",
                         light_name);
        goto out;
    }

    int capacity_received = MIN(capacity_missing, op->stats.food);
    light->stats.food += capacity_received;

    int percent = (double) light->stats.food / light->stats.maxhp * 100.0;
    draw_info_format(COLOR_WHITE, applier,
                     "You refill the %s and it's now at %d%% of its capacity.",
                     light_name,
                     percent);

    /* Check whether the refilling object was all used up. If so,
     * decrease it, otherwise split it from the stack (if any) and
     * decrease the amount of units it gives back. */
    if (op->stats.food - capacity_received <= 0) {
        decrease_ob(op);
    } else {
        op = object_stack_get_reinsert(op, 1);
        op->stats.food -= capacity_received;
    }

out:
    efree(light_name);
    return OBJECT_METHOD_OK;
}

/**
 * Initialize the light refill type object methods.
 */
OBJECT_TYPE_INIT_DEFINE(light_refill)
{
    OBJECT_METHODS(LIGHT_REFILL)->apply_func = apply_func;
}
