#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <limits.h>
#include <glob.h>

#define SIZE 256
#define NOT_END_OF_INPUT 1
#define ERROR -1
#define SUCCESS 0

int readPattern(char *pattern) {
    printf("Enter pattern: ");
    int patternLength = 0;
    while(NOT_END_OF_INPUT) {
        char currentSymbol = (char)fgetc(stdin);
        if (currentSymbol == EOF || currentSymbol == '\n') {
            break;
        }
        if (currentSymbol == '/') {
            fprintf(stderr, "Symbol '/' is not supported pattern\n");
            return ERROR;
        }
        pattern[patternLength] = currentSymbol;
        patternLength++;
    }
    if (patternLength == 0) {
        fprintf(stderr, "the pattern cannot be empty\n");
        return ERROR;
    }
    pattern[patternLength] = '\0';
    return EXIT_SUCCESS;
}

int print(char *pattern) {
    glob_t result;
    int flag = glob(pattern, GLOB_MARK | GLOB_ERR, NULL, &result);
    if (flag != SUCCESS) {
        printf("%s\n", pattern);
        perror("glob error");
        globfree(&result);
        return ERROR;
    }
    for (int i = 0; i < result.gl_pathc; i++) {
        printf("%s\n", result.gl_pathv[i]);
    }
    globfree(&result);
    return EXIT_SUCCESS;
}

int main(int argc, char* argv[]) {
    char pattern[SIZE];
    if (readPattern(pattern) == ERROR) {
        exit(EXIT_FAILURE);
    }
    if (print(pattern) == ERROR) {
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}
