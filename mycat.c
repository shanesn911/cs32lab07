/*
 * mycat.c - print files to standard output, like cat
 *
 * Usage: mycat [FILE]...
 *
 * Prints each FILE to standard output, in order. With no FILE arguments,
 * copies standard input to standard output.
 *
 * main() is done for you. Write copy_fd and cat_file using only the system
 * calls open, read, write, and close: no FILE *, printf, fopen, fread,
 * fgets, or fwrite. (perror is fine for error messages.)
 */

#include <fcntl.h>
#include <stdio.h>      // only for perror
#include <unistd.h>

/** How many bytes to read at a time */
#define BUFFER_SIZE 4096

/**
 * Copies everything that can be read from in_fd to out_fd, until read
 * returns 0 (end of file).
 *
 * Returns 0 on success, or -1 if a read or write fails.
 */
static int copy_fd(int in_fd, int out_fd) {
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    while ((bytes_read = read(in_fd, buffer, BUFFER_SIZE)) > 0) {
        ssize_t bytes_written = 0;
        while (bytes_written < bytes_read) {
            ssize_t result = write(out_fd, buffer + bytes_written, bytes_read - bytes_written);
            if (result < 0) {
                return -1;
            }
            bytes_written += result;
        }
    }
    if (bytes_read < 0) {
        return -1;
    }
    return 0;
}

/**
 * Opens the file at path, copies it to standard output with copy_fd, and
 * closes it. If anything fails, prints an error message with perror.
 *
 * Returns 0 on success, or -1 on failure.
 */
static int cat_file(const char *path) {
    int fd = open(path, O_RDONLY);
    if (fd < 0) {
        perror(path);
        return -1;
    }
    
    int result = copy_fd(fd, STDOUT_FILENO);
    if (result < 0) {
        perror(path);
    }
    close(fd);
    return result;
}

int main(int argc, char *argv[]) {
    // No file names: copy standard input (fd 0) to standard output (fd 1)
    if (argc == 1) {
        if (copy_fd(STDIN_FILENO, STDOUT_FILENO) < 0) {
            perror("mycat");
            return 1;
        }
        return 0;
    }

    // Print each file in order. If one fails, keep going with the rest,
    // but exit with status 1 at the end.
    int status = 0;
    for (int i = 1; i < argc; i++) {
        if (cat_file(argv[i]) < 0) {
            status = 1;
        }
    }
    return status;
}
