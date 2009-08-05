/*
	Stars Header File by Jessica Seibert
	Last recorded update: 1/31/09/
	
	Built to by architecturally flexible and modular for future games.

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


*/
#include <SDL/SDL.h>
#include <vector>
#include <math.h>


//❤`•.¸¸.•´´¯`••.¸¸.•´´¯`•´❤`•.¸¸.•´´¯`••.¸¸.•´´¯`•´❤`•.¸¸.•´´¯`••.¸¸.•´´¯`•´❤
//class:  cStarData
//
//Contains data for each star, as well as a method for moving
//	a star based on mouse coordinates
//❤`•.¸¸.•´´¯`••.¸¸.•´´¯`•´❤`•.¸¸.•´´¯`••.¸¸.•´´¯`•´❤`•.¸¸.•´´¯`••.¸¸.•´´¯`•´❤
class cStarData
{
private:
	int starSize;
	float starVelocity;
	float x,y;
	int width, height;


public:
	bool Initialize(int width, int height);
	bool Initialize();
	bool Draw(SDL_Surface* screen);
	void moveStar(int mouseX, int mouseY, int ticks);
};

//Initializes the data for a star, and assigns the maximum
//height and width it can travel.
bool cStarData::Initialize(int width, int height)
{
	this->width = width;
	this->height = height;
	int nDistance;
	nDistance = rand() % 20;
	if(nDistance >= 12)
	{
		starSize = 1;
		starVelocity = 1.0;
	}
	else if(nDistance >= 7)
	{
		starSize = 2;
		starVelocity = 2.0;
	}
	else if(nDistance >= 2)
	{
		starSize = 3;
		starVelocity = 4.0;
	}
	else
	{
		starSize = 4;
		starVelocity = 7.5;
	}
	x = (float)(rand() % width);
	y = (float)(rand() % height);

	return true;
}

//Re-initializes, assuming width and height are already defined
bool cStarData::Initialize()
{
	int nDistance;
	nDistance = rand() % 20;
	if(nDistance >= 12)
	{
		starSize = 1;
		starVelocity = 1.0;
	}
	else if(nDistance >= 7)
	{
		starSize = 2;
		starVelocity = 2.0;
	}
	else if(nDistance >= 2)
	{
		starSize = 3;
		starVelocity = 4.0;
	}
	else
	{
		starSize = 4;
		starVelocity = 7.5;
	}
	x = (float)(rand() % width);
	y = (float)(rand() % height);

	return true;
}

//Moves star relative to the mouse coordinates.
//Ticks is used to move depending on how much time has elapsed in milliseconds
//since the last draw.
void cStarData::moveStar(int mouseX, int mouseY, int ticks)
{
	double speed, slopex, slopey;
	float tempx, tempy;
	int d;

	//distance formula
	speed = (mouseX - width/2) * (mouseX - width/2)
		+ (mouseY - height/2) * (mouseY - height/2);
	speed = sqrt(speed);
	d = speed;
	speed /= (100.0);
	
	slopex = fabs(asin((float)(mouseX - width/2) / (float)d));
	slopey = fabs(asin((float)(mouseY - height/2) / (float)d));

	if (mouseX < width/2)
		tempx = (x - slopex*starVelocity*speed*(float(ticks)/60.0));
	else if (mouseX > width/2)
		tempx = (x + slopex*starVelocity*speed*(float(ticks)/60.0));

	if (mouseY < height/2)
		tempy = (y - slopey*starVelocity*speed*(float(ticks)/60.0));
	else if (mouseY > height/2)
		tempy = (y + slopey*starVelocity*speed*(float(ticks)/60.0));
	
	//Check boundaries.
	if (tempx < 0.0)
	{
		Initialize();
		tempx = (float)(width - 1);
		tempy = (float)(rand() % height);
	}
	else if (tempx >= width)
	{
		Initialize();
		tempx = 0.0;
		tempy = (float)(rand() % height);
	}
	else if(tempy < 0.0)
	{
		Initialize();
		tempy = (float)(height - 1);
		tempx = (float)(rand() %  width);
	}
	else if(tempy >= height)
	{
		Initialize();
		tempy = 0.0;
		tempx = (float)(rand() %  width);
	}
	
	x = tempx;
	y = tempy;
	
	

}

//Draw this star to screen
bool cStarData::Draw(SDL_Surface* screen)
{
	SDL_Rect area;

	area.x = x;
	area.y = y;
	area.w = starSize;
	area.h = starSize;
	SDL_FillRect(screen, &area, SDL_MapRGB(screen->format, 0xff, 0xff, 0xff ) );
		
	return true;
}


//❤`•.¸¸.•´´¯`••.¸¸.•´´¯`•´❤`•.¸¸.•´´¯`••.¸¸.•´´¯`•´❤`•.¸¸.•´´¯`••.¸¸.•´´¯`•´❤
//class:  cStarList
//
//Contains all of our stars in a STL vector
//
//❤`•.¸¸.•´´¯`••.¸¸.•´´¯`•´❤`•.¸¸.•´´¯`••.¸¸.•´´¯`•´❤`•.¸¸.•´´¯`••.¸¸.•´´¯`•´❤
class cStarList
{
private:
	std::vector<cStarData> vStars;
	int height, width;


public:

	cStarList(int StarCount, int width, int height);
	bool MoveStars(int mouseX, int mouseY, int ticks);
	bool DrawStars(SDL_Surface* screen);
	~cStarList();

};

cStarList::cStarList(int StarCount, int width, int height)
{
	this->height = height;
	this->width = width;

	cStarData _pStarData;// = new Sprite();
	for (int i = 0; i < StarCount; i++)
	{
		(&_pStarData)->Initialize(width, height);
		vStars.push_back(_pStarData);
	}
}


bool cStarList::MoveStars(int mouseX, int mouseY, int ticks)
{
	std::vector<cStarData>::iterator siStarData;
	if (mouseX != width/2 && mouseY !=height/2)
		for (siStarData = vStars.begin(); siStarData != vStars.end(); siStarData++)
		{
			siStarData->moveStar(mouseX, mouseY, ticks);
		}

	return true;
}

bool cStarList::DrawStars(SDL_Surface* screen)
{
	std::vector<cStarData>::iterator siStarData;
	for (siStarData = vStars.begin(); siStarData != vStars.end(); siStarData++)
	{
		(siStarData)->Draw(screen);
	}
	return true;
}


cStarList::~cStarList()
{
	// Delete and remove the sprites in the sprite vector
	std::vector<cStarData>::iterator siStarData;
	for (siStarData = vStars.begin(); siStarData != vStars.end(); siStarData++)
	{
		vStars.erase(siStarData);
		siStarData--;
	}

}
