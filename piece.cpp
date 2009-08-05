//-----------------------------------------------------------------
// piece code
// C++ Source - piece.cpp
//-----------------------------------------------------------------


//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "piece.h"


Piece::Piece()
{
	int i,j,k,l;

	int pieceRotations[7] = {2, 4, 4, 1, 2, 2, 4};
	int colorTable[7] = {1, 2, 3, 1, 3, 2, 1};
	
	int pieceData[7][4][4][4] = {{{			   {0, 0, 0, 0},  //0, 0, X, Y
							   {0, 0, 0, 0},
							   {1, 1, 1, 1},
							   {0, 0, 0, 0}},
							  {{0, 1, 0, 0},  //0, 1, X, Y
							   {0, 1, 0, 0},
							   {0, 1, 0, 0},
	                           {0, 1, 0, 0}}},
								{{{0, 0, 0, 0},//1,0,x,y
								{0, 0, 0, 0},
								{1, 1, 1, 0},
								{0, 0, 1, 0}},
								{{0, 0, 0, 0},
								{0, 1, 0, 0},
								{0, 1, 0, 0},
								{1, 1, 0, 0}},
								{{0, 0, 0, 0},
								{1, 0, 0, 0},
								{1, 1, 1, 0},
								{0, 0, 0, 0}},
								{{0, 0, 0, 0},
								{0, 1, 1, 0},
								{0, 1, 0, 0},
								{0, 1, 0, 0}}},
								{{{0, 0, 0, 0},//2,0,x,y
								{0, 0, 0, 0},
								{1, 1, 1, 0},
								{1, 0, 0, 0}},
								{{0, 0, 0, 0},
								{0, 1, 0, 0},
								{0, 1, 0, 0},
								{0, 1, 1, 0}},
								{{0, 0, 0, 0},
								{0, 0, 1, 0},
								{1, 1, 1, 0},
								{0, 0, 0, 0}},
								{{0, 0, 0, 0},
								{1, 1, 0, 0},
								{0, 1, 0, 0},
								{0, 1, 0, 0}}},
								{{{0, 0, 0, 0}, //3
								{0, 1, 1, 0},
								{0, 1, 1, 0},
								{0, 0, 0, 0}}},
								{{{0, 0, 0, 0}, //4
								{0, 1, 0, 0},
								{1, 1, 0, 0},
								{1, 0, 0, 0}},
								{{0, 0, 0, 0},
								{0, 0, 0, 0},
								{1, 1, 0, 0},
								{0, 1, 1, 0}}},
								{{{0, 0, 0, 0}, //5
								{1, 0, 0, 0},
								{1, 1, 0, 0},
								{0, 1, 0, 0}},
								{{0, 0, 0, 0},
								{0, 1, 1, 0},
								{1, 1, 0, 0},}},
								{{{0, 0, 0, 0}, //6
								{0, 1, 0, 0},
								{1, 1, 0, 0},
  								{0, 1, 0, 0}},
								{{0, 0, 0, 0},
								{0, 1, 0, 0},
								{1, 1, 1, 0},
								{0, 0, 0, 0}},
								{{0, 0, 0, 0}, 
								{0, 1, 0, 0},
								{0, 1, 1, 0},
								{0, 1, 0, 0}},
								{{0, 0, 0, 0}, 
								{0, 0, 0, 0},
								{1, 1, 1, 0},
								{0, 1, 0, 0}}}};

	for(i=0; i<7; i++)
	{
		this->pieceRotations[i] = pieceRotations[i];
		this->colorTable[i] = colorTable[i];
	}
	for(i=0; i<7; i++)
		for(j=0; j<4; j++)
			for(k=0; k<4; k++)
				for(l=0; l<4; l++)
					this->pieceData[i][j][k][l] = pieceData[i][j][k][l];
	drop = false;
	offsetx = 0;
	Rote = false;
}





void Piece::handle_input(SDL_Event &event)
{
	//If a key was pressed
	if( event.type == SDL_KEYDOWN)
	{	
		//Set block movements
		switch( event.key.keysym.sym )
		{
		        case SDLK_UP: Rote = true; break;
		        case SDLK_DOWN: this->downPressed = true; break;
		        case SDLK_LEFT: this->offsetx=-1; break;
		        case SDLK_RIGHT: this->offsetx=1; break;   
			default: break;
			 
		}
	}
	//If a key was released
	else if( event.type == SDL_KEYUP )
	{
		 //Set block movements
		switch( event.key.keysym.sym )
		{
		        case SDLK_UP: 
				break;
		        case SDLK_DOWN: 
				this->downPressed = false;
				break;
		        case SDLK_LEFT: 
				if (this->offsetx == -1)this->offsetx = 0;
				break;
		        case SDLK_RIGHT:
				if (this->offsetx == 1) this->offsetx = 0;
				break;
							
			default: break;
			 
		}
		
	}


}
void Piece::Random()
{
	int i,j;
	int top;
	type = rand()%7; 
	top = -1;
	i = 0;
	while(i<4 && top == -1)
	{
		j = 0;
		while(j<4 && top == -1)
		{
			if (this->pieceData[type][0][i][j] == 1)
				top = i;  
			j++;
		}
		i++;
	}
	x = 2;
	y = 0 - top;
	rotation = 0;
}

void Piece::Rotate(int field[10][21])
{
	Rote = false;
	if (CheckCollision(field, this->x, this->y, this->type, (rotation + 1) % pieceRotations[type]) == false)
	{
		rotation = (rotation + 1) % pieceRotations[type];
	}

}

int Piece::pieceAt(int x, int y)
{
	return ( pieceData[type][rotation][x][y] * colorTable[type]);
}

int Piece::getX()
{
	return this->x;
}

int Piece::getY()
{
	return this->y;
}


bool Piece::CheckIfCanDrop(int field[10][21])
{
	if (CheckCollision(field, this->x, this->y+1, this->type, this->rotation) == false)
	{
		return true;
	}
	return false;

};


//Return true if dropped, false if it can't
bool Piece::Drop(int field[10][21])
{
	if (CheckCollision(field, this->x, this->y+1, this->type, this->rotation) == false)
	{
		this->y++;
		return true;
	}
	return false;
}

bool Piece::CheckCollision(int field[10][21],int x,int y,int type,int rotation)
{
	int i,j;
	for(i=0; i<4; i++)
		for(j=0; j<4; j++)
		{
			if (y+i>0)
			{
				if ((pieceData[type][rotation][i][j] == 1) && (((x+j<0) || (x+j > 9)  || (y+i > 20))
					|| (field[x+j][y+i] != 0)))
				{
					return true;
				}
			}
		}								

	return false;
}


bool Piece::getDrop()
{
	return this->drop;

}
void Piece::setDrop(bool drop)
{
	this->drop = drop;
}

void Piece::MoveX(int field[10][21])
{
	if (CheckCollision(field, this->x+offsetx, this->y, this->type, this->rotation) == false)
	{
		this->x+=offsetx;
	}
}


int Piece::getOffset()
{
	return this->offsetx;
}

bool Piece::getRote()
{
	return Rote;
}

bool Piece::getdownPressed()
{
	return downPressed;
}
