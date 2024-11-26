#!/bin/bash
# Taken from https://github.com/hyrise/hyrise/blob/master/scripts/lint.sh and slightly modified.
# We dropped some linting steps related to tests, as we do not use any (yet).

# Install cpplint via pip.
/usr/bin/env python3 -m pip install --quiet cpplint

exitcode=0

# Switch to base directory of repository relative to this script file (allows this script to be called from arbitrary working directories).
cd "$(dirname "${BASH_SOURCE[0]}")/.."

# Run cpplint. Excluded checks:
#  - legal/copyright: We do not include copyright information in our source files.
#  - runtime/references: We prefer references to pointers in most cases.
#  - build/(c++11|c++17): cpplint complains about headers that are "unapproved" by Google devs (see https://github.com/google/styleguide/issues/194).
#  - readability/nolint: cpplint complains about excluded clang-tidy categories, which we widely use.
#  - whitespace/braces: Does not work with {} initialization (see https://github.com/cpplint/cpplint/issues/204#issuecomment-1146769949).
#  - build/include_subdir: Does not work well with our structure of using lib, test, and third_party directories.
find src \( -iname "*.cpp" -o -iname "*.hpp" \) -print0 | parallel --null --no-notice -j 100% --nice 17 cpplint --verbose=0 --extensions=hpp,cpp --counting=detailed --filter=-legal/copyright,-runtime/references,-build/c++11,-build/c++17,-readability/nolint,-whitespace/braces,-build/include_subdir --linelength=120 {} 2\>\&1 \| grep -v \'\^Done processing\' \| grep -v \'\^Total errors found: 0\' \; test \${PIPESTATUS[0]} -eq 0
let "exitcode |= $?"
#                             /------------------ runs in parallel -------------------\
# Conceptual: find | parallel python cpplint \| grep -v \| test \${PIPESTATUS[0]} -eq 0
#             ^      ^        ^                 ^          ^
#             |      |        |                 |          |
#             |      |        |                 |          Get the return code of the first pipeline item (here: cpplint)
#             |      |        |                 Removes the prints for files without errors
#             |      |        Regular call of cpplint with options
#             |      Runs the following in parallel
#             Finds all .cpp and .hpp files, separated by \0

# Check for included cpp files. You would think that this is not necessary, but history proves you wrong.
regex='#include .*\.cpp'
namecheck=$(find src \( -iname "*.cpp" -o -iname "*.hpp" \) -print0 | xargs -0 grep -rHn "$regex" | grep -v NOLINT)
let "exitcode |= ! $?"
while IFS= read -r line
do
	if [ ! -z "$line" ]; then
		echo $line | sed 's/^\([a-zA-Z/._]*:[0-9]*\).*/Include of cpp file:/' | tr '\n' ' '
		echo $line | sed 's/\(:[^:]*:\)/\1 /'
	fi
done <<< "$namecheck"

# The singleton pattern should not be manually implemented
output=$(grep -rHn 'static[^:]*instance;' --exclude singleton.hpp src | sed 's/^\([a-zA-Z/._]*:[0-9]*\).*/\1  Singletons should not be implemented manually. Have a look at src\/lib\/utils\/singleton.hpp/')
if [ ! -z "$output" ]; then
	echo "$output"
	exitcode=1
fi


# Check for Windows-style line endings
output=$(find src -type f -exec dos2unix -ic {} +)
if [[ ${output} ]]; then
	for file in ${output}
	do
		echo "Windows-style file ending: $file"
	done
	exitcode=1
fi

# Python linting
output=$(flake8 --max-line-length 120 --extend-ignore=E203 visualization)
if [ ! -z "$output" ]; then
	echo "$output"
	exitcode=1
fi

exit $exitcode