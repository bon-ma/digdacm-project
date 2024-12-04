#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>

#define VIRTUAL_SERIAL_PORT "/dev/ttyp1"

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

void receive_and_print_data() {
    int serial_fd;
    serial_fd = open(VIRTUAL_SERIAL_PORT, O_RDONLY);
    if (serial_fd == -1) {
        perror("Error opening virtual serial port");
        exit(EXIT_FAILURE);
    }

    if (configure_serial_port(serial_fd) < 0) {
        close(serial_fd);
        exit(EXIT_FAILURE);
    }

    printf("Opened serial port %s\n", VIRTUAL_SERIAL_PORT);

    char received_buffer[256];
    ssize_t bytes_read;

    while (1) {
        bytes_read = read(serial_fd, received_buffer, sizeof(received_buffer) - 1);
        if (bytes_read == -1) {
            perror("Error reading from virtual serial port");
            close(serial_fd);
            exit(EXIT_FAILURE);
        } else if (bytes_read > 0) {
            received_buffer[bytes_read] = '\0';
            printf("Received %ld bytes: %s\n", bytes_read, received_buffer);
        }
    }

    close(serial_fd);
}

int main() {
    receive_and_print_data();
    return 0;
}
