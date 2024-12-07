#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define BUFFER_SIZE 1024

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
    char buffer[BUFFER_SIZE];
    int length = 0;
    int c;
    int last_char = EOF;
    int is_echo = 0;
    while ((c = getchar()) != EOF && length < BUFFER_SIZE - 1) {
        buffer[length++] = c;
        last_char = c;
    }
    buffer[length] = '\0';

    if (last_char == '\n' && length < 50) {
        is_echo = 1;
        length--;
    }
	
	int i =0;
    
    for (i; i < length; i++) {
        printf("%02X", (unsigned char)buffer[i]);
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
    char buffer[BUFFER_SIZE];
    int length = 0;
    int c;
    int last_char = EOF;
    int is_echo = 0;
    
    while ((c = getchar()) != EOF && length < BUFFER_SIZE - 1) {
        buffer[length++] = c;
        last_char = c;
    }
    buffer[length] = '\0';
    if (last_char == '\n' && length < 50) {
        is_echo = 1;
        length--;
    }
    
    int i = 0;

    for (i; i < length; i++) {
        if (isalnum(buffer[i])) {
            printf("%02X", (unsigned char)buffer[i]);
        } else {
            printf("0411%02X", (unsigned char)buffer[i]);
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
