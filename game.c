#include "game.h"

#include <math.h>

#include "util.h"

#define PADDLE_WIDTH 16
#define PADDLE_HEIGHT 72

#define PADDLE_SPEED 256
#define BALL_SPEED 256

#define BALL_BOUNCE 1.04f
#define BALL_RANDOM 0.1f
#define BALL_INFLUENCE 0.3f

#define BALL_SIZE 16

#define WALL_OFFSET 0

typedef struct Paddle {
    Rectangle rect;
    int dir;
    int score;
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
void collideBallPaddle(Ball* b, const Paddle* p, int forward);

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
    collideBallPaddle(&ball, &p1, -1);
    collideBallPaddle(&ball, &p2, 1);
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
        .dir = 0,
        .score = 0
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
        p2.score++;
        initBall(b);
    }

    if (ball.rect.x > maxX) {
        p1.score++;
        initBall(b);
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

void collideBallPaddle(Ball* b, const Paddle* p, int forward) {

    float bFront = b->rect.x + forward * b->rect.width * 0.5f;
    float bBack = b->rect.x - forward * b->rect.width * 0.5f;
    float bTop = b->rect.y + b->rect.height * 0.5f;
    float bBottom = b->rect.y - b->rect.height * 0.5f;

    float pFront = p->rect.x - forward * p->rect.width * 0.5f;
    float pBack = p->rect.x + forward * p->rect.width * 0.5f;
    float pTop = p->rect.y + p->rect.height * 0.5f;
    float pBottom = p->rect.y - p->rect.height * 0.5f;

    if (bBottom < pTop && bTop > pBottom) {
        if (bFront * forward > pFront * forward) {
            b->rect.x += pFront - bFront;
            b->dx *= -BALL_BOUNCE;
            b->dy += lerp(-BALL_RANDOM, BALL_RANDOM, rand01());
            b->dy += BALL_INFLUENCE * p->dir;
        }
    }
}