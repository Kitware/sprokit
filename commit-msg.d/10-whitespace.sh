grep -hn -e '[:space:]$' "$@" && die "Trailing whitespace in commit message"
