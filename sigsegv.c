#ifdef DEBUG
#include "bt.h"
#endif

void cause_segfault2()
{
    int* ptr = 0;
    *ptr = 0;
}
void cause_segfault1() { cause_segfault2(); }

int main(int argc, char** argv)
{
#ifdef DEBUG
    setup_crash_handler(argv[0]);
#endif

    cause_segfault1();
    return 0;
}
