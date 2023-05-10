/******************************************************************************/
/* NuHash, by LoRd_MuldeR <MuldeR2@GMX.de>                                    */
/* This work has been released under the CC0 1.0 Universal license!           */
/******************************************************************************/

#include <stdint.h>

#ifdef _WIN32
#  define WIN32_LEAN_AND_MEAN 1
#  include <Windows.h>
#else
#  include <unistd.h>
#  include <time.h>
#  include <signal.h>
#endif

#define UNUSED(X) ((void)(X))
#define Q64(X) ((uint64_t)(X).QuadPart)

volatile int g_aborted = 0;

#ifdef _WIN32
static LARGE_INTEGER s_frequency;
#endif

static void sig_handler(const int signal_no)
{
	UNUSED(signal_no);
	g_aborted = 1;
}

#ifdef _WIN32
static BOOL WINAPI console_ctrl_handler(const DWORD type)
{
	switch (type)
	{
	case CTRL_C_EVENT:
	case CTRL_BREAK_EVENT:
	case CTRL_CLOSE_EVENT:
		sig_handler(2);
		return TRUE;
	default:
		return FALSE;
	}
}
#endif

void initialize_process(void)
{
#if defined(_WIN32)
	SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX);
	QueryPerformanceFrequency(&s_frequency);
	SetConsoleCtrlHandler(console_ctrl_handler, TRUE);
#else
	struct sigaction action;
	action.sa_handler = sig_handler;
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0;
	sigaction(SIGINT, &action, NULL);
#endif
}

uint64_t get_mtime(void)
{
#if defined(_WIN32)
	LARGE_INTEGER counter;
	QueryPerformanceCounter(&counter);
	return ((Q64(counter) / Q64(s_frequency)) * UINT64_C(1000)) + (((Q64(counter) % Q64(s_frequency)) * UINT64_C(1000)) / Q64(s_frequency));
#else
	struct timespec time_stamp;
	clock_gettime(CLOCK_MONOTONIC, &time_stamp);
	return ((uint64_t)time_stamp.tv_sec * UINT64_C(1000)) + ((uint64_t)time_stamp.tv_nsec / UINT64_C(1000000));
#endif
}
