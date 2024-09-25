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
 * Handles code related to @ref BOOK "books".
 */

#include <global.h>
#include <toolkit/packet.h>
#include <player.h>
#include <object.h>
#include <exp.h>
#include <object_methods.h>
#include <artifact.h>

#include "common/process_treasure.h"

/**
 * Maximum amount of difference in levels between the book's level and
 * the player's literacy skill.
 */
#define BOOK_LEVEL_DIFF 12

/**
 * Chance for generated random books to be empty - 1/x.
 */
#define BOOK_CHANCE_EMPTY 15

/**
 * Affects @ref BOOK_LEVEL_DIFF, depending on the player's intelligence
 * stat. If the player is intelligent enough, they may be able to read
 * higher level books; if their intelligence is too low, the maximum level
 * books they can read will decrease.
 */
static int book_level_mod[MAX_STAT + 1] = {
    -9,
    -8, -7, -6, -5, -4,
    -4, -3, -2, -2, -1,
    0, 0, 0, 0, 0,
    1, 1, 2, 2, 3,
    3, 3, 4, 4, 5,
    6, 7, 8, 9, 10
};

/**
 * The higher your intelligence, the more you are able to make use of the
 * knowledge you read from books. Thus, you get more experience by
 * reading books the more intelligence you have, and less experience if you
 * have unnaturally low intelligence.
 */
static double book_exp_mod[MAX_STAT + 1] = {
    -3.00f,
    -2.00f, -1.90f, -1.80f, -1.70f, -1.60f,
    -1.50f, -1.40f, -1.30f, -1.20f, -1.10f,
    1.00f, 1.00f, 1.00f, 1.00f, 1.00f,
    1.05f, 1.10f, 1.15f, 1.20f, 1.30f,
    1.35f, 1.40f, 1.50f, 1.55f, 1.60f,
    1.70f, 1.75f, 1.85f, 1.90f, 2.00f
};

/** @copydoc object_methods_t::apply_func */
static int
apply_func (object *op, object *applier, int aflags)
{
    HARD_ASSERT(op != NULL);
    HARD_ASSERT(applier != NULL);

    /* Non-players cannot apply books. */
    if (applier->type != PLAYER) {
        return OBJECT_METHOD_OK;
    }

    if (QUERY_FLAG(applier, FLAG_BLIND)) {
        draw_info(COLOR_WHITE, applier, "You are unable to read while blind.");
        return OBJECT_METHOD_OK;
    }

    if (op->msg == NULL) {
        draw_info_format(COLOR_WHITE, applier,
                         "You open the %s and find it empty.", op->name);
        return OBJECT_METHOD_OK;
    }

    /* Need a literacy skill to read stuff! */
    if (!change_skill(applier, SK_LITERACY)) {
        draw_info(COLOR_WHITE, applier,
                  "You are unable to decipher the strange symbols.");
        return OBJECT_METHOD_OK;
    }

    int lev_diff = op->level - (SK_level(applier) +
                                BOOK_LEVEL_DIFF +
                                book_level_mod[applier->stats.Int]);
    if (lev_diff > 0) {
        if (lev_diff < 2) {
            draw_info(COLOR_WHITE, applier,
                      "This book is just barely beyond your comprehension.");
        } else if (lev_diff < 3) {
            draw_info(COLOR_WHITE, applier,
                      "This book is slightly beyond your comprehension.");
        } else if (lev_diff < 5) {
            draw_info(COLOR_WHITE, applier,
                      "This book is beyond your comprehension.");
        } else if (lev_diff < 8) {
            draw_info(COLOR_WHITE, applier,
                      "This book is quite a bit beyond your comprehension.");
        } else if (lev_diff < 15) {
            draw_info(COLOR_WHITE, applier,
                      "This book is way beyond your comprehension.");
        } else {
            draw_info(COLOR_WHITE, applier,
                      "This book is totally beyond your comprehension.");
        }

        return OBJECT_METHOD_OK;
    }

    draw_info_format(COLOR_WHITE, applier,
                     "You open the %s and start reading.",
                     op->name);
    CONTR(applier)->stat_books_read++;

    packet_struct *packet = packet_new(CLIENT_CMD_BOOK, 512, 512);
    packet_debug_data(packet, 0, "Book interface header");
    packet_append_string(packet, "[book]");
    StringBuffer *sb = object_get_base_name(op, applier, NULL);
    packet_append_string_len(packet,
                             stringbuffer_data(sb),
                             stringbuffer_length(sb));
    stringbuffer_free(sb);
    packet_append_string(packet, "[/book]");
    packet_debug_data(packet, 0, "Book message");
    packet_append_string_terminated(packet, op->msg);
    socket_send_packet(CONTR(applier)->cs, packet);

    /* Gain xp from reading but only if not read before. */
    if (!QUERY_FLAG(op, FLAG_NO_SKILL_IDENT)) {
        CONTR(applier)->stat_unique_books_read++;

        /* Store original exp value. We want to keep the experience cap
         * from calc_skill_exp() below, so we temporarily adjust the exp
         * of the book, instead of adjusting the return value. */
        int64_t old_exp = op->stats.exp;
        /* Adjust the experience based on player's intelligence. */
        op->stats.exp *= book_exp_mod[applier->stats.Int];

        if (!QUERY_FLAG(op, FLAG_IDENTIFIED)) {
            /* Because they just identified it too. */
            op->stats.exp *= 1.5f;
            identify(op);
        }

        int64_t exp_gain = calc_skill_exp(applier, op, -1);
        add_exp(applier, exp_gain, applier->chosen_skill->stats.sp, 0);

        /* So no more exp gained from this book. */
        SET_FLAG(op, FLAG_NO_SKILL_IDENT);
        /* Restore old experience value. */
        op->stats.exp = old_exp;
    }

    return OBJECT_METHOD_OK;
}

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

    /* Avoid processing if the item is already special. */
    if (process_treasure_is_special(op)) {
        return OBJECT_METHOD_UNHANDLED;
    }

    /* If the book already has a message, don't overwrite it with a
     * generated one. */
    if (op->msg != NULL) {
        return OBJECT_METHOD_OK;
    }

    /* Chance for the book to be empty. */
    if (rndm_chance(BOOK_CHANCE_EMPTY)) {
        return OBJECT_METHOD_OK;
    }

    /* Calculate the level. Essentially -20 below the difficulty at worst, or
     * +20 above the difficulty at best. */
    int level = difficulty - 20 + rndm(0, 40);
    level = MIN(MAX(level, 1), MAXLEVEL);
    op->level = level;

    if (!artifact_generate(op, difficulty, affinity)) {
        tailor_readable_ob(op, -1);
    }

    /* Adjust the value and experience gains based on the message length. */
    size_t msg_len = op->msg != NULL ? strlen(op->msg) : 0;
    if (msg_len != 0) {
        int value_level = op->level > 10 ? op->level : (op->level + 1) / 2;
        op->value *= value_level * ((msg_len / 250) + 1);
        op->stats.exp = 105 + (msg_len / 25) + rndm(5, 15);
    }

    return OBJECT_METHOD_OK;
}

/**
 * Initialize the book type object methods.
 */
OBJECT_TYPE_INIT_DEFINE(book)
{
    OBJECT_METHODS(BOOK)->apply_func = apply_func;
    OBJECT_METHODS(BOOK)->process_treasure_func = process_treasure_func;
}
