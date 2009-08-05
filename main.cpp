/*
	Blocks Application by Jessica Seibert
	1/15/09/
	
	A spiffy Tetris clone.  Meant to demonstrate a game that requires
	several states of gameplay:  Intro, Paused, Main Game, "About", etc.

	Built to be architecturally flexible and modular for future games.

      _._
    .    ` .
   .       `.
   .________.
    `""``""'
      `  `		
	:  :	We are pillars of society. You can't run your computers, your fashion houses,   
       :  :		your publishing houses, your damn magazines, you can't do anything 
       ;  ;		in culture without psychedelic people in key positions. 
      .  .		- Terence McKenna
     '  .
    '  .
   .  .
   ;  :
  .    .
 '      ' mh

"I want to be free to modify my mind, body, spirit, and code."
This code is distributed under the GPLv3.


*/

//The headers
#include <iostream>

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include <string>
#include <vector>
#include <math.h>

//Local includes
#include "timer.h"
#include "sdlgui.h"
#include "SDLwindow.h"
#include "stars.h"
#include "piece.h"
#include "commongfx.h"

//Global types and constants
#define WANT_TASTY_MUSHROOM 1 //Always

//We got some game modes here buddy:
#define IntroScreen 1
#define GamePlay 2
#define LineKill 3
#define GameOver 4
#define Exitting 5
#define FullScreenToggle 6
#define AboutScroll 7


//The screen sttributes
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;


//The font that's going to be used
TTF_Font *font = NULL;
//TTF_Font *titlefont = NULL;

//The color of the font
SDL_Color textColor = { 255, 255, 255 };

class GameMode
{
private:


public:

	virtual bool Initialize()
	{
		return true;
	}

	virtual bool LoadFiles()
	{
		return 0;
	}

	virtual int GameCycle(int mode, int ticks)
	{
		return true;
	}

	virtual bool GamePaint(SDL_Surface *screen)
	{
		return 0;
	}

	virtual bool HandleEvents(SDL_Event &event)
	{
		return 0;
	}

	virtual bool CleanUp()
	{
		fprintf(stderr, "FAIL: %s\n", SDL_GetError());
		return true;
	}

};


class AboutMode : public GameMode
{
private:
	SDL_Surface *temp;
	SDL_Surface *AboutText;
	SDL_Surface *AboutBack;
	float x;


public:
	AboutMode();
	bool Initialize();
	bool LoadFiles();
	int GameCycle(int mode, int ticks);
	bool GamePaint(SDL_Surface *screen);
	bool CleanUp();
	~AboutMode();

};

AboutMode::AboutMode()
{
	temp = NULL;
	AboutText = NULL;
	AboutBack = NULL;
	Initialize();
}

bool AboutMode::Initialize()
{
	x = 650.0;
	return true;
}

bool AboutMode::LoadFiles()
{
	AboutText = load_image("Graphix/abouttext.png");
	AboutBack = load_image("Graphix/aboutback.png");
	SDL_SetColorKey( AboutText, SDL_RLEACCEL | SDL_SRCCOLORKEY, SDL_MapRGB( AboutText->format, 0x00, 0x00, 0x00) );
	
	if(AboutText == NULL) return false;
	if(AboutBack == NULL) return false;

	temp = SDL_CreateRGBSurface(SDL_SWSURFACE, 640, AboutBack->h, 32, 0, 0, 0, 0);
	SDL_SetColorKey( temp, SDL_RLEACCEL | SDL_SRCCOLORKEY, SDL_MapRGB( AboutText->format, 0x00, 0xFF, 0x00) );

	return true;
}

int AboutMode::GameCycle(int mode, int ticks)
{
	x -= (float)ticks * 0.11;
	//fprintf(stdout, "|%i| ", ticks);
	if ((x + 100.0) < float(0 - AboutText->w))
		x = 640.0;

	return mode;
}

bool AboutMode::GamePaint(SDL_Surface *screen)
{
	
	apply_surface( 0,0, AboutBack, temp);
	if (x > 0.0)
	{
	 	SDL_Rect area;

		area.x = 0;
		area.y = 0;
		area.w = (int)x;
		area.h = AboutBack->h;
		SDL_FillRect(temp, &area, SDL_MapRGB(temp->format, 0x00, 0xFF, 0x00 ) );
	}
	if (x- 640 < AboutText->w)
	{
	 	SDL_Rect area;

		area.x = (int)x+AboutText->w;
		area.y = 0;
		area.w = 640 - ((int)x+AboutText->w);
		area.h = AboutBack->h;
		SDL_FillRect(temp, &area, SDL_MapRGB(temp->format, 0x00, 0xFF, 0x00 ) );
	}

	apply_surface( (int)x,0, AboutText, temp);
	apply_surface( 0,190, temp, screen);
	return 0;
}

bool AboutMode::CleanUp()
{
	SDL_FreeSurface( AboutText );
	SDL_FreeSurface( AboutBack );
	SDL_FreeSurface(temp);
	return true;
}

AboutMode::~AboutMode()
{
	CleanUp();
	fprintf(stdout, "AboutMode deconstructed: %s\n", SDL_GetError());
}



//❤`•.¸¸.•´´¯`••.¸¸.•´´¯`•´❤`•.¸¸.•´´¯`••.¸¸.•´´¯`•´❤`•.¸¸.•´´¯`••.¸¸.•´´¯`•´❤
//class:  IntroMode
//	Inhererits GameMode
//
//Loads a couple images and displays them.
//Creates a 640x480 star field using the cStarList library I made.
//Creates a 4 buttons with an old version of my GUI library.
//	These buttons allow you to start the game, exit, toggle fullscreen,
//	and view the "about" information
//Creates an AboutMode instance for scrolling the "about" text.
//
//❤`•.¸¸.•´´¯`••.¸¸.•´´¯`•´❤`•.¸¸.•´´¯`••.¸¸.•´´¯`•´❤`•.¸¸.•´´¯`••.¸¸.•´´¯`•´❤
class IntroMode : public GameMode
{
private:
	AboutMode* myAboutMode;

	SDL_Surface *IntroImage;
	SDL_Surface *GPLImage;
	cStarList* myStars;
	GUI_Button* startButton;
	GUI_Button* aboutButton;
	GUI_Button* fsButton;
	GUI_Button* exitButton;

	bool keydown_enter;
	bool keyup;
	int msx, msy;
	int localmode;

public:
	IntroMode();
	bool Initialize();
	bool LoadFiles();
	int GameCycle(int mode, int ticks);
	bool GamePaint(SDL_Surface *screen);
	bool HandleEvents(SDL_Event &event);
	bool CleanUp();
	~IntroMode();

};

IntroMode::IntroMode()
{
	myAboutMode = new AboutMode;
	IntroImage = NULL;
	Initialize();
}

bool IntroMode::Initialize()
{
	myStars = new cStarList(100, 640, 480);
	keydown_enter = false;
	keyup = false;
	msx = msy = 0;
	localmode = IntroScreen;

	return true;
}

bool IntroMode::LoadFiles()
{
	if (myAboutMode->LoadFiles() == false)
	{
		fprintf(stderr, "Error loading AboutMode files: %s\n", SDL_GetError());
		return false;
	}
	startButton = new GUI_Button(220, 230, "Graphix/start1.png", "Graphix/start2.png"); 
	aboutButton = new GUI_Button(220, 290, "Graphix/about1.png", "Graphix/about2.png"); 
	fsButton =    new GUI_Button(220, 350, "Graphix/fullscreen1.png", "Graphix/fullscreen2.png"); 
	exitButton =  new GUI_Button(220, 410, "Graphix/quit1.png", "Graphix/quit2.png");
	IntroImage = load_image("Graphix/hawttitle.png");
	GPLImage = load_image("Graphix/gplv3-127x51.png");

	if(IntroImage == NULL) return false;
	if(GPLImage == NULL) return false;

	return true;
}

//Return mode, even though it may not change
int IntroMode::GameCycle(int mode, int ticks)
{
	if (mode == AboutScroll)
	{
		myAboutMode->GameCycle(mode, ticks);
	}

	myStars->MoveStars(msx, msy, ticks);

	if (startButton->getClicked() == true)
		mode = GamePlay;
	if (exitButton->getClicked() == true)
		mode = Exitting;
	if (fsButton->getClicked() == true)
		mode = FullScreenToggle;
	if (aboutButton->getClicked() == true)
	{
		if (mode == AboutScroll)
		{
			mode = IntroScreen;
		}
		else if (mode == IntroScreen)
		{
			mode = AboutScroll;
			myAboutMode->Initialize();	
		}
	}

	localmode = mode;
	return mode;
}

bool IntroMode::GamePaint(SDL_Surface *screen)
{
	SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 0x00, 0x00, 0x00 ) );
	SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 0x00, 0x00, 0x00 ) );
	myStars->DrawStars(screen);
	apply_surface( 0,0, IntroImage, screen);
	apply_surface( screen->w-128,screen->h-52, GPLImage, screen);
	startButton->Draw(screen);
	fsButton->Draw(screen);
	aboutButton->Draw(screen);
	exitButton->Draw(screen);

	if (localmode == AboutScroll)
		myAboutMode->GamePaint(screen);

	return 0;
}

bool IntroMode::HandleEvents(SDL_Event &event)
{
	if( event.key.keysym.sym == SDLK_RETURN )
	{
		if (keyup == true)
			keydown_enter = true;
	}
	if( event.type == SDL_KEYUP )
	{
		keyup = true;
	}
	//If the mouse moved
	if( event.type == SDL_MOUSEMOTION )
	{
		//Get the mouse offsets
		msx = event.motion.x;
		msy = event.motion.y;
	}
	startButton->HandleEvents(event);
	aboutButton->HandleEvents(event);
	fsButton->HandleEvents(event);
	exitButton->HandleEvents(event);
	return 0;
}


bool IntroMode::CleanUp()
{
	delete myStars;
	delete myAboutMode;

	fprintf(stdout, "deleting buttons:");
	delete startButton;
	delete fsButton;
	delete aboutButton;
	delete exitButton;
	fprintf(stdout,"\n");

	SDL_FreeSurface( GPLImage );
	SDL_FreeSurface( IntroImage );
	return true;
}

IntroMode::~IntroMode()
{
	CleanUp();
	fprintf(stdout, "IntroMode deconstructed: %s\n", SDL_GetError());
}



class GameOverMode : public GameMode
{
private:
	SDL_Surface *GameOverImage;
	bool keydown_enter;
	bool keyup;
	int RollUpCounter;
	bool RollUp;


public:
	GameOverMode();
	bool Initialize();
	bool LoadFiles();
	int GameCycle(int mode, int ticks);
	bool GamePaint(SDL_Surface *screen);
	bool HandleEvents(SDL_Event &event);
	bool CleanUp();
	~GameOverMode();

};

GameOverMode::GameOverMode()
{
	GameOverImage = NULL;
	Initialize();
}

bool GameOverMode::Initialize()
{
	keydown_enter = false;
	keyup = false;

	return true;
}

bool GameOverMode::LoadFiles()
{
	GameOverImage = load_image("Graphix/GameOver.png");
	if(GameOverImage == NULL) return false;

	return true;
}

int GameOverMode::GameCycle(int mode, int ticks)
{
	if (keydown_enter)
	{
		mode = IntroScreen;
	}

	return mode;
}

bool GameOverMode::GamePaint(SDL_Surface *screen)
{
	SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF ) );
	apply_surface( 0,0, GameOverImage, screen);
	return 0;
}

bool GameOverMode::HandleEvents(SDL_Event &event)
{
	if( event.key.keysym.sym == SDLK_RETURN )
	{
		if( keyup ==true)
			keydown_enter = true;
	}
	if( event.type == SDL_KEYUP )
	{
		keyup = true;
	}
	return 0;
}


bool GameOverMode::CleanUp()
{
	SDL_FreeSurface( GameOverImage );
	return true;
}

GameOverMode::~GameOverMode()
{
	CleanUp();
	fprintf(stdout, "GameOverMode deconstructed: %s\n", SDL_GetError());
}





//❤`•.¸¸.•´´¯`••.¸¸.•´´¯`•´❤`•.¸¸.•´´¯`••.¸¸.•´´¯`•´❤`•.¸¸.•´´¯`••.¸¸.•´´¯`•´❤
//class:  PlayMode
//	Inherits GameMode
//
//This is the main gameplay mode.
//
//Contains a game background and overlay for the game screen
//Creates two game "Pieces" - your current and the next piece.
//Contains two arrays - one for remembering where we found finished lines,
//	and another for field data (i.e. the pieces after they've fallen). 
//
//
//❤`•.¸¸.•´´¯`••.¸¸.•´´¯`•´❤`•.¸¸.•´´¯`••.¸¸.•´´¯`•´❤`•.¸¸.•´´¯`••.¸¸.•´´¯`•´❤
class PlayMode : public GameMode
{
private:
	SDL_Surface *text;
	SDL_Surface *overlay;
	SDL_Surface *normbg;
	int field[10][21];
	bool LinesFound[21];

    	//The Piece that will be used
	Piece current, next;

	int mode;
	int level, score, lines;
	float falltime;
//	bool entere;
	bool droponce;
	bool movetimer;


	float leftTime, rightTime, downTime;

public:
	PlayMode();
	bool Initialize();
	bool LoadFiles();
	bool CheckLines();
	void KillLines(bool lines[21], int field[10][21]);
	int GameCycle(int mode, int ticks);
	void DrawField(SDL_Surface* screen);
	void DrawScores(SDL_Surface* screen);
	bool GamePlayCycle(int Ticks); 
	bool LineKillCycle(int Ticks);
	void DrawBlock( SDL_Surface* screen, int x, int y, int colors);
	bool DrawBackground(SDL_Surface* screen);
	bool GamePaint(SDL_Surface* screen);
	bool HandleEvents(SDL_Event &event);
	bool CleanUp();
	~PlayMode();
};

//Initialize
PlayMode::PlayMode()
{
	Initialize();

}

bool PlayMode::Initialize()
{
	int i,j;	//For iteration

	for(i=0; i<10; i++)
		for(j=0; j<21; j++)
			field[i][j] = 0;

	for(i=0; i<21; i++)
		LinesFound[i] = false;

	//INITIALIZE THIS STUFF:
	current.Random();
	next.Random();
	level = score = lines = falltime = 0;
	//droponce = false;
	mode = GamePlay;

	leftTime = rightTime = downTime = 0.0;

	return true;
}


bool PlayMode::LoadFiles()
{
	normbg = load_image("Graphix/city.png");
	overlay = load_image("Graphix/overlay.png");

	if(normbg == NULL ) return false;
	if(overlay == NULL) return false;
	
	return true;
}

bool PlayMode::DrawBackground(SDL_Surface* screen)
{

	apply_surface(0, 0, normbg, screen );
	apply_surface(0,0,overlay, screen);

	return true;
}


void PlayMode::DrawBlock( SDL_Surface* screen, int x, int y, int colors)
{
	Uint32 primary, secondary, white, black;
	int i;
	SDL_Rect area;

	white = SDL_MapRGB(screen->format, 0xff, 0xff, 0xff);
	black = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);

	switch(level)
	{
		case -1: //goth flag
			primary = SDL_MapRGB(screen->format, 0xbb, 0xbb, 0xbb);
			secondary = SDL_MapRGB(screen->format, 0xcc, 0xcc, 0xcc);
			break;
		case 0:
			primary = SDL_MapRGB(screen->format, 0x00, 0x00, 0xFF);
			secondary = SDL_MapRGB(screen->format, 0x00, 0xcc, 0xFF);
			break;
		case 1:
			primary = SDL_MapRGB(screen->format, 0x16, 0x8F, 0x3D);
			secondary = SDL_MapRGB(screen->format, 0x35, 0xEC, 0x40);
			break;
		case 2://E1EC35#EFDD48
			primary = SDL_MapRGB(screen->format, 0xE1, 0xEC, 0x35);
			secondary = SDL_MapRGB(screen->format, 0xEF, 0xDD, 0x48);
			break;
		case 3://
			primary = SDL_MapRGB(screen->format, 0xEF,0x4E,0x48);
			secondary = SDL_MapRGB(screen->format, 0xCE,0x17,0x10);
			break;
		case 4:
			primary = SDL_MapRGB(screen->format, 0xD1,0x80,0x1E);
			secondary = SDL_MapRGB(screen->format, 0xF3,0x9E,0x39);
			break;

		default:
			primary = SDL_MapRGB(screen->format, 0xCC, 0x00, 0xFF);
			secondary = SDL_MapRGB(screen->format, 0xff, 0x00, 0xCC);
			break;
	}


	switch(colors)
	{
		case 0:
			area.x = x;
			area.y = y;
			area.w = 20;
			area.h = 20;
			SDL_FillRect(screen, &area, black );
			break;
		case 1:
			area.x = x;
			area.y = y;
			area.w = 20;
			area.h = 20;
			SDL_FillRect(screen, &area, white );

			for(i=0; i<19; i++)
			{
				put_pixel32(screen, x+i,   y, primary, 640, 480);
				put_pixel32(screen, x,   y+i, primary, 640, 480);
				put_pixel32(screen, x+i, y+18, primary, 640, 480);
				put_pixel32(screen, x+18, y+i, primary, 640, 480);
			}
			for(i=1; i<18; i++)
			{
				put_pixel32(screen, x+i,   y+1, primary, 640, 480);
				put_pixel32(screen, x+1,   y+i, primary, 640, 480);
				put_pixel32(screen, x+i, y+17, primary, 640, 480);
				put_pixel32(screen, x+17, y+i, primary, 640, 480);
			}
			for(i=0; i<19; i++)
			{
				put_pixel32(screen, x+i,y+19, black, 640, 480);
				put_pixel32(screen, x+19, y+i, black, 640, 480);
			}
			put_pixel32(screen, x,y, white, 640, 480);
			break;
		case 2:
			area.x = x;
			area.y = y;
			area.w = 20;
			area.h = 20;
			SDL_FillRect(screen, &area, primary );
		
			for(i=0; i<20; i++)
			{
				put_pixel32(screen, x+i,y+19, black, 640, 480);
				put_pixel32(screen, x+19, y+i, black, 640, 480);
			}
			put_pixel32(screen, x,y, white, 640, 480);
			put_pixel32(screen, x+1, y+1, white, 640, 480);
			put_pixel32(screen, x+2, y+1, white, 640, 480);
			put_pixel32(screen, x+1, y+2, white, 640, 480);

			break;
		case 3:
			area.x = x;
			area.y = y;
			area.w = 20;
			area.h = 20;
			SDL_FillRect(screen, &area, secondary );
		
			for(i=0; i<20; i++)
			{
				put_pixel32(screen, x+i,y+19, black, 640, 480);
				put_pixel32(screen, x+19, y+i, black, 640, 480);
			}
			put_pixel32(screen, x,y, white, 640, 480);
			put_pixel32(screen, x+1, y+1, white, 640, 480);
			put_pixel32(screen, x+2, y+1, white, 640, 480);
			put_pixel32(screen, x+1, y+2, white, 640, 480);
			break;
	}



}

bool PlayMode::CheckLines()
{
	int i, j;
	int grouplines = 0;
	bool linething = false;
	bool Found = false;

	//Check for lines!
	for(j=0; j<21; j++)
	{
		//grouplines = 0;
		linething = true;
		for(i=0; i<10; i++)
		{
			linething = (linething && field[i][j] > 0);
		}
		if (linething) //okay, there was a line
		{
			Found = true;
			grouplines++;
			lines++;
			score+=100;
			LinesFound[j] = true;
		}
	}
	level = (int)(lines/10);

	return Found;
}

void PlayMode::KillLines(bool lines[21], int field[10][21])
{
	int i, l, k;
	
	for(i=0; i<21; i++)
	{
		if (lines[i])
		{
			for(k=i; k>0; k--)
			{
				for(l=0; l<10;l++)
				{
					field[l][k] = field[l][k-1];
				}
			}
			for(l=0; l<10; l++)
			{
					field[l][0] = 0;
			}
		}
		lines[i] = false;
	}
}

void PlayMode::DrawField(SDL_Surface* screen)
{
	int i, j;
	int dummy;

	//Draw field
	for(i=0; i<10; i++)
		for(j=0; j<21; j++)
		{	
			dummy = field[i][j];
			DrawBlock(screen, 180+20*i, 30+20*j, dummy);
		}

	//Draw current piece
	for(i=0; i<4; i++)
		for(j=0; j<4; j++)
			if ((current.pieceAt(i,j) > 0) && (current.getY()+i >=0) ) 
				DrawBlock(screen, 180+20*(current.getX()+j), 30+20*(current.getY()+i),current.pieceAt(i,j));

	//Draw the "next" piece
	for(i=0; i<4; i++)
		for(j=0; j<4; j++)
			if ((next.pieceAt(i,j) > 0)) 
				DrawBlock(screen, 66+20*(j), 130+20*(i),next.pieceAt(i,j));

}

void PlayMode::DrawScores(SDL_Surface* screen)
{
	char buffer [33];

	sprintf(buffer, "%02d", level);//itoa (level,buffer,10);

	text = TTF_RenderText_Solid( font, buffer, textColor );
	apply_surface( 440,200, text, screen );
	SDL_FreeSurface(text);

	sprintf(buffer, "%02d", lines);//itoa (lines,buffer,10);
	text = TTF_RenderText_Solid( font, buffer, textColor );
	apply_surface( 440,302, text, screen );
	SDL_FreeSurface(text);

	sprintf(buffer, "%02d", score);//itoa (score,buffer,10);
	text = TTF_RenderText_Solid( font, buffer, textColor );
	apply_surface( 440,404, text, screen );
	SDL_FreeSurface(text);
}

bool PlayMode::GamePlayCycle(int Ticks)
{
	int i,j;	//For loop iterators
	float dropSpeed;
	bool justPressedLR, justPressedDown;

	if ( (current.getOffset() == -1) )
	{	
		justPressedLR = (leftTime == 0.0);
		leftTime += (float)(Ticks) / 1000.f;
	}
	else if ( (current.getOffset() == 1) )
	{	
		justPressedLR = (rightTime == 0.0);
		rightTime += (float)(Ticks) / 1000.f;
	}

	if ( current.getdownPressed() )
	{
		justPressedDown = (downTime == 0.0);
		downTime += (float)(Ticks) / 1000.f;
	}

	switch(level)
	{
		case 0:
			dropSpeed = 1.5;
			break;
		case 1:
			dropSpeed = 1.3;
			break;
		case 2:
			dropSpeed = 1.1;
				break;
		case 3:
			dropSpeed = .9;
				break;
		case 4:
			dropSpeed = .8;
				break;
		case 5:
			dropSpeed = 0.65;
			break;
		case 6:
			dropSpeed = 0.5;
			break;
		case 7:
			dropSpeed = 0.4;
				break;
		case 8:
			dropSpeed = 0.2;
				break;
		case 9:
			dropSpeed = 0.1;
				break;
		default:
			break;

	}


	current.setDrop(false);
	if (current.getdownPressed() && (downTime > .07 || justPressedDown) )
	{
		current.setDrop(true);
		if (downTime > .07)
		{
			falltime -= .07;
			downTime -= .07;
		}
	}


	//Move piece every so many seconds
	falltime += (float)(Ticks) / 1000.f;
	if (falltime >= dropSpeed)
	{
		falltime -= dropSpeed;
		current.setDrop(true);
	}


	//If left or right -- move
	if ( (current.getOffset() != 0) && (leftTime > .11 || rightTime > .11 ||  justPressedLR) )
	{				
		while (leftTime > .11)
			leftTime -= .11;
		while (rightTime > .11)
			rightTime -= .11;
		
		current.MoveX(field);
	}

	//If rotating...
	if (current.getRote())
	{
		current.Rotate(field);
	}

	if ( current.getDrop())
	{
		if (current.Drop(field) == false)
		{
			for(i=0; i<4; i++)
				for(j=0; j<4; j++)
					if (current.pieceAt(i,j) > 0)
						field[current.getX()+j][current.getY()+i] = current.pieceAt(i,j);
			current = next;
			next.Random();
			if (CheckLines() == true)
				mode = LineKill;	//hell yeah
			
			if (mode == GamePlay && current.CheckIfCanDrop(field) == false)
				mode = GameOver;	//hoshit			
		}
	}

	return false;

}

bool PlayMode::LineKillCycle(int Ticks)
{
	KillLines(LinesFound, field);
	mode = GamePlay;

	return true;
}



int PlayMode::GameCycle(int Mainmode, int ticks)
{
	switch(mode)
	{
		case GamePlay:
			GamePlayCycle(ticks);
			break;
		case LineKill:
			//fprintf(stderr, "linekill\n");
			LineKillCycle(ticks);
			break;
		case GameOver:
			return GameOver;
			//fprintf(stderr, ":(\n");
			break;
	}

	return Mainmode;
}


bool PlayMode::GamePaint(SDL_Surface* screen)
{
	SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF ) );
	//Draw Background
	DrawBackground(screen);

	//Draw field
	DrawField(screen);

	//Draw the scores
	DrawScores(screen);

	return 0;
}

bool PlayMode::HandleEvents(SDL_Event &event)
{
	//Handle events for the ???
	current.handle_input(event);
	leftTime = rightTime = downTime = 0.0;
	if( event.type == SDL_KEYUP )
	{
		switch( event.key.keysym.sym )
		{
		        case SDLK_UP: 
				break;
		        case SDLK_DOWN: 
				downTime = 0.0;
				break;
		        case SDLK_LEFT: 
				leftTime = 0.0;
				break;
		        case SDLK_RIGHT:  
				rightTime = 0.0;
			default: 
				break;
		}

	}

	if( event.type == SDL_KEYDOWN)
	{	
		//Set block movements
		switch( event.key.keysym.sym )
		{
		      	case SDLK_UP: 
				break;
		        case SDLK_DOWN: 
				downTime = 0.0;
				break;
		        case SDLK_LEFT: 
				leftTime = 0.0;
				break;
		        case SDLK_RIGHT:  
				rightTime = 0.0;
			default: 
				break;
			 
		}

		if (event.key.keysym.sym == SDLK_SPACE )
		{
			LinesFound[20] = true;
			mode = LineKill;	//hell yeah
			lines+=5;
			score=666;
		}
	}


	return true;
}

bool PlayMode::CleanUp()
{
    	//Free the surface

	SDL_FreeSurface( normbg);

	//Note:  "text" surface is drawn to for each use, then freed afterwards.
	//Thus, it does not need to be freed here.
	//SDL_FreeSurface( text );
	SDL_FreeSurface( overlay);

	return true;

}

PlayMode::~PlayMode()
{
	CleanUp();
	fprintf(stdout, "PlayMode Deconstructed: %s\n", SDL_GetError());
}



bool load_files()
{
	//Open the fonts
	font = TTF_OpenFont( "kingrich.ttf", 24 );

	//If there was a problem in loading
	if( font == NULL )return false;
	//if( titlefont == NULL) return false;
    
    //If everything loaded fine
    return true;
}


void clean_up()
{
    //Close the font that was used
    TTF_CloseFont( font );
    //TTF_CloseFont( titlefont );
    //Quit SDL_ttf
    TTF_Quit();
    //Quit SDL
    SDL_Quit();
}



//Initialize whatever you must
bool GameInitialize()
{

	//Initialize all SDL subsystems
	if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
	{
		return false;	
	}

	//Initialize SDL_ttf
	if( TTF_Init() == -1 )
	{
		return false;
	}

	/*//Initialize SDL_mixer
	if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 )
	{
		return false;
	}*/
	
	//If everything initialized fine
	return true;
}


//❤`•.¸¸.•´´¯`••.¸¸.•´´¯`•´❤`•.¸¸.•´´¯`••.¸¸.•´´¯`•´❤`•.¸¸.•´´¯`••.¸¸.•´´¯`•´❤
//function:  main
//
//The land of tasty mushrooms.  Accepts no arguments. 
//❤`•.¸¸.•´´¯`••.¸¸.•´´¯`•´❤`•.¸¸.•´´¯`••.¸¸.•´´¯`•´❤`•.¸¸.•´´¯`••.¸¸.•´´¯`•´❤
int main( int argc, char* args[] )
{
	//SDL_Surface* FreezeFrame;	//Save what the screen looks like
	SDL_Event event;		//The event structure
    	Timer delta;    		//Keeps track of time
	int lastmode, mode;		//The game mode
	bool quit = false; 		//Quit flag
	int ticks = 0;			//Save the ticks!


	delta.start();

	GameMode* currentMode;
  	Window myWindow(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, "Blocks!");
   
	//Initialize SDL and stuff
	if( GameInitialize() == false)
	{
		fprintf(stderr, "GameInitialize() failed: %s\n", SDL_GetError());
		return 1;
	}

	//Load the files
	if( load_files() == false )
	{
		fprintf(stderr, "load_files() failed: %s\n", SDL_GetError());
		return 1;
	}

	//Now that SDL is initialized, you can mess with files and whatnot

	IntroMode* myIntroMode = new IntroMode();
	PlayMode* myPlayMode = new PlayMode();
	GameOverMode* myGameOverMode = new GameOverMode;

	if( myIntroMode->LoadFiles() == false )
	{
		fprintf(stderr, "myIntroMode->LoadFiles() failed: %s\n", SDL_GetError());
		return 1;
	}

	if( myPlayMode->LoadFiles() == false )
	{
		fprintf(stderr, "myPlayMode->LoadFiles() failed: %s\n", SDL_GetError());
		return 1;
	}

	if( myGameOverMode->LoadFiles() == false )
	{
		fprintf(stderr, "myGameOverMode->LoadFiles() failed: %s\n", SDL_GetError());
		return 1;
	}
	
	srand(delta.get_ticks());
	delta.start();
	mode = IntroScreen;
	lastmode = mode;
	//While the user hasn't quit
	while(WANT_TASTY_MUSHROOM && !quit)
	{   
		switch(mode)
		{
			case IntroScreen:
			case AboutScroll:
				currentMode = myIntroMode;
				break;
			case GamePlay:
				currentMode = myPlayMode;
				break;
			case GameOver:
				currentMode = myGameOverMode;
				break;
		}

		//While there's events to handle
		while( SDL_PollEvent( &event ) )
		{
			myWindow.handle_events(event);
			if( event.key.keysym.sym == SDLK_ESCAPE)//Check for ESCAPE key
			{
				  quit = true;
			}
			if( event.type == SDL_QUIT )		//OS-level quit signal
			{
				quit = true;
			}
			currentMode->HandleEvents(event);
		}


		//Do the game work
		mode = currentMode->GameCycle(mode, ticks);

		if (mode == FullScreenToggle)
		{
			mode = lastmode;
			myWindow.toggle_fullscreen();
		}

		if ((lastmode == IntroScreen || lastmode == AboutScroll)  && mode == GamePlay)
			myPlayMode->Initialize();

		if (lastmode == GamePlay && mode == GameOver)
			myGameOverMode->Initialize();

		if (lastmode == GameOver && mode == IntroScreen)
			myIntroMode->Initialize();
		
		if(mode == Exitting)
			quit = true;


		//Paint the screen surface
		currentMode->GamePaint(myWindow.screen);

		//Update the screen
		if( SDL_Flip( myWindow.screen ) == -1 )
		{
			fprintf(stderr, "SDL_Flip() failed: %s\n", SDL_GetError());
			return 1;	
		}

		ticks = delta.get_ticks();
		delta.start();
		lastmode = mode;
	} //Farewell, game!


	delete myPlayMode;
	delete myIntroMode;
	delete myGameOverMode;

	//myWindow.CleanUp();

	//Clean up
	clean_up();
	fprintf(stdout, "Normal Quit: %s\n",  SDL_GetError());

	return 0;    
}


