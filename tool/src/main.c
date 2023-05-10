/******************************************************************************/
/* NuHash, by LoRd_MuldeR <MuldeR2@GMX.de>                                    */
/* This work has been released under the CC0 1.0 Universal license!           */
/******************************************************************************/

/* libnuhash*/
#include <nuhash.h>

/* Platfrom */
#include "platform.h"

/* wildcard support */
#ifdef __MINGW32__
const int _dowildcard = 1;
#endif

/* array length */
#define COUNT_OF(ARRAY) (sizeof(ARRAY) / sizeof(ARRAY[0]))

/* I/O buffer */
#define BUFFSIZE 4096U

/* update interval */
#define UPDATE_INTERVAL 997U

/* ------------------------------------------------------------------------ */
/* Print digest                                                             */
/* ------------------------------------------------------------------------ */

#define PRINTF_DIGEST(FORMAT, ...) do \
{ \
	if (!binary_mode) \
	{ \
		FPRINTF(stdout, FMT_HCHAR TEXT(" ") FORMAT TEXT("\n"), nuhash_tohex(digest, upper_case, hexstr), __VA_ARGS__); \
	} \
	else \
	{ \
		fwrite(digest, sizeof(uint8_t), NUHASH_BYTES, stdout); \
	} \
	fflush(stdout); \
} \
while (0)

/* ------------------------------------------------------------------------ */
/* Print progress                                                           */
/* ------------------------------------------------------------------------ */

#define SUFFIX(FLAG, STR1, STR2) ((FLAG) ? (STR1 STR2) : (STR1))
#define PREFIX(FLAG, STR1, STR2) ((FLAG) ? (STR1 STR2) : (STR2))

static void print_progress(const uint64_t total, const uint64_t current, const int finished)
{
	if (total > 0U)
	{
		FPRINTF(stderr, SUFFIX(finished, TEXT("\r%") TEXT(PRIu64) TEXT("/%") TEXT(PRIu64) TEXT(" bytes hashed... [%.1f%%]"), TEXT("\n")), current, total, 100.0 * (current / (double)total));
	}
	else
	{
		FPRINTF(stderr, SUFFIX(finished, TEXT("\r%") TEXT(PRIu64) TEXT(" bytes hashed..."), TEXT(" done\n")), current);
	}
	fflush(stderr);
}

/* ------------------------------------------------------------------------ */
/* Self-test                                                                */
/* ------------------------------------------------------------------------ */

typedef struct
{
	size_t iterations;
	const char* message;
}
testcase_t;

static const testcase_t TEST_VECTOR[] =
{
	{ 0x0000001, "" },
	{ 0x0000001, "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq" },
	{ 0x0000001, "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu" },
	{ 0x00F4240, "a" },
	{ 0x1000000, "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmno" },
	{ 0x1000000, "g4tB,=[TU%/|8lv_c5Y?F9Sqs1PnV#bNO6DW\\0em~p<ifJhM(2&Qo}`!-*:jrZ{KGX]$zkuLd+)xwC^.\"@AIHR;y'a>3E7" },
	{ 0x1000000, "\xA4\x2C\x55\xA5\xB0\x5E\xD8\x33\xF3\x18\xFB\xA7\x20\xC7\x34\xD7\x1D\x2D\x46\x8D\xCA\x8A\xE7\xFC\x68\x5B\x54\xDA\xF9\xC3\x76\x62\xA9\x3D\x39\xBF\x17\xB7\xB2\x8C\x10\xA2\x1F\xE3\x74\x6A\x4A\x6C\x7A\x92\xD1\x52\xEA\x88\xBA\x6B\xD6\x64\x96\x9E\xE6\x4C\x47\x42\xC5\x51\xF2\x50\xF4\x7B\x4F\xE9\x87\x6D\x94\x4B\x23\xF0\x45\x1A\x9A\xE4\x91\x0F\x3B\xB4\x71\x82\x99\xC9\x49\xE5\xBE\x28\x93\x98\x2A\xD9\x73\x65\x58\xC0\x83\x48\x8F\x03\x14\x40\x1B\x24\x1E\xA6\x3F\xF7\x67\xAB\x37\x06\xAC\xF6\x66\xB6\xDD\x1C\x0A\x35\x9F\x56\x15\xEC\xF1\x2B\x7C\x57\x5D\x02\xED\xA8\x63\x16\x75\xDC\x9B\x5C\x7D\x0D\x5F\xFD\x05\xA0\x6E\x90\x95\x85\xD2\x13\x61\x08\xA3\x01\xC8\xD3\x84\xAA\x09\xF5\x70\x6F\xDE\xB9\x04\xB5\xDB\x30\xBC\x07\x8E\xFE\x89\x60\x26\x4E\x59\xCB\x36\x86\x11\x72\xE2\xC6\xAF\x3A\x8B\x7F\xFA\x41\x38\xDF\xEE\x2F\x32\xA1\x27\x69\x31\x29\x43\xC4\x9C\x77\x21\x44\xB8\x79\xD0\xAD\x97\xE0\xE1\x0E\x19\x2E\x22\xBD\x80\xE8\xB1\xF8\x4D\x3E\xC2\x7E\x0C\xB3\x81\x12\xCC\xC1\x53\xCF\x78\xCE\x3C\xD5\x5A\xBB\x9D\xEB\xEF\xCD\xFF\x0B\x25\xAE\xD4" },
	{ 0x1000000, "\xB5\xC8\x9B\xD1\xBA\x66\x29\x19\xD2\x30\xED\x26\x5B\xC4\x91\x4D\x14\xF7\x16\x35\x5A\x69\xFE\x99\xA5\x49\xF1\x94\x2B\xBC\xDC\x4C\xDD\x4F\xCA\x1B\x88\x9C\xA9\xF6\x73\x2F\xFD\x3F\x40\x39\x13\xDA\xB3\x71\xF4\xDE\x7F\x02\x3D\x64\x78\x7A\x17\x0A\x93\xDF\xEB\x9F\x07\x04\x79\x8C\x98\xB8\x28\x80\xAD\x1F\xF2\x96\x6B\xBD\x45\xD9\xFA\x2C\x56\xCE\xF0\x87\x23\x22\xEA\xC5\x27\xD3\x63\xAF\x24\xC7\xB7\x06\xE4\x75\x83\x7B\xBF\x2E\xA4\xEF\xF5\x12\x5D\x60\x95\xA7\x53\x11\x74\xB4\xA0\x9E\xE3\xAE\xBB\x55\x72\x59\xA6\x9D\xD5\x89\x6D\x8F\x4A\x0C\x1D\xCD\x2A\xFB\xEE\xD7\xC3\xF3\x25\x62\x8E\xB0\xB2\xC2\x38\x3C\x6A\x6F\x4E\x3B\x08\xD4\x86\x36\x18\x5C\xE0\x1E\x81\x77\xF8\x68\xDB\x58\x54\xFF\x97\x7E\x61\xC0\x51\xC9\x6C\xE5\x82\xBE\xCF\x32\x43\x01\x3A\x5E\x50\x47\xA1\x34\xCC\x0D\x70\xAC\xC6\x44\x9A\x3E\x10\x0E\xEC\x03\xE2\x6E\xAA\xFC\x7D\x37\x31\x4B\xE6\xB6\xC1\x15\x8B\x05\x85\x8A\x67\xF9\xAB\x8D\xE1\xA2\x48\x1C\xCB\x52\x65\x76\x0F\x1A\x21\xA8\xA3\x42\x41\x90\x92\xD0\x84\x09\x2D\x33\x5F\xD8\x7C\xB1\xB9\x57\xE8\xD6\x0B\x20\xE7\x46\xE9" }
};

static const char *const EXPECTED[] =
{
	"d7964fe1bec2b5ecf21ecc88c86ce4f1e89fb1ef3669d52e34eb049d7fd6c42d4b2bbeeeb70d12c3fcaf43dd2229abc9",
	"b3f13f534034ae8d645d410e8828673761fe2d697218879f9ea528d976ba2e15a3f7485105e612b9a46cb3988635a70f",
	"63703476f4ffce4a8c758d0d3b91a857b9a2e3433ee864f931ba328fa7247b1b0ac9c2e0279243b83051aafac6a7e710",
	"c6808aeb911c8837cee2e6867e589ab28526a8153494cf35a40c4bd6e20312da33cac4a390cd3b51a2eb4226e60b53e1",
	"bcca6bc8d113dd3fc171d743ac7ad2d3e63163ea7107d3c78517cf5b53ee0b2f4a0fe079e6a74df350c667b7cfe3cf09",
	"56770e133b9eb4205f11824ad7280f5c31bc04dd333d1844b3589ab8b94ab95dc09b77be9a8128d7f155fa73de617597",
	"9a8579bb88112b2f3285781a61348d7b04a5632fec12a73a7e0e367a556794a24a32ca7bb14da68eaed83e99c2f8f4c0",
	"fbf4444b0329f508bfc6cf44920b311a718e5f4c4c1af978e78223ec3579365ff5d15c16bb5a33d69cb36f3d40564757"
};

static int self_test(const int upper_case, const int ignore_int, const int show_progress)
{
	int result;
	size_t len, index, round, errors = 0U;
	uint64_t total_size, bytes_done, tm_curr = 0U, tm_next = 0U;
	uint_fast16_t spinner = 0U;
	nuhash_t context;
	uint8_t digest[NUHASH_BYTES];
	char hex[NUHASH_CHARS];

	for (index = 0U; index < COUNT_OF(TEST_VECTOR); ++index)
	{
		len = strlen(TEST_VECTOR[index].message);
		total_size = (uint64_t)len * (uint64_t)TEST_VECTOR[index].iterations;
		bytes_done = 0U;

		if (show_progress && index)
		{
			print_progress(total_size, bytes_done, 0);
			tm_next = get_mtime() + UPDATE_INTERVAL;
		}

		nuhash_init(&context);

		for (round = 0U; round < TEST_VECTOR[index].iterations; ++round)
		{
			nuhash_update(&context, (const uint8_t*)TEST_VECTOR[index].message, len);
			bytes_done += len;
			if ((!ignore_int) && g_aborted)
			{
				FPUTS(PREFIX(show_progress, TEXT("\n"), TEXT("The process has been interrupted!\n")), stderr);
				return (-1);
			}
			if (show_progress && index && (!(++spinner & 0x1F)) && ((tm_curr = get_mtime()) >= tm_next))
			{
				print_progress(total_size, bytes_done, 0);
				tm_next = tm_curr + UPDATE_INTERVAL;
			}
		}

		if (show_progress && index)
		{
			print_progress(total_size, bytes_done, 1);
			fflush(stderr);
		}

		if (!(result = !strcasecmp(nuhash_tohex(nuhash_final(&context, digest), upper_case, hex), EXPECTED[index])))
		{
			errors += 1U;
		}

		FPRINTF(stdout, FMT_HCHAR TEXT(" Test#%u - ") FMT_TCHAR TEXT("\n"), hex, (unsigned)(index + 1U), result ? TEXT("OK") : TEXT("Failed!"));
		fflush(stdout);
	}

	FPUTS(errors ? TEXT("Error: At least one test failed !!!\n") : TEXT("All tests completed successfully.\n"), stderr);
	return (!errors);
}

/* ------------------------------------------------------------------------ */
/* Stress-test                                                              */
/* ------------------------------------------------------------------------ */

#define RUN_STRESS_TEST(TYPE, FORMAT) do \
{ \
	TYPE value = 0; do \
	{ \
		nuhash_compute((uint8_t*)&value, sizeof(TYPE), digest); \
		PRINTF_DIGEST(FORMAT, value); \
		if ((!ignore_int) && g_aborted) { return (-1); } \
	} \
	while (++value); \
} \
while (0)

static int stress_test(const int upper_case, const int binary_mode, const int ignore_int)
{
	uint8_t digest[NUHASH_BYTES];
	char hexstr[NUHASH_CHARS];

	RUN_STRESS_TEST(uint8_t , TEXT("%02" ) TEXT(PRIX8 ));
	RUN_STRESS_TEST(uint16_t, TEXT("%04" ) TEXT(PRIX16));
	RUN_STRESS_TEST(uint32_t, TEXT("%08" ) TEXT(PRIX32));
	RUN_STRESS_TEST(uint64_t, TEXT("%016") TEXT(PRIX64));

	return 1;
}

/* ------------------------------------------------------------------------ */
/* Process File                                                             */
/* ------------------------------------------------------------------------ */

#define CHECK_INTERRUPTED(SHOW_PROGRESS) do \
{ \
	if ((!ignore_int) && g_aborted) \
	{ \
		FPUTS(PREFIX((SHOW_PROGRESS), TEXT("\n"), TEXT("The process has been interrupted!\n")), stderr); \
		result = (-1); \
		goto clean_up; \
	} \
} \
while (0)

static int process_file(FILE *const file, const CHAR_T *const name, const int line_mode, const int binary_mode, const int upper_case, const int ignore_int, const int show_progress)
{
	int result = 0;
	uint64_t input_size = 0U, bytes_done = 0U, tm_curr = 0U, tm_next = 0U;
	uint_fast16_t spinner = 0U;
	nuhash_t context;
	struct STAT_STRUCT file_stat;
	uint8_t buffer[BUFFSIZE], digest[NUHASH_BYTES];
	char hexstr[NUHASH_CHARS];

	if(!FSTAT(file, &file_stat))
	{
		if (FILE_IS_DIR(file_stat.st_mode))
		{
			FPRINTF(stderr, TEXT("Error: Input file \"") FMT_TCHAR TEXT("\" is a directory!\n"), name);
			goto clean_up;
		}
		if (FILE_IS_REG(file_stat.st_mode) && (file_stat.st_size > 0))
		{
			input_size = (uint64_t)file_stat.st_size;
		}
	}

	if (!line_mode)
	{
		if (show_progress)
		{
			print_progress(input_size, bytes_done, 0);
			tm_next = get_mtime() + UPDATE_INTERVAL;
		}

		nuhash_init(&context);

		while ((!feof(file)) && (!ferror(file)))
		{
			const size_t len = fread(buffer, sizeof(uint8_t), BUFFSIZE, file);
			if (len > 0U)
			{
				nuhash_update(&context, buffer, len);
				bytes_done += len;
				if (show_progress && (!(++spinner & 0x1F)) && ((tm_curr = get_mtime()) >= tm_next))
				{
					print_progress(input_size, bytes_done, 0);
					tm_next = tm_curr + UPDATE_INTERVAL;
				}
			}
			CHECK_INTERRUPTED(show_progress);
		}

		if (!ferror(file))
		{
			if (show_progress)
			{
				print_progress(input_size, bytes_done, 1);
			}
			nuhash_final(&context, digest);
			PRINTF_DIGEST(FMT_TCHAR, name);
		}
	}
	else
	{
		while (fgets((char*)buffer, BUFFSIZE, file))
		{
			size_t len = strlen((char*)buffer);
			while ((len > 0U) && ((((char*)buffer)[len - 1U] == '\n') || (((char*)buffer)[len - 1U] == '\r')))
			{
				buffer[--len] = 0x00;
			}
			if (len > 0U)
			{
				nuhash_compute(buffer, len, digest);
				PRINTF_DIGEST(FMT_HCHAR, buffer);
			}
			CHECK_INTERRUPTED(0);
		}
	}

	if (ferror(file))
	{
		FPUTS(PREFIX(show_progress && (!line_mode), TEXT("\n"), TEXT("Error: Read operation has failed!\n")), stderr); \
		goto clean_up;
	}

	result = 1;

clean_up:

	if ((file != stdin) && (file != stdout) && (file != stderr))
	{
		fclose(file);
	}

	return result;
}

/* ------------------------------------------------------------------------ */
/* Options                                                                  */
/* ------------------------------------------------------------------------ */

#define OPTION_BINARY     0x001
#define OPTION_HELP       0x002
#define OPTION_IGNORE     0x004
#define OPTION_LINE_MODE  0x008
#define OPTION_PROGRESS   0x010
#define OPTION_SELF_TEST  0x020
#define OPTION_STOP       0x040
#define OPTION_STRESS     0x080
#define OPTION_UPPERCASE  0x100
#define OPTION_VERSION    0x200

static const struct
{
	int option;
	const CHAR_T short_name, *long_name;
}
OPTIONS[] =
{
	{ OPTION_BINARY,    TEXT('b'), TEXT("binary")    },
	{ OPTION_HELP,      TEXT('h'), TEXT("help")      },
	{ OPTION_IGNORE,    TEXT('i'), TEXT("ignore")    },
	{ OPTION_LINE_MODE, TEXT('l'), TEXT("line-mode") },
	{ OPTION_PROGRESS,  TEXT('p'), TEXT("progress")  },
	{ OPTION_SELF_TEST, TEXT('t'), TEXT("self-test") },
	{ OPTION_STOP,      TEXT('s'), TEXT("stop")      },
	{ OPTION_STRESS,    (CHAR_T)0, TEXT("stress")    },
	{ OPTION_UPPERCASE, TEXT('u'), TEXT("uppercase") },
	{ OPTION_VERSION,   TEXT('v'), TEXT("version")   }
};

static int parse_option_short(int *const options, const CHAR_T *arg_str)
{
	while (*arg_str)
	{
		const CHAR_T c = TOLOWER(*(arg_str++));
		size_t pos;
		for (pos = 0U; pos < COUNT_OF(OPTIONS); ++pos)
		{
			if (c == OPTIONS[pos].short_name)
			{
				*options |= OPTIONS[pos].option;
				goto success;
			}
		}
		return 0;
		success:;
	}
	return 1;
}

static int parse_option_long(int* const options, const CHAR_T *const arg_str)
{
	size_t pos;
	for (pos = 0U; pos < COUNT_OF(OPTIONS); ++pos)
	{
		if (!STRCASECMP(arg_str, OPTIONS[pos].long_name))
		{
			*options |= OPTIONS[pos].option;
			return 1;
		}
	}
	return 0;
}

/* ------------------------------------------------------------------------ */
/* Help screen                                                              */
/* ------------------------------------------------------------------------ */

static void print_helpscreen(FILE *const stream, const int full_info)
{
	uint16_t version[3U];
	char build[12];
	nuhash_version(version, build);

	FPRINTF(stream, TEXT("nuhash ") OS_NAME TEXT("-") CPU_ARCH TEXT(" v%u.%u.%u [") FMT_HCHAR TEXT("]\n"), version[0U], version[1U], version[2U], build);
	FPRINTF(stream, TEXT("built on ") FMT_HCHAR TEXT(" at ") FMT_HCHAR TEXT(" with ") COMPILER_FMT TEXT("\n\n"), __DATE__, __TIME__, COMPILER_ARG);

	if (full_info)
	{
		FPUTS(TEXT("Usage:\n"), stream);
		FPUTS(TEXT("  nuhash") EXE_SUFFIX TEXT(" [options] [<file_1> [... <file_n>]]\n\n"), stream);
		FPUTS(TEXT("Options:\n"), stream);
		FPUTS(TEXT("  -b --binary     Output digest as binary, default is hex-string.\n"), stream);
		FPUTS(TEXT("  -h --help       Print the help screen and exit.\n"), stream);
		FPUTS(TEXT("  -i --ignore     Ignore interrupt signals.\n"), stream);
		FPUTS(TEXT("  -l --line-mode  Process the input file(s) as text, line by line.\n"), stream);
		FPUTS(TEXT("  -p --progress   Print progress, while computing the hash.\n"), stream);
		FPUTS(TEXT("  -s --stop       Stop on all errors, default is to keep going.\n"), stream);
		FPUTS(TEXT("  -t --self-test  Run the built-in self-test.\n"), stream);
		FPUTS(TEXT("  -u --uppercase  Print digest as uppercase, default is lowercase.\n"), stream);
		FPUTS(TEXT("  -v --version    Print version information and exit.\n\n"), stream);
	}
}

/* ------------------------------------------------------------------------ */
/* MAIN                                                                     */
/* ------------------------------------------------------------------------ */

int MAIN(const int argc, const CHAR_T *const argv[])
{
	int exit_code = EXIT_SUCCESS, index = 1, options = 0, nostd = 0, result = 0;

	initialize_process();
	SETMODE(stderr, FMODE_TXT);

	if ((argc > 1) && (!STRCASECMP(argv[1], TEXT("/?"))))
	{
		options |= OPTION_HELP;
		index = argc;
	}

	while ((index < argc) && (argv[index][0U] == TEXT('-')) && (argv[index][1U] != TEXT('\0')))
	{
		const CHAR_T *const arg_str = argv[index++];
		if (arg_str[1U] == TEXT('-'))
		{
			if (arg_str[2U] == TEXT('\0'))
			{
				nostd = 1;
				break;
			}
			else if (parse_option_long(&options, arg_str + 2U))
			{
				continue;
			}
		}
		else if (parse_option_short(&options, arg_str + 1U))
		{
			continue;
		}
		FPRINTF(stderr, TEXT("Error: Unknown option \"") FMT_TCHAR TEXT("\" encountered!\n"), arg_str);
		return EXIT_FAILURE;
	}

	if (options & (OPTION_HELP | OPTION_VERSION))
	{
		print_helpscreen(stderr, options & OPTION_HELP);
		return EXIT_SUCCESS;
	}

	if ((options & OPTION_BINARY) && ISATTY(stdout))
	{
		FPUTS(TEXT("Error: Binray output is not supported when writing to terminal!\n"), stderr);
		return EXIT_FAILURE;
	}

	if ((options & (OPTION_SELF_TEST | OPTION_STRESS)) && (index < argc))
	{
		FPUTS(TEXT("Warning: Excess command-line argument(s) will be ignored!\n"), stderr);
	}

	SETMODE(stdin, FMODE_BIN);
	SETMODE(stdout, (options & OPTION_BINARY) ? FMODE_BIN : FMODE_TXT);

	if (options & OPTION_SELF_TEST)
	{
		result = self_test(options & OPTION_UPPERCASE, options & OPTION_IGNORE, options & OPTION_PROGRESS);
		return (result < 1) ? EXIT_FAILURE : EXIT_SUCCESS;
	}

	if (options & OPTION_STRESS)
	{
		result = stress_test(options & OPTION_UPPERCASE, options & OPTION_BINARY, options & OPTION_IGNORE);
		return (result < 1) ? EXIT_FAILURE : EXIT_SUCCESS;
	}

	if (index < argc)
	{
		for (; index < argc; ++index)
		{
			FILE *const input = (nostd || STRCASECMP(argv[index], TEXT("-"))) ? FOPEN(argv[index], TEXT("rb")) : stdin;
			if (input)
			{
				result = process_file(input, argv[index], options & OPTION_LINE_MODE, options & OPTION_BINARY, options & OPTION_UPPERCASE, options & OPTION_IGNORE, options & OPTION_PROGRESS);
				if (result < 1)
				{
					exit_code = EXIT_FAILURE;
				}
			}
			else
			{
				FPRINTF(stderr, TEXT("Error: Failed to open file \"") FMT_TCHAR TEXT("\" for reading!\n"), argv[index]);
				exit_code = EXIT_FAILURE;
			}

			if ((result < 0) || ((exit_code != EXIT_SUCCESS) && (options & OPTION_STOP)))
			{
				break;
			}
		}
	}
	else if (!(options & (OPTION_SELF_TEST | OPTION_STRESS)))
	{
		result = process_file(stdin, TEXT("-"), options & OPTION_LINE_MODE, options & OPTION_BINARY, options & OPTION_UPPERCASE, options & OPTION_IGNORE, options & OPTION_PROGRESS);
		if (result < 1)
		{
			exit_code = EXIT_FAILURE;
		}
	}

	return exit_code;
}
