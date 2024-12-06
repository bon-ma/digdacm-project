#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

        // Read hex value and Huffman code from each line
        // Format is: HEXCODE HUFFMANCODE (e.g., 41 00000)
        sscanf(line, "%s", table[*size].hexValue);

        // Dynamically allocate memory for the Huffman code
        table[*size].huffmanCode = (char*)malloc(256 * sizeof(char));

        // Read the Huffman code into the allocated memory
        if (sscanf(line, "%s %s", table[*size].hexValue, table[*size].huffmanCode) != 2) {
            fprintf(stderr, "Error reading line:%s\n", line);
            free(table[*size].huffmanCode);
            continue; // Skip this entry if reading fails
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
    return NULL; // Return NULL if no match found
}

// Function to trim spaces and newline characters from a string
void trimInput(char* input) {
    int length = strlen(input);

    // Trim leading spaces
    int start = 0;
    while (start < length && (input[start] == ' ' || input[start] == '\n' || input[start] == '\t')) {
        start++;
    }

    // Trim trailing spaces and newline characters
    int end = length - 1;
    while (end >= start && (input[end] == ' ' || input[end] == '\n' || input[end] == '\t')) {
        end--;
    }

    // Shift the trimmed string to the beginning
    for (int i = 0; i <= end - start; i++) {
        input[i] = input[start + i];
    }

    // Null-terminate the string
    input[end - start + 1] = '\0';
}

int main() {
    int lookupTableSize;
    struct HuffmanLookup* lookupTable = readLookupTable("huffman_lookup.txt", &lookupTableSize);
    if (!lookupTable) {
        return 1;
    }

    // Input: hex string
    char hexInput[1024];
    char* inputMessage = NULL;

    // Check if input is from a pipe or terminal
    if (!isatty(fileno(stdin))) {  // Input is from a pipe
        fread(hexInput, sizeof(char), 1024, stdin);
        hexInput[1023] = '\0';  // Null-terminate the string
        inputMessage = hexInput;

        // Prepend a single '0' buffer to the piped input
        char buffer[] = "0";  // Single '0' buffer
        char finalInput[1024];
        snprintf(finalInput, sizeof(finalInput), "%s%s", buffer, inputMessage);  // Concatenate '0' buffer with input
        inputMessage = finalInput; // Point to the new string with the buffer
    } else {  // Input is from the terminal
        printf("Enter the hex string: ");
        fgets(hexInput, sizeof(hexInput), stdin);
        hexInput[strcspn(hexInput, "\n")] = '\0'; // Remove newline
        inputMessage = hexInput;
    }

    // Trim the input string to remove unwanted spaces/newlines
    trimInput(inputMessage);

    // Initialize a buffer to store the entire compressed output
    char compressedOutput[1024 * 10]; // Make sure it's large enough to hold the result
    compressedOutput[0] = '\0'; // Initialize the string as empty

    // Process the input hex string by 2 characters at a time (representing one hex byte)
    for (int i = 0; i < strlen(inputMessage); i += 2) {
        // Extract each pair of hex characters (e.g., "63" for 'c')
        char hexValue[3] = { inputMessage[i], inputMessage[i + 1], '\0' };

        // Skip the buffer value '0' (we don't want to process this)
        if (strcmp(hexValue, "0") == 0) {
            continue; // Skip this hex value
        }

        // Get the Huffman code for the hex value
        const char* huffmanCode = getHuffmanCode(lookupTable, lookupTableSize, hexValue);

        if (huffmanCode) {
            // Concatenate the Huffman code to the output buffer
            strcat(compressedOutput, huffmanCode);
        }
        // Commenting out the error message for unrecognized hex values
        // else {
        //     printf("No Huffman code found for hex: %s\n", hexValue);
        // }
    }

    // Output the final compressed Huffman code in one line
    printf("%s\n", compressedOutput);

    // Free the allocated memory for the lookup table and Huffman codes
    for (int i = 0; i < lookupTableSize; i++) {
        free(lookupTable[i].huffmanCode);
    }
    free(lookupTable);

    return 0;
}
