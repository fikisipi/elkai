#include <Python.h>
#include "math.h"
#include "gb_string.h"

// These are implemented in the LKH-3.0.8/SRC directory.

extern int ElkaiSolveATSP(int dimension, float *weights, int *tour, int runs);
extern void ElkaiSolveProblem(gbString params, gbString problem, int *tourSize, int **tourPtr);

// Our copy of LKH is highly modified and does not correspond to the upstream. In the future,
// we should ship the original LKH folder and then apply a patch *at build time*.

static PyObject *PySolveProblem(PyObject *self, PyObject *args)
{
    if(PyObject_Length(args) != 2) {
        PyErr_SetString(PyExc_TypeError, "Expected two arguments");
        return 0;
    }

    PyObject *arg1 = PyObject_GetItem(args, PyLong_FromLong(0));
    PyObject *arg2 = PyObject_GetItem(args, PyLong_FromLong(1));

    if(!PyUnicode_Check(arg1) || !PyUnicode_Check(arg2)) {
        PyErr_SetString(PyExc_TypeError, "Arguments must be strings");
        return 0;
    }

    const char *paramsStr = PyUnicode_AsUTF8(arg1);
    const char *problemStr = PyUnicode_AsUTF8(arg2);

    gbString params = gb_make_string(paramsStr);
    gbString problem = gb_make_string(problemStr);

    int tourSize = 0;
    int *tourPtr;

    ElkaiSolveProblem(params, problem, &tourSize, &tourPtr);

    if(PyErr_Occurred() != 0) {
        return 0;
    }

    PyObject *list = PyList_New(tourSize);
    int i = 0;
    for (i = 0; i < tourSize; i++)
    {
        PyObject *tourElement = PyLong_FromLong((long)(tourPtr[i]));
        PyList_SetItem(list, i, tourElement);
    }
    gb_free_string(params);
    gb_free_string(problem);
    return list;
}

static PyObject *ElkSolve(PyObject *self, PyObject *args)
{
    // *args of a vararg Python function is a tuple
    // of unknown length.

    if(PyObject_Length(args) != 2) {
        PyErr_SetString(PyExc_TypeError, "Expected two arguments");
        return 0;
    }

    PyObject *arg = PyObject_GetItem(args, PyLong_FromLong(0));
    PyObject *runArg = PyObject_GetItem(args, PyLong_FromLong(1));

    long runCount = PyLong_AsLong(runArg);
    if(PyErr_Occurred() != 0 || runCount <= 0) {
        PyErr_SetString(PyExc_TypeError, "Second argument must be a positive int");
        return 0;
    } 

    int pyLen = PyObject_Length(arg);

    // We get the problem dimension as a square root of the flat NxN
    // matrix length. It could be done using recursive bit shifts too.

    int pyLenSqrt = (int)sqrt(pyLen);
    if (pyLen < 4 || pyLenSqrt * pyLenSqrt != pyLen)
    {
        PyErr_SetString(PyExc_TypeError, "Argument must be a list of integers with N^2 >= 4 elements.\n"
                                          "Example: [1, 1, 1, 1].");
        return 0;
    }
    int *matrixBuff = (int *)malloc(sizeof(int) * pyLen);
    float *weights = (float*)malloc(sizeof(float) * pyLen);
    int *tourBuff = (int *)malloc(sizeof(int) * pyLenSqrt);

    long i;
    for (i = 0; i < pyLen; i++)
    {
        PyObject *pyNumber = PyObject_GetItem(arg, PyLong_FromLong(i));
        long justNumber = PyLong_AsLong(pyNumber);
        if(PyErr_Occurred() != 0) {
            PyErr_SetString(PyExc_TypeError, "List must only contain integers");
            return 0;
        }
        int justNumber_i = (int)justNumber;
        // TODO: don't lose precision
        matrixBuff[i] = justNumber_i;
        weights[i] = (float)justNumber_i;
    }

    ElkaiSolveATSP(pyLenSqrt, weights, tourBuff, runCount);

    free(weights);
    free(matrixBuff);
    PyObject *list = PyList_New(pyLenSqrt);
    for (i = 0; i < pyLenSqrt; i++)
    {
        PyObject *tourElement = PyLong_FromLong((long)(tourBuff[i] - 1));
        PyList_SetItem(list, i, tourElement);
    }

    free(tourBuff);
    return list;
}

static char elk_docs[] =
    "solve(x): Solve a TSP problem.\n";

static PyMethodDef funcs[] = {
    {"solve", (PyCFunction) ElkSolve, METH_VARARGS, elk_docs},
    {"solve_problem", (PyCFunction) PySolveProblem, METH_VARARGS, "solve_problem(params: str, problem: str): Solve a LKH problem."},
    {NULL}
};

static struct PyModuleDef elkDef = {
    PyModuleDef_HEAD_INIT,
    "_elkai",
    "",
    -1,
    funcs
};

PyMODINIT_FUNC PyInit__elkai(void)
{
    return PyModule_Create(&elkDef);
}