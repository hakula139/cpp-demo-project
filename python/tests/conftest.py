"""Pytest configuration and fixtures for Python tests."""

import sys
from pathlib import Path

import pytest

# Add the python module to the path so we can import it
project_root = Path(__file__).parent.parent
python_module_path = project_root / "python"
sys.path.insert(0, str(python_module_path))


@pytest.fixture(scope="session")
def project_root_path():
    """Provide project root path for tests."""
    return project_root


@pytest.fixture(scope="session")
def python_module_path_fixture():
    """Provide Python module path for tests."""
    return python_module_path


@pytest.fixture
def sample_integers():
    """Provide sample integer data for tests."""
    return [1, 3, 2, 5, 4, 8, 7, 6, 9, 10]


@pytest.fixture
def sample_floats():
    """Provide sample float data for tests."""
    return [1.5, 3.2, 2.1, 5.8, 4.3]


@pytest.fixture
def sample_strings():
    """Provide sample string data for tests."""
    return ['apple', 'banana', 'cherry', 'date', 'elderberry']


@pytest.fixture
def cleanup_tracker():
    """Provide a cleanup tracker for testing resource management."""
    tracker = []
    yield tracker
    # Cleanup happens automatically when fixture goes out of scope
