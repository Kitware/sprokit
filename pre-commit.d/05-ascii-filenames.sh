if git rev-parse --quiet --verify HEAD >/dev/null; then
	against=HEAD
else
	# Initial commit: diff against an empty tree object
	against=4b825dc642cb6eb9a060e54bf8d69288fbee4904
fi

. git-sh-setup

# Cross platform projects tend to avoid non-ascii filenames; prevent
# them from being added to the repository. We exploit the fact that the
# printable range starts at the space character and ends with tilde.
if \
	# Note that the use of brackets around a tr range is ok here, (it's
	# even required, for portability to Solaris 10's /usr/bin/tr), since
	# the square bracket bytes happen to fall in the designated range.
	test $(git diff --cached --name-only --diff-filter=A -z $against |
	  LC_ALL=C tr -d '[ -~]\0' | wc -c) != 0
then
	die "Non-ascii filenames are prohibited:
$( git diff --cached --name-only --diff-filter=A $against )"
fi
