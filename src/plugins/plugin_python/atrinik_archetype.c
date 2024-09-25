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
 * Atrinik Python plugin archetype related code.
 *
 * @author Alex Tokar
 */

#include <plugin_python.h>

/**
 * Archetype fields.
 */
static fields_struct fields[] = {
    {"name", FIELDTYPE_SHSTR, offsetof(archetype_t, name), 0, 0,
            "Name of the archetype.; str (readonly)"},
    {"head", FIELDTYPE_ARCH, offsetof(archetype_t, head), 0, 0,
            "The main part of a linked object.; Atrinik.Object.Object or None "
            "(readonly)"},
    {"more", FIELDTYPE_ARCH, offsetof(archetype_t, more), 0, 0,
            "Next part of a linked object.; Atrinik.Object.Object or None "
            "(readonly)"},
    {"clone", FIELDTYPE_OBJECT2, offsetof(archetype_t, clone), 0, 0,
            "Archetype's default object from which new objects are created "
            "using functions such as :func:`Atrinik.CreateObject`.; "
            "Atrinik.Object.Object (readonly)"}
};

/**
 * Get archetype's attribute.
 * @param at
 * Python archetype wrapper.
 * @param context
 * Void pointer to the field ID.
 * @return
 * Python object with the attribute value, NULL on failure.
 */
static PyObject *get_attribute(Atrinik_Archetype *at, void *context)
{
    return generic_field_getter(context, at->at);
}

/**
 * Create a new archetype wrapper.
 * @param type
 * Type object.
 * @param args
 * Unused.
 * @param kwds
 * Unused.
 * @return
 * The new wrapper.
 */
static PyObject *Atrinik_Archetype_new(PyTypeObject *type, PyObject *args,
        PyObject *kwds)
{
    Atrinik_Archetype *at = (Atrinik_Archetype *) type->tp_alloc(type, 0);
    if (at != NULL) {
        at->at = NULL;
    }

    return (PyObject *) at;
}

/**
 * Free an archetype wrapper.
 * @param pl
 * The wrapper to free.
 */
static void Atrinik_Archetype_dealloc(Atrinik_Archetype *at)
{
    at->at = NULL;
#ifndef IS_PY_LEGACY
    Py_TYPE(at)->tp_free((PyObject *) at);
#else
    at->ob_type->tp_free((PyObject *) at);
#endif
}

/**
 * Return a string representation of an archetype.
 * @param at
 * The archetype.
 * @return
 * Python object containing the name of the archetype.
 */
static PyObject *Atrinik_Archetype_str(Atrinik_Archetype *at)
{
    return Py_BuildValue("s", at->at->name);
}

static int Atrinik_Archetype_InternalCompare(Atrinik_Archetype *left,
        Atrinik_Archetype *right)
{
    return (left->at < right->at ? -1 : (left->at == right->at ? 0 : 1));
}

static PyObject *Atrinik_Archetype_RichCompare(Atrinik_Archetype *left,
        Atrinik_Archetype *right, int op)
{
    if (left == NULL || right == NULL ||
            !PyObject_TypeCheck((PyObject *) left, &Atrinik_ArchetypeType) ||
            !PyObject_TypeCheck((PyObject *) right, &Atrinik_ArchetypeType)) {
        Py_INCREF(Py_NotImplemented);
        return Py_NotImplemented;
    }

    return generic_rich_compare(op,
            Atrinik_Archetype_InternalCompare(left, right));
}

/**
 * This is filled in when we initialize our archetype type.
 */
static PyGetSetDef getseters[NUM_FIELDS + 1];

/**
 * Initialize the archetype wrapper.
 * @param module
 * The Atrinik Python module.
 * @return
 * 1 on success, 0 on failure.
 */
int Atrinik_Archetype_init(PyObject *module)
{
    size_t i;

    /* Field getters */
    for (i = 0; i < NUM_FIELDS; i++) {
        PyGetSetDef *def = &getseters[i];

        def->name = fields[i].name;
        def->get = (getter) get_attribute;
        def->set = NULL;
        def->doc = fields[i].doc;
        def->closure = &fields[i];
    }

    getseters[i].name = NULL;

    Atrinik_ArchetypeType.tp_name = "Atrinik.Archetype";
    Atrinik_ArchetypeType.tp_alloc = PyType_GenericAlloc;
    Atrinik_ArchetypeType.tp_basicsize = sizeof(Atrinik_Archetype);
    Atrinik_ArchetypeType.tp_dealloc = (destructor) Atrinik_Archetype_dealloc;
    Atrinik_ArchetypeType.tp_repr = (reprfunc) Atrinik_Archetype_str;
    Atrinik_ArchetypeType.tp_doc = "Atrinik archetypes";
    Atrinik_ArchetypeType.tp_richcompare = (richcmpfunc) Atrinik_Archetype_RichCompare;
    Atrinik_ArchetypeType.tp_getset = getseters;
    Atrinik_ArchetypeType.tp_new = Atrinik_Archetype_new;

    if (PyType_Ready(&Atrinik_ArchetypeType) < 0) {
        return 0;
    }

    Py_INCREF(&Atrinik_ArchetypeType);
    PyModule_AddObject(module, "Archetype",
            (PyObject *) &Atrinik_ArchetypeType);

    return 1;
}

/**
 * Utility method to wrap an archetype.
 * @param what
 * Archetype to wrap.
 * @return
 * Python object wrapping the real archetype.
 */
PyObject *wrap_archetype(archetype_t *at)
{
    /* Return None if no archetype was to be wrapped. */
    if (at == NULL) {
        Py_INCREF(Py_None);
        return Py_None;
    }

    Atrinik_Archetype *wrapper = PyObject_NEW(Atrinik_Archetype,
            &Atrinik_ArchetypeType);
    if (wrapper != NULL) {
        wrapper->at = at;
    }

    return (PyObject *) wrapper;
}

