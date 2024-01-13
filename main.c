#include <stdio.h>
#include <stdlib.h>
#include <glad.h>
#include <time.h>
#include <GLFW/glfw3.h>

#include "renderer.h"
#include "input.h"
#include "game.h"
#include "util.h"

#define MAX_SHADER_LOG 512

#define VERTEX_SHADER_PATH "../main.vs"
#define FRAGMENT_SHADER_PATH "../main.fs"

#define WINDOW_INIT_WIDTH 800
#define WINDOW_INIT_HEIGHT 500
#define WINDOW_TITLE "PONG"

GLuint compileShaderFile(GLenum type, FILE* file);
GLuint linkShaderProgram(GLuint vertexShader, GLuint fragmentShader);
GLuint shaderProgramFromFiles(FILE* vertexFile, FILE* fragmentFile);
GLuint shaderProgramFromPaths(const char* vertexPath, const char* fragmentPath);
void updateViewport(int windowWidth, int windowHeight);
void onFramebufferResized(GLFWwindow* window, int width, int height);

GLFWwindow* window;

int main(void) {

    srand(time(NULL));

    if (!glfwInit()) EXIT_ERROR("Error initializing GLFW");

    glfwWindowHint(GLFW_VISIBLE, 0);
    glfwSwapInterval(0);

    window = glfwCreateWindow(WINDOW_INIT_WIDTH, WINDOW_INIT_HEIGHT, WINDOW_TITLE, NULL, NULL);
    if (!window) EXIT_ERROR("Error creating window");

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    int centerX = (mode->width - WINDOW_INIT_WIDTH) / 2;
    int centerY = (mode->height - WINDOW_INIT_HEIGHT) / 2;
    glfwSetWindowPos(window, centerX, centerY);
    glfwShowWindow(window);

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, onFramebufferResized);

    if (!gladLoadGL()) EXIT_ERROR("Error loading OpenGL");

    initRenderer((RendererArgs) {
        .shaderProgram = shaderProgramFromPaths(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH),
        .maxBatchSize = 3,
        .clearColor = { 0.04f, 0.04f, 0.04f, 1.0f },
        .renderWidth = GAME_WIDTH, .renderHeight = GAME_HEIGHT
    });

    updateViewport(WINDOW_INIT_WIDTH, WINDOW_INIT_HEIGHT);

    gameInit();
    float t0 = glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        float t = glfwGetTime();
        float dt = t - t0;
        t0 = t;
        gameUpdate(dt);
        beginDraw();
        gameDraw();
        endDraw();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return EXIT_SUCCESS;
}

GLuint compileShaderFile(GLenum type, FILE* file) {
    GLuint shader = glCreateShader(type);
    GLchar* source = readAllText(file);
    glShaderSource(shader, 1, (const GLchar**)&source, NULL);
    glCompileShader(shader);
    free(source);
    int compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        GLchar infoLog[MAX_SHADER_LOG];
        glGetShaderInfoLog(shader, sizeof(infoLog), NULL, infoLog);
        EXIT_ERROR("%s", infoLog);
    }
    return shader;
}

GLuint linkShaderProgram(GLuint vertexShader, GLuint fragmentShader) {

    GLuint shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    glLinkProgram(shaderProgram);

    int linked;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linked);
    if (!linked) {
        GLchar infoLog[MAX_SHADER_LOG];
        glGetProgramInfoLog(shaderProgram, sizeof(infoLog), NULL, infoLog);
        EXIT_ERROR("%s", infoLog);
    }

    glDetachShader(shaderProgram, vertexShader);
    glDetachShader(shaderProgram, fragmentShader);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

GLuint shaderProgramFromFiles(FILE* vertexFile, FILE* fragmentFile) {
    GLuint vertexShader = compileShaderFile(GL_VERTEX_SHADER, vertexFile);
    GLuint fragmentShader = compileShaderFile(GL_FRAGMENT_SHADER, fragmentFile);
    return linkShaderProgram(vertexShader, fragmentShader);;
}

GLuint shaderProgramFromPaths(const char* vertexPath, const char* fragmentPath) {
    FILE* vertexFile = fopen(vertexPath, "r");
    FILE* fragmentFile = fopen(fragmentPath, "r");
    GLuint shaderProgram = shaderProgramFromFiles(vertexFile, fragmentFile);
    fclose(vertexFile);
    fclose(fragmentFile);
    return shaderProgram;
}

void updateViewport(int windowWidth, int windowHeight) {
    float x, y, width, height;
    static float targetAspect = GAME_WIDTH / (float)GAME_HEIGHT;
    float currentAspect = windowWidth / (float)windowHeight;
    if (currentAspect < targetAspect) {
        width = windowWidth;
        height = width / targetAspect;
    } else {
        height = windowHeight;
        width = height * targetAspect;
    }
    x = (windowWidth - width) * 0.5f;
    y = (windowHeight - height) * 0.5f;
    glViewport(x, y, width, height);
    glScissor(x, y, width, height);
    // setRenderResolution(width, height);
}

void onFramebufferResized(GLFWwindow* window, int width, int height) {
    updateViewport(width, height);
}

bool isKeyDown(int key) {
    return glfwGetKey(window, key);
}