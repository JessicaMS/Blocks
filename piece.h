//-----------------------------------------------------------------
// Piece for a tetris-like game
// C++ Header - piece.h
//-----------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
//#include <windows.h>
#include <string>
#include "SDL/SDL.h"

class Piece
{
	bool drop;
	bool downPressed;
	bool Rote;
	int pieceData[7][4][4][4];//Piece #, rotation, then x,y
	int pieceRotations[7];
	int colorTable[7];
	int x, y, type, rotation;
	int offsetx;

public:
	Piece();

	void handle_input(SDL_Event &event);
	void Random();
	void Rotate(int field[10][21]);
	int pieceAt(int x, int y);  //Send X, Y to ask...  Piece knows # and rotation, returns color #
	int getX();
	int getY();
	bool CheckIfCanDrop(int field[10][21]);
	bool Drop(int field[10][21]);
	bool CheckCollision(int field[10][21],int x,int y,int type,int rotation);
	bool getDrop();
	void setDrop(bool drop);
	void MoveX(int field[10][21]);
	int getOffset();
	bool getRote();
	bool getdownPressed();

};
