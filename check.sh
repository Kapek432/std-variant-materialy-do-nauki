#!/usr/bin/env bash
# Uruchom z dowolnego katalogu: bash /sciezka/do/variant/check.sh
set -euo pipefail
cd -- "$(dirname -- "$0")"
compiler="${CXX:-c++}"
build_dir="$(mktemp -d "${TMPDIR:-/tmp}/variant-check.XXXXXX")"
trap 'rm -rf -- "$build_dir"' EXIT
"$compiler" --version | head -n 1
for source in examples/*.cpp; do
    name="$(basename "$source" .cpp)"
    "$compiler" -std=c++17 -Wall -Wextra -Wpedantic -Werror "$source" -o "$build_dir/$name"
    "$build_dir/$name" > "$build_dir/$name.txt"
    if [ "$name" = 08_edge_cases ]; then
        sed '/^sizeof wariantu:/,$d' "$build_dir/$name.txt" > "$build_dir/stable.txt"
        diff -u "expected/$name.txt" "$build_dir/stable.txt"
        # Liczby z pomiaru są zależne od biblioteki i platformy
        tail -n 3 "$build_dir/$name.txt"
    else
        diff -u "expected/$name.txt" "$build_dir/$name.txt"
    fi
    printf 'OK %s\n' "$name"
done
