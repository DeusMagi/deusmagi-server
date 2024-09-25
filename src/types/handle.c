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
 * Handles code for @ref HANDLE "handles".
 *
 * @author Alex Tokar
 */

#include <global.h>
#include <object.h>
#include <object_methods.h>
#include <key.h>

/** @copydoc object_methods_t::apply_func */
static int
apply_func (object *op, object *applier, int aflags)
{
    HARD_ASSERT(op != NULL);
    HARD_ASSERT(applier != NULL);

    if (op->slaying != NULL) {
        if (key_match(applier, op) == NULL) {
            draw_info_format(COLOR_WHITE, applier,
                             "The %s is locked.",
                             op->name);
            return OBJECT_METHOD_OK;
        }
    } else if (!DBL_EQUAL(op->speed, 0.0) ||
               (op->stats.exp == -1 && op->value != 0)) {
        if (op->msg != NULL) {
            draw_info(COLOR_WHITE, applier, op->msg);
        } else {
            draw_info_format(COLOR_WHITE, applier,
                             "The %s won't budge.",
                             op->name);
        }

        return OBJECT_METHOD_OK;
    }

    /* Toggle the state. */
    op->value = !op->value;
    op->state = op->value;
    SET_ANIMATION(op, (((NUM_ANIMATIONS(op) / NUM_FACINGS(op)) *
                        op->direction) + op->value));
    object_update(op, UP_OBJ_FACE);

    /* Inform the applier. */
    if (op->msg != NULL) {
        draw_info(COLOR_WHITE, applier, op->msg);
    } else {
        draw_info_format(COLOR_WHITE, applier,
                         "You turn the %s.",
                         op->name);
        play_sound_map(op->map,
                       CMD_SOUND_EFFECT,
                       "pull.ogg",
                       op->x,
                       op->y,
                       0,
                       0);
    }

    connection_trigger(op, op->value);

    if (op->stats.exp != 0) {
        op->speed = 1.0 / op->stats.exp;
        op->speed_left = -1;
    } else {
        op->speed = 0;
    }

    object_update_speed(op);
    return OBJECT_METHOD_OK;
}

/** @copydoc object_methods_t::trigger_func */
static int
trigger_func (object *op, object *cause, int state)
{
    HARD_ASSERT(op != NULL);
    HARD_ASSERT(cause != NULL);

    if (!DBL_EQUAL(op->speed, 0.0) ||
        (op->stats.exp == -1 && op->value != 0)) {
        return OBJECT_METHOD_OK;
    }

    op->value = state;
    op->state = op->value;
    SET_ANIMATION(op, (((NUM_ANIMATIONS(op) / NUM_FACINGS(op)) *
                        op->direction) + op->value));
    object_update(op, UP_OBJ_FACE);

    return OBJECT_METHOD_OK;
}

/** @copydoc object_methods_t::process_func */
static void
process_func (object *op)
{
    HARD_ASSERT(op != NULL);

    op->speed = 0;
    object_update_speed(op);

    if (op->stats.exp == -1) {
        return;
    }

    op->value = 0;
    op->state = op->value;
    SET_ANIMATION(op, (((NUM_ANIMATIONS(op) / NUM_FACINGS(op)) *
                        op->direction) + op->value));
    object_update(op, UP_OBJ_FACE);

    connection_trigger(op, op->value);
}

/**
 * Initialize the handle type object methods.
 */
OBJECT_TYPE_INIT_DEFINE(handle)
{
    OBJECT_METHODS(TYPE_HANDLE)->apply_func = apply_func;
    OBJECT_METHODS(TYPE_HANDLE)->trigger_func = trigger_func;
    OBJECT_METHODS(TYPE_HANDLE)->process_func = process_func;
}
