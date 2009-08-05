#
CC = g++
CFLAGS := -Wall `sdl-config --cflags` 
LIBS := `sdl-config --libs` -lSDL_ttf -lSDL_image

COMPILE = $(CC) 



all: Linux



Linux: 

	$(COMPILE) main.cpp sdlgui.cpp SDLwindow.cpp timer.cpp piece.cpp -o Execute/BlocksLin.out $(CFLAGS) $(LIBS)


Win:

	$(COMPILE) main.cpp sdlgui.cpp SDLwindow.cpp timer.cpp piece.cpp -o Execute/BlocksWin.exe -lmingw32  $(CFLAGS) $(LIBS)

OSX: 

	$(COMPILE) main.cpp sdlgui.cpp SDLwindow.cpp timer.cpp piece.cpp -o Execute/BlocksOSX $(CFLAGS) $(LIBS)


