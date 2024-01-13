#include "renderer.h"
#include "util.h"

GLuint vao, ssbo;
GLuint shaderProgram;

Rectangle* renderBatch;
unsigned maxBatchSize;
unsigned batchSize;

float renderWidth, renderHeight;

void initRenderer(RendererArgs args) {

    glClearColor(args.clearColor[0], args.clearColor[1], args.clearColor[2], args.clearColor[3]);
    maxBatchSize = args.maxBatchSize;
    shaderProgram = args.shaderProgram;
    renderBatch = malloc(maxBatchSize * sizeof(Rectangle));
    renderWidth = args.renderWidth;
    renderHeight = args.renderHeight;

    glUseProgram(shaderProgram);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &ssbo);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, maxBatchSize * sizeof(Rectangle), NULL, GL_DYNAMIC_DRAW);
}

void beginDraw(void) {
    batchSize = 0;
}

void drawRectangle(Rectangle rectangle) {
    if (batchSize >= maxBatchSize) LOG_ERROR("Render batch is full!");
    rectangle.x = remap(rectangle.x, -renderWidth * 0.5f, renderWidth * 0.5f, -1.0f, 1.0f);
    rectangle.y = remap(rectangle.y, -renderHeight * 0.5f, renderHeight * 0.5f, -1.0f, 1.0f);
    rectangle.width = remap(rectangle.width, 0.0f, renderWidth, 0.0f, 2.0f);
    rectangle.height = remap(rectangle.height, 0.0f, renderHeight, 0.0f, 2.0f);
    renderBatch[batchSize++] = rectangle;
}

void endDraw(void) {
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, batchSize * sizeof(Rectangle), renderBatch);
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, batchSize);
}