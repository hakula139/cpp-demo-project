"""Tests for the Python modules."""

import pathlib
import sys

python_dir = pathlib.Path(__file__).parent.parent
sys.path.append(str(python_dir / 'src'))
sys.path.append(str(python_dir.parent / 'build' / 'binding'))
