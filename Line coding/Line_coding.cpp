#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

// Lookup table for Manchester encoding (Biphase-M)
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

// Function to encode data using Manchester encoding
void manchester_encode(const uint8_t *data, size_t length, uint8_t *encoded, size_t *encoded_length) {
    size_t index = 0;
    for (size_t i = 0; i < length; i++) {
        uint8_t byte = data[i];
        // Encode each nibble (4 bits) using the lookup table
        encoded[index++] = manchester_table[(byte >> 4) & 0x0F];
        encoded[index++] = manchester_table[byte & 0x0F];
    }
    *encoded_length = index;
}

int main() {
    const char *output_file = "encoded_results.txt";

    FILE *output = fopen(output_file, "w");
    if (!output) {
        perror("Error opening file");
        return 1;
    }

    uint8_t buffer[256];
    size_t bytes_read;

    printf("Enter data to encode: ");
    fgets((char *)buffer, sizeof(buffer), stdin);

    // Remove newline character from input
    size_t input_length = strlen((char *)buffer);
    if (buffer[input_length - 1] == '\n') {
        buffer[input_length - 1] = '\0';
        input_length--;
    }

    // Print Read Buffer
    printf("Read Buffer: ");
    for (size_t i = 0; i < input_length; i++) {
        printf("%02X ", buffer[i]);
    }
    printf("\n");

    // Add delimiters (char(02) and char(07))
    uint8_t frame[260];
    frame[0] = 0x02; // Start delimiter
    memcpy(frame + 1, buffer, input_length);
    frame[input_length + 1] = 0x07; // End delimiter

    // Calculate CRC-8 and append it
    uint8_t crc = calculate_crc8(frame, input_length + 2);
    frame[input_length + 2] = crc;

    size_t frame_length = input_length + 3;

    // Print Raw Frame (with CRC)
    printf("Raw Frame (with CRC): ");
    for (size_t i = 0; i < frame_length; i++) {
        printf("%02X ", frame[i]);
    }
    printf("\n");

    // Manchester encode the frame
    uint8_t encoded_frame[520];
    size_t encoded_length;
    manchester_encode(frame, frame_length, encoded_frame, &encoded_length);

    // Print Manchester-Encoded Frame
    printf("Manchester-Encoded Frame: ");
    for (size_t i = 0; i < encoded_length; i++) {
        printf("%02X ", encoded_frame[i]);
    }
    printf("\n");

    // Write the Manchester-encoded frame to the output file
    fprintf(output, "Manchester-Encoded Frame: ");
    for (size_t i = 0; i < encoded_length; i++) {
        fprintf(output, "%02X ", encoded_frame[i]);
    }
    fprintf(output, "\n");

    fclose(output);

    printf("Encoded results have been saved to '%s'.\n", output_file);
    return 0;
}
