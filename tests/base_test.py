import sys
sys.path.pop(0)

import elkai

def test_base():
    print(elkai.DistanceMatrix([
        [0, 4, 0],
        [0, 0, 5],
        [0, 0, 0]
    ]).solve_tsp())

    print(elkai.Coordinates2D({
        'city1': (0, 0),
        'city2': (0, 4),
        'city3': (5, 0)
    }).solve_tsp())