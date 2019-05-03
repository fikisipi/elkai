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

elkai.solve(x) solves an instance of the asymmetric TSP problem.

Argument x:
   A 1D array representing a integer distance matrix between
   cities. For problems with N cities, len(x) must be N^2.
   An example for N=3:
   [
       0, 4, 9,    # dist(c1, c1), dist(c1, c2), dist(c1, c3)
       4, 0, 10,   # dist(c2, c1), dist(c2, c2), dist(c2, c3)
       2, 4, 0     # dist(c3, c1), dist(c3, c2), dist(c3, c3)
   ]

Return value:
  The tour, represented as a list of indices. The indices are
  zero-indexed and based on the distance matrix order.



>>> import elkai
>>> elkai.solve([
    1, 1,
    1, 1
])
