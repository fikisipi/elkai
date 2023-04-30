import sys
sys.path.pop(0)

import pytest
import elkai

def test_invalid_matrix():
    with pytest.raises(TypeError):
        elkai.DistanceMatrix(1)

    with pytest.raises(ValueError):
        elkai.DistanceMatrix([[1, 2, 3]])
    
    with pytest.raises(ValueError):
        elkai.DistanceMatrix([[1, 2, 3], [4, 5, 6]])

    with pytest.raises(ValueError):
        elkai.DistanceMatrix([[]])

def test_valid_matrix():
    elkai.DistanceMatrix([[1, 2, 3], [4, 5, 6], [7, 8, 9]])

def test_solution():
    assert len(elkai.DistanceMatrix([
        [0, 4, 0],
        [0, 0, 5],
        [0, 0, 0]
    ]).solve_tsp()) == 4

def test_solution_w():
    from .known_solutions import cases
    from elkai.utils import path_distance
    
    for case in cases:
        sol = elkai.DistanceMatrix(case.input).solve_tsp()
        assert path_distance(sol, case.input) == case.distance