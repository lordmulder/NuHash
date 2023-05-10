#!/bin/bash
set -e
cd -- "$(dirname -- "${BASH_SOURCE[0]}")/../../.."

function mk_nuhash() {
	local command="make -B CC=gcc STATIC=1 STRIP=1 FLTO=1"
	$BASH -x -c "${command}${2:+ ${2}}"
	mv -vf "${PWD}/bin/nuhash" "${PWD}/out/nuhash-${1}"
}

if [[ "$(uname -sm)" != "GNU i686-AT386" ]]; then
	echo "This script is supposed to run on the GNU Hurd (i686) platform !!!"
	exit 1
fi

rm -rf "${PWD}/out" && mkdir -p "${PWD}/out"
$BASH -x -c "make clean"

mk_nuhash "i686" "MARCH=pentiumpro MTUNE=generic"

printf "\033[1;32m\nBuild completed successfully.\033[0m\n\n"
