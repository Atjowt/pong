#include "game.h"

#include <math.h>

#include "util.h"

#define PADDLE_WIDTH 16
#define PADDLE_HEIGHT 72

#define PADDLE_SPEED 196
#define BALL_SPEED 256

#define BALL_SIZE 16

#define WALL_OFFSET 32

typedef struct Paddle {
    Rectangle rect;
    int dir;
} Paddle;

typedef struct Ball {
    Rectangle rect;
    float dx, dy;
} Ball;

Paddle p1, p2;
Ball ball;

void initPaddle(Paddle* p, float x);
void initBall(Ball* b);
void movePaddle(Paddle* p, float dt);
void moveBall(Ball* b, float dt);

void gameInit(void) {
    initPaddle(&p1, -GAME_WIDTH * 0.5f + 0.5f * PADDLE_WIDTH + WALL_OFFSET);
    initPaddle(&p2, GAME_WIDTH * 0.5f - 0.5f * PADDLE_WIDTH - WALL_OFFSET);
    initBall(&ball);
}

void gameUpdate(float dt) {
    p1.dir = isKeyDown(GLFW_KEY_W) - isKeyDown(GLFW_KEY_S);
    p2.dir = isKeyDown(GLFW_KEY_UP) - isKeyDown(GLFW_KEY_DOWN);
    movePaddle(&p1, dt);
    movePaddle(&p2, dt);
    moveBall(&ball, dt);
}

void gameDraw(void) {
    drawRectangle(ball.rect);
    drawRectangle(p1.rect);
    drawRectangle(p2.rect);
}

void initPaddle(Paddle* p, float x) {
    *p = (Paddle) {
        .rect = {
            .x = x, .y = 0.0f,
            .width = PADDLE_WIDTH, .height = PADDLE_HEIGHT
        },
        .dir = 0
    };
}

void initBall(Ball* b) {
    float angle = rand01() * 6.28f;
    *b = (Ball) {
        .rect = {
            .x = 0.0f, .y = 0.0f,
            .width = BALL_SIZE, .height = BALL_SIZE
        },
        .dx = cosf(angle), .dy = sinf(angle)
    };
}

void movePaddle(Paddle* p, float dt) {
    
    p->rect.y += PADDLE_SPEED * p->dir * dt;

    static float minY = -GAME_HEIGHT * 0.5f + PADDLE_HEIGHT * 0.5f;
    static float maxY = GAME_HEIGHT * 0.5f - PADDLE_HEIGHT * 0.5f;

    if (p->rect.y < minY) p->rect.y = minY;
    if (p->rect.y > maxY) p->rect.y = maxY;
}

void moveBall(Ball* b, float dt) {

    b->rect.x += BALL_SPEED * b->dx * dt;
    b->rect.y += BALL_SPEED * b->dy * dt;

    static float minX = -GAME_WIDTH * 0.5f + BALL_SIZE * 0.5f;
    static float maxX = GAME_WIDTH * 0.5f - BALL_SIZE * 0.5f;
    static float minY = -GAME_HEIGHT * 0.5f + BALL_SIZE * 0.5f;
    static float maxY = GAME_HEIGHT * 0.5f - BALL_SIZE * 0.5f;

    if (ball.rect.x < minX) {
        ball.rect.x = minX;
        ball.dx = -ball.dx;
    }

    if (ball.rect.x > maxX) {
        ball.rect.x = maxX;
        ball.dx = -ball.dx;
    }

    if (ball.rect.y < minY) {
        ball.rect.y = minY;
        ball.dy = -ball.dy;
    }

    if (ball.rect.y > maxY) {
        ball.rect.y = maxY;
        ball.dy = -ball.dy;
    }
}