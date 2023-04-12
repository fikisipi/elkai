from . import utils, _elkai
from typing import List


def solve(matrix: List[List[float]], runs=10, skip_end=True):
    """Given a 2D matrix of distances between cities, returns the indices of the cities ordered optimally for TSP."""

    flattened = utils.flatten_matrix(matrix)
    cities: List[int] = _elkai.solve(flattened, runs)
    if not skip_end:
        # include the return to first city
        cities.append(cities[0])
    return cities


def solve_int_matrix(matrix: List[List[int]], runs=10, skip_end=True):
    """[Deprecated] Given a 2D matrix of distances between cities, returns the indices of the cities ordered optimally for TSP."""

    # Note: this function (and the split between int and float solvers) is redundant.
    # The C library accepts floats natively now!
    return solve(matrix, runs, skip_end)


def solve_float_matrix(matrix, runs=10, skip_end=True):
    """[Deprecated] Given a 2D matrix of distances between cities, returns the indices of the cities ordered optimally for TSP."""

    # Note: this function (and the split between int and float solvers) is redundant.
    # The C library accepts floats natively now!
    return solve(matrix, runs, skip_end)