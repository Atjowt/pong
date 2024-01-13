#pragma once

#include "renderer.h"
#include "input.h"

#define GAME_WIDTH 480
#define GAME_HEIGHT 360

void gameInit(void);
void gameUpdate(float dt);
void gameDraw(void);