//-----------------------------------------------------------------
// SDL GUI code
// C++ Source - sdlgui.cpp
//-----------------------------------------------------------------

#include <string>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "sdlgui.h"




GUI_Button::GUI_Button()
{
	Initialize();
}





GUI_Button::GUI_Button(int x, int y)
{
	Initialize();
	coordX = x;
	coordY = y;

}

GUI_Button::GUI_Button(int x, int y,  std::string filename)
{
	Initialize();
	coordX = x;
	coordY = y;
	SetImgNormal(filename);
	fprintf(stderr, "Correct overloading!\n");
}

GUI_Button::GUI_Button(int x, int y,  std::string filename, 
			std::string filename2)
{
	Initialize();
	coordX = x;
	coordY = y;
	SetImgNormal(filename);
	SetImgMouseOver(filename2);
}

GUI_Button::GUI_Button(int x, int y,  std::string filename,
			std::string filename2, std::string filename3)
{
	Initialize();
	coordX = x;
	coordY = y;
	SetImgNormal(filename);
	SetImgMouseOver(filename2);
	SetImgMouseDown(filename3);
}

SDL_Surface* GUI_Button::load_image( std::string filename ) 
{
	//The image that's loaded
	SDL_Surface* loadedImage = NULL;

	//The optimized surface that will be used
	SDL_Surface* optimizedImage = NULL;

	//Load the image
	loadedImage = IMG_Load( filename.c_str() );

	//If the image loaded
	if( loadedImage != NULL )
	{
		//Create an optimized surface
		optimizedImage = SDL_DisplayFormat( loadedImage );

		//Free the old surface
		SDL_FreeSurface( loadedImage );

		//If the surface was optimized
		if( optimizedImage != NULL )
		{
		        //Color key surface
		        SDL_SetColorKey( optimizedImage, SDL_RLEACCEL | SDL_SRCCOLORKEY, SDL_MapRGB( optimizedImage->format, 0, 0xFF, 0x00) );
		}
	}

	//Return the optimized surface
	return optimizedImage;
}


bool GUI_Button::apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip)
{
    //Holds offsets
    SDL_Rect offset;
    
    //Get offsets
    offset.x = x;
    offset.y = y;
    
    //Blit
    SDL_BlitSurface( source, clip, destination, &offset );
	return true;
}

bool GUI_Button::Initialize()
{
	isMouseOver = false;
	isMouseDown = false;
	isMouseUp = false;
	wasClicked = false;
	ImgNormalSet = false;
	ImgMouseOverSet = false;
	ImgMouseDownSet = false;
	ImgNormal = NULL;
	ImgMouseDown = NULL;
	ImgMouseOver = NULL;
	coordX = coordY = 0;
	height = width = 0;

	return true;
}



bool GUI_Button::SetImgNormal( std::string filename)
{
	ImgNormal=load_image(filename);
	if(ImgNormal == NULL) return false;

	width = ImgNormal->w;
	height = ImgNormal->h;

	ImgNormalSet = true;
	return true;
}

bool GUI_Button::SetImgMouseOver( std::string filename)
{
	ImgMouseOver=load_image( filename);
	if(ImgMouseOver == NULL) return false;
	ImgMouseOverSet = true;
	return true;
}


bool GUI_Button::SetImgMouseDown( std::string filename)
{
	ImgMouseDown=load_image(filename);
	if(ImgMouseDown == NULL) return false;
	ImgMouseDownSet = true;
	return true;
}


bool GUI_Button::Draw(SDL_Surface* screen)
{
	if (ImgNormalSet == false && ImgMouseOverSet == false && ImgMouseDownSet == false)
		return false;

	if (ImgMouseDownSet == true && isMouseDown == true)
	{
		apply_surface( coordX , coordY, ImgMouseDown, screen);
		return true;
	}

	if (ImgMouseOverSet == true && isMouseOver == true)
	{
		apply_surface( coordX , coordY, ImgMouseOver, screen);
		return true;
	}

	if (ImgNormalSet == true)
	{
		apply_surface( coordX , coordY, ImgNormal, screen);
	}

	return false; //There aren't any appropriate images to print :(
}


bool GUI_Button::HandleEvents(SDL_Event &event)
{
	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);
	isMouseOver = wasClicked = false;

	if (mouseX>=coordX && mouseX<=(coordX+width) && mouseY>=coordY && mouseY<=(coordY+height))
	{
		isMouseOver = true;

		if ( event.type == SDL_MOUSEBUTTONUP )
		{

			if (isMouseDown == true)
			{
				wasClicked = true;
				
			}
			isMouseUp = true;
		} else isMouseUp = false;
	
		if ( event.type == SDL_MOUSEBUTTONDOWN )
		{
				isMouseDown = true;
			
		} else isMouseDown = false;
	}


	return true;
}

bool GUI_Button::CleanUp()
{
	if (ImgNormal != NULL) SDL_FreeSurface( ImgNormal );
	if (ImgMouseDown != NULL) SDL_FreeSurface( ImgMouseDown);
	if (ImgMouseOver != NULL) SDL_FreeSurface( ImgMouseOver);
	return true;
}


GUI_Button::~GUI_Button()
{
	CleanUp();
	//fprintf(stdout, "(Button deconstructed)");
}
