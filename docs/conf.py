import os
import sys
import sphinx_rtd_theme

# Add the project root directory to sys.path
sys.path.insert(0, os.path.abspath('..'))
# -- Project information -----------------------------------------------------

project = 'Rythm Forge'
copyright = '2024, Damian Baraniak Piotr Patek'
author = 'Damian Baraniak Piotr Patek'

# The full version, including alpha/beta/rc tags
release = '0.01'

# -- General configuration ---------------------------------------------------

extensions = [
    'sphinx.ext.autodoc',
    'sphinx.ext.napoleon',
    'sphinx.ext.viewcode',
]

templates_path = ['_templates']
autodoc_mock_imports = ['rythm_forge._lib']
exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store']

# -- Options for HTML output -------------------------------------------------

html_theme = 'sphinx_rtd_theme'
html_static_path = ['_static']
