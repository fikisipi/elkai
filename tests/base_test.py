import sys
sys.path.pop(0)

import pytest
import logging
import elkai
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
        dist = elkai.utils.path_distance(solution, case.input)
        assert dist == case.distance

def test_with_return():
    assert len(elkai.solve_int_matrix(
        [
            [0, 4, 3],
            [4, 0, 10],
            [2, 4, 0]
        ], skip_end=False
    )) == 4

def test_internal():
    res = elkai._elkai.solve_problem("TRACE_LEVEL = 0\nPROBLEM_FILE = :stdin:\nRUNS = 10\n",
                   "TYPE : ATSP\nDIMENSION : 3 \nEDGE_WEIGHT_TYPE: EXPLICIT\nEDGE_WEIGHT_FORMAT: FULL_MATRIX\nEDGE_WEIGHT_SECTION\n0 0 0\n0 0 0\n0 0 0\n")
    assert isinstance(res, list)
