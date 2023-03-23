from skbuild import setup
from os import path
this_directory = path.abspath(path.dirname(__file__))
with open(path.join(this_directory, 'README.md'), encoding='utf-8') as f:
    long_description = f.read()

setup(
    name="elkai",
    description="A Python solver for the travelling salesman problem - TSP based on native/fast LKH",
    #long_description=long_description,
    #long_description_content_type='text/markdown',
    url="https://github.com/fikisipi/elkai",
    version="0.1.3",
    packages=['elkai'],
    author="Filip Dimitrovski",
    license="MIT",
)
