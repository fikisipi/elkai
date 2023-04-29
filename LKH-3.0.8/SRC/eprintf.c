#include "LKH.h"
#include <stdarg.h>

#ifdef PYTHON_ERR_HANDLE

int ErrorHappened;

#include <Python.h>

void eprintf(const char *fmt, ...)
{
    va_list args;

    char err[1024];
    va_start(args, fmt);
    int res = vsprintf(err, fmt, args);
    va_end(args);

    PyErr_SetString(PyExc_TypeError, err);
    ErrorHappened = 1;
}

#else

/*
 * The eprintf function prints an error message and exits.
 */

void eprintf(const char *fmt, ...)
{
    va_list args;
    if (LastLine && *LastLine)
        fprintf(stderr, "\n%s\n", LastLine);
    fprintf(stderr, "\n*** Error ***\n");
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    fprintf(stderr, "\n");
    exit(EXIT_FAILURE);
}

#endif