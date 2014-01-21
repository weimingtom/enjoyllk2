#pragma once

class Picture
{
public:
	int type;
	bool visible; 
	int x;
	int y;
};

class Line
{
public:
	int x1;
	int y1;
	int x2;
	int y2;
};

class Game
{
public:
	Game();
	void new_stage(void);
	void on_point_down(int pointX, int pointY);
	bool load_data(const char* filename);
	void save_data(const char* filename);
	virtual ~Game();
	
private:
	void paint_black(Picture &a);
	void clear_lines(void);
	void draw_line(Picture& a, Picture& b);
	bool match_direct(Picture& a, Picture& b);
	bool match_one_corner(Picture& a, Picture& b);
	bool match_two_corner(Picture& a, Picture& b);
	bool match(Picture& a, Picture& b);
	
public:
	enum
	{
		marginX = 20,
		marginY = 20,
		N = 10,
		VERSION = 1,
	};

	Picture picArray[100][100];
	int remain_num;
	unsigned int score;
	unsigned int stage;
	int preClickX;
	int preClickY;
	
	Line lines[500];
	int linesSize;
	
	int lenx, leny, blockWidth, blockHeight;
	int lines_size_max;
	int save_version;
	
	bool show_menu;
	int buttonX, buttonY, buttonW, buttonH;
};
