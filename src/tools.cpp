#include <algorithm>
#include <vector>
#include <limits>
#include <fstream>
#include "SDL3/SDL.h"
#include "tools.h"
#include "error.h"
#include "save.h"

namespace TA
{
    SDL_Window *window;
    SDL_Renderer *renderer;

    int screenWidth, screenHeight, scaleFactor;
    double elapsedTime;

    std::string levelPath = "", previousLevelPath = "";
    std::set<std::string> arguments;

    namespace random
    {
        unsigned long long x = 1;
    }

    namespace eventLog
    {
        std::ifstream input;
        std::ofstream output;
    }
}

void TA::drawScreenRect(int r, int g, int b, int a)
{
    SDL_FRect rect;
    rect.x = rect.y = 0;
    rect.w = rect.h = 10000;

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

void TA::random::init(unsigned long long seed)
{
    seed = std::max(seed, 1ull);
    x = seed;
    for(int it = 0; it < 10; it ++) {
        next();
    }
}

long long TA::random::next()
{
    x ^= (x << 13);
    x ^= (x >> 7);
    x ^= (x << 17);
    return (long long)(x % max());
}

long long TA::random::max()
{
    return std::numeric_limits<long long>::max();
}

double TA::linearInterpolation(double left, double right, double pos)
{
    pos = fmod(pos, 2);
    if(pos < 1) {
        return left + (right - left) * pos;
    }
    return right - (right - left) * (pos - 1);
}
