# If there are whitespace errors, print the offending file names and fail.
git diff-index --check --cached $against -- || die "whitespace errors"
