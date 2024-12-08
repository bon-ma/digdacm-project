#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to decode Biphase-M (Manchester encoding) back to binary
void decode_biphase_m(const char *encoded, char *binary) {
    char *token = strtok((char *)encoded, " "); // Split the encoded string by spaces
    while (token) {
        if (strcmp(token, "05") == 0) {
            // Check for High-to-Low transition
            token = strtok(NULL, " ");
            if (token && strcmp(token, "-05") == 0) {
                strcat(binary, "0");
            } else {
                fprintf(stderr, "Invalid encoding pattern: Expected -05 after 05.\n");
                exit(EXIT_FAILURE);
            }
        } else if (strcmp(token, "-05") == 0) {
            // Check for Low-to-High transition
            token = strtok(NULL, " ");
            if (token && strcmp(token, "05") == 0) {
                strcat(binary, "1");
            } else {
                fprintf(stderr, "Invalid encoding pattern: Expected 05 after -05.\n");
                exit(EXIT_FAILURE);
            }
        } else {
            fprintf(stderr, "Unexpected voltage level '%s'.\n", token);
            exit(EXIT_FAILURE);
        }
        token = strtok(NULL, " "); // Move to the next voltage level
    }
}

int main() {
    char binary[256] = {0}; // To store the decoded binary result
    char encoded[512];

    printf("Enter Biphase-M encoded data (space-separated voltage levels):\n");
    if (!fgets(encoded, sizeof(encoded), stdin)) {
        perror("Error reading input");
        return EXIT_FAILURE;
    }

    // Remove trailing newline character if present
    char *newline = strchr(encoded, '\n');
    if (newline) *newline = '\0';

    // Decode the encoded data
    decode_biphase_m(encoded, binary);

    // Print the decoded binary result
    printf("Decoded Binary: %s\n", binary);

    return 0;
}
