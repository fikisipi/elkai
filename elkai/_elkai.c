#include <Python.h>
#include "math.h"

// TODO:
// - Delete obsolete problem types
// - LKH feature: Allow coordinate based input instead of distances?
// - LKH feature: Add a known optimal cost input variable?
// - LKH feature: Add known optimal subtours?
// - Make sure camel case is used everywhere
// - Use git submodule instead of a LKH copy

// static int InvokeSolver(int *matrixBuff, int matrixLen, int runCount, int *tourBuff, int *tourN);

extern int ElkaiSolveATSP(int dimension, float *weights, int *tour, int runs);

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
    int tourN = 0;

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

    // int norm_result = InvokeSolver(matrixBuff, pyLen, runCount, tourBuff, &tourN);
    ElkaiSolveATSP(pyLenSqrt, weights, tourBuff, runCount);

    free(weights);
    free(matrixBuff);
    PyObject *list = PyList_New(tourN);
    for (i = 0; i < tourN; i++)
    {
        PyObject *tourElement = PyLong_FromLong((long)tourBuff[i]);
        PyList_SetItem(list, i, tourElement);
    }

    free(tourBuff);
    return list;
}

static char elk_docs[] =
    "solve(x): Solve a TSP problem.\n";

static PyMethodDef funcs[] = {
    {"solve", (PyCFunction) ElkSolve,
     METH_VARARGS, elk_docs},
    {NULL}};

static struct PyModuleDef elkDef = {
    PyModuleDef_HEAD_INIT,
    "_elkai",
    "",
    -1,
    funcs};

PyMODINIT_FUNC PyInit__elkai(void)
{
    return PyModule_Create(&elkDef);
}