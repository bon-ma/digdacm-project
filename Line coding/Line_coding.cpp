#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to encode binary data into Biphase-M (Manchester encoding)
void encode_biphase_m(const char *binary) {
    while (*binary) {
        if (*binary == '0') {
            // '0' -> High-to-Low: 05 -05
            printf("05 -05 ");
        } else if (*binary == '1') {
            // '1' -> Low-to-High: -05 05
            printf("-05 05 ");
        } else {
            fprintf(stderr, " no output? then, u input something that isnt'%c'. '0' and '1' \n", *binary);
            exit(EXIT_FAILURE);
        }
        binary++;
    }
    printf("\n");
}

int main() {
    char buffer[256];
    printf("Enter binary data: ");
    if (!fgets(buffer, sizeof(buffer), stdin)) {
        perror("Error reading input");
        return EXIT_FAILURE;
    }

    // Remove newline character if present
    char *newline = strchr(buffer, '\n');
    if (newline) *newline = '\0';

    // Encode binary input using Biphase-M encoding
    encode_biphase_m(buffer);

    return 0;
}
