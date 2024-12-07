#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#define MAX_LOOKUP_ENTRIES 256
#define MAX_CODE_LENGTH 32

// Define a struct for the lookup table entry
struct HuffmanLookup {
    char character;       // The character represented (in hex)
    char code[MAX_CODE_LENGTH]; // Huffman code for the character
};

// Function to read the lookup table from a file
int readLookupTable(const char* filename, struct HuffmanLookup* table) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening lookup table file");
        return -1;
    }

    int count = 0;
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        // Parse each line of the format: HexCharacter BinaryCode
        if (sscanf(line, "%X %s", &table[count].character, table[count].code) == 2) {
            count++;
        } else {
            fprintf(stderr, "Malformed line in lookup file: %s", line);
        }
    }

    fclose(file);
    return count;
}

// Function to decode the binary message into hexadecimal
void decodeToHex(const struct HuffmanLookup* table, int tableSize, FILE* fp_in, FILE* fp_out) {
    char buffer[MAX_CODE_LENGTH] = {0};  // To hold the current binary code
    char ch;
    int index = 0;  // To track the current position in the buffer

    // Read bit-by-bit from the input file or stdin
    while ((ch = fgetc(fp_in)) != EOF) {
        if (ch == '\n' && isatty(fileno(fp_in))) {
            break; // Stop reading on newline for terminal input
        }

        if (ch == '0' || ch == '1') {  // Accept only '0' or '1' for binary input
            buffer[index++] = ch;  // Add the bit to the buffer
            buffer[index] = '\0';  // Null-terminate the string

            // Check if the buffer matches any code in the lookup table
            for (int i = 0; i < tableSize; i++) {
                if (strcmp(buffer, table[i].code) == 0) {
                    // Match found, convert the character to its ASCII hex value
                    unsigned char hexValue = (unsigned char)table[i].character;
                    fprintf(fp_out, "%02X", hexValue);  // Print hex representation

                    index = 0; // Reset the buffer for the next code
                    break;
                }
            }
        }
    }

    // After processing all input, check if there's any remaining code in the buffer
    if (index > 0) {
        // Check for any remaining matching code for the last part of the input
        for (int i = 0; i < tableSize; i++) {
            if (strcmp(buffer, table[i].code) == 0) {
                unsigned char hexValue = (unsigned char)table[i].character;
                fprintf(fp_out, "%02X", hexValue);
                break;
            }
        }
    }

    fprintf(fp_out, "\n"); // Add a newline at the end
}

int main() {
    struct HuffmanLookup lookupTable[MAX_LOOKUP_ENTRIES];
    int tableSize = readLookupTable("huffman_lookup.txt", lookupTable);
    if (tableSize < 0) {
        return 1;
    }

    FILE* fp_in = stdin;   // Default input is from stdin
    FILE* fp_out = stdout; // Default output is to stdout

    if (isatty(fileno(fp_in))) {
        // Terminal input
        printf("Enter the encoded binary message: ");
    }

    decodeToHex(lookupTable, tableSize, fp_in, fp_out);

    return 0;
}
