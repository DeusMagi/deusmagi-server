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
 * Attack and defense system macros, defines, etc.
 */

#ifndef ATTACK_H
#define ATTACK_H

/**
 * The attack IDs.
 */
typedef enum atnr {
    /** Impact. */
    ATNR_IMPACT,
    /** Slash. */
    ATNR_SLASH,
    /** Cleave. */
    ATNR_CLEAVE,
    /** Pierce. */
    ATNR_PIERCE,
    /** Weapon magic. Common for spells like magic bullet. */
    ATNR_WEAPON_MAGIC,

    /** Fire. */
    ATNR_FIRE,
    /** Cold. */
    ATNR_COLD,
    /** Electricity. */
    ATNR_ELECTRICITY,
    /** Poison, */
    ATNR_POISON,
    /** Acid. */
    ATNR_ACID,

    /** Magic. */
    ATNR_MAGIC,
    /** Mind. */
    ATNR_LIFESTEAL,
    /** Blind. */
    ATNR_BLIND,
    /** Paralyze. Affected object will be rooted to the spot. */
    ATNR_PARALYZE,
    /** Force. */
    ATNR_FORCE,

    /** Godpower. */
    ATNR_GODPOWER,
    /** Chaos. */
    ATNR_CHAOS,
    /** Drain. */
    ATNR_DRAIN,
    /** Slow. */
    ATNR_SLOW,
    /**
     * Confusion. Affected object will move in random directions until
     * the effect wears off.
     */
    ATNR_CONFUSION,

    /** Used for internal calculations. */
    ATNR_INTERNAL,

    /** Number of the attacks. */
    NROFATTACKS
} atnr_t;

/**
 * Last valid protection, used for treasure generation.
 */
#define LAST_PROTECTION (ATNR_CONFUSION + 1)

/* Prototypes */
const char *const attack_save[NROFATTACKS];
const char *const attack_name[NROFATTACKS];

int
attack_object(object *op, object *hitter);
int
attack_hit(object *op, object *hitter, int dam);
void
attack_hit_map(object *op, int dir, bool multi_reduce);
bool
attack_kill(object *op, object *hitter);
void
attack_perform_poison(object *op, object *hitter, double dam);
void
attack_perform_slow(object *op);
void
attack_perform_confusion(object *op);
void
attack_perform_blind(object *op, object *hitter, double dam);
void
attack_peform_paralyze(object *op, double dam);
void
attack_perform_fall(object *op, int fall_floors);
bool
attack_is_melee_range(object *hitter, object *enemy);

#endif
