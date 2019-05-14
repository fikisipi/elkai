elkai - a Python 3 TSP solver
====

elkai is a Python 3 TSP solver without external dependencies,
based on [LKH](http://akira.ruc.dk/~keld/research/LKH/) by
Keld Helsgaun, PhD.

💾 **To install it** run `pip install elkai`

💻 **Supported platforms:** elkai is available on Windows, Linux, OS X for Python 3.5 and above as a binary wheel.


[![Build status](https://ci.appveyor.com/api/projects/status/s6pc44ha0ouqkbs1?svg=true)](https://ci.appveyor.com/project/dimitrovskif/elkai-aydrn)
[![image](https://img.shields.io/pypi/v/elkai.svg)](https://pypi.org/project/elkai/)

Example usage 
----------

```python
import elkai
elkai.solve_int_matrix([
    [1, 1],
    [1, 1]
]) # Output: [0, 1]
```

Documentation
-------------


`solve_int_matrix(matrix, runs=10)` solves an instance of the asymmetric TSP problem with integer distances.

* `matrix`:
   N*N matrix representing an integer distance matrix between
   cities. An example of N=3 city arrangement:
   ```python
   [                  # cities are zero indexed, d() is distance
       [0, 4,  9],    # d(0, 0), d(0, 1), d(0, 2)
       [4, 0, 10],    # d(1, 0), d(1, 1), ... and so on
       [2, 4,  0] 
   ]
   ```

* `runs`:
  An integer representing how many iterations the solver should
  perform. By default, 10.

* *Return value*: The tour, represented as a list of indices. The indices are
   zero-indexed and based on the distance matrix order.




`solve_float_matrix(matrix, runs=10)` has the same signature as the previous, but allows floating point distances.
It may be inaccurate.

Multithreading & leaks
----------------------

The library doesn't not release the GIL during the solution search, so other threads will be blocked. If you want to solve multiple problems
concurrently you may try the [multiprocessing](https://docs.python.org/3.7/library/multiprocessing.html) module.

*On memory leaks:* The original C solver was written with one problem per process in mind. We try to clean up the state before and after solving a problem, but leaks are definitely possible. If you want to help out, run Valgrind or similar software and submit a PR.
