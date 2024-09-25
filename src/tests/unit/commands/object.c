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

#include <global.h>
#include <check.h>
#include <checkstd.h>
#include <check_proto.h>
#include <arch.h>
#include <player.h>
#include <object.h>

START_TEST(test_put_object_in_sack)
{
    mapstruct *map;
    object *sack, *obj, *pl;

    check_setup_env_pl(&map, &pl);

    sack = arch_get("sack");
    object_insert_map(sack, map, NULL, 0);
    ck_assert_ptr_eq(GET_MAP_OB(map, 0, 0), sack);

    obj = arch_get("letter");
    obj->nrof = 1;
    obj->x = 1;
    object_insert_map(obj, map, NULL, 0);
    put_object_in_sack(pl, sack, obj, 1);
    ck_assert_ptr_eq(GET_MAP_OB(map, 1, 0), obj);
    ck_assert_ptr_eq(sack->inv, NULL);
    object_remove(sack, 0);
    object_destroy(sack);

    /* Basic insertion. */
    sack = arch_get("sack");
    sack->nrof = 1;
    ck_assert_uint_eq(sack->type, CONTAINER);
    object_insert_map(sack, map, NULL, 0);
    ck_assert_ptr_eq(GET_MAP_OB(map, 0, 0), sack);

    SET_FLAG(sack, FLAG_APPLIED);
    put_object_in_sack(pl, sack, obj, 1);
    ck_assert_ptr_eq(sack->inv, obj);
    ck_assert_ptr_eq(GET_MAP_OB(map, 1, 0), NULL);

    object_remove(obj, 0);
    obj->x = 1;
    object_insert_map(obj, map, NULL, 0);
    sack->weight_limit = 1;
    obj->weight = 5;

    put_object_in_sack(pl, sack, obj, 1);
    ck_assert_ptr_eq(sack->inv, NULL);
    ck_assert_ptr_eq(GET_MAP_OB(map, 1, 0), obj);
}

END_TEST

static Suite *suite(void)
{
    Suite *s = suite_create("object");
    TCase *tc_core = tcase_create("Core");

    tcase_add_unchecked_fixture(tc_core, check_setup, check_teardown);
    tcase_add_checked_fixture(tc_core, check_test_setup, check_test_teardown);

    suite_add_tcase(s, tc_core);
    tcase_add_test(tc_core, test_put_object_in_sack);

    return s;
}

void check_commands_object(void)
{
    check_run_suite(suite(), __FILE__);
}
