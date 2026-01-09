#pragma once
#include "utils.h"
#include "renderState.h"

static void RenderStateInit();
void ClearScreen(u32 color);
void PixelDrawRect(int posX, int posY, int sizeX, int sizeY, u32 color);
void PixelDrawRectEmpty(int posX, int posY, int sizeX, int sizeY, u32 border, u32 color);
void DrawRect(float posX, float posY, float halfSizeX, float halfSizeY, u32 color);
void DrawRectScaling(float posX, float posY, float halfSizeX, float halfSizeY, u32 color);