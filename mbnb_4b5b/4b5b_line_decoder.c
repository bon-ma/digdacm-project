#include <stdlib.h>
#include <string.h>
#include <stdio.h>

const char* decodeTable[] = 
{
    "0000", "0001", "0010", "0011", // 0-3
    "0100", "0101", "0110", "0111", // 4-7
    "1000", "1001", "1010", "1011", // 8-11
    "1100", "1101", "1110", "1111"  // 12-15
};

// 4b5b decoding 5->4 (direct mapping) 
void decode5b4b(const char* input, char* output) {
    switch (input[0]) {
        case '1':
            if (strcmp(input, "11110") == 0) strcpy(output, "0000");
            else if (strcmp(input, "01001") == 0) strcpy(output, "0001");
            else if (strcmp(input, "10100") == 0) strcpy(output, "0010");
            else if (strcmp(input, "10101") == 0) strcpy(output, "0011");
            else if (strcmp(input, "01010") == 0) strcpy(output, "0100");
            else if (strcmp(input, "01011") == 0) strcpy(output, "0101");
            else if (strcmp(input, "01110") == 0) strcpy(output, "0110");
            else if (strcmp(input, "01111") == 0) strcpy(output, "0111");
            else if (strcmp(input, "10010") == 0) strcpy(output, "1000");
            else if (strcmp(input, "10011") == 0) strcpy(output, "1001");
            else if (strcmp(input, "10110") == 0) strcpy(output, "1010");
            else if (strcmp(input, "10111") == 0) strcpy(output, "1011");
            else if (strcmp(input, "11010") == 0) strcpy(output, "1100");
            else if (strcmp(input, "11011") == 0) strcpy(output, "1101");
            else if (strcmp(input, "11100") == 0) strcpy(output, "1110");
            else if (strcmp(input, "11101") == 0) strcpy(output, "1111");
            break;
    }
}

// main mbnb decoder
void mbnbDecode() {
    char input;
    char tempInput[6] = {'\0'};  // holding 5-bits as a string
    char decodedOutput[5];      

    // read input binary char 1 at a time
    while ((input = getchar()) != EOF) { 
        if (input == '0' || input == '1') { 
            // Append the bit to the tempInput array
            strncat(tempInput, &input, 1); 

            if (strlen(tempInput) == 5) { 
                decode5b4b(tempInput, decodedOutput); // decode the 5-bit input
                fputs(decodedOutput, stdout);         /`
                strcpy(tempInput, ""); // reset 
            }
        }
    }

    // remaining bits when != multiple of 5
    if (strlen(tempInput) > 0) {
        fprintf(stderr, "Input length is not a multiple of 5 (decoding).\n");
    }
}

// main function
int main(int argc, char *argv[]) {
    mbnbDecode();  // decoder 

    return 0;
}

