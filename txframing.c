#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SEQUENCE_NUMBER 1000 // Maximum sequence number
#define MAX_DATA_BLOCK_SIZE 2    // Maximum size of data block
#define PREAMBLE "FF"            // Fixed preamble
#define FRAME_TYPE 1             // Frame type (fixed)
#define FCS_PLACEHOLDER "00"     // Placeholder for FCS (CRC-8)

// Function to convert a binary string to its hexadecimal representation
char* binaryToHexadecimal(const char* input) {
    size_t binaryLength = strlen(input);
    size_t hexLength = (binaryLength + 3) / 4; // Calculate the number of hexadecimal characters
    char* hexadecimal = (char*)malloc((hexLength + 1) * sizeof(char));
    hexadecimal[hexLength] = '\0'; // Null-terminate the string

    for (size_t i = 0; i < hexLength; i++) {
        int byte = 0;
        for (int j = 0; j < 4 && i * 4 + j < binaryLength; j++) {
            byte = (byte << 1) | (input[i * 4 + j] - '0');
        }
        sprintf(hexadecimal + i, "%X", byte);
    }
    return hexadecimal;
}

int main() {
    char input[MAX_SEQUENCE_NUMBER * 8]; // Input buffer for binary sequence
    int sequenceNumber = 1;             // Initialize sequence number

    // Read binary input from stdin
    if (fgets(input, sizeof(input), stdin) == NULL) {
        perror("Error reading input from stdin");
        return 1;
    }

    // Remove trailing newline (if any)
    input[strcspn(input, "\n")] = '\0';

    // Convert binary input to hexadecimal
    char* hexData = binaryToHexadecimal(input);
    size_t hexLength = strlen(hexData);

    // Process the hexadecimal data into frames
    for (size_t i = 0; i < hexLength; i += MAX_DATA_BLOCK_SIZE * 2) {
        // Extract payload of size MAX_DATA_BLOCK_SIZE (2 bytes)
        char payload[MAX_DATA_BLOCK_SIZE * 2 + 1] = {0};
        strncpy(payload, hexData + i, MAX_DATA_BLOCK_SIZE * 2);

        // Print the frame
        printf("%s%04X%02X%02X%s%s", // Format: Preamble, SeqNum, FrameType, PayloadSize, Payload, FCS
               PREAMBLE,
               sequenceNumber,
               FRAME_TYPE,
               MAX_DATA_BLOCK_SIZE,
               payload,
               FCS_PLACEHOLDER);
       
        // Increment and wrap the sequence number
        sequenceNumber = (sequenceNumber + 1) % MAX_SEQUENCE_NUMBER;
    }

    free(hexData);
    return 0;
}

