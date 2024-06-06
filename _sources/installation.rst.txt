Installation
============

This guide will help you install the `rythm_forge` package. Follow the steps below to get started.

Installing with pip
-------------------

To install the `rythm_forge` package using `pip3`, follow these steps:

1. Open your terminal.
2. Navigate to the directory containing the `setup.py` file of the `rythm_forge` package.
3. Run the following command:

   .. code-block:: bash

       pip3 install .

Installing in a Anaconda Virtual Environment
----------------------------------------------

It is recommended to install the package in a virtual environment to avoid conflicts with other packages. Follow these steps:

1. Open your terminal.
2. Navigate to your project directory.
3. Create a virtual environment:

   .. code-block:: bash

       conda create env -n <env_name> python=3.10

4. Activate the virtual environment:
   .. code-block:: bash

       conda activate <env_name>

5. Navigate to the directory containing the `setup.py` file of the `rythm_forge` package.
6. Install the package:

   .. code-block:: bash

       pip3 install .

Deactivating the Virtual Environment
------------------------------------

To deactivate the virtual environment, run the following command:

   .. code-block:: bash

       conda deactivate

