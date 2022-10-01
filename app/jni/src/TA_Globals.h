#ifndef TA_GLOBALS_H
#define TA_GLOBALS_H

#include <SDL.h>

extern SDL_Window *gWindow;
extern SDL_Renderer *gRenderer;
extern SDL_DisplayMode gDisplayMode;

extern int gScreenWidth, gScreenHeight;
extern double gElapsedTime, gWidthMultiplier, gHeightMultiplier;

#endif // TA_GLOBALS_H
