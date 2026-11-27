"""Check the Git working tree against the repository's C++ formatting rules."""

import argparse
import os
from pathlib import Path
import re
import subprocess
import sys


VERSION = "23.1.6"
EXTENSIONS = {".cpp", ".cc", ".cxx", ".h", ".hh", ".hpp", ".hxx", ".ipp", ".inl", ".tpp"}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--fix", action="store_true", help="format files in place")
    args = parser.parse_args()
    root = Path(__file__).resolve().parents[1]
    formatter = os.environ.get("CLANG_FORMAT", "clang-format")
    version = subprocess.check_output([formatter, "--version"], text=True)
    if not re.search(rf"\bversion {re.escape(VERSION)}\b", version):
        sys.exit(f"Expected clang-format {VERSION}, found: {version.strip()}")

    files = subprocess.check_output(
        ["git", "ls-files", "--cached", "--others", "--exclude-standard", "-z"], cwd=root
    )
    paths = []
    for entry in files.split(b"\0"):
        path = root / os.fsdecode(entry)
        if path.suffix.lower() in EXTENSIONS and not path.is_symlink() and path.is_file():
            paths.append(str(path))

    print(f"Checking {len(paths)} C++ source/header files with clang-format {VERSION}", flush=True)
    flags = ["-i"] if args.fix else ["--dry-run", "--Werror"]
    for path in paths:
        subprocess.run([formatter, f"--style=file:{root / '.clang-format'}", *flags, path], check=True)


if __name__ == "__main__":
    try:
        main()
    except (OSError, subprocess.CalledProcessError) as error:
        sys.exit(str(error))
