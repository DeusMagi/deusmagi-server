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
 * Handles code for @ref MAP_EVENT_OBJ "map event objects".
 *
 * @author Alex Tokar
 */

#include <global.h>
#include <object.h>
#include <object_methods.h>

/** @copydoc object_methods_t::remove_map_func */
static void
remove_map_func (object *op)
{
    HARD_ASSERT(op != NULL);
    HARD_ASSERT(op->map != NULL);

    if (op->map->in_memory == MAP_SAVING) {
        return;
    }

    for (map_event *tmp = op->map->events, *prev = NULL;
         tmp != NULL;
         prev = tmp, tmp = tmp->next) {
        if (tmp->event == op) {
            if (prev == NULL) {
                op->map->events = tmp->next;
            } else {
                prev->next = tmp->next;
            }

            map_event_free(tmp);
            break;
        }
    }
}

/**
 * Initialize the map event object type object methods.
 */
OBJECT_TYPE_INIT_DEFINE(map_event_obj)
{
    OBJECT_METHODS(MAP_EVENT_OBJ)->remove_map_func = remove_map_func;
}
