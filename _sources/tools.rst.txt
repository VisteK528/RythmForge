Tools and Automation
====================

During the development of the library, various tools were used such as linters `ruff` for Python and `cppcheck` for C++. They were used both manually on the authors' local machines and in an automated CI pipeline.

Additionally, formatters `black` for Python and `clang-format` for C++ were utilized.

As mentioned, an automated integration environment was created using GitHub Actions. Upon committing changes to the remote repository, linting is performed first. If successful, the package is built for both `Ubuntu` and `Windows` systems using `g++` for Ubuntu and `MSVC` for Windows. After compilation, C++ unit tests are run. If they pass, the Python package is built and Python unit tests are executed.
