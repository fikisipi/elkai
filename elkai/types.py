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
        
        problem_type = "ATSP"
        if utils.is_symmetric_matrix(self.distances):
            problem_type = "TSP"

        problem = f"TYPE : {problem_type}\nDIMENSION : {dimension}\nEDGE_WEIGHT_TYPE : EXPLICIT\nEDGE_WEIGHT_FORMAT : FULL_MATRIX\nEDGE_WEIGHT_SECTION\n"
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
        
        keys_to_numbers = {k: (i + 1) for i, k in enumerate(keys)}
        numbers_to_keys = {num: key for key, num in keys_to_numbers.items()}

        dimension = len(keys)
        params = f"RUNS = {runs}\nPROBLEM_FILE = :stdin:\n"
        problem = f"TYPE : TSP\nDIMENSION : {dimension}\nEDGE_WEIGHT_TYPE : EUC_2D\nNODE_COORD_SECTION\n"
        for key, num in keys_to_numbers.items():
            coord_list = [str(x) for x in self.coords[key]]
            problem += f"{num} {' '.join(coord_list)}\n"

        print(problem)

        solution = _elkai.solve_problem(params, problem)
        solution.append(solution[0])
        solution_as_keys = [numbers_to_keys[num] for num in solution]
        return solution_as_keys