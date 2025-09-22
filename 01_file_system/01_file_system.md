# Linux File System

Linux file system is a hierarchical structure that organizes files and directories on a Linux operating system. It follows the Filesystem Hierarchy Standard (FHS) which defines the directory structure and directory contents in Unix-like operating systems.

In Linux, everything is treated as a file, including hardware devices and system resources. The file system is organized in a tree-like structure, starting from the root directory ("/") and branching out into various subdirectories.

***Why is this powerful?***

You can use the same commands (cat, echo, ls) to interact with your hard drive, check CPU temperature, read process information, or control hardware
It creates a unified, consistent interface for system interaction
Makes scripting and automation much easier.

Example: `cat /proc/cpuinfo` -> read CPU information like a text file.
Example: `echo 50 > /sys/class/backlight/.../brightness` -> controls screen brightness by writing to a "file".

## Preliminary of Files in Linux

In Linux, each file has the permission settings that determine who can read, write, or execute the file. The permissions are represented by a combination of letters and symbols.

- `r` (read): Permission to read the file.
- `w` (write): Permission to modify or delete the file.
- `x` (execute): Permission to execute the file as a program.

The permissions are divided into three categories:
- User (u): The owner of the file.
- Group (g): The group that the file belongs to.
- Others (o): All other users.

For example, the permission string `rwxr-xr--` can be broken down as follows:
- `rwx`: The owner has read, write, and execute permissions.
- `r-x`: The group has read and execute permissions.
- `r--`: Others have read permission only.

You can view the permissions of files and directories using the `ls -l` command. For example:
```
total 24
-rw-r--r-- 1 kskozgun kskozgun   345 Jul 10  2024 const_test.c
-rwxr-xr-x 1 kskozgun kskozgun 16040 Jul 10  2024 output
-rw-r--r-- 1 kskozgun kskozgun   883 Jul 10  2024 two_sum.c
```

Permission can be changed using the `chmod` command. For example, to give the owner execute permission on a file:

```chmod u+x filename``` or ```chmod 755 filename``` (where 7 = rwx, 5 = r-x, 5 = r-x).

## Linux File Types

In Linux, files can be of different types, each serving a specific purpose. The main file types are:

| File Type       | Description                                      | Example                          |
|-----------------|--------------------------------------------------|----------------------------------|
| Regular File    | Contains data, text, or program instructions.    | `document.txt`, `image.png`      |
| Directory       | A container that holds files and other directories.| `/home/user`, `/etc`             |
| Symbolic Link   | A pointer to another file or directory.          | `ln -s target link_name`            |
| Block Device    | Represents devices that transfer data in blocks. | `/dev/sda` (hard drives)  |
| Character Device| Represents devices that transfer data character by character.| `/dev/tty` (terminals) |
| FIFO (Named Pipe)| A special file that allows for inter-process communication.| `mkfifo my_pipe` |    
| Socket          | A special file used for network communication.   | `/var/run/docker.sock`           |   

## POSIX Standard for File System

The POSIX (Portable Operating System Interface) standard defines a set of operating system interfaces, including file system operations, to ensure compatibility and interoperability between different Unix-like operating systems. Some key POSIX file system operations include:

`open()`, `read()`, `write()`, `close()`, `lseek()`, `stat()`, `fstat()`, `mkdir()`, `rmdir()`, `unlink()`, and `rename()`.

In linux, all process file operations are done through file descriptors. A file descriptor is a non-negative integer that uniquely identifies an opened file within a process. Standard input, output, and error are represented by file descriptors 0, 1, and 2, respectively.

***Note:*** The C standard library provides higher-level functions for file operations, such as `fopen()`, `fclose()`, `fread()`, `fwrite()`, and `fprintf()`, which are built on top of these POSIX system calls. It provides buffering and formatted I/O capabilities. In linux, you might need more control like device files, non-blocking I/O, etc. In such cases, you would use the POSIX system calls directly.

### Open Files

`open()` function is used to open a file and returns a file descriptor, which is a non-negative integer that uniquely identifies the opened file within the process. The function takes two main arguments: the file path and the access mode (read, write, or both).

```int open(const char *pathname, int flags, ... /* mode */ );```

For example, to open a file for reading:

```c
#include <fnctl.h>

//...
int fd = open("example.txt", O_RDONLY);
if (fd == -1) {
    // Handle error
}
//...
```

Table For `flags`:

| Flag        | Description                          |Notes |
|-------------|--------------------------------------|------|
| O_RDONLY    | Open for reading only                |------|
| O_WRONLY    | Open for writing only                |------|
| O_RDWR      | Open for reading and writing         |------|
| O_CREAT     | Create the file if it does not exist | Requires a third argument specifying file permissions (e.g., 0644) |


```c
#include <stdio.h>
#include <fcntl.h>

// ...
int fd = open("example.txt", O_RDWR | O_CREAT, 0644);
if (fd == -1) {
    perror("Error opening file");
    return 1;
}
```

### Reading and Writing Files

`read()` and `write()` functions are used to read from and write to files, respectively. They operate on file descriptors returned by the `open()` function.

```ssize_t read(int fd, void *buf, size_t count);```

```c
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

//...

int fd = open("example.txt", O_RDONLY);
if (fd == -1) {
    perror("Error opening file");
    return 1;
}

char buffer[100];
ssize_t bytesRead = read(fd, buffer, sizeof(buffer) - 1);

```

If read returns 0, it indicates the end of the file (EOF). If it returns -1, an error occurred during the read operation. Otherwise, it returns the number of bytes actually read, which may be less than the requested count.

```ssize_t write(int fd, const void *buf, size_t count);```

```c
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

//...
int fd = open("example.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);

char *data = "Hello, World!";
ssize_t bytesWritten = write(fd, data, strlen(data));
if (bytesWritten == -1) {
    perror("Error writing to file");
    close(fd);
    return 1;
}
```
`O_TRUNC` flag can be used with open to truncate the file to zero length if it already exists.

`O_APPEND` flag can be used to open the file in append mode, where data is written at the end of the file.

```int fd = open("example.txt", O_WRONLY | O_CREAT | O_APPEND /* O_TRUNC */, 0644);```

### Lseek

File offset can be changed using `lseek()` function. It repositions the file offset of the open file description associated with the file descriptor `fd` to the argument `offset` according to the directive `whence`.

```off_t lseek(int fd, off_t offset, int whence);```

`whence` can be one of the following:
| Constant    | Description                          | Notes |
|-------------|--------------------------------------| -----|
| SEEK_SET    | The offset is set to offset bytes.   | Beginning of the file |
| SEEK_CUR    | The offset is set to its current location plus offset bytes. | Current position in the file |
| SEEK_END    | The offset is set to the size of the file plus offset bytes. | End of the file |

```c
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int fd = open("example.txt", O_RDWR);
if (fd == -1) {
    perror("Error opening file");
    return 1;
}

off_t newOffset = lseek(fd, 0, SEEK_END); // Move to the end of the file
if (newOffset == -1) {
    perror("Error seeking in file");
    close(fd);
    return 1;
}

printf("New file offset: %ld\n", newOffset);

int sz = write(fd, "Appending this text.\n", 20);
//...
```

### Close the file

File descriptor should be closed using `close()` function when it is no longer needed to free up system resources.

```int close(int fd);```

```c
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int fd = open("example.txt", O_RDWR);
if (fd == -1) {
    perror("Error opening file");
    return 1;
}

// Perform file operations...

if (close(fd) == -1) {
    perror("Error closing file");
    return 1;
}
```