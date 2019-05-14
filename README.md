elkai - a Python 3 TSP solver
====

elkai is a Python 3 TSP solver without external dependencies,
based on [LKH](http://akira.ruc.dk/~keld/research/LKH/) by
Keld Helsgaun, PhD.

💾 **To install it** run `pip install elkai`

💻 **Supported platforms:** elkai is available on Windows, Linux, OS X for Python 3.5 and above as a binary wheel.


[![Build status](https://ci.appveyor.com/api/projects/status/ltwfnqjgxafdx3gv?svg=true)](https://ci.appveyor.com/project/dimitrovskif/elkai)
[![image](https://img.shields.io/pypi/v/elkai.svg)](https://pypi.org/project/elkai/)

How to use
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

Functions in the `elkai` module:
   * `elkai.solve_int_matrix(x)`
   * `elkai.solve_float_matrix(x)`

`solve_int_matrix` solves an instance of the asymmetric TSP problem with integer distances.

```js
solve_int_matrix(x)

x:
   N*N matrix representing an integer distance matrix between
   cities. An example for N=3:
   [
       [0, 4,  9],    # dist(c1, c1), dist(c1, c2), dist(c1, c3)
       [4, 0, 10],    # dist(c2, c1), dist(c2, c2), dist(c2, c3)
       [2, 4,  0]     # dist(c3, c1), dist(c3, c2), dist(c3, c3)
   ]

Return value:
   The tour, represented as a list of indices. The indices are
   zero-indexed and based on the distance matrix order.
```


`solve_float_matrix` is exactly the same as the int version,
but it allows real numbers as distances.

It reduces the float problem into an integer problem and inaccuraries may happen. 
```js
solve_float_matrix(x)

x:
  N*N matrix representing a float distance matrix between
  cities. An example for N=2:
  [
      [0.0, 1.0],     # dist(c1, c1), dist(c1, c2)
      [1.0, 0.0]      # dist(c2, c1), dist(c2, c2)
  ]

Return value:
   The tour, represented as a list of indices. The indices are
   zero-indexed and based on the distance matrix order. Example:
   
   [0, 2, 1] # the path is: "first city => third city => second city"
```

Multithreading & leaks
----------------------

The library doesn't not release the GIL during the solution search, so other threads will be blocked. If you want to solve multiple problems
concurrently you may try the [multiprocessing](https://docs.python.org/3.7/library/multiprocessing.html) module.

*On memory leaks:* The original C solver was written with one problem per process in mind. We try to clean up the state before and after solving a problem, but leaks are definitely possible. If you want to help out, run Valgrind or similar software and submit a PR.