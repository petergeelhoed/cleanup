#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NAME_SIZE 6

struct PERSON
{
    char* name;
    char* destructor;
};
void cleanPerson(struct PERSON** p)
{
    if (p && *p)
    {
        if ((*p)->name)
        {
            printf("%12s %p %p\n", __func__, *p, (*p)->name);
            free((*p)->name);
        }
        free(*p);
        *p = NULL; // Prevent dangling pointer
    }
}

struct PERSON* allocPerson()
{
    struct PERSON* p = malloc(sizeof(struct PERSON));
    if (!p)
        return NULL;
    p->name = malloc(NAME_SIZE);
    if (!p->name)
    {
        free(p);
        return NULL;
    }
    return p;
}

void make()
{
    struct PERSON* p __attribute__((cleanup(cleanPerson))) = allocPerson();
    if (!p || !p->name)
    {
        fprintf(stderr, "Out of memory\n");
        exit(-2);
    }
    strncpy(p->name, "hello", NAME_SIZE);
    p->name[NAME_SIZE - 1] = '\0';
    puts(p->name);
}
int main()
{
    puts(__func__);
    make();
    puts(__func__);
}
