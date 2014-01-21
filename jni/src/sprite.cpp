#include <SDL.h>
#include "sprite.h"

/* Adapted from SDL's testspriteminimal.c */
Sprite::Sprite(const char* file, SDL_Renderer* renderer)
: texture(NULL)
, width(0)
, height(0)
{
	SDL_Surface* temp = SDL_LoadBMP(file);
    if (!temp)
	{
        SDL_Log("Couldn't load %s: %s", file, SDL_GetError());
    }
    width = temp->w;
    height = temp->h;
    texture = SDL_CreateTextureFromSurface(renderer, temp);
    if (!texture)
    {
        SDL_Log("Couldn't create texture: %s", SDL_GetError());
        SDL_FreeSurface(temp);
    }
    SDL_FreeSurface(temp);
}

void Sprite::draw(SDL_Renderer *renderer, int id, int blockNum, int dx, int dy, int dw, int dh, SDL_Rect &canvasRect, SDL_Rect &viewRect)
{
	float x_percent = (float)canvasRect.w / (float)viewRect.w;
	float y_percent = (float)canvasRect.h / (float)viewRect.h;
	int bx = (int)(canvasRect.x + x_percent * dx);
	int by = (int)(canvasRect.y + y_percent * dy);
	int bw = (int)(dw * x_percent);
	int bh = (int)(dh * y_percent);
	
	SDL_Rect srcRect = {0, height / blockNum * id, width, height / blockNum};
	SDL_Rect destRect = {bx, by, bw+1, bh+1};
	SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
}
