elkai is a Python 3 TSP solver without external dependencies,
based on LKH (http://akira.ruc.dk/~keld/research/LKH/) by
K. Helsgaun, PhD.

TODO: Upload to pypi and add binary wheels so that you can just 
do "pip install elkai"

== How to install ==

1. Make sure you have Python 3 and a C compiler (MSVC, gcc, clang)
2. Run the following

pip install cmake
pip install scikit-build
python setup.py install

== How to use ==

```
import elkai
elkai.solve_int_matrix([
    [1, 1],
    [1, 1]
]) # Output: [0, 1]
```

== Docs ==

Functions in the module:
   elkai.solve_int_matrix(x)
   elkai.solve_float_matrix(x)

elkai.solve_int_matrix(x) solves an instance of the asymmetric TSP problem.

Argument x:
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



elkai.solve_float_matrix(x) is exactly the same as the int version above,
but it allows real numbers as distances. It reduces the float problem into
an integer one and inaccuraries may happen. 

Argument x:
  N*N matrix representing a float distance matrix between
  cities.

Return value:
   The tour, represented as a list of indices. The indices are
   zero-indexed and based on the distance matrix order.

== Multithreading & leaks ==

elkai doesn't not release the GIL during processing, so you can't
solve multiple problems concurrently.

We don't use Valgrind, but leaks are possible because the original
C solver was written with one problem per executed process in mind. 