#!/usr/bin/env bash

set -eu

list-source() {
  find \
    . \
    -type f \
    -and \( -name '*.cc' -or -name '*.h' \) \
    -and -not -path './out_*' \
    "$@"
}

format-check() {
  diff -u <(cat "$@") <(clang-format "$@")
}

call-cpplint() {
  cpplint \
    --quiet \
    --filter=-legal/copyright,-build/c++20,-readability/todo \
    "$@"
}

call-cppcheck() {
  cppcheck \
    -j 6 \
    --language=c++ \
    --std=c++20 \
    --enable=style \
    --quiet \
    -I"." \
    "$@"
}

sources="$(list-source)"

call-cpplint $sources 2>&1 >/dev/null
call-cppcheck $sources
format-check $sources
