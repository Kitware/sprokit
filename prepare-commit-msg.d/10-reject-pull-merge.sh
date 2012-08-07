cat "$1" | head -n1 | grep -q -e "Merge branch \'.*\' of .*" &&
	die "Non-fast forward merges when pulling is not allowed.\n" \
	    "Pull then remerge or set branch.<branch>.rebase = true."
