#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#include <SDL.h>
#include "game.h"
#include "sprite.h"
#include "font.h"
#include "main.h"

static void init(Game *game)
{
	if (game)
	{
		if (!game->load_data(DATAFILE))
		{
			game->new_stage();
		}
	}
}

/* Call this instead of exit(), so we can clean up SDL: atexit() is evil. */
static void quit(int rc, Game *game)
{
	if (game)
	{
		game->save_data(DATAFILE);
		delete game;
	}
    exit(rc);
}

void drawLine(SDL_Renderer* renderer, const Game &game, const Picture& a, const Picture& b, SDL_Rect &canvasRect, SDL_Rect &viewRect)
{
	float x_percent = (float)canvasRect.w / (float)viewRect.w;
	float y_percent = (float)canvasRect.h / (float)viewRect.h;
	int x1 = (int)(canvasRect.x + x_percent * (game.marginX + a.x * game.blockWidth + game.blockWidth / 2));
	int y1 = (int)(canvasRect.y + y_percent * (game.marginY + a.y * game.blockHeight + game.blockHeight / 2));
	int x2 = (int)(canvasRect.x + x_percent * (game.marginX + b.x * game.blockWidth + game.blockWidth / 2));
	int y2 = (int)(canvasRect.y + y_percent * (game.marginY + b.y * game.blockHeight + game.blockHeight / 2));
	SDL_SetRenderDrawColor(renderer, 0xff, 0x00, 0x00, 0xff);
	SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

void fillRect(SDL_Renderer* renderer, SDL_Rect *rect)
{
	SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
	SDL_RenderFillRect(renderer, rect);
}

void drawButton(SDL_Renderer *renderer, Font &font, const char *text, int dx, int dy, int dw, int dh, SDL_Rect &canvasRect, SDL_Rect &viewRect)
{
	float x_percent = (float)canvasRect.w / (float)viewRect.w;
	float y_percent = (float)canvasRect.h / (float)viewRect.h;
	int bx = (int)(canvasRect.x + x_percent * dx);
	int by = (int)(canvasRect.y + y_percent * dy);
	int bw = (int)(dw * x_percent);
	int bh = (int)(dh * y_percent);
	SDL_Rect destRect = {bx, by, bw, bh};
	SDL_SetRenderDrawColor(renderer, 0xCC, 0xCC, 0xCC, 0xff);
	SDL_RenderFillRect(renderer, &destRect);
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff);
	SDL_RenderDrawRect(renderer, &destRect);
	
	int textHeight = Font::GLYPH_SIZE_SCREEN;
	int textWidth = SDL_strlen(text) * Font::GLYPH_SIZE_SCREEN;
	int textX = dx + (dw - textWidth) / 2;
	int textY = dy + (dh - textHeight) / 2;
	
	font.drawString(renderer, text, textX, textY, false, canvasRect, viewRect);
}

void drawBox(SDL_Renderer *renderer, const Game &game, SDL_Rect &canvasRect, SDL_Rect &viewRect)
{
	if (game.preClickX > 0 && game.preClickY > 0)
	{
		int px = (game.preClickX - Game::marginX) / game.blockWidth; 
		int py =  (game.preClickY - Game::marginY) / game.blockHeight;
		
		if (px > 0 && px < game.lenx && py >= 0 && py < game.leny)
		{
			int dx = game.marginX + px * game.blockWidth; 
			int dy = game.marginY + py * game.blockHeight; 
			int dw = game.blockWidth; 
			int dh = game.blockHeight;
			
			float x_percent = (float)canvasRect.w / (float)viewRect.w;
			float y_percent = (float)canvasRect.h / (float)viewRect.h;
			int bx = (int)(canvasRect.x + x_percent * dx);
			int by = (int)(canvasRect.y + y_percent * dy);
			int bw = (int)(dw * x_percent);
			int bh = (int)(dh * y_percent);
			
			SDL_Rect destRect = {bx, by, bw, bh};
			SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xff, 0xff);
			SDL_RenderDrawRect(renderer, &destRect);
		}
	}
}

Uint32 _timerTestCallback(Uint32 interval, void *param)
{
	SDL_Log("_timerTestCallback");
	return interval;
}

void handleMouseButtonDown(int x, int y, Game *game)
{
    //SDL_Log("mouse down : %d, %d", x, y);
    if (!game->show_menu)
    {
		game->on_point_down(x, y);
	}
	else
	{
		if (x > game->buttonX && x < game->buttonX + game->buttonW)
		{
			if (y > game->buttonY - 2 * game->buttonH &&
				y < game->buttonY - 2 * game->buttonH + game->buttonH)
			{
				SDL_Log("button1 down : return");
				game->show_menu = false;
			}
			else if (y > game->buttonY && 
			    y < game->buttonY + game->buttonH)
			{
				SDL_Log("button2 down : new");
				game->new_stage();
				game->show_menu = false;
			}
			else if (y > game->buttonY + 2 * game->buttonH &&
				y < game->buttonY + 2 * game->buttonH + game->buttonH)
			{
				SDL_Log("button3 down : exit");
				quit(0, game);
			}
		}
	}
}

void render(SDL_Renderer *renderer, SDL_Window *window, Game &game, Sprite** sprites, int sprites_size, Font &font, SDL_Rect &canvasRect, SDL_Rect &viewRect)
{
	/* Draw a gray background */
	SDL_SetRenderDrawColor(renderer, 0xA0, 0xA0, 0xA0, 0xFF);
	SDL_RenderClear(renderer);
	
	int w, h;
	SDL_GetWindowSize(window, &w, &h);
	//x = w/2 - sprite.w/2;
	//y = h/2 - sprite.h/N /2;
	
	if (w > 0 && h > 0)
	{
		float dx, dy, dw, dh;
		float sw = VIEW_WIDTH;
		float sh = VIEW_HEIGHT;
		if (sw / w > sh / h) {
			dh = sh / sw * w;
			dw = w;
		} else {
			dw = sw / sh * h;
			dh = h;
		}
		dx = 0 + w / 2 - dw / 2;
		dy = 0 + h / 2 - dh / 2;
		canvasRect.x = (int)dx;
		canvasRect.y = (int)dy;
		canvasRect.w = (int)dw;
		canvasRect.h = (int)dh;
		fillRect(renderer, &canvasRect);
		
		int i, j;
		for (i = 0; i < game.leny; ++i)
		{
			for (j = 0; j < game.lenx; ++j)
			{
				if (game.picArray[i][j].visible == true) {
					sprites[game.stage % sprites_size]->draw(renderer, game.picArray[i][j].type, Game::N, 
						game.marginX + j * game.blockWidth, 
						game.marginY + i * game.blockHeight, 
						game.blockWidth, game.blockHeight,
						canvasRect, viewRect);
				}
			}
		}
		//draw(window, renderer, sprite, 1, x, y);
		//drawLine(renderer, game.picArray[0][0], game.picArray[0][1]);
		for (i = 0; i < game.linesSize; i++)
		{
			int x1 = game.lines[i].x1;
			int y1 = game.lines[i].y1;
			int x2 = game.lines[i].x2;
			int y2 = game.lines[i].y2;
			drawLine(renderer, game, 
				game.picArray[y1][x1], 
				game.picArray[y2][x2], 
				canvasRect, viewRect);
		}
		char str[255] = "";
		sprintf(str, "SCORE:%d STAGE:%d", game.score, game.stage);
		font.drawString(renderer, str, 
			Game::marginX, VIEW_HEIGHT - Font::GLYPH_SIZE_SCREEN - 3, 
			false, canvasRect, viewRect);
		
		drawBox(renderer, game, canvasRect, viewRect);
		
		if (game.show_menu)
		{
			game.buttonX = VIEW_WIDTH / 4;
			game.buttonY = (VIEW_HEIGHT - Font::GLYPH_SIZE_SCREEN) / 2;
			game.buttonH = Font::GLYPH_SIZE_SCREEN * 2;
			game.buttonW = VIEW_WIDTH / 2;
			
			drawButton(renderer, font, "RETURN", 
				game.buttonX, game.buttonY - game.buttonH * 2, 
				game.buttonW, game.buttonH, 
				canvasRect, viewRect);
				
			drawButton(renderer, font, "NEW", 
				game.buttonX, game.buttonY, 
				game.buttonW, game.buttonH, 
				canvasRect, viewRect);
				
			drawButton(renderer, font, "EXIT", 
				game.buttonX, game.buttonY + game.buttonH * 2, 
				game.buttonW, game.buttonH, 
				canvasRect, viewRect);
		}
	}
	
	/* Update the screen! */
	SDL_RenderPresent(renderer);
}

int main(int argc, char *argv[])
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    Uint32 startFrame, endFrame, delay;
    Game *game = new Game();
    
#ifdef __ANDROID__
	//SDL_SetHint("SDL_HINT_ORIENTATIONS", "LandscapeLeft LandscapeRight");
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengles");
#endif

#if USE_TIMER
    if (SDL_Init(SDL_INIT_TIMER) != 0)
    {
        SDL_Log("Unable to initialize SDL:  %s", SDL_GetError());
        quit(2, game);
    } 
#endif

    if (SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer) < 0)
    {
        quit(2, game);
    }    
    init(game);
    
	Font font(DATAPATH "kromasky_16x16.bmp", renderer);
	//Sprite sprite(DATAPATH /*"image.bmp"*/ "click1.bmp", renderer);
    Sprite spriteSport(DATAPATH "sport.bmp", renderer);
    if (spriteSport.texture == NULL)
    {
		SDL_Log("load sport.bmp failed");
        quit(2, game);
    }
    Sprite spriteFruit(DATAPATH "fruit.bmp", renderer);
    if (spriteFruit.texture == NULL)
    {
		SDL_Log("load fruit.bmp failed");
        quit(2, game);
    }
    Sprite spriteFood(DATAPATH "food.bmp", renderer);
    if (spriteFood.texture == NULL)
    {
		SDL_Log("load food.bmp failed");
        quit(2, game);
    }
    Sprite spriteAnimal(DATAPATH "animal.bmp", renderer);
    if (spriteAnimal.texture == NULL)
    {
		SDL_Log("load animal.bmp failed");
        quit(2, game);
    }
    const int sprites_size = 4;
    Sprite *sprites[sprites_size] = {&spriteSport, &spriteFruit, &spriteFood, &spriteAnimal};
    
#if USE_TIMER
    Uint32 interval = 500;
    SDL_AddTimer(interval, _timerTestCallback, NULL);
#endif

    /* Main render loop */
    Uint8 done = 0;
    SDL_Event event;
    
    SDL_Rect canvasRect = {0, 0, VIEW_WIDTH, VIEW_HEIGHT};
	SDL_Rect viewRect = {0, 0, VIEW_WIDTH, VIEW_HEIGHT};
    //paint_black(picArray[0][0]);
    while(!done)
	{
		int px, py;
		float x_percent = (float)canvasRect.w / (float)viewRect.w;
		float y_percent = (float)canvasRect.h / (float)viewRect.h;
		
		startFrame = SDL_GetTicks();
        /* Check for events */
        while(SDL_PollEvent(&event))
		{
            switch (event.type)
			{
			case SDL_MOUSEBUTTONDOWN:
				{
					int x, y;
					SDL_GetMouseState(&x, &y);
					SDL_Log("Mouse: button %d pressed at %d,%d in window %d",
							event.button.button, event.button.x, event.button.y,
							event.button.windowID);
					px = (int)((x - canvasRect.x) / x_percent);
					py = (int)((y - canvasRect.y) / y_percent);
					handleMouseButtonDown(px, py, game);
				}
				break;
				
			case SDL_FINGERDOWN:
				{
					SDL_Log("Finger: %s touch=%ld, finger=%ld, x=%f, y=%f, dx=%f, dy=%f, pressure=%f",
							(event.type == SDL_FINGERDOWN) ? "down" : "up",
							(long) event.tfinger.touchId,
							(long) event.tfinger.fingerId,
							event.tfinger.x, event.tfinger.y,
							event.tfinger.dx, event.tfinger.dy, event.tfinger.pressure);
					px = (int)((event.tfinger.x - canvasRect.x) / x_percent);
					py = (int)((event.tfinger.y - canvasRect.y) / y_percent);
					handleMouseButtonDown(px, py, game);
				}
				break;
				
			case SDL_KEYDOWN:
				SDL_Log("Keyboard: key pressed  in window %d: scancode 0x%08X = %s, keycode 0x%08X = %s",
					event.key.windowID,
					event.key.keysym.scancode,
					SDL_GetScancodeName(event.key.keysym.scancode),
					event.key.keysym.sym, SDL_GetKeyName(event.key.keysym.sym));
				switch (event.key.keysym.sym) 
				{
				case SDLK_AC_BACK:
				case SDLK_ESCAPE:
					//done = 1;
					game->show_menu = !game->show_menu;
					break;
				}
				break;
				
			case SDL_QUIT:
                SDL_Log("SDL_QUIT");
                done = 1;
                quit(0, game);
                break;
                
            case SDL_APP_WILLENTERBACKGROUND:
                SDL_Log("SDL_APP_WILLENTERBACKGROUND");
                done = 1;
                quit(0, game);
                break;
                
            default:
				//SDL_Log("default event : %d ", event.type);
				break;
            }
        }
		
		//SDL_Log("render", event.type);
		render(renderer, window, *game, sprites, 4, font, canvasRect, viewRect);
        endFrame = SDL_GetTicks();
        
        delay = MILLESECONDS_PER_FRAME - (endFrame - startFrame);
        if (delay < 0) {
            delay = 0;
            SDL_Log("delay == 0");
        } else if (delay > MILLESECONDS_PER_FRAME) {
            delay = MILLESECONDS_PER_FRAME;
        }
        SDL_Delay(delay);
    }

    quit(0, game);
    
    return 0;
}
