from glob import glob
from setuptools import setup
from pybind11.setup_helpers import Pybind11Extension, build_ext


__version__ = "0.0.0"

ext_modules = [
    Pybind11Extension(
        "rythm_forge.lib.rythm_forge_load_cpp",
        sorted(glob("src/src/core/*.cpp") + glob("src/src/load/*.cpp") + ["src/modules/load_module.cpp"]),
        cxx_std=20,
    ),
]

setup(
    name="rythm_forge",
    version=__version__,
    author="Piotr Patek Damian Baraniak",
    author_email="piotrpatek17@gmail.com baraniak.damian@gmail.com",
    description="RythmForge audio&music processing library",
    url="https://github.com/VisteK528/RythmForge",
    packages=["rythm_forge"],
    classifiers=[
        "Programming Language :: Python :: 3",
        "License :: OSI Approved :: MIT License",
        "Operating System :: OS Independent",
    ],
    python_requires='>=3.10',
    install_requires=[
        "numpy>=1.2",
        "matplotlib>=3.4",
        "pybind11"
    ],
    cmdclass={"build_ext": build_ext},
    ext_modules=ext_modules,
    zip_safe=False,
)
