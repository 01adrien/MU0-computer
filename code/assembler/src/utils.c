#include "includes/utils.h"

char*
int_to_bin(uint32_t num, int n) {
    char* address = malloc(n + 1);
    if (n == 4) {
        strcpy(address, "0000");
    } else if (n == 28) {
        strcpy(address, "0000000000000000000000000000");
    }

    int i = 0;
    int j = n - 1;
    while (num > 0) {
        address[j] = (num & 1) + '0';
        num = num >> 1;
        i++;
        j--;
    }
    return address;
}

char*
read_file(const char* path) {
    FILE* file = fopen(path, "rb");
    if (file == NULL) {
        fprintf(stderr, "Could not open file \"%s\".\n", path);
        exit(74);
    }
    fseek(file, 0L, SEEK_END);
    size_t fileSize = ftell(file);
    rewind(file);
    char* buffer = (char*)malloc(fileSize + 1);
    if (buffer == NULL) {
        fprintf(stderr, "Not enough memory to read \"%s\".\n", path);
        exit(74);
    }
    size_t bytesRead = fread(buffer, sizeof(char), fileSize, file);
    if (bytesRead < fileSize) {
        fprintf(stderr, "Could not read file \"%s\".\n", path);
        exit(74);
    }
    buffer[bytesRead] = '\0';
    fclose(file);
    return buffer;
}
