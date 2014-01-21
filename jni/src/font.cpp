#include <SDL.h>
#include "font.h"

const SDL_Color Font::bg_color = 
	//{50, 50, 100, 255};
	{255, 255, 255, 255};

Font::Font(const char *file, SDL_Renderer *renderer)
: texture(0)
{
    SDL_Surface *surface = SDL_LoadBMP(file);

    if (!surface) 
    {
        SDL_Log("Error loading bitmap: %s", SDL_GetError());
    } 
    else 
    {
        SDL_SetColorKey(surface, 1, SDL_MapRGB(surface->format, 238, 0, 252));
        int format = SDL_PIXELFORMAT_ABGR8888;
        Uint32 Rmask, Gmask, Bmask, Amask;
        int bpp;
        SDL_PixelFormatEnumToMasks(format, &bpp, &Rmask, &Gmask, &Bmask, &Amask);
        SDL_Surface *converted = SDL_CreateRGBSurface(0, surface->w, surface->h, bpp, Rmask, Gmask, Bmask, Amask);
        SDL_BlitSurface(surface, NULL, converted, NULL);
        texture = SDL_CreateTextureFromSurface(renderer, converted);
        if (!texture) 
        {
            SDL_Log("texture creation failed: %s", SDL_GetError());
        }
        else 
        {
            SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
        }
        SDL_FreeSurface(surface);
        SDL_FreeSurface(converted);
    }
}

void Font::drawIndex(SDL_Renderer *renderer, int index, int numChars, int x_padding, int y_padding, bool is_blank_bg, SDL_Rect &canvasRect, SDL_Rect &viewRect)
{
	float x_percent = (float)canvasRect.w / (float)viewRect.w;
	float y_percent = (float)canvasRect.h / (float)viewRect.h;
	int dx = (int)(canvasRect.x + x_percent * (numChars * GLYPH_SIZE_SCREEN + x_padding));
    int dy = (int)(canvasRect.y + y_percent * y_padding);
    int dw = (int)(x_percent * GLYPH_SIZE_SCREEN);
    int dh = (int)(y_percent * GLYPH_SIZE_SCREEN);
    SDL_Rect srcRect = { GLYPH_SIZE_IMAGE * index, 0, GLYPH_SIZE_IMAGE, GLYPH_SIZE_IMAGE };
    SDL_Rect dstRect = { dx, dy, dw, dh };
    if (is_blank_bg)
    {
		SDL_SetRenderDrawColor(renderer, bg_color.r, bg_color.g, bg_color.b, bg_color.a);
		SDL_RenderFillRect(renderer, &dstRect);
    }
    SDL_RenderCopy(renderer, texture, &srcRect, &dstRect);
}

void Font::drawString(SDL_Renderer *renderer, const char* str, int x_padding, int y_padding, bool is_blank_bg, SDL_Rect &canvasRect, SDL_Rect &viewRect)
{
	typedef struct
	{
		char scancode;
		int index;
	} fontMapping;
	#define TABLE_SIZE 51
	const fontMapping map[TABLE_SIZE] = 
	{
		{' ', 0}, /* ' ' */
		{'!', 1}, /* ! */
		{'"', 2}, /* " */
		
		
		{'%', 5}, /* % */
		
		{'\'', 7}, /* ' */
		 
		 
		
		{'+', 11}, /* = */
		{',', 12}, /* , */
		{'-', 13}, /* - */
		{'.', 14}, /* . */
		{'/', 15}, /* / */

		{'0', 16}, /* 0 */
		{'1', 17}, /* 1 */
		{'2', 18}, /* 2 */
		{'3', 19}, /* 3 */
		{'4', 20}, /* 4 */
		{'5', 21}, /* 5 */
		{'6', 22}, /* 6 */
		{'7', 23}, /* 7 */
		{'8', 24}, /* 8 */
		{'9', 25}, /* 9 */

		{':', 26}, /* : */
		{';', 27}, /* ; */
		
		{'|', 29},
		
		{'?', 31}, /* ? */
		
		{'A', 33}, /* A */
		{'B', 34}, /* B */
		{'C', 35}, /* C */
		{'D', 36}, /* D */
		{'E', 37}, /* E */
		{'F', 38}, /* F */
		{'G', 39}, /* G */
		{'H', 40}, /* H */
		{'I', 41}, /* I */
		{'J', 42}, /* J */
		{'K', 43}, /* K */
		{'L', 44}, /* L */
		{'M', 45}, /* M */
		{'N', 46}, /* N */
		{'O', 47}, /* O */
		{'P', 48}, /* P */
		{'Q', 49}, /* Q */
		{'R', 50}, /* R */
		{'S', 51}, /* S */
		{'T', 52}, /* T */
		{'U', 53}, /* U */
		{'V', 54}, /* V */
		{'W', 55}, /* W */
		{'X', 56}, /* X */
		{'Y', 57}, /* Y */
		{'Z', 58}, /* Z */
		
		{'\0', -1},
	};

	const char* strp;
	int i;
	for (strp = str; *strp != 0; ++strp)
	{
		int index = -1;
		char ch = *strp;
		for (i = 0; i < TABLE_SIZE && map[i].index >= 0; i++)
		{
			if (map[i].scancode == ch)
			{
				index = map[i].index;
				break;
			}
		}
		if (index != -1)
		{
			drawIndex(renderer, index, strp - str, x_padding, y_padding, is_blank_bg, canvasRect, viewRect);
		}
	}
}
