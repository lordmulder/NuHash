#!/bin/bash
# See "etc/utils/linux/mk-libmusl.sh" in order to build musl libc!
set -e
cd -- "$(dirname -- "${BASH_SOURCE[0]}")/../../.."

function mk_nuhash() {
	local command="make -B CC=/usr/local/musl/${1}-${3:-clang}/bin/musl-${3:-clang} STATIC=1 STRIP=1 FLTO=1"
	$BASH -x -c "${command}${2:+ ${2}}"
	mv -vf "${PWD}/bin/nuhash" "${PWD}/out/nuhash-${1}"
}

if [[ "$(uname -sm)" != "Linux x86_64" ]]; then
	echo "This script is supposed to run on the Linux (x86_64) platform !!!"
	exit 1
fi

rm -vrf "${PWD}/out" && mkdir -vp "${PWD}/out"
$BASH -x -c "make clean"

mk_nuhash "x86_64" "MARCH=x86-64 MTUNE=znver3"
mk_nuhash "i686" "MARCH=pentiumpro MTUNE=generic"
mk_nuhash "armel"
mk_nuhash "armhf"
mk_nuhash "arm64"
mk_nuhash "mips"
mk_nuhash "mipsel"
mk_nuhash "mips64" "" "gcc"
mk_nuhash "mips64el" "" "gcc"
mk_nuhash "mips64r6" "" "gcc"
mk_nuhash "mips64r6el" "" "gcc"
mk_nuhash "s390x"
mk_nuhash "riscv64"

$BASH -x -c "pandoc --standalone --embed-resources --css \"${PWD}/etc/style/gh-pandoc.min.css\" --metadata title=\"NuHash README\" -o \"${PWD}/out/README.html\" \"${PWD}/README.md\""

printf "\033[1;32m\nBuild completed successfully.\033[0m\n\n"
