#pragma once

#include <SDL.h>

class Sprite
{
public:
	Sprite(const char* file, SDL_Renderer* renderer);
	void draw(SDL_Renderer *renderer, int id, int blockNum, int dx, int dy, int dw, int dh, SDL_Rect &canvasRect, SDL_Rect &viewRect);
	
public:
	SDL_Texture* texture;
	Uint16 width;
	Uint16 height;
};
