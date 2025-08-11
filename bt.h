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

const char* bt_signal_name(int sig)
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

static void bt_error_callback(void* data, const char* msg, int errnum)
{
    fprintf(stderr, "libbacktrace error: %s (%d)\n", msg, errnum);
}

static int bt_context_lines = 3;

static int bt_full_callback(void* data,
                            uintptr_t pc,
                            const char* filename,
                            int lineno,
                            const char* function_name)
{
    if (filename || function_name || lineno != 0)
    {
        fprintf(stderr, "%s:%d: %s()\n", filename, lineno, function_name);

        if (filename && lineno > 0)
        {
            FILE* file = fopen(filename, "r");
            if (file)
            {
                char line[1024];
                int current_line = 1;
                while (fgets(line, sizeof(line), file))
                {
                    if (current_line >= lineno - bt_context_lines)
                    {
                        if (bt_context_lines > 0 && current_line == lineno)
                        {
                            fprintf(stderr,
                                    "%4d: \033[31m%s\033[0m",
                                    current_line,
                                    line);
                        }
                        else
                        {
                            fprintf(stderr, "%4d: %s", current_line, line);
                        }
                    }
                    if (current_line == lineno)
                    {
                        break;
                    }

                    current_line++;
                }
                bt_context_lines = 0;
                fclose(file);
            }
        }
    }
    return 0;
}

static void bt_fault_handler(int sig, siginfo_t* info, void* ucontext)
{
    fprintf(stderr, "Caught signal %d (%s)\n", sig, bt_signal_name(sig));
    fprintf(stderr, "Backtrace:\n");

    // skip 1 to not list the fault handler
    backtrace_full(state, 1, bt_full_callback, bt_error_callback, NULL);
    exit(EXIT_FAILURE);
}

void setup_crash_handler(const char* argv0)
{
    state = backtrace_create_state(argv0, 1, bt_error_callback, NULL);

    struct sigaction sa;
    sa.sa_sigaction = bt_fault_handler;
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
