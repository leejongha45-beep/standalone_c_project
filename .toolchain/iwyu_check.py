#!/usr/bin/env python3
"""Run project IWYU with the flags from the editor's compilation database."""
from pathlib import Path
import json
import re
import shlex
import subprocess
import sys

ROOT = Path(__file__).resolve().parent.parent

def main():
    source = Path(sys.argv[1]).resolve()
    database_dir = ROOT / "build/O0"
    for line in (ROOT / ".clangd").read_text().splitlines():
        match = re.match(r"\s*CompilationDatabase:\s*(\S+)", line)
        if match:
            database_dir = ROOT / match.group(1).strip("\"'")
            break
    database = json.loads((database_dir / "compile_commands.json").read_text())
    exact = [entry for entry in database if Path(entry["file"]).resolve() == source]
    associated = [entry for entry in database if Path(entry["file"]).stem == source.stem]
    entry = (exact or associated or database)[0]
    argv = entry.get("arguments") or shlex.split(entry["command"])
    cwd = Path(entry["directory"])
    original = Path(entry["file"]).resolve()
    flags = []
    i = 1
    while i < len(argv):
        arg = argv[i]
        if arg in ("-o", "-MF", "-MT", "-MQ", "-MJ", "-x"):
            i += 2
            continue
        if arg in ("-c", "-MD", "-MMD", "-MP"):
            i += 1
            continue
        if not arg.startswith("-") and (cwd / arg).resolve() == original:
            i += 1
            continue
        flags.append(arg)
        i += 1
    command = [
        str(ROOT / ".toolchain/bin/include-what-you-use"),
        "-Xiwyu", "--no_internal_mappings",
        "-Xiwyu", "--verbose=3",
        *flags, "-x", "c", str(source),
    ]
    result = subprocess.run(command, cwd=cwd, text=True,
                            stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                            timeout=15)
    sys.stdout.write(result.stdout)
    return result.returncode

if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except (OSError, ValueError, KeyError, IndexError, subprocess.TimeoutExpired) as error:
        filename = sys.argv[1] if len(sys.argv) > 1 else str(ROOT)
        print(f"{filename}:1:1: warning: IWYU: {error}")
        raise SystemExit(1)
