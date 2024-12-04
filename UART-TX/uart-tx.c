#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024

// Function to convert hex string to byte
unsigned char hexToByte(const char* hexStr) {
    unsigned char byte = 0;
    sscanf(hexStr, "%2hhx", &byte);
    return byte;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s INPUT_FILE OUTPUT_FILE\n", argv[0]);
        fprintf(stderr, "Example: %s input.txt output.txt\n", argv[0]);
        return 1;
    }

    FILE* inputFile = fopen(argv[1], "r");
    if (inputFile == NULL) {
        perror("Error opening input file");
        return 1;
    }

    FILE* outputFile = fopen(argv[2], "wb");
    if (outputFile == NULL) {
        perror("Error opening output file");
        fclose(inputFile);
        return 1;
    }

    char buffer[3] = {0};  // Buffer for hex digits or binary digits
    int bufferIndex = 0;
    char c;
    int isNegative = 0;

    // Read input one character at a time
    while ((c = fgetc(inputFile)) != EOF) {
        if (c == '0' || c == '1') {
            // Binary input from Channel Coding block
            unsigned char byte = (c == '1') ? 0x01 : 0x00;
            fwrite(&byte, 1, 1, outputFile);
        } 
        else if (c == '-') {
            isNegative = 1;
            continue;
        } 
        else if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f')) {
            // Hex input from Line Coding block
            buffer[bufferIndex++] = c;
            
            if (bufferIndex == 2) {
                unsigned char byte = hexToByte(buffer);
                if (isNegative) {
                    byte = ~byte + 1;  // Two's complement for negative
                    isNegative = 0;
                }
                fwrite(&byte, 1, 1, outputFile);
                bufferIndex = 0;
                memset(buffer, 0, sizeof(buffer));
            }
        }
    }

    fclose(inputFile);
    fclose(outputFile);
    printf("Processing complete. Output saved to %s\n", argv[2]);
    return 0;
}
