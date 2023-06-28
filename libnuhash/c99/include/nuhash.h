/******************************************************************************/
/* NuHash, by LoRd_MuldeR <MuldeR2@GMX.de>                                    */
/* This work has been released under the CC0 1.0 Universal license!           */
/******************************************************************************/

#ifndef _INC_LIBNUHASH_H
#define _INC_LIBNUHASH_H

#ifndef __cplusplus
#  include <stdlib.h>
#  include <stdint.h>
#else
#  include <cstdlib>
#  include <cstdint>
#endif

#define NUHASH_WORDS 6U
#define NUHASH_BYTES (sizeof(uint64_t) * NUHASH_WORDS)
#define NUHASH_CHARS ((2U * NUHASH_BYTES) + 1U)

/* Must be defined to build/use DLL */
#ifdef LIBNUHASH_DLL
#  ifdef LIBNUHASH_EXPORTS
#    define NUHASH_API __declspec(dllexport)
#  else
#    define NUHASH_API __declspec(dllimport)
#  endif
#else
#  define NUHASH_API
#endif

/* ------------------------------------------------------------------------ */
/* C99 API                                                                  */
/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	uint64_t hash[NUHASH_WORDS];
}
nuhash_t;

/* Stream API */
NUHASH_API void nuhash_init(nuhash_t *const ctx);
NUHASH_API void nuhash_init_with_key(nuhash_t *const ctx, const uint8_t *const key, const size_t len);
NUHASH_API void nuhash_update(nuhash_t *const ctx, const uint8_t *const src, const size_t len);
NUHASH_API uint8_t *nuhash_final(nuhash_t *const ctx, uint8_t *const out);

/* Simple API */
NUHASH_API uint8_t *nuhash_compute(const uint8_t *const src, const size_t len, uint8_t *const out);
NUHASH_API uint8_t *nuhash_compute_with_key(const uint8_t *const key, const size_t key_len, const uint8_t *const src, const size_t in_len, uint8_t *const out);

/* Utilities */
NUHASH_API char *nuhash_tohex(const uint8_t *const hash, const int upper, char *const out);
NUHASH_API char *nuhash_version(uint16_t version[3U], char *const build);

#ifdef __cplusplus
}
#endif

/* ------------------------------------------------------------------------ */
/* C++ API                                                                  */
/* ------------------------------------------------------------------------ */

#if defined(__cplusplus) && !defined(NUHASH_NO_CPLUSPLUS)

#include <vector>
#include <array>
#include <string>

class NuHash
{
public:
	NuHash(void) { reset(); }
	NuHash(const std::vector<uint8_t> &key) { reset(key); }
	void update(const uint8_t *const src, const size_t len) { ::nuhash_update(&m_ctx, src, len); }
	void update(const uint8_t src) { ::nuhash_update(&m_ctx, &src, sizeof(uint8_t)); }
	void update(const std::vector<uint8_t> &src) { ::nuhash_update(&m_ctx, src.data(), src.size()); }
	void update(const std::string &src) { ::nuhash_update(&m_ctx, reinterpret_cast<const uint8_t*>(src.c_str()), src.size()); }
	std::string do_final(const bool uppercase = false);
	void do_final(std::array <uint8_t, NUHASH_BYTES> &out) { ::nuhash_final(&m_ctx, out.data()); }
	void reset(void) { ::nuhash_init(&m_ctx); }
	void reset(const std::vector<uint8_t> &key) { ::nuhash_init_with_key(&m_ctx, key.data(), key.size()); }
	static std::string version(std::array <uint16_t, 3U> &version);
private:
	nuhash_t m_ctx;
	NuHash(const NuHash&) { std::abort(); }
	NuHash &operator=(const NuHash&) { std::abort(); }
};

std::string NuHash::do_final(const bool uppercase)
{
	std::array<uint8_t, NUHASH_BYTES> digest;
	std::array<char, NUHASH_CHARS> hex;
	return std::string(::nuhash_tohex(::nuhash_final(&m_ctx, digest.data()), uppercase, hex.data()));
}

std::string NuHash::version(std::array <uint16_t, 3U> &version)
{
	std::array<char, 12U> build;
	return std::string(::nuhash_version(version.data(), build.data()));
}

#endif

#endif /*_INC_LIBNUHASH_H*/
