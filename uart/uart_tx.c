#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>

#define VIRTUAL_SERIAL_PORT "/dev/ttyp0"
#define FILENAME "tx_linecoding_output.txt"

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

void transmit_data_from_file() {
    int serial_fd, file_fd;
    char buffer[256];
    ssize_t bytes_read, bytes_written;

    serial_fd = open(VIRTUAL_SERIAL_PORT, O_WRONLY);
    if (serial_fd == -1) {
        perror("Error opening virtual serial port for writing");
        exit(EXIT_FAILURE);
    }

    if (configure_serial_port(serial_fd) < 0) {
        close(serial_fd);
        exit(EXIT_FAILURE);
    }

    printf("Opened serial port %s for transmission\n", VIRTUAL_SERIAL_PORT);

    file_fd = open(FILENAME, O_RDONLY);
    if (file_fd == -1) {
        perror("Error opening file to transmit");
        close(serial_fd);
        exit(EXIT_FAILURE);
    }

    while ((bytes_read = read(file_fd, buffer, sizeof(buffer))) > 0) {
        bytes_written = write(serial_fd, buffer, bytes_read);
        if (bytes_written == -1) {
            perror("Error writing to serial port");
            close(serial_fd);
            close(file_fd);
            exit(EXIT_FAILURE);
        }
        printf("Transmitting data: %.*s", (int)bytes_read, buffer);
    }

    if (bytes_read == -1) {
        perror("Error reading from file");
    }

    close(file_fd);
    close(serial_fd);
}

int main() {
    transmit_data_from_file();
    return 0;
}
