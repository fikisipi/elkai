from setuptools import setup
from os import path
this_directory = path.abspath(path.dirname(__file__))
with open(path.join(this_directory, 'README.md'), encoding='utf-8') as f:
    long_description = f.read()

def build_native(spec):
    build = spec.add_external_build(
        cmd=["make"],
        path="./LKH-2.0.9/SRC",
    )

    spec.add_cffi_module(
        module_path="elkai._native",
        dylib=lambda: build.find_dylib("elkai", in_path=".."),
        header_filename=lambda: build.find_header("elkai.h", in_path="INCLUDE"),
    )

setup(
    name="elkai",
    description="A TSP solver for Python without any dependencies.",
    #long_description=long_description,
    #long_description_content_type='text/markdown',
    url="https://github.com/filipArena/elkai",
    version="0.0.9",
    packages=['elkai'],
    author="Filip Dimitrovski",
    license="MIT",
    include_package_data=True,
    zip_safe=False,
    platforms='any',
    install_requires=['milksnake'],
    setup_requires=['milksnake'],
    milksnake_tasks=[
        build_native,
    ],
)
