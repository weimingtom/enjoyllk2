#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <SDL.h>
#include "main.h"
#include "game.h"

#if USE_MT_RANDOM
#include "mt19937ar.h"
#endif

Game::Game()
: remain_num(lenx * leny)
, score(0)
, stage(0)
, preClickX(-1)
, preClickY(-1)
, linesSize(0)
, save_version(VERSION)
, show_menu(false)
, buttonX(0)
, buttonY(0)
, buttonW(0)
, buttonH(0)
{
	//lenx = 10;
	//leny = 8;
	lenx = 8;
	leny = 6;
	
#if USE_MT_RANDOM
	init_genrand(time(0));
#else
	srand(time(0));
#endif
}

Game::~Game()
{

}

void Game::paint_black(Picture &a) 
{
	a.visible = false;
}

void Game::clear_lines(void)
{
	linesSize = 0;
}

void Game::draw_line(Picture& a, Picture& b)
{
	if (linesSize < lines_size_max - 1)
	{
		lines[linesSize].x1 = a.x;
		lines[linesSize].y1 = a.y;
		lines[linesSize].x2 = b.x;
		lines[linesSize].y2 = b.y;
		
		linesSize++;
	}
}

bool Game::match_direct(Picture& a, Picture& b)
{
	if (!(a.x==b.x || a.y == b.y))
	{
		return false ;
	}
	int i;
	bool match_x = false;
	if (a.x == b.x)
	{
		match_x = true;
		if (a.y > b.y)
		{
			for (i = a.y - 1; i > b.y; --i)
			{
				if (picArray[i][a.x].visible == true)
				{
					match_x = false;
				}
			}
		}
		if (b.y > a.y) 
		{
			for (i = b.y - 1; i > a.y; --i)
			{
				if (picArray[i][a.x].visible == true)
				{
					match_x = false;
				}
			}
		}
	}
	bool match_y = false;
	if (a.y == b.y)
	{
		match_y = true;
		if (a.x > b.x)
		{ 
			for (i = a.x - 1; i > b.x; --i)
			{ 
				if(picArray[a.y][i].visible == true)
				{
					match_y = false;
				}
			}
		}
		if (b.x > a.x)
		{
			for (i = b.x - 1; i > a.x; --i)
			{ 
				if (picArray[a.y][i].visible == true)
				{
					match_y = false;
				}
			}
		}
	}
	if (match_x || match_y)
	{
		return true;
	}
	return false;
}

bool Game::match_one_corner(Picture& a, Picture& b)
{
	if (picArray[b.y][a.x].visible == false &&
		match_direct(a, picArray[b.y][a.x]) &&
		match_direct(b, picArray[b.y][a.x]))
	{
		draw_line(a, picArray[b.y][a.x]);  
		draw_line(b, picArray[b.y][a.x]);
		return true;
	}
	if (picArray[a.y][b.x].visible == false &&
		match_direct(a, picArray[a.y][b.x]) &&
		match_direct(b, picArray[a.y][b.x]))
	{
		draw_line(a, picArray[a.y][b.x]);  
		draw_line(b, picArray[a.y][b.x]);
		return true;
	}
	return false ;
}

bool Game::match_two_corner(Picture& a, Picture& b)
{
	int i;
	for (i = a.x - 1; i >= 0; --i)
	{
		if (picArray[a.y][i].visible == true)
		{
			break;
		}
		else
		{
			if (match_one_corner(picArray[a.y][i], b))
			{
				draw_line(a, picArray[a.y][i]);
				return true;
			}
		}
	}
	for (i = a.x + 1; i < lenx ; ++i)
	{
		if (picArray[a.y][i].visible == true)
		{
			break;
		}
		else
		{
			if (match_one_corner(picArray[a.y][i], b))
			{
				draw_line(a, picArray[a.y][i]);
				return true ;
			}
		}
	}
	for (i = a.y - 1; i >= 0; --i)
	{
		if (picArray[i][a.x].visible == true)
		{
			break;
		}
		else
		{
			if (match_one_corner(picArray[i][a.x], b))
			{
				draw_line(a, picArray[i][a.x]);
				return true;
			}
		}
	}
	for (i = a.y + 1; i < leny; ++i)
	{
		if (picArray[i][a.x].visible == true)
		{
			break;
		}
		else
		{
			if (match_one_corner(picArray[i][a.x], b))
			{
				draw_line(a, picArray[i][a.x]);
				return true;
			}
		}
	}
	return false;
}

bool Game::match(Picture& a, Picture& b) 
{
	clear_lines();
	if (a.type != b.type)
	{
		return false;
	}
	if (match_direct(a, b))
	{
		draw_line(a, b);
		for (int i = 0; i < leny; ++i)
		{
			for (int j = 0; j < lenx; ++j)
			{
				if (picArray[i][j].visible == false)
				{
					paint_black(picArray[i][j]);
				}
			}
		}
		return true ;
	}

	if (match_one_corner(a, b) || match_two_corner(a, b))
	{
		for (int i = 0; i < leny; ++i)
		{
			for (int j = 0; j < lenx; ++j)
			{
				if (picArray[i][j].visible == false)
				{
					paint_black(picArray[i][j]);
				}
			}
		}
		return true;
	}
	return false;
}

void Game::new_stage(void) 
{
	save_version = VERSION;

	blockWidth = (VIEW_WIDTH - 2 * marginX) / lenx;
	blockHeight = (VIEW_HEIGHT - 2 * marginY) / leny;
	if (blockWidth < blockHeight)
	{
		blockHeight = blockWidth;
	}
	else
	{
		blockWidth = blockHeight;
	}
	lines_size_max = lenx * leny;

#if 0
#if USE_MT_RANDOM
	init_genrand(time(0));
#else
	srand(time(0));
#endif
#endif
	int i, j;
	for (i = 0; i < leny; ++i)
	{
		for(j = 0; j < lenx; ++j)
		{
			picArray[i][j].visible = false;
		}
	}
	int x, y; 
	for (i = 0; i < lenx - 1; ++i)
	{
		for (j = 0; j < leny; ++j)
		{
			bool re = true;
			while(re) 
			{
#if USE_MT_RANDOM
				y = genrand_int32() % leny;
				x = genrand_int32() % lenx;
#else
				y = rand() % leny;
				x = rand() % lenx;
#endif
				if(picArray[y][x].visible == false)
				{
					picArray[y][x].type = i; 
					picArray[y][x].visible = true;
					picArray[y][x].x = x; 
					picArray[y][x].y = y;
					re = false;
				}
			}
		}
	}
	for (i = 0; i < leny; ++i)
	{
		for (j = 0; j < lenx; ++j)
		{
			if (picArray[i][j].visible == false)
			{	
				picArray[i][j].type = lenx - 1; 
				picArray[i][j].visible = true;
				picArray[i][j].x = j;
				picArray[i][j].y = i;
			}
		}
	}
	preClickX = -1;
	preClickY = -1;
	remain_num = lenx * leny;
	clear_lines();
}

void Game::on_point_down(int pointX, int pointY) 
{
	if (pointX <= marginX || 
		pointY <= marginY ||
		pointX >= marginX + lenx * blockWidth || 
		pointY >= marginY + leny * blockWidth)
	{
		return;
	}
	int x = (pointX - marginX) / blockWidth;
	int y = (pointY - marginY) / blockHeight; 
	int px = (preClickX - marginX) / blockWidth; 
	int py =  (preClickY - marginY) / blockHeight;

	if (preClickX == -1 && preClickY == -1)
	{
		preClickX = pointX;
		preClickY = pointY; 
		return;
	}

	preClickX = pointX;
	preClickY = pointY; 
	
	if (x == px && y == py)
	{
		return;
	}
	else
	{
		if (px < 0 || py < 0)
		{
			return;
		}
		else if (picArray[y][x].visible == true &&
			picArray[py][px].visible == true &&
			picArray[y][x].type == picArray[py][px].type &&
			match(picArray[y][x], picArray[py][px]))
		{
			picArray[y][x].visible = false;
			picArray[py][px].visible = false;
			paint_black(picArray[y][x]);
			paint_black(picArray[py][px]) ;
			score += 10;
			remain_num -= 2;
			
			//FIXME:
			preClickX = -1;
			preClickY = -1;
			
			if (remain_num == 0)
			{
				score += 100; 
				stage++;
				new_stage() ;
			}
		}
	}
}

bool Game::load_data(const char* filename)
{
	int i, j;
	size_t ret;
	bool isSucess = false;
	int visible_num = 0;
    SDL_RWops *rwops = SDL_RWFromFile(filename, "rb");
    if (rwops)
    {
		ret = rwops->read(rwops, &save_version, sizeof(save_version), 1);
    	if (ret != 1)
    	{
			SDL_Log("rwops->read failed : save_version");
			goto close_file;
    	}
    	if (save_version != VERSION)
    	{
			SDL_Log("rwops->read failed : save_version is wrong value");
			goto close_file;
    	}
		SDL_Log("Game::load_data save_version = %d", save_version);
		ret = rwops->read(rwops, &lenx, sizeof(lenx), 1);
    	if (ret != 1)
    	{
			SDL_Log("rwops->read failed : lenx");
			goto close_file;
    	}
    	SDL_Log("Game::load_data lenx = %d", lenx);
		ret = rwops->read(rwops, &leny, sizeof(leny), 1);
    	if (ret != 1)
    	{
			SDL_Log("rwops->read failed : leny");
			goto close_file;
    	}
    	SDL_Log("Game::load_data leny = %d", leny);
		ret = rwops->read(rwops, &score, sizeof(score), 1);
    	if (ret != 1)
    	{
			SDL_Log("rwops->read failed : score");
			goto close_file;
    	}
    	SDL_Log("Game::load_data score = %d", score);
		ret = rwops->read(rwops, &stage, sizeof(stage), 1);
    	if (ret != 1)
    	{
			SDL_Log("rwops->read failed : stage");
			goto close_file;
    	}
    	SDL_Log("Game::load_data stage = %d", stage);
		ret = rwops->read(rwops, &remain_num, sizeof(remain_num), 1);
    	if (ret != 1)
    	{
			SDL_Log("rwops->read failed : remain_num");
			goto close_file;
    	}
    	SDL_Log("Game::load_data remain_num = %d", remain_num);
    	if (remain_num > lenx * leny || remain_num <= 0)
    	{
			SDL_Log("Game::load_data remain_num = %d is too small or too large", remain_num);
			goto close_file;
    	}
		for (i = 0; i < leny; ++i)
		{
			for(j = 0; j < lenx; ++j)
			{
				ret = rwops->read(rwops, &(picArray[i][j].type), sizeof(picArray[i][j].type), 1);
				if (ret != 1)
				{
					SDL_Log("rwops->read failed : picArray[%d][%d].type", i, j);
					goto close_file;
				}
				ret = rwops->read(rwops, &(picArray[i][j].visible), sizeof(picArray[i][j].visible), 1);
				if (ret != 1)
				{
					SDL_Log("rwops->read failed : picArray[%d][%d].visible", i, j);
					goto close_file;
				}
				ret = rwops->read(rwops, &(picArray[i][j].x), sizeof(picArray[i][j].x), 1);
				if (ret != 1)
				{
					SDL_Log("rwops->read failed : picArray[%d][%d].x", i, j);
					goto close_file;
				}
				ret = rwops->read(rwops, &(picArray[i][j].y), sizeof(picArray[i][j].y), 1);
				if (ret != 1)
				{
					SDL_Log("rwops->read failed : picArray[%d][%d].y", i, j);
					goto close_file;
				}
				SDL_Log("Game::load_data picArray[%d][%d] = %d, %d, %d, %d", 
					i, j, 
					picArray[i][j].type, picArray[i][j].visible ? 1 : 0, 
					picArray[i][j].x, picArray[i][j].y);
				if (picArray[i][j].visible)
				{
					visible_num++;
				}
			}
		}
		if (visible_num != remain_num)
		{
			SDL_Log("Game::load_data remain_num != visible_num: %d, %d", remain_num, visible_num);
			goto close_file;
    	}
		isSucess = true;
		
close_file:
		rwops->close(rwops);
		
		if (isSucess)
		{
			blockWidth = (VIEW_WIDTH - 2 * marginX) / lenx;
			blockHeight = (VIEW_HEIGHT - 2 * marginY) / leny;
			if (blockWidth < blockHeight)
			{
				blockHeight = blockWidth;
			}
			else
			{
				blockWidth = blockHeight;
			}
			lines_size_max = lenx * leny;
			preClickX = -1;
			preClickY = -1;
			clear_lines();
			return true;
		}
    }
    return false;
}

void Game::save_data(const char* filename)
{
	int i, j;
	size_t ret;
    SDL_RWops *rwops = SDL_RWFromFile(filename, "wb");
    if (rwops)
    {
		ret = rwops->write(rwops, &save_version, sizeof(save_version), 1);
		if (ret != 1)
		{
			SDL_Log("rwops->write failed : save_version");
			goto close_file;
		}
		ret = rwops->write(rwops, &lenx, sizeof(lenx), 1);
		if (ret != 1)
		{
			SDL_Log("rwops->write failed : lenx");
			goto close_file;
		}
		ret = rwops->write(rwops, &leny, sizeof(leny), 1);
		if (ret != 1)
		{
			SDL_Log("rwops->write failed : leny");
			goto close_file;
		}
		ret = rwops->write(rwops, &score, sizeof(score), 1);
		if (ret != 1)
		{
			SDL_Log("rwops->write failed : score");
			goto close_file;
		}
		ret = rwops->write(rwops, &stage, sizeof(stage), 1);
		if (ret != 1)
		{
			SDL_Log("rwops->write failed : stage");
			goto close_file;
		}
		ret = rwops->write(rwops, &remain_num, sizeof(remain_num), 1);
		if (ret != 1)
		{
			SDL_Log("rwops->write failed : remain_num");
			goto close_file;
		}
		for (i = 0; i < leny; ++i)
		{
			for(j = 0; j < lenx; ++j)
			{
				ret = rwops->write(rwops, &(picArray[i][j].type), sizeof(picArray[i][j].type), 1);
				if (ret != 1)
				{
					SDL_Log("rwops->write failed : picArray[%d][%d].type", i, j);
					goto close_file;
				}
				ret = rwops->write(rwops, &(picArray[i][j].visible), sizeof(picArray[i][j].visible), 1);
				if (ret != 1)
				{
					SDL_Log("rwops->write failed : picArray[%d][%d].visible", i, j);
					goto close_file;
				}
				ret = rwops->write(rwops, &(picArray[i][j].x), sizeof(picArray[i][j].x), 1);
				if (ret != 1)
				{
					SDL_Log("rwops->write failed : picArray[%d][%d].x", i, j);
					goto close_file;
				}
				ret = rwops->write(rwops, &(picArray[i][j].y), sizeof(picArray[i][j].y), 1);
				if (ret != 1)
				{
					SDL_Log("rwops->write failed : picArray[%d][%d].y", i, j);
					goto close_file;
				}
			}
		}
		
close_file:
		rwops->close(rwops);
    }
}

