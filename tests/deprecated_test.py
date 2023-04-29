import sys
sys.path.pop(0)

import pytest
import logging
import elkai
from elkai.utils import path_distance
from .known_solutions import cases

logging.basicConfig(level=logging.DEBUG)
log = logging.getLogger()

def test_invalid_inputs():
    with pytest.raises(Exception):
        elkai.solve_int_matrix([])

    with pytest.raises(Exception):
        elkai.solve_int_matrix([1, 2, 3])

    with pytest.raises(Exception):
        elkai.solve_int_matrix("")

    with pytest.raises(Exception):
        elkai.solve_int_matrix([[1, 2, 3], [1, 2, 3]])

def test_valid():
    for case in cases:
        solution = elkai.solve_int_matrix(case.input)
        log.info("%s %s %s %s", case.name, case.input, case.distance, solution)
        dist = path_distance(solution, case.input)
        assert dist == case.distance

def test_with_return():
    assert len(elkai.solve_int_matrix(
        [
            [0, 4, 3],
            [4, 0, 10],
            [2, 4, 0]
        ], skip_end=False
    )) == 4