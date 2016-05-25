#include <Python.h>
#include <structmember.h>
#include <blossom.h>


/********************
 * Object Structure *
 ********************/
typedef struct {
    PyObject_HEAD
    bloom_t * filter;
} pybl_obj;


/**************
 * Exceptions *
 **************/
static PyObject *pybl_at_capacity;

/******************
 * Initialization *
 ******************/

/**
 * deallocate object
 */
static void
pybl_dealloc(pybl_obj * self)
{
    bloom_destroy(self->filter);
    Py_TYPE(self)->tp_free((PyObject *) self);
}

/**
 * Create a new pybl_obj
 */
static PyObject *
pybl_new(PyTypeObject *type, PyObject *args, PyObject *kwargs)
{
    pybl_obj * self;
    self = (pybl_obj *)type->tp_alloc(type, 0);
    if (self != NULL)
    {
        self->filter = NULL;
    }
    return (PyObject *) self;
}

/**
 * Initialize new pybl_obj
 */
static int
pybl_init(pybl_obj * self, PyObject * args, PyObject * kwargs)
{
    size_t capacity;
    ssize_t scapacity;
    double error_rate;

    static char *kwlist[] = {"capacity", "error_rate", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "ld", kwlist,
                                     &scapacity, &error_rate))
    {
        return -1;
    }
    capacity = (size_t) scapacity;

    self->filter = bloom_create(capacity, error_rate);
    if (self->filter == NULL)
    {
        return -1;
    }
    return 0;
}

/******************
 * Object Members *
 ******************/
static PyMemberDef pybl_members[] = {
    /*
     *{"size",       T_PYSSIZET, offsetof(pybl_obj, filter) + offsetof(bloom_t, size),       READONLY, "Size in bits of this bloom filter."},
     *{"capacity",   T_PYSSIZET, offsetof(pybl_obj, filter) + offsetof(bloom_t, capacity),   READONLY, "Total items which may be stored in this bloom filter."},
     *{"error_rate", T_DOUBLE,   offsetof(pybl_obj, filter) + offsetof(bloom_t, error_rate), READONLY, "Maximum error rate for this bloom filter."},
     *{"nitems",     T_PYSSIZET, offsetof(pybl_obj, filter) + offsetof(bloom_t, nitems),     READONLY, "Number of items inserted into bloom filter."},
     */
    NULL  /* Sentinel */
};

/******************
 * Object Methods *
 ******************/

/**
 * pybl.add(key) Method
 */
static PyObject *
pybl_add(pybl_obj * self, PyObject * args)
{
    const char * key;
    size_t key_len;
    if (!PyArg_ParseTuple(args, "s#", &key, &key_len))
        return NULL;
    
    if (self->filter->nitems >= self->filter->capacity)
    {
        PyErr_SetString(pybl_at_capacity, "The bloom filter is at capacity.");
        return NULL;
    }

    int ret = bloom_add(self->filter, key, key_len);
    return Py_BuildValue("O", ret ? Py_True: Py_False);
}

/**
 * pybl.check(key) Method
 */
static PyObject *
pybl_check(pybl_obj * self, PyObject * args)
{
    const char * key;
    size_t key_len;
    if (!PyArg_ParseTuple(args, "s#", &key, &key_len))
        return NULL;
    
    int ret = bloom_get(self->filter, key, key_len);
    return Py_BuildValue("O", ret ? Py_True : Py_False);
}

/**
 * pybl.size() Method
 */
static PyObject *
pybl_size(pybl_obj * self)
{
    return Py_BuildValue("n", (Py_ssize_t) self->filter->size);
}

/**
 * pybl.capacity() method
 */
static PyObject *
pybl_capacity(pybl_obj * self)
{
    return Py_BuildValue("n", (Py_ssize_t) self->filter->capacity);
}

/**
 * pybl.nitems() method
 */
static PyObject *
pybl_nitems(pybl_obj * self)
{
    return Py_BuildValue("n", (Py_ssize_t) self->filter->nitems);
}

/**
 * pybl.error_rate() method
 */
static PyObject *
pybl_error_rate(pybl_obj * self)
{
    return Py_BuildValue("d", (Py_ssize_t) self->filter->error_rate);
}

static PyMethodDef pybl_methods[] = {
    {"add", (PyCFunction)pybl_add, METH_VARARGS,
        "Add a string to the bloom filter.\n"
        "\n"
        "Parameters\n"
        "----------\n"
        "key : str\n"
        "      Key to insert into Bloom filter.\n"
        "\n"
        "Returns\n"
        "-------\n"
        "bool\n"
        "    True if already inserted, False otherwise.\n"
    },
    {"check", (PyCFunction)pybl_check, METH_VARARGS,
        "Check if a string is in the bloom filter."
        "\n"
        "Parameters\n"
        "----------\n"
        "key : str\n"
        "      Key to check for presence in the Bloom filter.\n"
        "\n"
        "Returns\n"
        "-------\n"
        "bool\n"
        "    True if present, False otherwise.\n"
    },
    {"error_rate", (PyCFunction)pybl_error_rate, METH_NOARGS,  "Returns the max error_rate."},
    {"size",       (PyCFunction)pybl_size,       METH_NOARGS,  "Returns the size in bits of the bloom filter."},
    {"capacity",   (PyCFunction)pybl_capacity,   METH_NOARGS,  "Returns the maximum supported capacity."},
    {"nitems",     (PyCFunction)pybl_nitems,     METH_NOARGS,  "Returns the number of items currently in the filter."},
    NULL  /* Sentinel */
};



/************************
 * PyBl Type Definition *
 ************************/

PyDoc_STRVAR(pybl_doc,
"pyblossom(capacity, error_rate) -> new empty bloom filter\n"
"   capacity: int\n"
"       Maximum capacity of the bloom filter\n"
"   error_rate: float\n"
"       Theoretical maximum error limit"
);

static PyTypeObject pybl_obj_type= {
    PyVarObject_HEAD_INIT(NULL, 0)
    "pyblossom.Bloom",         /* tp_name */
    sizeof(pybl_obj),          /* tp_basicsize */
    0,                         /* tp_itemsize */
    (destructor)pybl_dealloc,  /* tp_dealloc */
    0,                         /* tp_print */
    0,                         /* tp_getattr */
    0,                         /* tp_setattr */
    0,                         /* tp_reserved */
    0,                         /* tp_repr */
    0,                         /* tp_as_number */
    0,                         /* tp_as_sequence */
    0,                         /* tp_as_mapping */
    0,                         /* tp_hash  */
    0,                         /* tp_call */
    0,                         /* tp_str */
    0,                         /* tp_getattro */
    0,                         /* tp_setattro */
    0,                         /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT |
        Py_TPFLAGS_BASETYPE,   /* tp_flags */
    pybl_doc,  /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    pybl_methods,              /* tp_methods */
    pybl_members,              /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)pybl_init,       /* tp_init */
    0,                         /* tp_alloc */
    pybl_new,                  /* tp_new */
};

/********
 * Init *
 ********/

static PyModuleDef pybl_module = {
    PyModuleDef_HEAD_INIT,
    "pyblossom",
    "Atomic bloom filters for python",
    -1,
    NULL, NULL, NULL, NULL, NULL
};

PyMODINIT_FUNC
PyInit_pyblossom(void)
{
    PyObject * m;
    if (PyType_Ready(&pybl_obj_type) < 0)
        return NULL;
    
    m = PyModule_Create(&pybl_module);
    if (m == NULL)
        return NULL;

    pybl_at_capacity = PyErr_NewException("pyblossom.AtCapacityError", NULL, NULL);
    Py_INCREF(pybl_at_capacity);
    PyModule_AddObject(m, "AtCapacityError", pybl_at_capacity);
    

    Py_INCREF(&pybl_obj_type);
    PyModule_AddObject(m, "Bloom", (PyObject *)&pybl_obj_type);
    return m;

}
