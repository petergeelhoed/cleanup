#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#include "bt.h"

int cause_buserror()
{
    const char* filename = "bus_trigger.txt";

    // Create and write to file
    int fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0666);
    if (fd == -1)
    {
        perror("open");
        return 1;
    }

    // Write 1 page of data
    size_t page_size = getpagesize();
    ftruncate(fd, page_size);
    write(fd, "Hello", 5);

    // Map the file
    char* map = mmap(NULL, page_size, PROT_READ, MAP_SHARED, fd, 0);
    if (map == MAP_FAILED)
    {
        perror("mmap");
        close(fd);
        return 1;
    }

    // Truncate the file to 0 bytes
    ftruncate(fd, 0);
    close(fd);

    // Access the now-invalid memory
    char c = map[0]; // Should trigger SIGBUS

    printf("Read character: %c\n", c);
    return 0;
}

int main(int argc, char** argv)
{
#ifdef DEBUG
    setup_crash_handler(argv[0]);
#endif

    return cause_buserror();
}
