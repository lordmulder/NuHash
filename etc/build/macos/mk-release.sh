#!/bin/sh
set -e
cd -- "$(dirname -- "${0}")/../../.."

mk_nuhash() {
	local command="make -B TARGET=${1}-apple-darwin FLTO=1"
	$SHELL -x -c "${command}"
	mv -vf "${PWD}/bin/nuhash" "${PWD}/out/nuhash-${1}"
}

if [ "$(uname)" != "Darwin" ]; then
	echo "This script is supposed to run on the MacOS platform !!!"
	exit 1
fi

rm -rf "${PWD}/out" && mkdir -p "${PWD}/out"
$SHELL -x -c "make clean"

mk_nuhash "x86_64"
mk_nuhash "aarch64"

find "${PWD}/out" -type f -name 'nuhash-*' -exec $SHELL -x -c "strip \"{}\"" \;

printf "\033[1;32m\nBuild completed successfully.\033[0m\n\n"
