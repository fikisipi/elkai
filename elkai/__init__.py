from ._elkai import solve

demo_list = [x for x in range(10000)]
def demo():
    """Run a random N=10000 example."""
    return solve(demo_list)