#!/bin/bash
# Taken from https://github.com/hyrise/hyrise/blob/master/scripts/format.sh

unamestr=$(uname)
if [[ "$unamestr" == 'Darwin' ]]; then
	clang_format="$(brew --prefix llvm)/bin/clang-format"
	format_cmd="$clang_format -i -style=file '{}'"
elif [[ "$unamestr" == 'Linux' ]]; then
	format_cmd="clang-format -i -style=file '{}'"
fi


if [ "${1}" = "all" ]; then
    find src -iname "*.cpp" -o -iname "*.hpp" -o -iname "*.ipp" | xargs -I{} sh -c "${format_cmd}"
elif [ "$1" = "modified" ]; then
    # Run on all changed as well as untracked cpp/hpp files, as compared to the current HEAD. Skip deleted files.
    { git diff --diff-filter=d --name-only & git ls-files --others --exclude-standard; } | grep -E "^src.*\.[chi]pp$" | xargs -I{} sh -c "${format_cmd}"
elif [ "$1" = "staged" ]; then
    # Run on all files that are staged to be committed.
    git diff --diff-filter=d --cached --name-only | grep -E "^src.*\.[chi]pp$" | xargs -I{} sh -c "${format_cmd}"
else
    # Run on all changed as well as untracked cpp/hpp files, as compared to the current main branch. Skip deleted files.
    { git diff --diff-filter=d --name-only main & git ls-files --others --exclude-standard; } | grep -E "^src.*\.[chi]pp$" | xargs -I{} sh -c "${format_cmd}"
fi

# Python formatting
black --line-length 120 scripts -q