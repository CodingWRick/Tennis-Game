#include "renderer.h"

RenderState renderState;

// Initialize default render state to avoid null dereferences
static void RenderStateInit() {
    if (renderState.width == 0 || renderState.height == 0 || renderState.memory == nullptr) {
        renderState.width = 1060;
        renderState.height = 1060;
        int renderSize = renderState.width * renderState.height * sizeof(u32);
        renderState.memory = VirtualAlloc(0, renderSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

        ZeroMemory(&renderState.bitMapInfo, sizeof(renderState.bitMapInfo));
        renderState.bitMapInfo.bmiHeader.biSize        = sizeof(renderState.bitMapInfo.bmiHeader);
        renderState.bitMapInfo.bmiHeader.biWidth       = renderState.width;
        renderState.bitMapInfo.bmiHeader.biHeight      = -renderState.height;
        renderState.bitMapInfo.bmiHeader.biPlanes      = 1;
        renderState.bitMapInfo.bmiHeader.biBitCount    = 32;
        renderState.bitMapInfo.bmiHeader.biCompression = BI_RGB;
    }
}

void ClearScreen(u32 color) {
    RenderStateInit();

    u32* pixel = (u32*)renderState.memory;
    for (int y = 0; y < renderState.height; y++) {
        for (int x = 0; x < renderState.width; x++) {
            *pixel++ = color;
        }
    }
}

void PixelDrawRect(int posX, int posY, int sizeX, int sizeY, u32 color) {
    if (!renderState.memory || renderState.width <= 0 || renderState.height <= 0) return;

    posX = Clamp(0, posX, renderState.width);
    posY = Clamp(0, posY, renderState.height);
    sizeX = Clamp(0, sizeX, renderState.width);
    sizeY = Clamp(0, sizeY, renderState.height);

    for(int y = posY; y < sizeY; y++) {
        u32* pixel = (u32*)renderState.memory + posX + y * renderState.width;
        for(int x = posX; x < sizeX; x++) {
            *pixel++ = color;
        }
    }
}

void PixelDrawRectEmpty(int posX, int posY, int sizeX, int sizeY, u32 border, u32 color) {
    posX = Clamp(0, posX, renderState.width);
    posY = Clamp(0, posY, renderState.height);
    sizeX = Clamp(0, sizeX, renderState.width);
    sizeY = Clamp(0, sizeY, renderState.height);

    for(int y = posY; y < sizeY; y++) {
        u32* pixel = (u32*)renderState.memory + posX + y * renderState.width;
        for(int x = posX; x < sizeX; x++){
            *pixel++ = (x < posX + border || x >= sizeX - border || y < posY + border || y >= sizeY - border) ? color : *pixel;
        }
    }
}

void DrawRect(float posX, float posY, float halfSizeX, float halfSizeY, u32 color) {
    posX *= renderState.width;
    posY *= renderState.height;
    halfSizeX *= 1060;
    halfSizeY *= 1060;

    posX += renderState.width / 2.f;
    posY += renderState.height / 2.f;

    int x0 = posX - halfSizeX;
    int x1 = posX + halfSizeX;
    int y0 = posY - halfSizeY;
    int y1 = posY + halfSizeY;

    PixelDrawRect(x0, y0, x1, y1, color);
}

void DrawRectScaling(float posX, float posY, float halfSizeX, float halfSizeY, u32 color) {
    int multiplier = Bigger(renderState.width, renderState.height);

    posX *= renderState.width;
    posY *= renderState.height;
    halfSizeX *= multiplier;
    halfSizeY *= multiplier;

    posX += renderState.width / 2.f;
    posY += renderState.height / 2.f;

    int x0 = posX - halfSizeX;
    int x1 = posX + halfSizeX;
    int y0 = posY - halfSizeY;
    int y1 = posY + halfSizeY;

    PixelDrawRect(x0, y0, x1, y1, color);
}