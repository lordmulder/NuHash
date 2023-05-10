#!/bin/bash
# sudo apt install clang crossbuild-essential-{i386,armel,armhf,arm64,mips,mipsel,mips64,mips64el,mips64r6,mips64r6el,s390x,riscv64}
set -e
cd -- "$(dirname -- "${BASH_SOURCE[0]}")"

function mk_musl() {
	for cc in gcc clang; do
		local outdir="/usr/local/musl/${1}-${cc}"
		local build="${PWD}/musl-build"
		rm -rf "${build}" && mkdir -p "${build}"
		tar -xvf "${PWD}/musl-latest.tar.gz" --strip-components=1 -C "${build}"
		pushd "${build}"
		local optdirs="$(find './src' -mindepth 1 -maxdepth 1 -type d -printf '%f,' | sed 's/,$//g')"
		if [ "${cc}" == "clang" ]; then
			CC="${cc} --target=${2}" ./configure --enable-optimize="${optdirs}" --disable-shared --enable-wrapper=${cc} --prefix="${outdir}" --host="${2}"
		else
			CC="${2}-${cc}" ./configure --enable-optimize="${optdirs}" --disable-shared --enable-wrapper=${cc} --prefix="${outdir}" --host="${2}"
		fi
		make
		sudo rm -rf "${outdir}" && sudo make install
		popd
	done
}

if [[ "$(uname -sm)" != "Linux x86_64" ]]; then
	echo "This script is supposed to run on the Linux (x86_64) platform !!!"
	exit 1
fi

if ! which gcc clang > /dev/null; then
	echo "Please make sure that \"gcc\" and \"clang\" are installed !!!"
	exit 1
fi

rm -f "${PWD}/musl-latest.tar.gz"
curl -vkf -o "${PWD}/musl-latest.tar.gz" "https://musl.libc.org/releases/musl-latest.tar.gz"

mk_musl x86_64 x86_64-linux-gnu
mk_musl i686 i686-linux-gnu
mk_musl armel arm-linux-gnueabi
mk_musl armhf arm-linux-gnueabihf
mk_musl arm64 aarch64-linux-gnu
mk_musl mips mips-linux-gnu
mk_musl mipsel mipsel-linux-gnu
mk_musl mips64 mips64-linux-gnuabi64
mk_musl mips64el mips64el-linux-gnuabi64
mk_musl mips64r6 mipsisa64r6-linux-gnuabi64
mk_musl mips64r6el mipsisa64r6el-linux-gnuabi64
mk_musl s390x s390x-linux-gnu
mk_musl riscv64 riscv64-linux-gnu

rm -rvf "${PWD}/musl-build"
