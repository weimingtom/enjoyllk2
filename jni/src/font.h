#pragma once

class Font
{
public:
	Font(const char *file, SDL_Renderer *renderer);
	void drawIndex(SDL_Renderer *renderer, int index, int numChars, int x_padding, int y_padding, bool is_blank_bg, SDL_Rect &canvasRect, SDL_Rect &viewRect);
	void drawString(SDL_Renderer *renderer, const char* str, int x_padding, int y_padding, bool is_blank_bg, SDL_Rect &canvasRect, SDL_Rect &viewRect);
	
private:
	SDL_Texture* texture;
	static const SDL_Color bg_color;
	
public:
	enum {
		GLYPH_SIZE_IMAGE = 16,
		GLYPH_SIZE_SCREEN = 16,
	};
};
