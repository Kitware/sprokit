. git-sh-setup

"$GIT_DIR/hooks/commit-msg" ${1+"$@"} ||
	die "commit-msg hook failed"
