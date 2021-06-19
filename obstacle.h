#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <bits/stdc++.h>
//#include "texturelib.h"



class Obstacle
{
	public:
		int Obstacle_WIDTH = 55;
		int Obstacle_HEIGHT = 55;
		double mPosX, mPosY;
		int mVelX;
		int flag_of_obstacle;
		int hitten;
		//Maximum axis velocity of the Obstacle
		static const int Obstacle_VEL = -4;
		//double Obstacle_VEL = -(8+hello);

		//Initializes the variables
		Obstacle();

		//Takes key presses and adjusts the Obstacle's velocity
		//Moves the Obstacle
		void move(double speed);
		void close();
		void init();
		//Shows the Obstacle on the screen
		void render();
		SDL_Rect Obstacle_rect; 

    private:
		//The X and Y offsets of the Obstacle
		//The velocity of the Obstacle
		int fuck;
};
void Obstacle::init()
{
	mVelX=Obstacle_VEL;
	flag_of_obstacle=0;
	mPosY=GROUND+30;
	mPosX=840 + rand()%15000;
	Obstacle_rect.w=Obstacle_WIDTH;
	Obstacle_rect.h=Obstacle_HEIGHT;
	hitten=0;
}
Obstacle::Obstacle()
{
	
	mVelX=Obstacle_VEL;
	flag_of_obstacle=0;
	mPosY=GROUND+30;
	mPosX=840 + rand()%15000;
	Obstacle_rect.w=Obstacle_WIDTH;
	Obstacle_rect.h=Obstacle_HEIGHT;
	hitten=0;
}	

void Obstacle::move(double speed)
{
	mPosX+=mVelX-speed;
	Obstacle_rect.x=mPosX;
	Obstacle_rect.y=mPosY;
}







