#!/bin/bash
# pkgman install llvm12_clang
set -e
cd -- "$(dirname -- "${BASH_SOURCE[0]}")/../../.."

mk_nuhash() {
	local command="make -B CC=${2} MARCH=${3} MTUNE=${4} STATIC=1 STRIP=1"
	$BASH -x -c "${command}"
	mv -vf "bin/nuhash" "out/nuhash-${1}"
}

if [ "$(uname)" != "Haiku" ]; then
	echo "This script is supposed to run on the Haiku platform !!!"
	exit 1
fi

rm -rf "out" && mkdir -p "out"
$BASH -x -c "make clean"

case "${BE_HOST_CPU}" in
	x86_64)
		mk_nuhash "x86_64" "clang-12" "x86-64" "znver3"
		;;
	x86)
		mk_nuhash "i686" "gcc-x86" "pentiumpro" "generic"
		;;
	*)
		echo "Unknown host CPU type !!!"
		exit 1
esac

echo "Build completed successfully."
