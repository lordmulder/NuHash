#!/bin/bash
set -e
cd -- "$(dirname -- "${BASH_SOURCE[0]}")/../../.."

mk_nuhash() {
	local command="make -B CC=${2} MARCH=${3} MTUNE=${4} STATIC=1 STRIP=1"
	$BASH -x -c "${command}"
	mv -vf "${PWD}/bin/nuhash" "${PWD}/out/nuhash-${1}"
}

if [[ "$(uname -s)" != "CYGWIN_NT"* ]]; then
	echo "This script is supposed to run on the Cygwin platform !!!"
	exit 1
fi

[[ "${1}" == "--no-remove" ]] || rm -vrf "${PWD}/out" && mkdir -vp "${PWD}/out"
$BASH -x -c "make clean"

case "$(uname -m)" in
	x86_64)
		mk_nuhash "x86_64" "clang" "x86-64" "znver3"
		;;
	i686)
		mk_nuhash "i686" "gcc" "pentiumpro" "generic"
		;;
	*)
		echo "Unknown host CPU type !!!"
		exit 1
esac

cp -vf "$(which cygwin1.dll)" "${PWD}/out"

echo "Build completed successfully."
