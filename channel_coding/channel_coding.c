#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// Constants
#define FRAME_SIZE 18            // Size of each frame in hex digits (16 hex digits + FCS)
#define CRC8_POLYNOMIAL 0x07     // CRC-8 polynomial
#define MAX_HEX_INPUT 4096       // Maximum size of input hex sequence

// Function to calculate the CRC-8 checksum for a given data sequence
uint8_t calculate_crc8(uint8_t *data, size_t len) {
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

// Function to convert a hex string to a byte
uint8_t hex_to_byte(const char *hex) {
    uint8_t byte;
    sscanf(hex, "%2hhx", &byte);
    return byte;
}

// Function to serialize a byte into binary bits
void byte_to_binary(uint8_t byte, char *output) {
    for (int i = 7; i >= 0; i--) {
        *output++ = ((byte >> i) & 1) + '0';
    }
}

// Main function
int main() {
    char hex_frame[FRAME_SIZE + 1];  // Buffer to store a frame (hex format)
    char binary_output[FRAME_SIZE * 4 + 9]; // Buffer to store serialized binary output
    uint8_t frame_bytes[FRAME_SIZE / 2];    // Buffer for frame bytes

    // Read input hex frames from stdin
    while (fread(hex_frame, 1, FRAME_SIZE, stdin) == FRAME_SIZE) {
        hex_frame[FRAME_SIZE] = '\0';  // Null-terminate the frame

        // Convert hex frame to byte array
        for (size_t i = 0; i < FRAME_SIZE / 2; i++) {
            frame_bytes[i] = hex_to_byte(hex_frame + 2 * i);
        }

        // Exclude the last byte (FCS) when calculating CRC
        uint8_t crc = calculate_crc8(frame_bytes, FRAME_SIZE / 2 - 1);

        // Serialize the frame (excluding FCS) to binary
        char *output_ptr = binary_output;
        for (size_t i = 0; i < FRAME_SIZE / 2 - 1; i++) {
            byte_to_binary(frame_bytes[i], output_ptr);
            output_ptr += 8;
        }

        // Append the CRC in binary format
        byte_to_binary(crc, output_ptr);
        output_ptr += 8;

        // Null-terminate the output binary sequence
        *output_ptr = '\0';

        // Print the binary sequence to stdout
        printf("%s", binary_output);
    }

    return 0;
}