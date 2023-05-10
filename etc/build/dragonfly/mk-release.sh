#!/bin/sh
# pkg install llvm14
set -e
cd -- "$(dirname -- "${0}")/../../.."

mk_nuhash() {
	local command="gmake -B CC=clang14 STATIC=1 STRIP=1 ${2}"
	$SHELL -x -c "${command}"
	mv -vf "${PWD}/bin/nuhash" "${PWD}/out/nuhash-${1}"
}

if [ "$(uname -sm)" != "DragonFly x86_64" ]; then
	echo "This script is supposed to run on the DragonFly (x86_64) platform !!!"
	exit 1
fi

rm -rf "${PWD}/out" && mkdir -p "${PWD}/out"
$SHELL -x -c "gmake clean"

mk_nuhash "x86_64" "MARCH=x86-64 MTUNE=znver3"

printf "\033[1;32m\nBuild completed successfully.\033[0m\n\n"
