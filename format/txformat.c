#include <stdio.h>

int main(int argc, char *argv[]) {
	
	char filename[100];
	
    if (argc == 5 && strcmp(argv[1], "-i1") == 0 && strcmp(argv[2], "-o1") == 0 && strcmp(argv[3], "-f") == 0) {
		char *filename = argv[4];
		char x;
		FILE *file = fopen(filename, "r");
		while ((x = fgetc(file)) != EOF) {
       		fprintf(stdout, "%02X", x);
    	}
    	fclose(file);
    }
    
	else if (argc == 3 && strcmp(argv[1], "-i2") == 0 && strcmp(argv[2], "-o1") == 0) {
		char x;
		while ((x = fgetc(stdin)) != EOF) {
       		fprintf(stdout, "%02X", x);
    	}
	}
	
	else if (argc == 5 && strcmp(argv[1], "-i1") == 0 && strcmp(argv[2], "-o2") == 0 && strcmp(argv[3], "-f") == 0) {
    	char *filename = argv[4];
    	char x, prev_char = '\0';
    	FILE *file = fopen(filename, "r");

    	while ((x = fgetc(file)) != EOF) {
        	if (isalpha(x)) {
            	if (prev_char != '\0' && !isalpha(prev_char)) {
                	printf("&");
            	}
            	printf("%02X", x);
        	} else if (isdigit(x)) {
            	if (prev_char != '\0' && !isdigit(prev_char)) {
                	printf("&");
            	}
            	printf("%02X", x);
        	} else {
            	printf("&%02X", x);
       	 	}
        	prev_char = x;
    	}
    	fclose(file);
	}
    
	else if (argc == 3 && strcmp(argv[1], "-i2") == 0 && strcmp(argv[2], "-o2") == 0) {
		char x;
		char prev_char = '\0';
		while ((x = fgetc(stdin)) != EOF) {
       		if (isalpha(x)) {
            	if (prev_char != '\0' && !isalpha(prev_char)) {
                	printf("&");
            	}
            	printf("%02X", x);
        	} else if (isdigit(x)) {
            	if (prev_char != '\0' && !isdigit(prev_char)) {
                	printf("&");
            	}
            	printf("%02X", x);
        	} else {
            	printf("&%02X", x);
       	 	}
        	prev_char = x;
    	}
	}
	
	else {
        printf("Incorrect usage. Please use:\n", argv[0]);
        printf("%s -i1 -o1 -f [filename].txt\n", argv[0]);
        printf("%s -i1 -o2 -f [filename].txt\n", argv[0]);
        printf("type [filename].txt | %s -i2 -o1\n", argv[0]);
        printf("type [filename].txt | %s -i2 -o2", argv[0]);   
    }
	
    return 0;
}
