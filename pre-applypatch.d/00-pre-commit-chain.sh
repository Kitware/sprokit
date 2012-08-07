#!/bin/sh

. git-sh-setup

"$GIT_DIR/hooks/pre-commit" ${1+"$@"} ||
	die "pre-commit hook failed"
