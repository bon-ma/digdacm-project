#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define CRC8_POLYNOMIAL 0x07
#define MAX_SEQUENCE_NUMBER 1000
#define MAX_DATA_BLOCK_SIZE 2
#define FRAME_SIZE (2 + 4 + 2 + 2 + MAX_DATA_BLOCK_SIZE * 2 + 2) // Preamble + SeqNum + FrameType + PayloadSize + Payload + CRC

// Function to calculate CRC-8
uint8_t crc8(uint8_t *data, size_t len) {
    uint8_t crc = 0;
    for (size_t i = 0; i < len; i++) {
        crc ^= data[i];
        for (uint8_t bit = 0; bit < 8; bit++) {
            if (crc & 0x80) {
                crc = (crc << 1) ^ CRC8_POLYNOMIAL;
            } else {
                crc <<= 1;
            }
        }
    }
    return crc;
}

// Function to convert binary string (0/1) to a byte array
void binary_to_bytes(const char *binary, uint8_t *bytes, size_t len) {
    for (size_t i = 0; i < len; i++) {
        uint8_t byte = 0;
        for (int bit = 0; bit < 8; bit++) {
            byte = (byte << 1) | (binary[i * 8 + bit] - '0');
        }
        bytes[i] = byte;
    }
}

int main() {
    char input[FRAME_SIZE * 10]; // Buffer to store binary sequence
    uint8_t frame[FRAME_SIZE];   // Buffer to hold the constructed frame
    char hexOutput[3];           // Buffer to hold hexadecimal output (2 hex digits + null terminator)

    // Read binary sequence from stdin
    if (fgets(input, sizeof(input), stdin) == NULL) {
        perror("Error reading input from stdin");
        return 1;
    }

    // Remove trailing newline, if any
    input[strcspn(input, "\n")] = '\0';

    size_t inputLength = strlen(input);
    size_t index = 0;
    int sequence_number = 0;

    while (index + 8 * MAX_DATA_BLOCK_SIZE <= inputLength) {
        // Prepare frame
        frame[0] = 0xFF; // Preamble (not outputted)
        frame[1] = (sequence_number >> 8) & 0xFF;
        frame[2] = sequence_number & 0xFF;
        frame[3] = 0x01; // Frame Type
        frame[4] = MAX_DATA_BLOCK_SIZE; // Payload size

        // Convert binary payload to bytes
        binary_to_bytes(input + index, frame + 5, MAX_DATA_BLOCK_SIZE);
        index += 8 * MAX_DATA_BLOCK_SIZE;

        // Calculate CRC-8
        uint8_t crc = crc8(frame + 1, 4 + MAX_DATA_BLOCK_SIZE); // Exclude preamble
        frame[5 + MAX_DATA_BLOCK_SIZE] = crc;

        // Output frame as hex (excluding preamble)
        for (size_t i = 1; i < 6 + MAX_DATA_BLOCK_SIZE; i++) {
            sprintf(hexOutput, "%02X", frame[i]);
            printf("%s", hexOutput);
        }

        printf("\n"); // Newline between frames
        sequence_number = (sequence_number + 1) % MAX_SEQUENCE_NUMBER;
    }

    return 0;
}

