import os
import re
import subprocess
import sys
from pathlib import Path

from setuptools import Extension, setup, find_packages
from setuptools.command.build_ext import build_ext

__version__ = "1.0.0"


class CMakeExtension(Extension):
    def __init__(self, name):
        Extension.__init__(self, name, sources=[])


class CMakeBuild(build_ext):
    def run(self):
        try:
            out = subprocess.check_output(['cmake', '--version'])
        except OSError:
            raise RuntimeError(
                "CMake must be installed to build the following extensions: " +
                ", ".join(e.name for e in self.extensions))

        build_directory = os.path.abspath(self.build_temp)

        cmake_args = [
            '-DCMAKE_LIBRARY_OUTPUT_DIRECTORY=' + build_directory,
            '-DPYTHON_EXECUTABLE=' + sys.executable,
            '-DCMAKE_TOOLCHAIN_FILE=' + os.path.join(build_directory, "conan_toolchain.cmake")
        ]

        cfg = 'Release'
        build_args = ['--config', cfg]

        cmake_args += ['-DCMAKE_BUILD_TYPE=' + cfg]

        # Assuming Makefiles
        # build_args += ['--', '-j2']

        self.build_args = build_args

        env = os.environ.copy()
        env['CXXFLAGS'] = '{} -DVERSION_INFO=\\"{}\\"'.format(
            env.get('CXXFLAGS', ''),
            self.distribution.get_version())
        if not os.path.exists(self.build_temp):
            os.makedirs(self.build_temp)
        subprocess.check_call(['conan', 'profile', 'detect', '--force'])
        subprocess.check_call(['conan', 'install', '.', f'--output-folder={self.build_temp}', '--build=missing'])
        # CMakeLists.txt is in the same directory as this setup.py file
        cmake_list_dir = os.path.abspath(os.path.dirname(__file__))
        print('-' * 10, 'Running CMake prepare', '-' * 40)
        subprocess.check_call(['cmake', cmake_list_dir] + cmake_args,
                              cwd=self.build_temp, env=env)

        print('-' * 10, 'Building extensions', '-' * 40)
        cmake_cmd = ['cmake', '--build', '.'] + self.build_args
        subprocess.check_call(cmake_cmd,
                              cwd=self.build_temp)

        # Move from build temp to final position
        for ext in self.extensions:
            self.move_output(ext)

    def move_output(self, ext):
        build_temp = Path(self.build_temp).resolve()
        dest_path = Path(self.get_ext_fullpath(ext.name)).resolve()
        if sys.platform == "win32":
            source_path = build_temp / os.path.basename(build_temp)
            source_path /= os.path.basename(self.get_ext_filename(ext.name))
        else:
            source_path = build_temp / os.path.basename(self.get_ext_filename(ext.name))
        dest_directory = dest_path.parents[0]
        dest_directory.mkdir(parents=True, exist_ok=True)
        self.copy_file(source_path, dest_path)


setup(
    name="rythm_forge",
    version=__version__,
    author="Piotr Patek Damian Baraniak",
    author_email="piotrpatek17@gmail.com baraniak.damian@gmail.com",
    description="RythmForge audio&music processing library",
    url="https://github.com/VisteK528/RythmForge",
    packages=find_packages(),
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
    ext_modules=[CMakeExtension("rythm_forge._lib.rythm_forge_load_cpp"),
                 CMakeExtension("rythm_forge._lib.rythm_forge_core_cpp")],
    cmdclass={"build_ext": CMakeBuild},
    zip_safe=False,
)
