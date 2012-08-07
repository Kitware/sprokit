line=$( head -n 2 "$1" | tail -n 1 )

[ -n "$line" ] && die "The second line must be empty"
