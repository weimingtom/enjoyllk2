# Makefile to build the SDL library
# mingw
# cd /d/ugame/cocos2dx/enjoyllk2/jni/SDL2-2.0.1

SDL2PATH := .
CC := gcc
CPP := g++
AR	:= ar
RANLIB := ranlib
WINDRES	:= windres
RM := rm -rf

CFLAGS := 
CFLAGS += -g -O3 -mmmx -msse -mpreferred-stack-boundary=2 -Wall 
CFLAGS += -D__WIN32__ -DDECLSPEC=
CFLAGS += -I$(SDL2PATH)/include -I../src

CPPFLAGS := $(CFLAGS)

LDFLAGS := 
LDFLAGS += -lwinmm -lgdi32 -limm32 -luuid -loleaut32 -lole32 -lversion
#LDFLAGS += -luser32 -lgdi32 -lwinmm 
#LDFLAGS += -limm32 -lole32 -loleaut32 
#LDFLAGS += -lshell32 -lversion -luuid 
LDFLAGS += -static-libgcc

TARGET := libSDL2.a
SOURCES := \
	src/*.c \
	src/audio/*.c \
	src/audio/dummy/*.c \
	src/audio/winmm/*.c \
	src/audio/disk/*.c \
	src/cpuinfo/*.c \
	src/events/*.c \
	src/file/*.c \
	src/haptic/*.c \
	src/haptic/dummy/*.c \
	src/joystick/*.c \
	src/joystick/dummy/*.c \
	src/loadso/windows/*.c \
	src/power/*.c \
	src/power/windows/*.c \
	src/filesystem/windows/*.c \
	src/render/*.c \
	src/render/software/*.c \
	src/render/opengl/*.c \
	src/stdlib/*.c \
	src/thread/*.c \
	src/thread/windows/*.c \
	src/timer/*.c \
	src/timer/windows/*.c \
	src/video/*.c \
	src/video/dummy/*.c \
	src/video/windows/*.c \
	src/core/windows/*.c \
	src/libm/*.c \
	src/atomic/*.c

#	src/thread/generic/*.c \
#	src/loadso/dummy/*.c \
#	src/filesystem/dummy/*.c \
#	src/timer/dummy/*.c \	
# 	src/audio/directsound/*.c \
# 	src/render/direct3d/*.c \
#	src/audio/xaudio2/*.c \
#	src/haptic/windows/*.c \
#	src/joystick/windows/*.c \

SDLMAIN_TARGET := libSDL2main.a
SDLMAIN_SOURCES := src/main/windows/*.c

APP_TARGET := ../src/enjoyllk2.exe
APP_SOURCES := ../src/main.cpp ../src/game.cpp ../src/sprite.cpp ../src/font.cpp ../src/mt19937ar.cpp

OBJECTS := $(shell echo $(SOURCES) | sed -e 's,\.c,\.o,g')
SDLMAIN_OBJECTS := $(shell echo $(SDLMAIN_SOURCES) | sed -e 's,\.c,\.o,g')
APP_OBJECTS := $(shell echo $(APP_SOURCES) | sed -e 's,\.cpp,\.o,g')

all: $(TARGET) $(APP_TARGET)
# $(SDLMAIN_TARGET)

$(APP_TARGET) : $(TARGET) $(APP_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(SDLMAIN_SOURCES) $^ -L. -lSDL2 $(LDFLAGS) -lstdc++

# src/main/windows/SDL_windows_main.o

$(SDLMAIN_TARGET) : $(SDLMAIN_OBJECTS)
	$(AR) cru $@ $^
	$(RANLIB) $@

$(TARGET) : $(OBJECTS)
	$(AR) cru $@ $^
	$(RANLIB) $@

clean:
	$(RM) $(TARGET) $(OBJECTS) 
	$(RM) $(SDLMAIN_TARGET) $(SDLMAIN_OBJECTS) 
	$(RM) $(APP_TARGET) $(APP_OBJECTS)

cleanapp:
	$(RM) $(APP_TARGET) $(APP_OBJECTS)

test: all
	../src/enjoyllk2.exe
