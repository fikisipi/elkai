import sys
sys.path.pop(0)

import pytest
import elkai

def test_invalid_cities():
    with pytest.raises(ValueError):
        elkai.Coordinates2D({})

    with pytest.raises(ValueError):
        elkai.Coordinates2D({'onlyOneCity': (0, 0)})

    with pytest.raises(ValueError):
        elkai.Coordinates2D({'city1': (0, 0), 'city2': (1, 1), 'city3': "A"})

    with pytest.raises(ValueError):
        elkai.Coordinates2D({'city1': (0, 0), 'city2': (1, 1), 'city3': (1, "A")})
    
def test_valid_matrix():
        elkai.Coordinates2D({'city1': (0, 0), 'city2': (1, 1), 'city3': (1, 3)})

def test_solution_w():
    from .known_solutions import cases
    from elkai.utils import path_distance

    assert elkai.Coordinates2D({'city1': (0, 0), 'city2': (1, 1), 'city3': (2, 2)}).solve_tsp() == ['city1', 'city2', 'city3', 'city1']
