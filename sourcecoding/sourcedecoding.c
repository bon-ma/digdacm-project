#include <stdio.h>
#include <stdlib.h>

struct Node {
    char data;
    struct Node *left, *right;
};

struct Node* createNode(char data) {
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    node->data = data;
    node->left = node->right = NULL;
    return node;
}

struct Node* deserializeHuffmanTree(FILE* fp_in) {
    char ch = fgetc(fp_in);
    if (ch == 'L') {
        ch = fgetc(fp_in);
        struct Node* leaf = createNode(ch);
        return leaf;
    } else if (ch == 'I') {
        struct Node* internal = createNode('$');
        internal->left = deserializeHuffmanTree(fp_in);
        internal->right = deserializeHuffmanTree(fp_in);
        return internal;
    }
    return NULL;
}

void decodeMessage(struct Node* root, const char* encodedFileName, const char* outputFileName) {
    FILE *fp_in = fopen(encodedFileName, "r");
    if (fp_in == NULL) {
        perror("Error opening encoded file");
        exit(1);
    }

    FILE *fp_out = fopen(outputFileName, "w");
    if (fp_out == NULL) {
        perror("Error opening output file");
        exit(1);
    }

    struct Node* current = root;
    char ch;
    while ((ch = fgetc(fp_in)) != EOF) {
        if (ch == '0') {
            current = current->left;
        } else if (ch == '1') {
            current = current->right;
        }

        if (current->left == NULL && current->right == NULL) {
            if (current->data == '/') {
                fputc(' ', fp_out);
            } else {
                fputc(current->data, fp_out);
            }
            current = root;
        }
    }

    fclose(fp_in);
    fclose(fp_out);
}

int main() {
    FILE *fp_tree = fopen("huffman_tree.txt", "r");
    if (fp_tree == NULL) {
        perror("Error opening tree file");
        return 1;
    }

    struct Node* root = deserializeHuffmanTree(fp_tree);
    fclose(fp_tree);

    decodeMessage(root, "sourcecoding_outputfile.txt", "decoded_output.txt");

    return 0;
}
