#!/usr/bin/env python3
"""Flatten an SPR solution header into an Online Judge submission."""

from __future__ import annotations

import argparse
import re
import subprocess
from pathlib import Path


LOCAL_INCLUDE = re.compile(r'^\s*#\s*include\s*"([^"]+)"\s*(?://.*)?$')
NAMESPACE = re.compile(r"^[A-Za-z_]\w*(?:::[A-Za-z_]\w*)*$")


def is_beneath(path: Path, root: Path) -> bool:
    try:
        path.relative_to(root)
        return True
    except ValueError:
        return False


def flatten(source: Path, include_root: Path, seen: set[Path]) -> str:
    source = source.resolve()
    if source in seen:
        return ""
    if not is_beneath(source, include_root):
        raise ValueError(f"project header is outside include root: {source}")

    seen.add(source)
    output: list[str] = []
    for line_number, line in enumerate(source.read_text(encoding="utf-8").splitlines(), 1):
        if re.match(r"^\s*#\s*pragma\s+once\s*$", line):
            continue

        match = LOCAL_INCLUDE.match(line)
        if not match:
            output.append(line)
            continue

        include = match.group(1)
        candidates = (source.parent / include, include_root / include)
        dependency = next((path.resolve() for path in candidates if path.is_file()), None)
        if dependency is None or not is_beneath(dependency, include_root):
            raise ValueError(
                f'{source}:{line_number}: cannot inline local include "{include}"'
            )

        inlined = flatten(dependency, include_root, seen)
        if inlined:
            output.extend((f"// begin {dependency.relative_to(include_root)}", inlined,
                           f"// end {dependency.relative_to(include_root)}"))

    return "\n".join(output)


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--source", type=Path, required=True)
    parser.add_argument("--include-root", type=Path, required=True)
    parser.add_argument("--output", type=Path, required=True)
    parser.add_argument("--namespace", required=True)
    parser.add_argument("--compiler")
    args = parser.parse_args()

    source = args.source.resolve()
    include_root = args.include_root.resolve()
    if not NAMESPACE.fullmatch(args.namespace):
        parser.error(f"invalid C++ namespace: {args.namespace}")
    if not source.is_file():
        parser.error(f"solution header does not exist: {source}")
    if not is_beneath(source, include_root):
        parser.error(f"solution header is outside include root: {source}")

    body = flatten(source, include_root, set())
    submission = f"""// Generated from {source.relative_to(include_root)}. Do not edit.
#include <iostream>

{body}

int main()
{{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    {args.namespace}::solve(std::cin, std::cout);
    return 0;
}}
"""

    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(submission, encoding="utf-8")

    try:
        if args.compiler:
            subprocess.run(
                [args.compiler, "-std=c++11", "-fsyntax-only", str(args.output)],
                check=True,
            )
    except Exception:
        args.output.unlink(missing_ok=True)
        raise


if __name__ == "__main__":
    main()
