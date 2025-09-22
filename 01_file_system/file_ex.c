#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(void) {
    printf("Posix File System Operations\n");

    /* Open a file with Read-Write access */
    int fd = open("example.txt", O_RDWR);
    if (fd == -1) {
        perror("Error opening file");
        return 1;
    }

    // read
    char buffer[100];
    ssize_t bytesRead = read(fd, buffer, sizeof(buffer) - 1);
    if (bytesRead == -1) {
        perror("Error reading file");
        close(fd);
        return 1;
    }

    // write
    char *data = "Hello, World!\n";
    ssize_t bytesWritten = write(fd, data, 14);
    if (bytesWritten == -1) {
        perror("Error writing to file");
        close(fd);
        return 1;
    }

    // lseek 
    off_t offset = lseek(fd, 0, SEEK_SET);
    if (offset == (off_t)-1) {
        perror("Error seeking in file");
        close(fd);
        return 1;
    }

    // close
    if (close(fd) == -1) {
        perror("Error closing file");
        return 1;
    }
    return 0;
}