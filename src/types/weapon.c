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
 * Handles code for @ref WEAPON "weapons".
 *
 * @author Alex Tokar
 */

#include <global.h>
#include <object_methods.h>
#include <object.h>

#include "common/process_treasure.h"

/** @copydoc object_methods_t::process_treasure_func */
static int
process_treasure_func (object              *op,
                       object             **ret,
                       int                  difficulty,
                       treasure_affinity_t *affinity,
                       int                  flags)
{
    HARD_ASSERT(op != NULL);
    HARD_ASSERT(difficulty > 0);

    /* Avoid processing if the item is not special. */
    if (!process_treasure_is_special(op)) {
        return OBJECT_METHOD_UNHANDLED;
    }

    /* Only handle adding a slaying race for weapons of assassination or
     * slaying. */
    if (op->slaying != shstr_cons.none) {
        return OBJECT_METHOD_UNHANDLED;
    }

    ob_race *race = race_get_random();
    if (race != NULL) {
        FREE_AND_COPY_HASH(op->slaying, race->name);
    }

    return OBJECT_METHOD_OK;
}

/**
 * Initialize the weapon type object methods.
 */
OBJECT_TYPE_INIT_DEFINE(weapon)
{
    OBJECT_METHODS(WEAPON)->apply_func = object_apply_item;
    OBJECT_METHODS(WEAPON)->process_treasure_func = process_treasure_func;
}
