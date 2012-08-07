text=$( sed -e '/^#/d' -e '/^diff --git/q' "$1" )
lines=$( echo -n "$text" | wc -l )
line=$( echo -n "$text" | head -n 2 | tail -n 1 )

[ "$lines" -gt 1 ] && [ -n "$line" ] &&
	die "The second line must be empty"

:
