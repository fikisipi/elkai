from ._elkai import solve

def demo_run():
    """Run a random N=10000 example."""
    return solve([x for x in range(10000)])