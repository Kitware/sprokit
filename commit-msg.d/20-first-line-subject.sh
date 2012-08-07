#!/bin/sh

line=$( head -n 1 "$1" )
len=$( echo -n "$line" | wc -c )

is_merge () {
	[ -f "$GIT_DIR/MERGE_MSG" ] &&
		echo "$line" | grep -q "^Merge "
}

is_pull_merge () {
	is_merge &&
		echo "$line" | grep -q "^Merge branch .* of .*$"
}

is_revert () {
	echo "$line" | grep -q "^Revert "
}

is_capitalized () {
	echo "$line" | grep -q "^[A-Z]"
}

is_punctuated () {
	echo "$line" | grep -q '[\.!?]$'
}

is_conjunction () {
	echo "$line" | grep -q ' and '
}

. git-sh-setup

if [ $len -lt 8 ]; then
	die "First line is too short:
$line"
elif [ $len -gt 78 ] && ! is_merge && ! is_revert; then
	die "First line is too long:
$line"
elif ! is_capitalized; then
	die "First line must be capitalized:
$line"
elif is_punctuated; then
	die "First line is not a sentence (remote the punctuation at the end):
$line"
elif is_conjunction; then
	echo >&2 "WARNING: This commit may be conflated (rework it to remove the ' and ')
$line"
fi
