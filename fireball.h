#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
//#include "texturelib.h"



class Fireball
{
	public:
		static const int Fireball_WIDTH = 20;
		static const int Fireball_HEIGHT = 20;
		int mPosX, mPosY;
		int mVelX;
		int flag_of_fireball;
		//Maximum axis velocity of the Naruto
		static const int Fireball_VEL = 12;

		//Initializes the variables
		Fireball();

		//Takes key presses and adjusts the Naruto's velocity
		//Moves the Naruto
		void move();
		void close();
		//Shows the Naruto on the screen
		void render();
		SDL_Rect Fireball_rect;

    private:
		//The X and Y offsets of the Naruto
		//The velocity of the Naruto
		int f;
};

Fireball::Fireball()
{
	
	mVelX=Fireball_VEL;
	flag_of_fireball=0;
	Fireball_rect.w=Fireball_WIDTH;
	Fireball_rect.h=Fireball_HEIGHT;
}	

void Fireball::move()
{
	mPosX+=mVelX;
	Fireball_rect.x=mPosX;
	Fireball_rect.y=mPosY;
}





