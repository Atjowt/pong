#include "util.h"

long fileLength(FILE* file) {
    long pos = ftell(file);
    fseek(file, 0, SEEK_END);
    long end = ftell(file);
    fseek(file, pos, SEEK_SET);
    return end;
}

char* readAllText(FILE* file) {
    long length = fileLength(file);
    char* string = calloc(length + 1, sizeof(char));
    fread(string, sizeof(char), length, file);
    return string;
}

float lerp(float a, float b, float t) {
    return a + (b - a) * t;
}

float unlerp(float x, float a, float b) {
    return (x - a) / (b - a);
}

float remap(float x, float a1, float b1, float a2, float b2) {
    return lerp(a2, b2, unlerp(x, a1, b1));
}

float rand01(void) {
    return rand() / (float)RAND_MAX;
}