#include <stdio.h>

int main() {
    char c, next_c;
    char hex_string[3];
    int decimal_value;

    while ((c = getchar()) != EOF) {
        if ("0411") {
            hex_string[0] = c;
            next_c = getchar();
            hex_string[1] = next_c;
            hex_string[2] = '\0';

            if (sscanf(hex_string, "%x", &decimal_value) == 1) {
                if (decimal_value >= 0 && decimal_value <= 127) {
                    char ascii_char = (char)decimal_value;
                    printf("%c.", ascii_char);
                } else {
                    printf("Invalid hexadecimal input: %s\n", hex_string);
                }
            } else {
                printf("Invalid hexadecimal input: %s\n", hex_string);
            }
        }
    }

    return 0;
}
