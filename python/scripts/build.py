"""Complete package build script that includes .so files automatically."""

import atexit
import shutil
import subprocess  # nosec B404
import sys
from collections.abc import Generator
from contextlib import contextmanager
from enum import Enum
from pathlib import Path
from typing import Any

# Global flag to track build result
_is_build_success = False


class Color(Enum):
    RESET = '\033[0m'
    RED = '\033[91m'
    GREEN = '\033[92m'
    YELLOW = '\033[93m'
    BLUE = '\033[94m'
    MAGENTA = '\033[95m'
    CYAN = '\033[96m'


def print_colored(message: str, color: Color, **kwargs: Any) -> None:
    print(f'{color.value}{message}{Color.RESET.value}', **kwargs)


def exit_success(dist_dir: Path) -> None:
    """Exit with a message if build succeeded."""
    global _is_build_success
    _is_build_success = True

    print_colored('\nBuild complete! The package is ready in', Color.GREEN, end=' ')
    print_colored(str(dist_dir), Color.CYAN)


@atexit.register
def exit_failure() -> None:
    """Exit with a message if build failed."""
    if not _is_build_success:
        print_colored('\nBuild failed! Check the logs above for details.', Color.RED)


@contextmanager
def tracker() -> Generator[set[Path], None, None]:
    """Automatically track files for cleanup."""
    files: set[Path] = set()
    try:
        yield files
    finally:
        print_colored('\n4. Clean up temporary files', Color.BLUE)

        for file in files:
            if file.exists():
                print(f'   Removing {file}')
                file.unlink()

        print_colored(f'   Removed {len(files)} temporary files', Color.GREEN)


def find_so_files(build_dir: Path) -> list[Path]:
    """Find and validate .so files in the build directory."""
    print_colored('\n1. Prepare .so files for packaging', Color.BLUE)

    so_files = list(build_dir.glob('*.so'))

    if not so_files:
        print_colored(f'   No .so files found in {build_dir}!', Color.RED)
        print('   Please build the C++ project first with:')
        print_colored('       cmake --preset release-python', Color.CYAN)
        print_colored('       cmake --build --preset release-python', Color.CYAN)
        sys.exit(1)

    print(f'   Found {len(so_files)} .so files in build directory:')
    for so_file in so_files:
        print(f'       {so_file}')

    return so_files


def sync_so_files(
    so_files: list[Path], src_dir: Path, tracked_files: set[Path] | None = None
) -> None:
    """Sync .so files to the source directory for packaging."""
    src_dir.mkdir(parents=True, exist_ok=True)

    for so_file in so_files:
        dst_file = src_dir / so_file.name
        print(f'   Syncing {so_file.name}...')
        shutil.copy2(so_file, dst_file)

        if tracked_files:
            tracked_files.add(dst_file)

    print_colored(f'   Synced {len(so_files)} .so files to {src_dir}', Color.GREEN)


def build_package(python_dir: Path) -> None:
    """Build the package using poetry."""
    print_colored('\n2. Build package with poetry', Color.BLUE)

    poetry_path = shutil.which('poetry')
    if not poetry_path:
        print_colored('   Poetry not found in PATH!', Color.RED)
        sys.exit(1)

    build_result = subprocess.run(
        [poetry_path, 'build'],
        cwd=python_dir,
        capture_output=True,
        text=True,  # nosec B603
    )

    if build_result.returncode != 0:
        print_colored(f'   Build failed: {build_result.stderr}', Color.RED)
        sys.exit(1)

    print_colored('   Package built successfully!', Color.GREEN)


def verify_package(dist_dir: Path, expected_so_count: int) -> None:
    """Verify that .so files are included in the built package."""
    print_colored('\n3. Verify .so files are included', Color.BLUE)

    unzip_path = shutil.which('unzip')
    if not unzip_path:
        print_colored('   unzip not found in PATH!', Color.RED)
        sys.exit(1)

    wheel = next(dist_dir.glob('*.whl'))
    if not wheel.exists() or not str(wheel).endswith('.whl'):
        print_colored(f'   Invalid wheel: {wheel}', Color.RED)
        sys.exit(1)

    verify_result = subprocess.run(
        [unzip_path, '-l', str(wheel)],
        capture_output=True,
        text=True,  # nosec B603
    )

    so_in_package = [
        line.split()[-1]
        for line in verify_result.stdout.split('\n')
        if line.endswith('.so')
    ]

    if so_in_package:
        print(f'   Package includes {len(so_in_package)} .so files:')
        for so_file in so_in_package:
            print(f'       {so_file}')
    else:
        print_colored('   No .so files found in package!', Color.RED)
        sys.exit(1)

    if len(so_in_package) != expected_so_count:
        print_colored(
            f'   Expected {expected_so_count} .so files in package, '
            f'but found {len(so_in_package)}!',
            Color.RED,
        )
        sys.exit(1)

    print_colored(
        f'   Verified {len(so_in_package)} .so files are included in package',
        Color.GREEN,
    )


def main() -> None:
    """Build the package with automatic .so file handling."""
    # Set up paths
    python_dir = Path(__file__).parent.parent
    project_root = python_dir.parent
    build_dir = project_root / 'build' / 'binding'
    src_dir = python_dir / 'src' / 'demo'
    dist_dir = python_dir / 'dist'

    so_files = find_so_files(build_dir)
    sync_so_files(so_files, src_dir)
    build_package(python_dir)
    verify_package(dist_dir, len(so_files))

    exit_success(dist_dir)


if __name__ == '__main__':
    main()
