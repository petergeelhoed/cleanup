#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct PERSON
{
    char* name;
    int age;
};

void cleanPerson(struct PERSON** p)
{
    printf("%s %p\n", __func__, (*p)->name);
    free((*p)->name);
    free(*p);
}

void make()
{
    struct PERSON* p __attribute__((cleanup(cleanPerson))) = malloc(sizeof(struct PERSON));
    p->name = (char*)malloc(sizeof(char) * 7);
    printf("%s %p\n", __func__, p->name);
    strncpy(p->name, "hello", 6);
    puts(p->name);
}

int main()
{
    puts(__func__);
    make();
    puts(__func__);
}
