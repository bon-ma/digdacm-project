#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // For islower and tolower
#include <unistd.h> // For isatty

// Define a struct for the lookup table entry
struct HuffmanLookup {
    char hexValue[5];  // Hex value as string (e.g., "41")
    char* huffmanCode; // Huffman code as string
};

// Function to read the lookup table from the file
struct HuffmanLookup* readLookupTable(const char* filename, int* size) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening lookup table file");
        return NULL;
    }

    int capacity = 50;
    struct HuffmanLookup* table = (struct HuffmanLookup*)malloc(capacity * sizeof(struct HuffmanLookup));
    *size = 0;

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (*size == capacity) {
            capacity *= 2;
            table = (struct HuffmanLookup*)realloc(table, capacity * sizeof(struct HuffmanLookup));
        }

        sscanf(line, "%s", table[*size].hexValue);

        table[*size].huffmanCode = (char*)malloc(256 * sizeof(char));

        if (sscanf(line, "%s %s", table[*size].hexValue, table[*size].huffmanCode) != 2) {
            fprintf(stderr, "Error reading line:%s\n", line);
            free(table[*size].huffmanCode);
            continue;
        }

        (*size)++;
    }

    fclose(file);
    return table;
}

// Function to get the Huffman code for a given hex value
const char* getHuffmanCode(struct HuffmanLookup* table, int tableSize, const char* hexValue) {
    for (int i = 0; i < tableSize; i++) {
        if (strcmp(table[i].hexValue, hexValue) == 0) {
            return table[i].huffmanCode;
        }
    }
    return NULL;
}

// Function to trim spaces and newline characters from a string
void trimInput(char* input) {
    int length = strlen(input);

    int start = 0;
    while (start < length && (input[start] == ' ' || input[start] == '\n' || input[start] == '\t')) {
        start++;
    }

    int end = length - 1;
    while (end >= start && (input[end] == ' ' || input[end] == '\n' || input[end] == '\t')) {
        end--;
    }

    for (int i = 0; i <= end - start; i++) {
        input[i] = input[start + i];
    }

    input[end - start + 1] = '\0';
}

// Function to convert uppercase letters to their lowercase hex equivalents
void convertToLowerHex(char* hexValue) {
    if (strlen(hexValue) == 2) {
        int decimalValue = strtol(hexValue, NULL, 16);
        if (decimalValue >= 0x41 && decimalValue <= 0x5A) { // 'A' to 'Z'
            decimalValue += 0x20; // Convert to lowercase equivalent ('a' to 'z')
            snprintf(hexValue, 3, "%02X", decimalValue); // Update hexValue with the new value
        }
    }
}

int main() {
    int lookupTableSize;
    struct HuffmanLookup* lookupTable = readLookupTable("huffman_lookup.txt", &lookupTableSize);
    if (!lookupTable) {
        return 1;
    }

    char hexInput[1024];
    char* inputMessage = NULL;

    if (!isatty(fileno(stdin))) {  
        fread(hexInput, sizeof(char), 1024, stdin);
        hexInput[1023] = '\0';
        inputMessage = hexInput;

        char buffer[] = "0";
        char finalInput[1024];
        snprintf(finalInput, sizeof(finalInput), "%s%s", buffer, inputMessage);
        inputMessage = finalInput;
    } else {  
        printf("Enter the hex string: ");
        fgets(hexInput, sizeof(hexInput), stdin);
        hexInput[strcspn(hexInput, "\n")] = '\0';
        inputMessage = hexInput;
    }

    trimInput(inputMessage);

    char compressedOutput[1024 * 10]; 
    compressedOutput[0] = '\0';

    for (int i = 0; i < strlen(inputMessage); i += 2) {
        char hexValue[3] = { inputMessage[i], inputMessage[i + 1], '\0' };

        if (strcmp(hexValue, "0") == 0) {
            continue;
        }

        // Convert uppercase letters to lowercase equivalent hex
        convertToLowerHex(hexValue);

        const char* huffmanCode = getHuffmanCode(lookupTable, lookupTableSize, hexValue);

        if (huffmanCode) {
            strcat(compressedOutput, huffmanCode);
        }
    }

    printf("%s\n", compressedOutput);

    for (int i = 0; i < lookupTableSize; i++) {
        free(lookupTable[i].huffmanCode);
    }
    free(lookupTable);

    return 0;
}
