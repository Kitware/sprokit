. git-sh-setup

test "" = "$( grep '^Signed-off-by: ' "$1" |
	sort | uniq -c | sed -e '/^[ 	]*1[ 	]/d' )" ||
	die "Duplicate Signed-off-by lines."
