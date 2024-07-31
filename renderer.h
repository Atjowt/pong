#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

typedef struct RendererArgs {
    float clearColor[4];
    unsigned maxBatchSize;
    GLuint shaderProgram;
    float renderWidth;
    float renderHeight;
} RendererArgs;

typedef struct Rectangle {
    float x, y, width, height;
} Rectangle;

void initRenderer(RendererArgs args);
void drawRectangle(Rectangle rectangle);
void beginDraw(void);
void endDraw(void);
