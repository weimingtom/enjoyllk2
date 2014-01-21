#pragma once

#ifdef __ANDROID__
#define WINDOW_WIDTH    0
#define WINDOW_HEIGHT   0
#define DATAPATH ""
#define DATAFILE "enjoyllk2_data.txt"
#else
#define WINDOW_WIDTH    700
#define WINDOW_HEIGHT   480
#define DATAPATH "../../assets/"
#define DATAFILE "../../bin/enjoyllk2_data.txt"
#endif

#define USE_TIMER 0
#define VIEW_WIDTH 640
#define VIEW_HEIGHT 480
#define MILLESECONDS_PER_FRAME ((int)(1000 / 24))
#define USE_MT_RANDOM 1
