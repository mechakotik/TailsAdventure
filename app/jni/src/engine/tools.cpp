#include <algorithm>
#include "SDL.h"
#include "tools.h"
#include "error.h"

namespace TA
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_DisplayMode displayMode;

    int screenWidth, screenHeight;
    double elapsedTime, widthMultiplier, heightMultiplier;
}

std::string TA::readStringFromFile(std::string filename)
{
    SDL_RWops *input = SDL_RWFromFile(filename.c_str(), "r+b");
    if(input == nullptr) {
        handleSDLError("Open %s failed", filename.c_str());
    }
    int dataBytes = SDL_RWseek(input, 0, SEEK_END);
    SDL_RWseek(input, 0, SEEK_SET);
    char* data = (char*)malloc(dataBytes);
    SDL_RWread(input, data, 1, dataBytes);

    std::string str(dataBytes + 1, 0);
    for(int pos = 0; pos < dataBytes; pos ++) {
        str[pos] = data[pos];
    }
    str += '\0';
    return str;
}

void TA::drawScreenRect(int r, int g, int b, int a)
{
    SDL_DisplayMode displayMode;
    SDL_GetCurrentDisplayMode(0, &displayMode);

    SDL_Rect rect;
    rect.x = rect.y = 0;
    rect.w = displayMode.w;
    rect.h = displayMode.h;

    a = std::max(a, 0);
    a = std::min(a, 255);
    SDL_SetRenderDrawColor(TA::renderer, r, g, b, a);
    SDL_RenderFillRect(TA::renderer, &rect);
}

void TA::drawShadow(int factor)
{
    drawScreenRect(0, 0, 0, factor);
}

bool TA::equal(double a, double b)
{
    return std::abs(a - b) < TA::epsilon;
}
