#ifdef DEBUG
#include "bt.h"
#endif

void cause_segfault2()
{
    int* ptr = 0;
    *ptr = 0;
}
void cause_segfault1(const char* /*a*/,
                     const char* /*b*/,
                     const char* /*c*/,
                     const char* /*d*/)
{
    cause_segfault2();
}

int main(int argc, char** argv)
{
#ifdef DEBUG
    setup_crash_handler(argv[0]);
#endif
    const char* qwertyuiopasdfghjkl;
    const char* qwertyuiopasdfghjk;
    const char* qwertyuiopasdfghj;
    const char* qwertyuiopasdfgh;

    cause_segfault1(qwertyuiopasdfghjkl,
                    qwertyuiopasdfghjk,
                    qwertyuiopasdfghj,
                    qwertyuiopasdfgh);
    return 0;
}
