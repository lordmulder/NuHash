/******************************************************************************/
/* NuHash, by LoRd_MuldeR <MuldeR2@GMX.de>                                    */
/* This work has been released under the CC0 1.0 Universal license!           */
/******************************************************************************/

#ifndef _INC_NUHASH_PLATFORM_H
#define _INC_NUHASH_PLATFORM_H

/* CRT */
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/types.h>

/* platform-specific */
#ifdef _WIN32
#  include <fcntl.h>
#  include <io.h>
#  include <share.h>
#else
#  include <unistd.h>
#endif

/* initialization */
void initialize_process(void);

/* monotonic clock */
uint64_t get_mtime(void);

/* abort flag */
extern volatile int g_aborted;

/* integer types */
#if !defined(_MSC_VER) || (_MSC_VER >= 1800)
#  include <inttypes.h>
#else
#  define PRIX8  "hhX"
#  define PRIX16 "hX"
#  define PRIX32 "X"
#  define PRIX64 "llX"
#endif

/* Unicode support*/
#ifdef _WIN32
#  define CHAR_T wchar_t
#  define __TEXT(X) L##X
#  define TEXT(X) __TEXT(X)
#  define MAIN wmain
#  define FPRINTF fwprintf
#  define FPUTS fputws
#  define FOPEN(X,Y) _wfsopen((X),(Y),_SH_SECURE)
#  define STRCASECMP _wcsicmp
#  define strcasecmp _stricmp
#  define TOLOWER(X) towlower((unsigned short)(X))
#  ifndef __USE_MINGW_ANSI_STDIO
#    define FMT_TCHAR L"%s"
#    define FMT_HCHAR L"%S"
#  else
#    define FMT_TCHAR L"%ls"
#    define FMT_HCHAR L"%hs"
#  endif
#else
#  define CHAR_T char
#  define TEXT(X) X
#  define MAIN main
#  define FPRINTF fprintf
#  define FPUTS fputs
#  define FOPEN fopen
#  define STRCASECMP strcasecmp
#  define TOLOWER(X) tolower((unsigned char)(X))
#  define FMT_TCHAR "%s"
#  define FMT_HCHAR "%s"
#endif

/* CPU arch */
#if defined(__x86_64__) || defined(__amd64__) || defined(_M_X64)
#  define CPU_ARCH TEXT("x64")
#elif defined(__i386__) || defined(_X86_) || defined(_M_IX86) || defined(__IA32__)
#  define CPU_ARCH TEXT("x86")
#elif defined(__aarch64__) || defined(_M_ARM64)
#  define CPU_ARCH TEXT("arm64")
#elif defined(__arm__) && defined(__ARM_FP)
#  define CPU_ARCH TEXT("armhf")
#elif defined(__arm__) && defined(__ARMEL__)
#  define CPU_ARCH TEXT("armel")
#elif defined(__arm__) || defined(_M_ARM)
#  define CPU_ARCH TEXT("arm")
#elif defined(__mips__) && defined(_MIPS_ARCH_MIPS64R6) && defined(_MIPSEL)
#  define CPU_ARCH TEXT("mips64r6el")
#elif defined(__mips__) && defined(_MIPS_ARCH_MIPS64R6)
#  define CPU_ARCH TEXT("mips64r6")
#elif defined(__mips__) && defined(_MIPS_ARCH_MIPS32R6) && defined(_MIPSEL)
#  define CPU_ARCH TEXT("mipsr6el")
#elif defined(__mips__) && defined(_MIPS_ARCH_MIPS32R6)
#  define CPU_ARCH TEXT("mipsr6")
#elif defined(__mips__) && (defined(__mips64) || (__mips == 64)) && defined(_MIPSEL)
#  define CPU_ARCH TEXT("mips64el")
#elif defined(__mips__) && (defined(__mips64) || (__mips == 64))
#  define CPU_ARCH TEXT("mips64")
#elif (defined(__mips__) || defined(_mips) || defined(__mips)) && defined(_MIPSEL)
#  define CPU_ARCH TEXT("mipsel")
#elif defined(__mips__) || defined(_mips) || defined(__mips)
#  define CPU_ARCH TEXT("mips")
#elif defined(__powerpc64__) || defined(__PPC64__) || defined(_ARCH_PPC64)
#  define CPU_ARCH TEXT("ppc64")
#elif defined(__powerpc__) || defined(__PPC__) || defined(_ARCH_PPC)
#  define CPU_ARCH TEXT("ppc")
#elif defined(__s390x__) || defined(__zarch__)
#  define CPU_ARCH TEXT("s390x")
#elif defined(__riscv) && (__riscv_xlen == 64)
#  define CPU_ARCH TEXT("riscv64")
#else
#  error Unsupported CPU architecture encountered!
#endif

/* OS family */
#if defined(__linux__)
#  define OS_NAME TEXT("linux")
#elif defined(__gnu_hurd__)
#  define OS_NAME TEXT("hurd")
#elif defined(__FreeBSD__)
#  define OS_NAME TEXT("freebsd")
#elif defined(__OpenBSD__)
#  define OS_NAME TEXT("openbsd")
#elif defined(__NetBSD__)
#  define OS_NAME TEXT("netbsd")
#elif defined(__DragonFly__)
#  define OS_NAME TEXT("dragonfly")
#elif defined(__illumos__)
#  define OS_NAME TEXT("illumos")
#elif defined(__sun) && defined(__svr4__)
#  define OS_NAME TEXT("solaris")
#elif defined(__QNX__)
#  define OS_NAME TEXT("qnx")
#elif defined(__APPLE__) && defined(__MACH__)
#  define OS_NAME TEXT("darwin")
#elif defined(__HAIKU__)
#  define OS_NAME TEXT("haiku")
#elif defined(__CYGWIN__)
#  define OS_NAME TEXT("cygwin")
#elif defined(_WIN32)
#  define OS_NAME TEXT("windows")
#elif defined(__unix__)
#  define OS_NAME TEXT("unix")
#else
#  error Unsupported operating system encountered!
#endif

/* Compiler */
#if defined(__INTEL_LLVM_COMPILER)
#  define COMPILER_FMT TEXT("Intel C++ Compiler v%u.%u.%u")
#  define COMPILER_ARG __INTEL_LLVM_COMPILER / 10000U, (__INTEL_LLVM_COMPILER % 10000U) / 100U, __INTEL_LLVM_COMPILER % 100U
#elif defined(__clang__)
#  define COMPILER_FMT TEXT("Clang v%u.%u.%u")
#  define COMPILER_ARG __clang_major__, __clang_minor__, __clang_patchlevel__
#elif defined(__GNUC__)
#  define COMPILER_FMT TEXT("GCC v%u.%u.%u")
#  define COMPILER_ARG __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__
#elif defined(_MSC_VER)
#  define COMPILER_FMT TEXT("MSVC v%u.%02u.%u")
#  define COMPILER_ARG _MSC_VER / 100U, _MSC_VER % 100U, _MSC_FULL_VER % 100000U
#else
#  error Unsupported compiler encountered!
#endif

/* fstat() support */
#ifdef _WIN32
#  define FSTAT(X,Y) _fstat64(_fileno(X),(Y))
#  define STAT_STRUCT _stat64
#  define FILE_IS_DIR(X) (((X) & _S_IFMT) == _S_IFDIR)
#  define FILE_IS_REG(X) (((X) & _S_IFMT) == _S_IFREG)
#else
#  define FSTAT(X,Y) fstat(fileno(X),(Y))
#  define STAT_STRUCT stat
#  define FILE_IS_DIR(X) S_ISDIR((X))
#  define FILE_IS_REG(X) S_ISREG((X))
#endif

/* assert file size */
#define STATIC_ASSERT(X,Y) typedef char _static_assertion_##X[(Y) ? 1 : (-1)]
STATIC_ASSERT(file_size, sizeof(((struct STAT_STRUCT*)NULL)->st_size) >= 8U);

/* setmode() support */
#ifdef _WIN32
#  define FMODE_BIN _O_BINARY
#  define FMODE_TXT _O_U8TEXT
#  define SETMODE(X,Y) _setmode(_fileno((X)), (Y))
#else
#  define FMODE_BIN 0x1
#  define FMODE_TXT 0x2
#  define SETMODE(X,Y) ((void)0)
#endif

/* isatty() support */
#ifdef _WIN32
#  define ISATTY(X) _isatty(_fileno((X)))
#else
#  define ISATTY(X) isatty(fileno((X)))
#endif

/* executable name */
#ifdef _WIN32
#  define EXE_SUFFIX TEXT(".exe")
#else
#  define EXE_SUFFIX
#endif

#endif /* _INC_NUHASH_PLATFORM_H */
