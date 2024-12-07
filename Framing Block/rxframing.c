#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>


#define CRC8_POLYNOMIAL 0x07
#define MAX_BINARY_SEQUENCE 1000 * 8 
#define MAX_SEQUENCE_NUMBER 1000     
#define PAYLOAD_SIZE 2               
#define FRAME_TYPE 1                 
#define PREAMBLE "FF"                

// Function to calculate CRC-8
uint8_t crc8(uint8_t *data, size_t len) {
    uint8_t crc = 0;
    for (size_t i = 0; i < len; i++) {
        crc ^= data[i];
        for (uint8_t bit = 0; bit < 8; bit++) {
            crc = (crc & 0x80) ? (crc << 1) ^ CRC8_POLYNOMIAL : (crc << 1);
        }
    }
    return crc;
}

// Convert binary string to hexadecimal
char *binaryToHexadecimal(const char *input) {
    size_t binaryLength = strlen(input);
    size_t hexLength = (binaryLength + 3) / 4;
    char *hexadecimal = (char *)malloc(hexLength + 1);
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

// Function to convert hexadecimal string to byte array
void hexStringToBytes(const char *hexString, uint8_t *bytes, size_t len) {
    for (size_t i = 0; i < len; i++) {
        sscanf(hexString + 2 * i, "%2hhx", &bytes[i]);
    }
}

// Main 
int main() {
    char input[MAX_BINARY_SEQUENCE];
    char result[20];
    uint8_t frameBytes[18];
    int sequence_number = 1;

    // Input from stdin
    if (fgets(input, sizeof(input), stdin) == NULL) {
        perror("Error reading input");
        return 1;
    }

    
    input[strcspn(input, "\n")] = '\0';

    // Convert binary input to hexadecimal
    char *hexadecimal = binaryToHexadecimal(input);
    size_t hexLength = strlen(hexadecimal);

    // Process hexadecimal data into frames
    for (size_t i = 0; i < hexLength; i += PAYLOAD_SIZE) {
        char payload[PAYLOAD_SIZE + 1] = {0};
        size_t remaining = hexLength - i;

        // Fill payload with up to PAYLOAD_SIZE characters
        strncpy(payload, hexadecimal + i, remaining >= PAYLOAD_SIZE ? PAYLOAD_SIZE : remaining);

        // Construct frame
        sprintf(result, "%s%04X%02X%04X%s0000", PREAMBLE, sequence_number, FRAME_TYPE, PAYLOAD_SIZE, payload);

        // Convert frame to bytes
        size_t frameLen = strlen(result) / 2;
        hexStringToBytes(result, frameBytes, frameLen - 1); 

        // Calculate CRC-8 (FCS)
        uint8_t fcs = crc8(frameBytes, frameLen - 1); 
        sprintf(result + strlen(result) - 4, "%02X", fcs); 


        printf("%s", result);

       
        sequence_number = (sequence_number % MAX_SEQUENCE_NUMBER) + 1;
    }

   
    free(hexadecimal);
    return 0;
}
