#include <stdio.h>
#include <stdlib.h>
void clean(char** p)
{
    puts(__func__);
    free(*p);
}
char* make()
{
    puts(__func__);
    char* c __attribute__((cleanup(clean))) = malloc(sizeof(char) * 7);
    puts(__func__);
    return c;
}
int main()
{
    puts(__func__);
    char* y = make();
    puts(__func__);
}
