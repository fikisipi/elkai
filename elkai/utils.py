from typing import List, Any

def path_distance(path: List[int], distance_matrix: List[List[float]]):
    """Returns the distance of a path given a distance matrix."""
    dist = 0
    last_idx = path[0]
    tour = path[1:]

    if len(path) != len(distance_matrix[0]) + 1:
        tour.append(path[0])

    for idx in tour:
        dist += distance_matrix[last_idx][idx]
        last_idx = idx

    return dist

def flatten_matrix(matrix: List[List[float]]) -> List[float]:
    """Flattens a 2D matrix into a list."""
    N = len(matrix)
    if N <= 2:
        raise TypeError("Argument must be a N*N matrix with N > 2")
    flattened = []
    for row in matrix:
        if len(row) != N:
            raise TypeError("Argument must be a N*N matrix")
        for column in row:
            flattened.append(column)
    return flattened

def is_2d_matrix(obj: Any) -> bool:
    """Checks if obj is a 2d matrix of ints/floats."""
    row_count = len(obj)
    for row in obj:
        if len(row) != row_count:
            return False
        for col in row:
            if not isinstance(col, (int, float)):
                return False
    return True

def is_symmetric_matrix(obj: List[List[float]]) -> bool:
    """Returns True if obj is a symmetric matrix, False otherwise."""
    N = len(obj)
    for i in range(N):
        for j in range(N):
            if (obj[i][j] != obj[j][i]):
                return False
    return True