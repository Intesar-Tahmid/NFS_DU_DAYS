#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <bits/stdc++.h>
//#include "texturelib.h"


using namespace std;

class Moving_Obstacle
{
	public:
		int Moving_Obstacle_WIDTH = 55;
		int Moving_Obstacle_HEIGHT = 55;
		double mPosX, mPosY;
		int mVelX;
		int flag_of_obstacle;
		int hitten;
		
		char path_of_Moving_Obstacle;
		//Maximum axis velocity of the Moving_Obstacle
		static const int Moving_Obstacle_VEL = -10;

		//Initializes the variables
		Moving_Obstacle();

		//Takes key presses and adjusts the Moving_Obstacle's velocity
		//Moves the Moving_Obstacle
		void move(double speed);
		void close();
		void init();
		//Shows the Moving_Obstacle on the screen
		void render();
		SDL_Rect Moving_Obstacle_rect; 

    private:
		//The X and Y offsets of the Moving_Obstacle
		//The velocity of the Moving_Obstacle
		int f;
};

void Moving_Obstacle::init()
{
	mVelX=Moving_Obstacle_VEL;
	flag_of_obstacle=0;

	mPosX=SCREEN_WIDTH+ rand()%15000;
	Moving_Obstacle_rect.w=Moving_Obstacle_WIDTH;
	Moving_Obstacle_rect.h=Moving_Obstacle_HEIGHT;
	hitten=0;
	
}

Moving_Obstacle::Moving_Obstacle()
{
	
	mVelX=Moving_Obstacle_VEL;
	flag_of_obstacle=0;
	mPosY=GROUND -150;
	mPosX=SCREEN_WIDTH+ rand()%15000;
	Moving_Obstacle_rect.w=Moving_Obstacle_WIDTH;
	Moving_Obstacle_rect.h=Moving_Obstacle_HEIGHT;
	hitten=0;

}	

void Moving_Obstacle::move(double speed)
{
	mPosX+=mVelX-speed;
	Moving_Obstacle_rect.x=mPosX;
	Moving_Obstacle_rect.y=mPosY;
}





