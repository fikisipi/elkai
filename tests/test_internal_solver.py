import sys
sys.path.pop(0)

import pytest

def test_invalid_syntax():
    import elkai._elkai as _elkai
    with pytest.raises(Exception):
        _elkai.solve_problem("NON_EXISTANT", "SYNTAX")