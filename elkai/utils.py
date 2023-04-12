def path_distance(path, matrix):
    dist = 0
    last_idx = path[0]
    tour = path[1:]

    if len(path) != len(matrix[0]) + 1:
        tour.append(path[0])

    for idx in tour:
        dist += matrix[last_idx][idx]
        last_idx = idx

    return dist
