/* This adds the backtrace after a crash
   simply include this header and at the start of main add:
       setup_crash_handler(argv[0]);

   compile with -g -lbacktrace -DDEBUG
*/

#pragma once
#ifdef DEBUG
#include <backtrace.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_FRAMES 64

static struct backtrace_state* state = NULL;

const char* signal_name(int sig)
{
    switch (sig)
    {
    case SIGSEGV:
        return "SIGSEGV";
    case SIGABRT:
        return "SIGABRT";
    case SIGFPE:
        return "SIGFPE";
    case SIGILL:
        return "SIGILL";
    case SIGBUS:
        return "SIGBUS";
    default:
        return "UNHANDLED SIGNAL";
    }
}

static void error_callback(void* data, const char* msg, int errnum)
{
    fprintf(stderr, "libbacktrace error: %s (%d)\n", msg, errnum);
}

static int full_callback(void* data,
                         uintptr_t pc,
                         const char* filename,
                         int lineno,
                         const char* function)
{
    if (filename || function || lineno != 0)
    {
        fprintf(stderr, "  %s:%d: %s\n", filename, lineno, function);
    }
    return 0;
}

static void fault_handler(int sig, siginfo_t* info, void* ucontext)
{
    fprintf(stderr, "Caught signal %d (%s)\n", sig, signal_name(sig));
    fprintf(stderr, "Backtrace:\n");

    backtrace_full(state, 0, full_callback, error_callback, NULL);
    exit(EXIT_FAILURE);
}

void setup_crash_handler(const char* argv0)
{
    state = backtrace_create_state(argv0, 1, error_callback, NULL);

    struct sigaction sa;
    sa.sa_sigaction = fault_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_SIGINFO;

    int signals[] = {SIGSEGV, SIGABRT, SIGFPE, SIGILL, SIGBUS};
    for (int i = 0; i < sizeof(signals) / sizeof(signals[0]); ++i)
    {
        if (sigaction(signals[i], &sa, NULL) == -1)
        {
            fprintf(
                stderr, "Failed to set handler for signal %d\n", signals[i]);
        }
    }
}
#endif
