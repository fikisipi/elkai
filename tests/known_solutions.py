import dataclasses

@dataclasses.dataclass
class Known:
    name: str
    input: [[int]]
    distance: float

cases = [
    Known(
        name="T1",
        input=[
            [0, 10, 15, 20],
            [10, 0, 35, 25],
            [15, 35, 0, 30],
            [20, 25, 30, 0]
        ],
        distance=80
    ),
    Known(
        name="T2",
        input=[
            [9999, 64, 378, 519, 434, 200],
            [64, 9999, 318, 455, 375, 164],
            [378, 318, 9999, 170, 265, 344],
            [519, 455, 170, 9999,223, 428],
            [434, 375, 265, 223, 9999,273],
            [200, 164, 344, 428, 273, 9999]
        ],
        distance=1248
    )
]