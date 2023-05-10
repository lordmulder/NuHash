#!/bin/ksh
set -e
cd -- "$(dirname -- "${0}")/../../.."

mk_nuhash() {
	local command="gmake -B CC=clang CPU=${1} MARCH=${3} MTUNE=${4} STATIC=1 STRIP=1"
	$SHELL -x -c "${command}"
	mv -vf "bin/nuhash" "out/nuhash-${2}"
}

if [ "$(uname)" != "NetBSD" ]; then
	echo "This script is supposed to run on the OpenBSD platform !!!"
	exit 1
fi

rm -rf "out" && mkdir -p "out"
$SHELL -x -c "gmake clean"

mk_nuhash 32 "i686" "pentiumpro" "generic"
mk_nuhash 64 "x86_64" "x86-64" "znver3"

echo "Build completed successfully."
