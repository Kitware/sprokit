#!/bin/sh
#=============================================================================
# Copyright 2010-2012 Kitware, Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#=============================================================================

bad=$( git diff-index --check --cached $against -- ) || die "$bad"

# Approximate whitespace=tab-in-indent check with Git < 1.7.2.
git --version | grep -q " \(1\.[0-6]\|1\.7\.[01]\)" &&
	approx_tab_in_indent=true || approx_tab_in_indent=false

check_tab() {
	lines=$( git diff-index -p --cached $against -- "$1" |
	        grep '^+	' ) &&
	echo "$lines" |
	while read line; do
		echo "$1: tab in indent." &&
		echo "$line"
	done
}

# Reject addition of a line without a newline at end-of-file.
check_no_lf_at_eof() {
	lines=$( git diff-index -p --cached $against -- "$1" | tail -2 )
	if echo "$lines" | head -1 | grep -q '^+' &&
	   echo "$lines" | tail -1 | grep -q '^\\ No newline'; then
		echo "$1: No newline at end of file"
	fi
}

# Custom whitespace checks.
check_whitespace() {
	binary=$( git check-attr binary -- "$file" |
	     sed -ne '/binary: set$/p' )
	[ -n "$binary" ] && continue

	ws=$( git check-attr whitespace -- "$file" |
	     sed -e 's/^[^:]*: whitespace: //' )
	if $approx_tab_in_indent; then
		case ",$ws," in
			*,tab-in-indent,*) check_tab "$1" ;;
		esac
	fi
	case ",$ws," in
		*,no-lf-at-eof,*) check_no_lf_at_eof "$1" ;;
	esac
}

bad=$( git diff-index --name-only --cached $against -- |
	while read file; do
		check_whitespace "$file"
	done )

test -z "$bad" || die "$bad"
