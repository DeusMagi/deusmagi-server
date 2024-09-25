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
 * Atrinik Python plugin region related code.
 *
 * @author Alex Tokar
 */

#include <plugin_python.h>

/**
 * Region fields.
 */
static fields_struct fields[] = {
    {"next", FIELDTYPE_REGION, offsetof(region_struct, next), 0, 0,
            "Next region in a linked list.; Atrinik.Region.Region or None "
            "(readonly)"},
    {"parent", FIELDTYPE_REGION, offsetof(region_struct, parent), 0, 0,
            "Region's parent.; Atrinik.Region.Region or None (readonly)"},
    {"name", FIELDTYPE_CSTR, offsetof(region_struct, name), 0, 0,
            "Name of the region (eg, 'world').; str (readonly)"},
    {"longname", FIELDTYPE_CSTR, offsetof(region_struct, longname), 0, 0,
            "Long name (eg, 'Strakewood Island').; str or None (readonly)"},
    {"msg", FIELDTYPE_CSTR, offsetof(region_struct, msg), 0, 0,
            "Description of the region.; str or None (readonly)"},
    {"jailmap", FIELDTYPE_CSTR, offsetof(region_struct, jailmap), 0, 0,
            "Path to the region's jail map.; str or None (readonly)"},
    {"jailx", FIELDTYPE_INT16, offsetof(region_struct, jailx), 0, 0,
            "X coordinate of the region's jail.; int (readonly)"},
    {"jaily", FIELDTYPE_INT16, offsetof(region_struct, jaily), 0, 0,
            "Y coordinate of the region's jail.; int (readonly)"},
    {"map_first", FIELDTYPE_CSTR, offsetof(region_struct, map_first), 0, 0,
            "Path to some beginning map in the region. Used for world maker "
            "generation purposes.; str or None (readonly)"}
};

/**
 * Get region's attribute.
 * @param r
 * Python region wrapper.
 * @param context
 * Void pointer to the field ID.
 * @return
 * Python object with the attribute value, NULL on failure.
 */
static PyObject *Region_GetAttribute(Atrinik_Region *r, void *context)
{
    return generic_field_getter(context, r->region);
}

/**
 * Create a new region wrapper.
 * @param type
 * Type object.
 * @param args
 * Unused.
 * @param kwds
 * Unused.
 * @return
 * The new wrapper.
 */
static PyObject *Atrinik_Region_new(PyTypeObject *type, PyObject *args,
        PyObject *kwds)
{
    Atrinik_Region *self = (Atrinik_Region *) type->tp_alloc(type, 0);
    if (self != NULL) {
        self->region = NULL;
    }

    return (PyObject *) self;
}

/**
 * Free a region wrapper.
 * @param self
 * The wrapper to free.
 */
static void Atrinik_Region_dealloc(Atrinik_Region *self)
{
    self->region = NULL;
#ifndef IS_PY_LEGACY
    Py_TYPE(self)->tp_free((PyObject *) self);
#else
    self->ob_type->tp_free((PyObject *) self);
#endif
}

/**
 * Return a string representation of a region.
 * @param self
 * The region object.
 * @return
 * Python object containing the name of the region.
 */
static PyObject *Atrinik_Region_str(Atrinik_Region *self)
{
    return Py_BuildValue("s", self->region->name);
}

static int Atrinik_Region_InternalCompare(Atrinik_Region *left,
        Atrinik_Region *right)
{
    return (left->region < right->region ? -1 :
        (left->region == right->region ? 0 : 1));
}

static PyObject *Atrinik_Region_RichCompare(Atrinik_Region *left,
        Atrinik_Region *right, int op)
{
    if (left == NULL || right == NULL ||
            !PyObject_TypeCheck((PyObject *) left, &Atrinik_RegionType) ||
            !PyObject_TypeCheck((PyObject *) right, &Atrinik_RegionType)) {
        Py_INCREF(Py_NotImplemented);
        return Py_NotImplemented;
    }

    return generic_rich_compare(op,
            Atrinik_Region_InternalCompare(left, right));
}

/**
 * This is filled in when we initialize our region type.
 */
static PyGetSetDef getseters[NUM_FIELDS + 1];

/**
 * Initialize the region wrapper.
 * @param module
 * The Atrinik Python module.
 * @return
 * 1 on success, 0 on failure.
 */
int Atrinik_Region_init(PyObject *module)
{
    size_t i;

    /* Field getters */
    for (i = 0; i < NUM_FIELDS; i++) {
        PyGetSetDef *def = &getseters[i];

        def->name = fields[i].name;
        def->get = (getter) Region_GetAttribute;
        def->set = NULL;
        def->doc = fields[i].doc;
        def->closure = &fields[i];
    }

    getseters[i].name = NULL;

    Atrinik_RegionType.tp_name = "Atrinik.Region";
    Atrinik_RegionType.tp_alloc = PyType_GenericAlloc;
    Atrinik_RegionType.tp_basicsize = sizeof(Atrinik_Region);
    Atrinik_RegionType.tp_dealloc = (destructor) Atrinik_Region_dealloc;
    Atrinik_RegionType.tp_repr = (reprfunc) Atrinik_Region_str;
    Atrinik_RegionType.tp_doc = "Atrinik regions";
    Atrinik_RegionType.tp_richcompare = (richcmpfunc) Atrinik_Region_RichCompare;
    Atrinik_RegionType.tp_getset = getseters;
    Atrinik_RegionType.tp_new = Atrinik_Region_new;

    if (PyType_Ready(&Atrinik_RegionType) < 0) {
        return 0;
    }

    Py_INCREF(&Atrinik_RegionType);
    PyModule_AddObject(module, "Region", (PyObject *) &Atrinik_RegionType);

    return 1;
}

/**
 * Utility method to wrap a region.
 * @param what
 * Region to wrap.
 * @return
 * Python object wrapping the real region.
 */
PyObject *wrap_region(region_struct *what)
{
    /* Return None if no region was to be wrapped. */
    if (what == NULL) {
        Py_INCREF(Py_None);
        return Py_None;
    }

    Atrinik_Region *wrapper = PyObject_NEW(Atrinik_Region, &Atrinik_RegionType);
    if (wrapper != NULL) {
        wrapper->region = what;
    }

    return (PyObject *) wrapper;
}
