#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// 4b/5b encoding table
const char* encodeTable[] = 
{
    "11110", "01001", "10100", "10101", 
    "01010", "01011", "01110", "01111",
    "10010", "10011", "10110", "10111", 
    "11010", "11011", "11100", "11101"
};

// direct mapping
void printMapping() 
{
    const char* fourBits[] = 
	{
        "0000", "0001", "0010", "0011", // 0-4
        "0100", "0101", "0110", "0111", // 5-8
        "1000", "1001", "1010", "1011", // 9-12
        "1100", "1101", "1110", "1111" //13 - 16
    };

//    printf("4-bit to 5-bit Mapping:\n"); //print
//    int i;  
//    for (i = 0; i < 16; i++) {  
//    	printf("%s -> %s\n", fourBits[i], encodeTable[i]);
//    }
}

// 4b/5b encoding 4->5 (direct mapping)
void encode4b5b(const char* input, char* output) {
    if (strcmp(input, "0000") == 0) strcpy(output, encodeTable[0]);
    else if (strcmp(input, "0001") == 0) strcpy(output, encodeTable[1]);
    else if (strcmp(input, "0010") == 0) strcpy(output, encodeTable[2]);
    else if (strcmp(input, "0011") == 0) strcpy(output, encodeTable[3]);
    else if (strcmp(input, "0100") == 0) strcpy(output, encodeTable[4]);
    else if (strcmp(input, "0101") == 0) strcpy(output, encodeTable[5]);
    else if (strcmp(input, "0110") == 0) strcpy(output, encodeTable[6]);
    else if (strcmp(input, "0111") == 0) strcpy(output, encodeTable[7]);
    else if (strcmp(input, "1000") == 0) strcpy(output, encodeTable[8]);
    else if (strcmp(input, "1001") == 0) strcpy(output, encodeTable[9]);
    else if (strcmp(input, "1010") == 0) strcpy(output, encodeTable[10]);
    else if (strcmp(input, "1011") == 0) strcpy(output, encodeTable[11]);
    else if (strcmp(input, "1100") == 0) strcpy(output, encodeTable[12]);
    else if (strcmp(input, "1101") == 0) strcpy(output, encodeTable[13]);
    else if (strcmp(input, "1110") == 0) strcpy(output, encodeTable[14]);
    else if (strcmp(input, "1111") == 0) strcpy(output, encodeTable[15]);
}

// main mbnb encoder
void mbnbEncode() {
    char input;
    char tempInput[5] = {'\0'}; //  holding 4 bits as a string
    char encodedOutput[6];     

    // read input from user input or piped input/ echo
    while ((input = getchar()) != EOF) { 
        if (input == '0' || input == '1') { 
            // append the bit to the tempInput array
            strncat(tempInput, &input, 1); 

            if (strlen(tempInput) == 4) { 
                encode4b5b(tempInput, encodedOutput); // encode the 4-bit input
                fputs(encodedOutput, stdout);         
                strcpy(tempInput, ""); // reset 
            }
        }
    }

    // remaining bits when != multiple of 4 
    if (strlen(tempInput) > 0) {
        fprintf(stderr, "Input length is not a multiple of 4 (encoding).\n");
        exit(1);
    }
}

// main function
int main(int argc, char *argv[]) {
//    printMapping(); //if print display output is needed

    
    mbnbEncode();  // encoder

    return 0;
}


