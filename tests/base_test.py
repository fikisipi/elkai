# import unittest
#
# class SimpleTest(unittest.TestCase):
#     def runTest(self):
#         import elkai
#         res = elkai.solve_int_matrix([[0, 0], [0, 0]])
#         print(res)
#         self.assertEqual(1, 1)

import sys
sys.path.pop(0)

import logging
import elkai
from .known_solutions import cases

logging.basicConfig(level=logging.DEBUG)
log = logging.getLogger()


def test_elkai():
    for case in cases[1:]:
        log.info("%s %s %s", case.name, case.input, case.distance)
        solution = elkai.solve_int_matrix(case.input)
        dist = elkai.utils.path_distance(solution, case.input)
        assert dist == case.distance
