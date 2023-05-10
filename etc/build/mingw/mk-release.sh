#!/bin/bash
set -e
cd -- "$(dirname -- "${BASH_SOURCE[0]}")/../../.."

mk_nuhash() {
	local command="make -B CC=${2} MARCH=${3} MTUNE=${4} STATIC=1 STRIP=1 FLTO=1"
	$BASH -x -c "${command}"
	mv -vf "${PWD}/bin/nuhash" "${PWD}/out/nuhash-${1}"
}

if [[ "$(uname -so)" != "MINGW"*" Msys" ]]; then
	echo "This script is supposed to run on the Mingw-w64 platform !!!"
	exit 1
fi

[[ "${1}" == "--no-remove" ]] || rm -vrf "${PWD}/out" && mkdir -vp "${PWD}/out"
$BASH -x -c "make clean"

case "$MSYSTEM" in
	MINGW64)
		mk_nuhash "x86_64" "gcc" "x86-64" "znver3"
		;;
	MINGW32)
		mk_nuhash "i686" "gcc" "pentiumpro" "generic"
		;;
	CLANG64)
		mk_nuhash "x86_64" "clang" "x86-64" "znver3"
		;;
	CLANG32)
		mk_nuhash "i686" "clang" "pentiumpro" "generic"
		;;
	*)
		echo "Unknown host CPU type !!!"
		exit 1
esac

echo "Build completed successfully."
