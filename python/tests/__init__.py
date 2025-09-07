"""Tests for the Python modules."""

import sys
from pathlib import Path


python_dir = Path(__file__).parent.parent
sys.path.append(str(python_dir / 'src'))
sys.path.append(str(python_dir.parent / 'build' / 'binding'))
