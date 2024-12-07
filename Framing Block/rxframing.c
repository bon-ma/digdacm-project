#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SEQUENCE_NUMBER 1000  
#define MAX_DATA_BLOCK_SIZE 2     
#define MAX_BINARY_SEQUENCE 8000 


// Convert a binary string to its hexadecimal representation
char* binaryToHexadecimal(const char* input) {
    size_t binaryLength = strlen(input);
    size_t hexLength = (binaryLength + 3) / 4; // Calculate the number of hexadecimal characters required
    char* hexadecimal = (char*)malloc((hexLength + 1) * sizeof(char));

    for (size_t i = 0; i < hexLength; i++) {
        int byte = 0;
        for (int j = 0; j < 4 && i * 4 + j < binaryLength; j++) {
            byte = (byte << 1) | (input[i * 4 + j] - '0');
        }
        sprintf(hexadecimal + i, "%X", byte);
    }
    hexadecimal[hexLength] = '\0'; 
    return hexadecimal;
}

int main() {
    char input[MAX_BINARY_SEQUENCE + 1]; 

    // Read binary input from stdin
    if (fgets(input, sizeof(input), stdin) == NULL) {
        perror("Error reading input from stdin");
        return 1;
    }

    // Remove trailing newline (if any)
    input[strcspn(input, "\n")] = '\0';

    // Convert binary input to hexadecimal
    char* hexadecimal = binaryToHexadecimal(input);
    if (!hexadecimal) {
        fprintf(stderr, "Error: Conversion to hexadecimal failed\n");
        return 1;
    }

    // Constants
    const char preamble[] = "FF";
    const int frame_type = 1;
    const int payload_size = MAX_DATA_BLOCK_SIZE;
    const int fcs = 0;

    int sequence_number = 1;
    size_t length = strlen(hexadecimal);

    // Iterate through hexadecimal string in blocks of MAX_DATA_BLOCK_SIZE
    for (size_t i = 0; i < length; i += MAX_DATA_BLOCK_SIZE) {
        int remaining_chars = length - i; // Calculate remaining characters

        // Print frame fields continuously
        printf("%s%04X%02d%04d", preamble, sequence_number, frame_type, payload_size);

        
        for (int j = 0; j < MAX_DATA_BLOCK_SIZE; j++) {
            if (i + j < length) {
                printf("%c", hexadecimal[i + j]);
            } else {
                printf("0"); 
            }
        }

       
        printf("%04d", fcs);

     
        sequence_number = (sequence_number % MAX_SEQUENCE_NUMBER) + 1;
    }

    free(hexadecimal); 
    return 0;
}
