#!/bin/ksh
set -e
cd -- "$(dirname -- "${0}")/../../.."

mk_nuhash() {
	local command="gmake -B MARCH=${2} MTUNE=${3} STATIC=1 STRIP=1 FLTO=1"
	$SHELL -x -c "${command}"
	mv -vf "bin/nuhash" "out/nuhash-${1}"
}

if [ "$(uname)" != "OpenBSD" ]; then
	echo "This script is supposed to run on the OpenBSD platform !!!"
	exit 1
fi

rm -rf "out" && mkdir -p "out"
$SHELL -x -c "gmake clean"

case "$(arch -s)" in
	amd64)
		mk_nuhash "x86_64" "x86-64" "znver3"
		;;
	i386)
		mk_nuhash "i686" "pentiumpro" "generic"
		;;
	*)
		echo "Unknown host CPU type !!!"
		exit 1
esac

echo "Build completed successfully."
