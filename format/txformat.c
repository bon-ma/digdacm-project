#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void hex_dump_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    int c;
    while ((c = fgetc(file)) != EOF) {
        printf("%02X", c);
    }

    fclose(file);
}

void hex_dump_word_based_input() {
    char c;
    while ((c = getchar()) != EOF) {
        if (c == '\n') {
            // Handle newline character here, e.g., ignore it or print a specific output
        } else {
            printf("%02X", c);
        }
    }
}

void encode_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    int c;
    while ((c = fgetc(file)) != EOF) {
        if (isalnum(c)) {
            printf("%02X", c);
        } else {
            printf("0411%02X", c);
        }
    }

    fclose(file);
}

void encode_word_based_input() {
    int c;
    int i = 0;
    
    while ((c = getchar()) != EOF) {
        if (isalnum(c)) {
            printf("%02X", c);
        } else {
            printf("0411%02X", c);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc == 5 && strcmp(argv[1], "-i1") == 0 && strcmp(argv[2], "-o1") == 0 && strcmp(argv[3], "-f") == 0) {
        hex_dump_file(argv[4]);
    } else if (argc == 3 && strcmp(argv[1], "-i2") == 0 && strcmp(argv[2], "-o1") == 0) {
        hex_dump_word_based_input();
    } else if (argc == 5 && strcmp(argv[1], "-i1") == 0 && strcmp(argv[2], "-o2") == 0 && strcmp(argv[3], "-f") == 0) {
        encode_file(argv[4]);
    } else if (argc == 3 && strcmp(argv[1], "-i2") == 0 && strcmp(argv[2], "-o2") == 0) {
        encode_word_based_input();
    } else {
    	printf("Incorrect usage. Please use:\n", argv[0]);
        printf("%s -i1 -o1 -f [filename].txt\n", argv[0]);
        printf("%s -i1 -o2 -f [filename].txt\n", argv[0]);
        printf("type [filename].txt | %s -i2 -o1\n", argv[0]);
        printf("type [filename].txt | %s -i2 -o2", argv[0]);  
    }

    return 0;
}
