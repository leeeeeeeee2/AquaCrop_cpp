"""
Setup script for AquaCrop Python package.

This module provides installation configuration for the AquaCrop Python wrapper.
"""

from setuptools import setup, find_packages
import os

# Get version from package
here = os.path.abspath(os.path.dirname(__file__))
with open(os.path.join(here, 'aquacrop', '__init__.py'), 'r') as f:
    for line in f:
        if line.startswith('__version__'):
            version = line.split('=')[1].strip().strip('"\'')
            break
else:
    version = "1.0.0"

# Read long description from README
here = os.path.abspath(os.path.dirname(__file__))
with open(os.path.join(here, '..', 'README.md'), 'r', encoding='utf-8') as f:
    long_description = f.read()

setup(
    name="aquacrop",
    version=version,
    description="Python wrapper for the FAO AquaCrop crop-water productivity model",
    long_description=long_description,
    long_description_content_type="text/markdown",
    author="AquaCrop C++ Developers",
    author_email="developers@example.com",
    url="https://github.com/your-org/AquaCrop_cpp",
    license="MIT",
    
    # Package structure
    packages=find_packages(exclude=['tests', 'tests.*']),
    package_dir={
        'aquacrop': 'aquacrop',
    },
    
    # Python requirements
    python_requires=">=3.8",
    
    # Dependencies
    install_requires=[
        "pybind11>=2.6.0",
    ],
    extras_require={
        "dev": [
            "pytest>=6.0",
            "pytest-cov",
            "black",
            "flake8",
            "mypy",
        ],
        "docs": [
            "sphinx",
            "sphinx-rtd-theme",
        ],
    },
    
    # Entry points
    entry_points={
        "console_scripts": [
            "aquacrop=aquacrop.cli:main",
        ],
    },
    
    # Include C++ extension
    zip_safe=False,
    
    # Classifiers
    classifiers=[
        "Development Status :: 3 - Alpha",
        "Intended Audience :: Science/Research",
        "Intended Audience :: End Users/Desktop",
        "License :: OSI Approved :: MIT License",
        "Operating System :: POSIX :: Linux",
        "Operating System :: MacOS :: MacOS X",
        "Operating System :: Microsoft :: Windows",
        "Programming Language :: C++",
        "Programming Language :: Python :: 3",
        "Programming Language :: Python :: 3.8",
        "Programming Language :: Python :: 3.9",
        "Programming Language :: Python :: 3.10",
        "Programming Language :: Python :: 3.11",
        "Topic :: Scientific/Engineering :: Agriculture",
    ],
    
    # Keywords
    keywords=[
        "agriculture",
        "crop",
        "water",
        "modeling",
        "simulation",
        "aquacrop",
        "fao",
    ],
    
    # Project URLs
    project_urls={
        "Bug Reports": "https://github.com/your-org/AquaCrop_cpp/issues",
        "Source": "https://github.com/your-org/AquaCrop_cpp",
        "Documentation": "https://aquacrop-cpp.readthedocs.io/",
    },
)
