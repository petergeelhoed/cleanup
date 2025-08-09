#pragma once
#ifdef DEBUG
#include <backtrace.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_FRAMES 64

static struct backtrace_state* state = NULL;

static void error_callback(void* data, const char* msg, int errnum)
{
    fprintf(stderr, "libbacktrace error: %s (%d)\n", msg, errnum);
}

static int
full_callback(void* data, uintptr_t pc, const char* filename, int lineno, const char* function)
{
    if (filename || function || lineno != 0)
    {
        fprintf(stderr, "  %s:%d: %s\n", filename, lineno, function);
    }
    return 0;
}

static void segfault_handler(int sig, siginfo_t* info, void* ucontext)
{
    fprintf(stderr, "Caught signal %d (Segmentation Fault)\n", sig);
    fprintf(stderr, "Backtrace:\n");

    backtrace_full(state, 0, full_callback, error_callback, NULL);
    exit(EXIT_FAILURE);
}

void setup_segfault_handler(const char* argv0)
{
    state = backtrace_create_state(argv0, 1, error_callback, NULL);

    struct sigaction sa;
    sa.sa_sigaction = segfault_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_SIGINFO;

    if (sigaction(SIGSEGV, &sa, NULL) == -1)
    {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }
}

#endif
