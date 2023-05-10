#!/bin/sh
# pkg install {armv7,aarch64,powerpc,powerpc64,powerpc64le,riscv64}-freebsd-sysroot
set -e
cd -- "$(dirname -- "${0}")/../../.."

mk_nuhash1() {
	local command="gmake -B CPU=${1} STATIC=1 STRIP=1 FLTO=1 ${3}"
	$SHELL -x -c "${command}"
	mv -vf "${PWD}/bin/nuhash" "${PWD}/out/nuhash-${2}"
}

mk_nuhash2() {
	local command="gmake -B TARGET=${1}-unknown-freebsd SYSROOT=/usr/local/freebsd-sysroot/${1} STATIC=1 STRIP=1"
	$SHELL -x -c "${command}"
	mv -vf "${PWD}/bin/nuhash" "${PWD}/out/nuhash-${1}"
}

if [ "$(uname -sm)" != "FreeBSD amd64" ]; then
	echo "This script is supposed to run on the FreeBSD (amd64) platform !!!"
	exit 1
fi

rm -rf "${PWD}/out" && mkdir -p "${PWD}/out"
$SHELL -x -c "gmake clean"

mk_nuhash1 32 "i686" "MARCH=pentiumpro MTUNE=generic"
mk_nuhash1 64 "x86_64" "MARCH=x86-64 MTUNE=znver3"
mk_nuhash2 "armv7"
mk_nuhash2 "aarch64"
mk_nuhash2 "powerpc"
mk_nuhash2 "powerpc64"
mk_nuhash2 "powerpc64le"
mk_nuhash2 "riscv64"

printf "\033[1;32m\nBuild completed successfully.\033[0m\n\n"
