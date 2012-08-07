. git-sh-setup

sed -e '/^#/d' -e '/^diff --git/q' "$@" | grep -hn -e '[[:space:]]$' &&
	die "Trailing whitespace in commit message"

:
