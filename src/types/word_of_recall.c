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
 * Handles code for @ref WORD_OF_RECALL "word of recall" objects.
 *
 * @author Alex Tokar
 */

#include <global.h>
#include <object.h>
#include <object_methods.h>

/** @copydoc object_methods_t::process_func */
static void
process_func (object *op)
{
    HARD_ASSERT(op != NULL);

    if (op->env != NULL && op->env->map && op->env->type == PLAYER) {
        if (blocks_magic(op->env->map, op->env->x, op->env->y)) {
            draw_info(COLOR_WHITE, op, "You feel something fizzle inside you.");
        } else {
            object_enter_map(op->env, op, NULL, 0, 0, false);
        }
    }

    object_remove(op, 0);
    object_destroy(op);
}

/**
 * Initialize the word of recall type object methods.
 */
OBJECT_TYPE_INIT_DEFINE(word_of_recall)
{
    OBJECT_METHODS(WORD_OF_RECALL)->process_func = process_func;
}
