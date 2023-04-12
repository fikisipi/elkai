def path_distance(path, distance_matrix):
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

def flatten_matrix(matrix):
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