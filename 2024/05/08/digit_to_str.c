#include <Python.h>
#include <stdbool.h>

#ifdef __APPLE__
#include <stdint.h>
typedef uint_least16_t char16_t;
#else
#include <uchar.h>
#endif

static const char* numbers[] = {
    "zero elephant", "one elephant is having fun", "two elephants are having fun", "three elephants are meeting with the president", "four elephants",
    "five elephants in an hexagon", "six elephants are playing the saxophone", "seven elephants are visiting the school", "eight elephants are at Church", "nine elephants are having a party"
};

static PyObject* buffer[10];

static PyObject* digit_to_str(PyObject* self, PyObject* args) {
    long num = PyLong_AsLong(args);
    if(num < 0) {
        PyErr_SetString(PyExc_ValueError, "The number must be positive");
        return NULL;
    }
    if(num > 9) {
        PyErr_SetString(PyExc_ValueError, "The number must be less than 10");
        return NULL;
    }
    return Py_BuildValue("s", numbers[num]);
}


static PyObject* digit_to_str_from(PyObject* self, PyObject* args) {
    long num = PyLong_AsLong(args);
    if(num < 0) {
        PyErr_SetString(PyExc_ValueError, "The number must be positive");
        return NULL;
    }
    if(num > 9) {
        PyErr_SetString(PyExc_ValueError, "The number must be less than 10");
        return NULL;
    }
    return PyUnicode_FromString(numbers[num]);
}



static PyObject* digit_to_str_buffer(PyObject* self, PyObject* args) {
    long num = PyLong_AsLong(args);
    if(num < 0) {
        PyErr_SetString(PyExc_ValueError, "The number must be positive");
        return NULL;
    }
    if(num > 9) {
        PyErr_SetString(PyExc_ValueError, "The number must be less than 10");
        return NULL;
    }
    PyObject* answer = buffer[num];
    Py_INCREF(answer);
    return answer;
}

// https://peps.python.org/pep-0393/
static PyObject* digit_to_str_new(PyObject* self, PyObject* args) {
    long num = PyLong_AsLong(args);
    if(num < 0) {
        PyErr_SetString(PyExc_ValueError, "The number must be positive");
        return NULL;
    }
    if(num > 9) {
        PyErr_SetString(PyExc_ValueError, "The number must be less than 10");
        return NULL;
    }
    const char * str = numbers[num];
    size_t len = strlen(str);
    PyObject* py_string = PyUnicode_New(len, 127);
    Py_UCS1* data = PyUnicode_1BYTE_DATA(py_string);
    memcpy(data, str, len);
    return py_string;
}

static PyObject* pyListPermanent;

static PyObject* all_strings(PyObject* self) {
    PyObject* pyList = PyList_New(0);
    for (size_t i = 0; i < sizeof(numbers) / sizeof(numbers[0]); ++i) {
        PyObject* pyString = PyUnicode_FromString(numbers[i]);
        PyList_Append(pyList, pyString);
        Py_DECREF(pyString); // Decrement reference count
    }
    return pyList;
}

static bool is_ascii(const char* str, size_t len) {
    for(size_t i = 0; i < len; i++) {
        if(str[i] & 0x80) {
            return false;
        }
    }
    return true;
}

static PyObject* all_strings_new_with_check(PyObject* self) {
    size_t len = sizeof(numbers) / sizeof(numbers[0]);
    PyObject* pyList = PyList_New(len);
    for (size_t i = 0; i < len; ++i) {
        const char * str = numbers[i];
        size_t len = strlen(str);
        if(!is_ascii(str, len)) {
            PyErr_SetString(PyExc_ValueError, "The string must be ASCII");
            return NULL;
        }
        PyObject* py_string = PyUnicode_New(len, 127);
        Py_UCS1* data = PyUnicode_1BYTE_DATA(py_string);
        memcpy(data, str, len);
        PyList_SET_ITEM(pyList, i, py_string);
    }
    return pyList;
}


static PyObject* all_strings_new(PyObject* self) {
    size_t len = sizeof(numbers) / sizeof(numbers[0]);
    PyObject* pyList = PyList_New(len);
    for (size_t i = 0; i < len; ++i) {
        const char * str = numbers[i];
        size_t len = strlen(str);
        PyObject* py_string = PyUnicode_New(len, 127);
        Py_UCS1* data = PyUnicode_1BYTE_DATA(py_string);
        memcpy(data, str, len);
        PyList_SET_ITEM(pyList, i, py_string);
    }
    return pyList;
}

static PyObject* all_strings_buffered(PyObject* self) {
    Py_INCREF(pyListPermanent);
    return pyListPermanent;
}
// Method definition table
static PyMethodDef methods[] = {
    {"digit_to_str", digit_to_str, METH_O, "Convert an integer to a string"},
    {"digit_to_str_from", digit_to_str_from, METH_O, "Convert an integer to a string"},
    {"digit_to_str_new", digit_to_str_new, METH_O, "Convert an integer to a string"},
    {"digit_to_str_buffer", digit_to_str_new, METH_O, "Convert an integer to a string"},
    {"all_strings", all_strings, METH_NOARGS, "All strings"},
    {"all_strings_new", all_strings_new, METH_NOARGS, "All strings"},
    {"all_strings_new_with_check", all_strings_new_with_check, METH_NOARGS, "All strings"},
    {"all_strings_buffered", all_strings_buffered, METH_NOARGS, "All strings"},
    {NULL, NULL, 0, NULL} // Sentinel
};

// Module initialization function
static struct PyModuleDef module = {
    PyModuleDef_HEAD_INIT,
    "digit_to_str", // Module name
    NULL,           // Module documentation
    -1,             // Global state
    methods         // Method table
};

// Module initialization
PyMODINIT_FUNC PyInit_digit_to_str(void) {
    for(int i = 0; i < 10; i++) {
        buffer[i] = PyUnicode_FromString(numbers[i]);
    }
    pyListPermanent = PyList_New(0);
    for (size_t i = 0; i < sizeof(numbers) / sizeof(numbers[0]); ++i) {
        PyObject* pyString = PyUnicode_FromString(numbers[i]);
        PyList_Append(pyListPermanent, pyString);
        Py_DECREF(pyString); // Decrement reference count
    }
    return PyModule_Create(&module);
}