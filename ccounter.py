#!/usr/bin/env python3
import os

HEADER_EXTS = {".h", ".hpp"}
SOURCE_EXTS = {".c", ".cpp"}

def count_lines(root_dir):
    total_lines = 0
    header_lines = 0
    source_lines = 0

    for root, _, files in os.walk(root_dir):
        for name in files:
            ext = os.path.splitext(name)[1].lower()
            if ext not in HEADER_EXTS | SOURCE_EXTS:
                continue

            path = os.path.join(root, name)
            try:
                with open(path, "r", encoding="utf-8", errors="ignore") as f:
                    lines = sum(1 for _ in f)
            except OSError:
                continue

            total_lines += lines
            if ext in HEADER_EXTS:
                header_lines += lines
            elif ext in SOURCE_EXTS:
                source_lines += lines

    return total_lines, header_lines, source_lines


if __name__ == "__main__":
    script_dir = os.path.dirname(os.path.abspath(__file__))

    total, headers, sources = count_lines(script_dir)

    print("C/C++ Line Count")
    print("----------------")
    print(f"Headers (.h/.hpp): {headers}")
    print(f"Sources (.c/.cpp): {sources}")
    print(f"TOTAL:             {total}")