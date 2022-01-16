elkai - a Python 3 TSP solver
====

elkai is a Python 3 library for solving [travelling salesman problems](https://en.wikipedia.org/wiki/Travelling_salesman_problem) without external dependencies,
based on [LKH](http://akira.ruc.dk/~keld/research/LKH/) by
Keld Helsgaun.

💾 **To install it** run `pip install elkai`

💻 **Supported platforms:** elkai is available on Windows, Linux, OS X for Python 3.5 and above as a binary wheel.


[![Build status](https://ci.appveyor.com/api/projects/status/obq0anulb8t4y46m?svg=true)](https://ci.appveyor.com/project/filipArena/elkai)
[![image](https://img.shields.io/pypi/v/elkai.svg)](https://pypi.org/project/elkai/)

Example usage 
----------

```python
import numpy as np
import elkai

M = np.zeros((3, 3), dtype=int)
M[0, 1] = 4
M[1, 2] = 5
elkai.solve_int_matrix(M) # Output: [0, 2, 1]
```

Documentation
-------------


**elkai.solve_int_matrix(matrix, runs=10)** solves an instance of the asymmetric TSP problem with integer distances.

* `matrix`:
   *an N\*N matrix representing an integer distance matrix between cities.*
   
   *An example of N=3 city arrangement:*
   ```python
   [                  # cities are zero indexed, d() is distance
       [0, 4,  9],    # d(0, 0), d(0, 1), d(0, 2)
       [4, 0, 10],    # d(1, 0), d(1, 1), ...
       [2, 4,  0]     # ... and so on
   ]
   ```

* `runs`:
  *An integer representing how many iterations the solver should
  perform. By default, 10.*

* Return value: *The tour represented as a list of indices. The indices are
   zero-indexed and based on the distance matrix order.*




**elkai.solve_float_matrix(matrix, runs=10)** has the same signature as the previous, but allows floating point distances.
It may be inaccurate.

FAQ
----------------------

**How to manually build the library?**

You need CMake, a C compiler and Python 3.5+. You need to install the dev dependencies first: `pip install scikit-build ninja`. To build and install, run `python setup.py install`.

**How accurately does it solve asymmetric TSP problems?**

Instances with known solutions, which are up to N=315 cities, [can be solved optimally](http://akira.ruc.dk/~keld/research/LKH/Soler_ATSP_results.html).

**What's the difference between LKH and elkai?**

elkai packages the C LKH code into a nicer C library and then wraps it and compiles it into a Python wheel. **Note:** Dr. Helsgaun has released the LKH project for non-commercial use only, so elkai as a derivative work must be used this way too.

**Does it lock the GIL during a run?**

Yes.
