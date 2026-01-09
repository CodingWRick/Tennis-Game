#pragma once
#include "utils.h"
#include "renderState.h"

void ClearScreen(u32 color);
void PixelDrawRect(int posX, int posY, int sizeX, int sizeY, u32 color);
void PixelDrawRectEmpty(int posX, int posY, int sizeX, int sizeY, int border, u32 color);
void DrawRect(float posX, float posY, float halfSizeX, float halfSizeY, u32 color);
void DrawRectScaling(float posX, float posY, float halfSizeX, float halfSizeY, u32 color);