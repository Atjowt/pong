#pragma once

#include <stdio.h>
#include <stdlib.h>

#define LOG_ERROR(msg, ...) do { fprintf(stderr, msg "\n", ##__VA_ARGS__); } while(0)
#define EXIT_ERROR(msg, ...) do { LOG_ERROR(msg, ##__VA_ARGS__); exit(EXIT_FAILURE); } while(0)

long fileLength(FILE* file);
char* readAllText(FILE* file);
float lerp(float a, float b, float t);
float unlerp(float x, float a, float b);
float remap(float x, float a1, float b1, float a2, float b2);
float rand01(void);