//-----------------------------------------------------------------
// SDL UI controls for a game engine.
// C++ Header - sdlgui.h
//-----------------------------------------------------------------

#pragma once

//All of our GUI Objects will inherit from this more generic class of basic functionality.
//This way you can iterate through an STL container to:
//	re-initialize
//	draw
//	check input from
//	or destroy your objects
//

class GUI_Object
{
private:
	bool isMouseOver;
	bool isMouseDown;
		

public:

	virtual bool Initialize()
	{
		return true;
	}

	virtual bool Draw(SDL_Surface* screen)
	{
		return true;
	}

	virtual bool HandleEvents(SDL_Event &event)
	{
		return true;
	}

	virtual bool CleanUp()
	{
		return true;
	}

	
};


class GUI_Button : public GUI_Object
{
private:
	bool isMouseOver;
	bool isMouseDown;
	bool isMouseUp;
	bool wasClicked;
	bool ImgNormalSet;
	bool ImgMouseOverSet;
	bool ImgMouseDownSet;

	SDL_Surface* ImgNormal;
	SDL_Surface* ImgMouseOver;
	SDL_Surface* ImgMouseDown;

	int coordX, coordY;
	int width, height;


	
public:
	GUI_Button();
	GUI_Button(int x, int y);
	GUI_Button(int x, int y,  std::string filename);
	GUI_Button(int x, int y,  std::string filename, 
		   std::string filename2);
	GUI_Button(int x, int y,  std::string filename,
		   std::string filename2, std::string filename3);
	bool Initialize();
	SDL_Surface* load_image(std::string filename ); 
	bool apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL );
	bool Draw(SDL_Surface* screen);
	bool SetImgNormal( std::string filename);
	bool SetImgMouseOver( std::string filename);
	bool SetImgMouseDown( std::string filename);
	bool HandleEvents(SDL_Event &event);
	bool CleanUp();
	~GUI_Button();

	void setX(int x)
	{
		coordX = x;
		
	}
	void setY(int y)
	{
		coordY = y;
	}
	int getX()
	{
		return coordX;
	}
	int getY()
	{
		return coordY;
	}
	bool getMouseDown()
	{
		return isMouseDown;
	}
	bool getMouseOver()
	{
		return isMouseOver;
	}
	bool getClicked()
	{
		if (wasClicked == true)
		{
			wasClicked = false;
			return true;
		}
		return wasClicked;
	}

};
