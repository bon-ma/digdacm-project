#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

#define RECEIVER_PORT "/dev/ttyp1"

int configure_serial_port(int fd) {
    struct termios options;

    if (tcgetattr(fd, &options) < 0) {
        perror("Error getting serial port attributes");
        return -1;
    }

    cfsetispeed(&options, B9600);
    cfsetospeed(&options, B9600);

    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;

    options.c_cflag &= ~CRTSCTS;

    if (tcsetattr(fd, TCSANOW, &options) < 0) {
        perror("Error setting serial port attributes");
        return -1;
    }

    return 0;
}

int main() {
    int fd;
    char buffer[256];
    ssize_t bytes_read;

    fd = open(RECEIVER_PORT, O_RDONLY);
    if (fd == -1) {
        perror("Error opening receiver port");
        return EXIT_FAILURE;
    }

    if (configure_serial_port(fd) < 0) {
        close(fd);
        return EXIT_FAILURE;
    }

    printf("Listening on %s...\n", RECEIVER_PORT);

    while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0) {
        printf("Received %zd bytes: %.*s\n", bytes_read, (int)bytes_read, buffer);
    }

    if (bytes_read == -1) {
        perror("Error reading from receiver port");
    }

    close(fd);
    return 0;
}
