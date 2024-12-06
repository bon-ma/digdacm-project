#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

// Lookup table for Manchester decoding (Biphase-M)
const uint8_t manchester_table[16] = {
    0xA0, 0x90, 0xE0, 0xD0, // 0000-0011
    0xB0, 0x80, 0xF0, 0xC0, // 0100-0111
    0xA8, 0x98, 0xE8, 0xD8, // 1000-1011
    0xB8, 0x88, 0xF8, 0xC8  // 1100-1111
};

// Function to calculate CRC-8 (polynomial x^8 + x^5 + x^4 + x^3 + 1)
uint8_t calculate_crc8(const uint8_t *data, size_t length) {
    uint8_t crc = 0;
    for (size_t i = 0; i < length; i++) {
        crc ^= data[i];
        for (int j = 0; j < 8; j++) {
            if (crc & 0x80) {
                crc = (crc << 1) ^ 0x97;
            } else {
                crc <<= 1;
            }
        }
    }
    return crc;
}

// Function to decode Manchester-encoded data
void manchester_decode(const uint8_t *encoded, size_t encoded_length, uint8_t *decoded, size_t *decoded_length) {
    size_t index = 0;
    for (size_t i = 0; i < encoded_length; i += 2) {
        uint8_t high_nibble = 0;
        uint8_t low_nibble = 0;
        // Find corresponding nibble value in the lookup table
        for (int j = 0; j < 16; j++) {
            if (manchester_table[j] == encoded[i]) {
                high_nibble = j;
            }
            if (manchester_table[j] == encoded[i + 1]) {
                low_nibble = j;
            }
        }
        // Combine the nibbles to form the original byte
        decoded[index++] = (high_nibble << 4) | low_nibble;
    }
    *decoded_length = index;
}

int main(int argc, char *argv[]) {
    FILE *input = NULL;
    FILE *output = fopen("decoded_results.txt", "w"); // Open the output file to write the decoded results

    // Check command-line arguments
    if (argc == 2) {
        input = fopen(argv[1], "rb");
    } else if (argc == 1) {
        printf("No input file provided. Reading from stdin.\n");
        input = stdin; // Read from standard input
    } else {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    if (!input) {
        perror("Error opening file");
        return 1;
    }

    uint8_t buffer[256];
    size_t bytes_read;

    while ((bytes_read = fread(buffer, 1, sizeof(buffer), input)) > 0) {
        // Print Read Buffer
        printf("Read Buffer: ");
        for (size_t i = 0; i < bytes_read; i++) {
            printf("%02X ", buffer[i]);
        }
        printf("\n");

        // Check for start and end delimiters
        if (buffer[0] != 0x02 || buffer[bytes_read - 1] != 0x07) {
            fprintf(stderr, "Invalid frame: missing delimiters.\n");
            continue;
        }

        // Calculate CRC-8 and verify it
        uint8_t crc = calculate_crc8(buffer, bytes_read);
        if (crc != buffer[bytes_read - 2]) {
            fprintf(stderr, "CRC mismatch: expected %02X, got %02X\n", buffer[bytes_read - 2], crc);
            continue;
        }

        // Remove start and end delimiters
        uint8_t frame[256];
        memcpy(frame, buffer + 1, bytes_read - 2);
        size_t frame_length = bytes_read - 3;

        // Decode Manchester-encoded frame
        uint8_t decoded_frame[256];
        size_t decoded_length;
        manchester_decode(frame, frame_length, decoded_frame, &decoded_length);

        // Print decoded frame
        printf("Decoded Frame: ");
        for (size_t i = 0; i < decoded_length; i++) {
            printf("%02X ", decoded_frame[i]);
        }
        printf("\n");

        // Print the decoded frame to the output file
        fprintf(output, "Decoded Frame: ");
        for (size_t i = 0; i < decoded_length; i++) {
            fprintf(output, "%02X ", decoded_frame[i]);
        }
        fprintf(output, "\n");
    }

    fclose(input);
    fclose(output);

    printf("\n");
    return 0;
}