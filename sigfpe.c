#include "bt.h"

#include <signal.h>

int main(int argc, char** argv)
{
#ifdef DEBUG
    setup_crash_handler(argv[0]);
#endif

    raise(SIGFPE);
    return 0;
}
