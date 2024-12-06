#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main() {
    char hex_prefix[5]; // Buffer to hold "0411" (4 characters + '\0')
    char hex_char[3];   // Buffer to hold 2-character hexadecimal values
    int decimal_value;

    while (1) {
        // Attempt to read the "0411" prefix
        if ((hex_prefix[0] = getchar()) == EOF) break;
        if (hex_prefix[0] == '0') {
            hex_prefix[1] = getchar();
            hex_prefix[2] = getchar();
            hex_prefix[3] = getchar();
            hex_prefix[4] = '\0';

            if (hex_prefix[1] == '4' && hex_prefix[2] == '1' && hex_prefix[3] == '1') {
                // "0411" prefix detected, now read the actual hexadecimal value
                if ((hex_char[0] = getchar()) != EOF && (hex_char[1] = getchar()) != EOF) {
                    hex_char[2] = '\0'; // Null-terminate the hex string

                    if (sscanf(hex_char, "%x", &decimal_value) == 1) {
                        if (decimal_value >= 0 && decimal_value <= 127) {
                            printf("%c", (char)decimal_value); // Print the ASCII character
                        } else {
                            fprintf(stderr, "Invalid hexadecimal value: %s\n", hex_char);
                        }
                    } else {
                        fprintf(stderr, "Failed to parse hexadecimal value: %s\n", hex_char);
                    }
                }
            } else {
                // Not "0411", put characters back into the stream and process normally
                ungetc(hex_prefix[3], stdin);
                ungetc(hex_prefix[2], stdin);
                ungetc(hex_prefix[1], stdin);
                ungetc(hex_prefix[0], stdin);

                // Process this character as a regular hex pair
                if ((hex_char[0] = getchar()) != EOF && (hex_char[1] = getchar()) != EOF) {
                    hex_char[2] = '\0';

                    if (sscanf(hex_char, "%x", &decimal_value) == 1) {
                        if (decimal_value >= 0 && decimal_value <= 127) {
                            printf("%c", (char)decimal_value);
                        } else {
                            fprintf(stderr, "Invalid hexadecimal value: %s\n", hex_char);
                        }
                    } else {
                        fprintf(stderr, "Failed to parse hexadecimal value: %s\n", hex_char);
                    }
                }
            }
        } else {
            // Process non-"0411" characters as regular hex pairs
            hex_char[0] = hex_prefix[0];
            if ((hex_char[1] = getchar()) != EOF) {
                hex_char[2] = '\0';

                if (sscanf(hex_char, "%x", &decimal_value) == 1) {
                    if (decimal_value >= 0 && decimal_value <= 127) {
                        printf("%c", (char)decimal_value);
                    } else {
                        fprintf(stderr, "Invalid hexadecimal value: %s\n", hex_char);
                    }
                } else {
                    fprintf(stderr, "Failed to parse hexadecimal value: %s\n", hex_char);
                }
            }
        }
    }

    return 0;
}

