. git-sh-setup

head -n 1 "$1" | grep -q -e '^Merge branch .* of .*$' &&
	die "Non-fast forward merges when pulling is not allowed.
Pull then remerge or set branch.<branch>.rebase = true."

:
