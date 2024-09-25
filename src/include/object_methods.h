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
 * Object methods header file.
 */

#ifndef OBJECT_METHODS_H
#define OBJECT_METHODS_H

#include <decls.h>

typedef struct object_methods {
    /**
     * Initializes an object.
     *
     * @param op
     * The object to initialize.
     */
    void (*init_func)(object *op);

    /**
     * De-initializes an object.
     *
     * @param op
     * The object to de-initialize.
     */
    void (*deinit_func)(object *op);

    /**
     * Applies an object.
     *
     * @param op
     * The object to apply.
     * @param applier
     * The object that executes the apply action.
     * @param aflags
     * Special (always apply/unapply) flags.
     */
    int (*apply_func)(object *op,
                      object *applier,
                      int     aflags);

    /**
     * Processes an object, giving it the opportunity to move or react.
     *
     * @param op
     * The object to process.
     */
    void (*process_func)(object *op);

    /**
     * Returns the description of an object, as seen by the given observer.
     *
     * @param op
     * The object to describe.
     * @param observer
     * The object to which the description is made.
     * @param buf
     * Buffer that will contain the description.
     * @param size
     * Size of 'buf'.
     */
    void (*describe_func)(object *op,
                          object *observer,
                          char   *buf,
                          size_t  size);

    /**
     * Triggered when an object moves moves off a square and when object
     * moves onto a square.
     *
     * @param op
     * The object that wants to catch this event.
     * @param victim
     * The object moving.
     * @param originator
     * The object that is the cause of the move.
     * @param state
     * 1 if the object is moving onto a square, 0 if moving
     * off a square.
     */
    int (*move_on_func)(object *op,
                        object *victim,
                        object *originator,
                        int     state);

    /**
     * An object is triggered by another one.
     *
     * @param op
     * The object being triggered.
     * @param cause
     * The object that is the cause of the trigger.
     * @param state
     * Trigger state.
     */
    int (*trigger_func)(object *op,
                        object *cause,
                        int     state);

    /**
     * An object is triggered by a button.
     *
     * @param op
     * The object being triggered.
     * @param cause
     * The object that is the cause of the trigger; the button.
     * @param state
     * Trigger state.
     */
    int (*trigger_button_func)(object *op,
                               object *cause,
                               int     state);

    /**
     * Called when an object is inserted on a map.
     *
     * @param op
     * The object being inserted.
     */
    void (*insert_map_func)(object *op);

    /**
     * Called when an object is removed from map.
     *
     * @param op
     * The object being removed.
     */
    void (*remove_map_func)(object *op);

    /**
     * Called when an object is removed from inventory.
     *
     * @param op
     * The object being removed.
     */
    void (*remove_inv_func)(object *op);

    /**
     * Function to handle firing a projectile, eg, an arrow being fired
     * from a bow, or a potion being thrown.
     *
     * @param op
     * What is being fired.
     * @param shooter
     * Who is doing the firing.
     * @param dir
     * Direction to fire into.
     * @return
     * The fired object on success, NULL on failure.
     */
    object *(*projectile_fire_func)(object *op,
                                    object *shooter,
                                    int     dir);

    /**
     * Function to handle a fired object moving, eg, arrow moving to the
     * next square along its path.
     *
     * @param op
     * The fired object.
     * @return
     * The fired object, NULL if it was destroyed for some reason.
     */
    object *(*projectile_move_func)(object *op);

    /**
     * Called when a fired object finds an alive object on the square it
     * just moved to.
     *
     * @param op
     * The fired object.
     * @param victim
     * The found alive object. Note that this just means that the object
     * is on the @ref LAYER_LIVING layer, which may or may not imply that
     * the object is actually alive.
     * @retval OBJECT_METHOD_OK
     * Successfully processed and should stop the fired arch.
     * @retval OBJECT_METHOD_UNHANDLED
     * Did not handle the event, should continue trying to look for another
     * alive object.
     * @retval OBJECT_METHOD_ERROR
     * 'op' was destroyed.
     */
    int (*projectile_hit_func)(object *op,
                               object *victim);

    /**
     * Called to stop a fired object.
     *
     * @param op
     * The fired object.
     * @param reason
     * Reason for stopping, one of @ref OBJECT_PROJECTILE_STOP_xxx.
     * @return
     * The fired object if it still exists, NULL otherwise.
     */
    object *(*projectile_stop_func)(object *op,
                                    int     reason);

    /**
     * Used to fire a ranged weapon, eg, a bow firing arrows, throwing
     * weapons/potions, firing wands/rods, etc.
     *
     * @param op
     * The weapon being fired (bow, wand, throwing object).
     * @param shooter
     * Who is doing the firing.
     * @param dir
     * Direction to fire into.
     * @param[out] delay
     * If non-NULL, will contain delay caused by this action.
     * @return
     * One of @ref OBJECT_METHOD_xxx.
     */
    int (*ranged_fire_func)(object *op,
                            object *shooter,
                            int     dir,
                            double *delay);

    /**
     * Processes an object with #FLAG_AUTO_APPLY.
     *
     * @param op
     * The object to process.
     */
    void (*auto_apply_func)(object *op);

    /**
     * Process generated treasure.
     *
     * @param op
     * Object to process.
     * @param[out] ret
     * If the function returns OBJECT_METHOD_OK, this variable will
     * contain the processed treasure object, which may be different
     * from 'op' (which may be destroyed). Indeterminate on any other
     * return value.
     * @param difficulty
     * Difficulty level.
     * @param affinity
     * Treasure affinity.
     * @param flags
     * A combination of @ret GT_xxx flags.
     * @return
     * One of @ref OBJECT_METHOD_xxx.
     * @warning
     * If OBJECT_METHOD_ERROR is returned from this function, it is
     * possible that the original object has been destroyed and thus
     * any further processing should stop.
     */
    int (*process_treasure_func)(object               *op,
                                 object             **ret,
                                 int                  difficulty,
                                 treasure_affinity_t *affinity,
                                 int                  flags);

    /**
     * If true, will override the standard treasure processing
     * such as setting a magic bonus, generating artifacts, etc.
     */
    bool override_treasure_processing:1;

    /**
     * Fallback methods.
     */
    struct object_methods *fallback;
} object_methods_t;

/**
 * @defgroup OBJECT_METHOD_xxx Object method return values
 * Object method return values.
 *@{*/
/** The object was not handled. */
#define OBJECT_METHOD_UNHANDLED 0
/** Successfully handled. */
#define OBJECT_METHOD_OK 1
/** Error handling the object. */
#define OBJECT_METHOD_ERROR 2
/*@}*/

/**
 * @defgroup OBJECT_PROJECTILE_STOP_xxx Projectile stop reasons
 * Reasons for projectile to stop.
 *@{*/
/** Projectile has reached its end of the line. */
#define OBJECT_PROJECTILE_STOP_EOL 1
/** Project has hit an alive object. */
#define OBJECT_PROJECTILE_STOP_HIT 2
/** Projectile has hit a wall. */
#define OBJECT_PROJECTILE_STOP_WALL 3
/** Projectile has been picked up. */
#define OBJECT_PROJECTILE_PICKUP 4
/*@}*/

/**
 * Begins a definition for a single object type init function.
 *
 * @param what
 * Name of the object type.
 */
#define OBJECT_TYPE_INIT_DEFINE(what)                   \
    void CONCAT(object_type_init_, what)(void);         \
    void CONCAT(object_type_init_, what)(void)

/**
 * Acquire object methods of the specified type.
 *
 * @param type
 * Object type to get methods for.
 */
#define OBJECT_METHODS(type) (object_methods_get(type))

/* Prototypes */

void
object_methods_init(void);
object_methods_t *
object_methods_get(int type);
void
object_cb_init(object *op);
void
object_cb_deinit(object *op);
int
object_apply(object *op, object *applier, int aflags);
void
object_process(object *op);
char *
object_describe(object *op, object *observer, char *buf, size_t size);
int
object_move_on(object *op, object *victim, object *originator, int state);
int
object_trigger(object *op, object *cause, int state);
int
object_trigger_button(object *op, object *cause, int state);
void
object_cb_insert_map(object *op);
void
object_cb_remove_map(object *op);
void
object_cb_remove_inv(object *op);
object *
object_projectile_fire(object *op, object *shooter, int dir);
object *
object_projectile_move(object *op);
int
object_projectile_hit(object *op, object *victim);
object *
object_projectile_stop(object *op, int reason);
int
object_ranged_fire(object *op, object *shooter, int dir, double *delay);
void
object_auto_apply(object *op);
int
object_process_treasure(object              *op,
                        object             **ret,
                        int                  difficulty,
                        treasure_affinity_t *affinity,
                        int                  flags);

#endif
