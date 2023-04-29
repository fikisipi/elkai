from typing import List, Dict, Tuple
from . import _elkai, utils

class DistanceMatrix(object):
    def __init__(self, distances: List[List[float]]):
        if not utils.is_2d_matrix(distances):
            raise ValueError("distances must be a 2D matrix of floats/ints")
        
        self.distances = distances

    def solve_tsp(self, runs=10):
        if not isinstance(runs, int) or runs < 1:
            raise ValueError("runs must be a positive integer")
        
        dimension = len(self.distances)
        if dimension < 3:
            raise ValueError("dimension must be at least 3")

        params = f"RUNS = {runs}\nPROBLEM_FILE = :stdin:\n"
        problem = f"TYPE : ATSP\nDIMENSION : {dimension}\nEDGE_WEIGHT_TYPE : EXPLICIT\nEDGE_WEIGHT_FORMAT : FULL_MATRIX\nEDGE_WEIGHT_SECTION\n"
        for row in self.distances:
            problem += " ".join(map(str, row)) + "\n"

        solution = _elkai.solve_problem(params, problem)
        solution.append(solution[0])

        return solution


class Coordinates2D(object):
    def __init__(self, coords: Dict[str, Tuple[float, float]]):
        if not isinstance(coords, dict):
            raise ValueError("coords must be a dictionary 'city name': (x, y)")
        if len(coords) < 3:
            raise ValueError("there must be at least 3 cities")
        for key in coords:
            if not len(coords[key]) == 2:
                raise ValueError("coords must be a dictionary 'city name': (x, y)")
        
        self.coords = coords

    def solve_tsp(self, runs=10):
        if not isinstance(runs, int) or runs < 1:
            raise ValueError("runs must be a positive integer")
        
        keys = list(self.coords.keys())
        dimension = len(keys)
        params = f"RUNS = {runs}\nPROBLEM_FILE = :stdin:\n"
        problem = f"TYPE : TSP\nDIMENSION : {dimension}\nEDGE_WEIGHT_TYPE : EUC_2D\nNODE_COORD_SECTION\n"
        for idx, key in enumerate(keys):
            one_idx = idx + 1
            problem += f"{one_idx} {self.coords[key][0]} {self.coords[key][1]}\n"
        problem += "EOF\n"

        solution = _elkai.solve_problem(params, problem)
        solution.append(solution[0])
        solution_as_keys = []
        for one_idx in solution:
            zero_idx = one_idx - 1
            solution_as_keys.append(keys[zero_idx])

        return solution_as_keys