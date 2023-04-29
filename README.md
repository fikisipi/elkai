<p align="center">
 <img src="https://raw.githubusercontent.com/fikisipi/elkai/assets/elkaiv2.png" alt="" />
</p>
<p align="center">
<em>elkai - a Python library for solving TSP problems</em>
</p>
<p align="center">
<a href="https://github.com/fikisipi/elkai/actions/workflows/python-app.yml"><img src="https://github.com/fikisipi/elkai/actions/workflows/python-app.yml/badge.svg" alt="Python build"></a>
<a href="https://pypi.org/project/elkai/"><img src="https://img.shields.io/pypi/v/elkai.svg" alt="elkai on PyPi"></a>
</p>

----

* **based on [LKH](http://akira.ruc.dk/~keld/research/LKH/) by Keld Helsgaun**: with proven optimal solutions up to N=315 and more accurate results than [Google's OR tools](https://developers.google.com/optimization/routing/tsp)
* **asymmetric and symmetric** [travelling salesman problems](https://en.wikipedia.org/wiki/Travelling_salesman_problem) support
* **clean and simple API**: get results with one line calls

## Installation

💾 **To install it** run `pip install elkai`

## Example usage

```python
import elkai

cities = elkai.Coordinates2D({
    'city1': (0, 0),
    'city2': (0, 4),
    'city3': (5, 0)
})

print(cities.solve_tsp())
```

```python
import elkai

cities = elkai.DistanceMatrix([
    [0, 4, 0],
    [0, 0, 5],
    [0, 0, 0]
])

print(cities.solve_tsp())
```

> **Note**
> [solve_int_matrix](https://github.com/fikisipi/elkai/blob/55187e83e7d91ee597b408c8644632fb0ef2687f/elkai/__init__.py#L33) and [solve_float_matrix](https://github.com/fikisipi/elkai/blob/55187e83e7d91ee597b408c8644632fb0ef2687f/elkai/__init__.py#L38) are deprecated in v1. Also, they don't contain the origin arrival in the result list by default.

## License

The LKH native code by Helsgaun is released for non-commercial use only. Therefore the same restriction applies to elkai, which is explained in the `LICENSE` file. 

## How it works internally

* We refactored LKH such that it doesn't have global state and you don't need to restart the program in order to run another input problem
* We added a hook in ReadProblem that allows reading problems from memory/stdin instead of filenames
* We read the solution from the `Tour` variable and put it in a PyObject (Python list).

⚠️ elkai takes the **global interpreter lock (GIL)** during the solving phase which means two threads cannot solve problems at the same time. If you want to run other workloads at the same time, you have to run another process - for example by using the `multiprocessing` module.

If there isn't a prebuilt wheel for your platform, you'll have to follow the `scikit-build` process.