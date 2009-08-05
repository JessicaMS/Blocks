//-----------------------------------------------------------------
// Window code
// C++ Source - SDLwindow.cpp
//-----------------------------------------------------------------


//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "SDLwindow.h"


Window::Window(int screenWidth, int screenHeight, int screenBPP, std::string title)
{

	this->screenWidth = screenWidth;
	this->screenHeight= screenHeight;
	this->screenBPP = screenBPP;
	this->title = title;
	
	
    //Set up the screen
    screen = SDL_SetVideoMode(screenWidth,screenHeight, screenBPP, SDL_HWSURFACE);

    //If there's an error
    if( screen == NULL )
    {
        windowOK = false;
        return;
    }
    else
    {
        windowOK = true;
    }

    //Set the window caption
    SDL_WM_SetCaption( this->title.c_str(), NULL );

    //Set window flag
    windowed = true;
}

void Window::handle_events(SDL_Event &event)
{
    //If there's something wrong with the window
    if( windowOK == false )
    {
        return;
    }

      
    //If the window focus changed
    else if( event.type == SDL_ACTIVEEVENT )
    {

    /*
        //If something happened to the keyboard focus
        else if( event.active.state & SDL_APPINPUTFOCUS )
        {
            //If the application lost keyboard focus
            if( event.active.gain == 0 )
            {
                SDL_WM_SetCaption( "Stars: Keyboard focus lost", NULL );
            }
            else
            {
                SDL_WM_SetCaption( "Stars", NULL );
            }
        }
        //If something happened to the mouse focus
        else if( event.active.state & SDL_APPMOUSEFOCUS )
        {
            //If the application lost mouse focus
            if( event.active.gain == 0 )
            {
                SDL_WM_SetCaption( "Stars: Mouse Focus Lost", NULL );
            }
            else
            {
                SDL_WM_SetCaption( "Stars", NULL );
            }
        }*/
    }
    //If the window's screen has been altered
    else if( event.type == SDL_VIDEOEXPOSE )
    {
        //Update the screen
        if( SDL_Flip( screen ) == -1 )
        {
            //If there's an error
            windowOK = false;
            return;
        }
    }
}

void Window::toggle_fullscreen()
{
    //If the screen is windowed
    if( windowed == true )
    {
        //Set the screen to fullscreen
        screen = SDL_SetVideoMode(screenWidth, screenHeight, screenBPP, SDL_SWSURFACE | SDL_FULLSCREEN );
	
        //If there's an error
        if( screen == NULL )
        {
            windowOK = false;
            return;
        }

        //Set the window state flag
        windowed = false;
    }
    //If the screen is fullscreen
    else if( windowed == false )
    {
        //Window the screen
        screen = SDL_SetVideoMode( screenWidth, screenHeight, screenBPP, SDL_SWSURFACE );

        //If there's an error
        if( screen == NULL )
        {
            windowOK = false;
            return;
        }

        //Set the window state flag
        windowed = true;
    }
}

bool Window::error()
{
    return !windowOK;
}


	
