#ifdef DEBUG
#include "bt.h"
#endif

void cause_segfault()
{
    int* ptr = 0;
    *ptr = 0;
}
int main(int argc, char** argv)
{
#ifdef DEBUG
    setup_crash_handler(argv[0]);
#endif

    cause_segfault();
    return 0;
}
