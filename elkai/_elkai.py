from ._native import lib as _lib, ffi as _ffi


def solve(x, runCount):
    """solve(x): Solve a TSP problem.\n"""
    n = int(len(x) ** .5)
    if len(x) < 4 or len(x) != n ** 2:
        raise TypeError(
            "Argument must be a list of integers with N^2 >= 4 elements.\n"
            + "Example: [1, 1, 1, 1]."
        )

    matrixBuff = _ffi.new("int[%s]" % len(x), x)
    tourBuff = _ffi.new("int[%s]" % n)
    tourN = _ffi.new("int*")
    norm_result = _lib.InvokeSolver(matrixBuff, len(x), runCount, tourBuff, tourN)
    return list(tourBuff[0:tourN[0]])
