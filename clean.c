#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SAFE_PERSON(p)                                                                             \
    struct PERSON* p __attribute__((cleanup(cleanPerson))) = malloc(sizeof(struct PERSON));        \
    if (p)                                                                                         \
    {                                                                                              \
        p->name = (char*)malloc(sizeof(char) * 7);                                                 \
        if (!p->name)                                                                              \
        {                                                                                          \
            puts("Out of memory");                                                                 \
            free(p);                                                                               \
            exit(-2);                                                                              \
        }                                                                                          \
    }                                                                                              \
    else                                                                                           \
    {                                                                                              \
        {                                                                                          \
            puts("Out of memory");                                                                 \
            exit(-2);                                                                              \
        }                                                                                          \
    }

struct PERSON
{
    char* name;
    int age;
};

void cleanPerson(struct PERSON** p)
{
    if (*p && (*p)->name)
    {
        printf("%12s %p %p\n", __func__, *p, (*p)->name);
        free((*p)->name);
        free(*p);
    }
}

void make()
{
    SAFE_PERSON(p);
    printf("%12s %p %p\n", __func__, p, p->name);
    strncpy(p->name, "hello", 6);
    p->name[6] = '\0';
    puts(p->name);
}

int main()
{
    puts(__func__);
    make();
    puts(__func__);
}
