#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void clean(char** p)
{
    puts(__func__);
    free(*p);
}

void make()
{
    puts(__func__);
    char* c __attribute__((cleanup(clean))) = malloc(sizeof(char) * 7);
    strncpy(c, "hello", 6);
    puts(c);
}

int main()
{
    puts(__func__);
    make();
    puts(__func__);
}
