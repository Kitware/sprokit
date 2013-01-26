#!/bin/sh

# This script checks files being committed that have a copyright statement for
# said statement giving the current year.
#
# By default, it checks files matching:
#   *.c, *.cxx, *.cpp, *.h, *.hxx, *.hpp, *.in, *.txt, *.cmake
#
# To change this, set the git configuration value 'check.copyright.filepattern'
# to a valid shell 'case' pattern matching the file names that should be
# checked.

#------------------------------------------------------------------------------
check_copyright () {
	copyright="$( git show :"$fn" | \
	              sed -n -e 's/^.*\(Copyright [0-9, -]\+\).*$/\1/p' )"
	[ -z "$copyright" ] && return 0
	[ -z "$( echo "$copyright" | grep -vlE "\b$year\b[, ]*" )" ]
}

#------------------------------------------------------------------------------
show_errors () {
	git show :"$1" | \
	grep -HE "Copyright [0-9, -]*" | \
	sed -e "s*[(]standard input[)]*$1*"
}


#------------------------------------------------------------------------------
check_files () {
	result=0
	while read s fn; do
		[ "$s" = "D" ] && continue
		[ "${fn:0:1}" = '"' ] && eval fn=\$\'${fn:1:${#fn}-2}\'
		attr="$( git check-attr hooks.checkcopyright -- "$fn" | \
		         sed -e 's/^[^:]*: hooks.checkcopyright: //' )"
		case "$attr" in
			unset)
				# Ignore copyright checks.
				;;
			set)
				# Force copyright checks.
				if ! check_copyright "$fn"; then
					show_errors "$fn" >&2
					result=1
				fi
				;;
			unspecified)
				# Default based on ckwg.
				git show :"$fn" | head -n 10 | grep -q -e 'ckwg' || return 0
				#[ -n "$( git show :"$fn" | head -n 10 | grep -q -e 'ckwg' )" ] || return 0
				if ! check_copyright "$fn"; then
					show_errors "$fn" >&2
					result=1
				fi
				;;
			*)
				# Anything else is an error.
				echo >&2 "The file '$fn' has an invalid hooks.checkcopyright value: $val"
				result=2
				;;
		esac
	done
	return $result
}

###############################################################################

[ -n "$NO_VERIFY_COPYRIGHT" ] && exit 0

if git rev-parse -q --verify HEAD >/dev/null; then
	against=HEAD
else
	# Initial commit: diff against an empty tree object
	against=4b825dc642cb6eb9a060e54bf8d69288fbee4904
fi

year="$( date +%Y )"

# Walk list of staged files

if ! git diff-index --cached --name-status $against | check_files; then
	cat <<EOF >&2
The above files do not appear to have correct (current) copyright statements,
so your commit has been aborted. Please fix them before continuing.

If you are certain the above copyright statements are correct (e.g. because
the files are not actually changed, or you are reverting the only change made
this year), you can bypass this check by setting (temporarily, please!)
NO_VERIFY_COPYRIGHT to a non-empty value.
EOF
	exit 1
fi

:
