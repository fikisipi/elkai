from skbuild import setup

# read the contents of your README file
from os import path
this_directory = path.abspath(path.dirname(__file__))
with open(path.join(this_directory, 'README.txt'), encoding='utf-8') as f:
    long_description = f.read()

setup(
    name="elkai",
    description="A TSP solver for Python without any dependencies.",
    long_description=long_description,
    url="https://github.com/dimitrovskif/elkai",
    version="0.0.1",
    packages=['elkai'],
    author="Filip Dimitrovski",
    license="MIT",
)