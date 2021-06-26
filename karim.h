#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
//#include "constants.h"
//#include "texturelib.h"
#include <bits/stdc++.h>
using namespace std;

class Karim
{
    public:
		//The dimensions of the Karim
		static const int Karim_WIDTH = 63;
		static const int Karim_HEIGHT = 85;
		int mPosX, mPosY;
		//Maximum axis velocity of the Karim
		static const int Karim_VEL = 10;
		int jumped;
        int fireball_throwed;
		int Life;
		
		int hitten;
		//Initializes the variables
		Karim();

		//Takes key presses and adjusts the Karim's velocity
		void handleEvent( SDL_Event& e );

		//Moves the Karim
		void move();
		void jump();
		void init();
		//Shows the Karim on the screen
		void render(SDL_Rect* currentClip);
		double initial ;
		double velocity;
		double gravity;
		SDL_Rect Karim_Rect;
    private:
		//The X and Y offsets of the Karim
		

		//The velocity of the Karim
		int mVelX, mVelY;
};
void Karim::init()
{
	mPosX = 40;
    mPosY = GROUND;

    //Initialize the velocity

    mVelX = 0;
    mVelY = 0;
    jumped=0;
    hitten = 0;
    fireball_throwed=0;
    initial = -13;
    velocity=initial;
 	gravity=0.5;
 	Karim_Rect.w=Karim_WIDTH;
 	Karim_Rect.h=Karim_HEIGHT;
 	Life=100;
 	
}
Karim::Karim()
{
    //Initialize the offsets
    mPosX = 40;
    mPosY = GROUND;

    //Initialize the velocity
    hitten = 0;
    mVelX = 0;
    mVelY = 0;
    jumped=0;
    fireball_throwed=0;
    initial = -13;
    velocity=initial;
 	gravity=0.5;
 	Karim_Rect.w=Karim_WIDTH;
 	Karim_Rect.h=Karim_HEIGHT;
 	Life=100;
 	
}

void Karim::jump()
{
	if(velocity >= abs(initial)+1)
	{
		velocity=initial;
		jumped=0;
	}
	else
	{
		mPosY+=velocity;
		velocity+=gravity;

	}
}

void Karim::handleEvent( SDL_Event& e )
{
    // cout << mVelX << ' ';

    //If a key was pressed
	if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
    {
        //Adjust the velocity
        if(!PauseOn)
        switch( e.key.keysym.sym )
        {
        	case 'p': PauseOn=1;break;                  //if P is pressed, game is paused
            case SDLK_UP: mVelY -= Karim_VEL; break;
            case SDLK_DOWN: mVelY += Karim_VEL; break;
            case 'a': mVelX -= Karim_VEL; break;        //if A is pressed,runner goes left
            case 'd': mVelX += Karim_VEL; break;        //if D is pressed, runner goes right
            case 'w': jumped=1;break;                   //if W is pressed, runner jumps
            case 'q': fireball_throwed=1;break;         //if q is pressed, runner shoots fireballs
            
        }
        else
        {
        	if(e.key.keysym.sym=='p')
        	{
        		PauseOn=0;
        	}
        	mVelX=0;
        }
    }
    //If a key was released
    else if( e.type == SDL_KEYUP && e.key.repeat == 0 )
    {
        //Adjust the velocity
        if(!PauseOn)
        switch( e.key.keysym.sym )
        {
            case SDLK_UP: mVelY += Karim_VEL; break;
            case SDLK_DOWN: mVelY -= Karim_VEL; break;
            case 'a': mVelX += Karim_VEL; break;
            case 'd': mVelX -= Karim_VEL; break;
            case 'p': mVelX =0;break;
            //case 'q': fireball_throwed=0;break;

        }
        else
        	mVelX=0;
    }
   // cout << "Vel changed to " << mVelX <<endl;
   // cout << PauseOn << endl;
    if(PauseOn)
    {
    	mVelX=0;
    }
    // mVelX = min(0, mVelX);
    // cout << mVelX << endl;
}

void Karim::move()
{   
    //Move the Karim left or right
    if(jumped)
	{
		jump();
	}

    mPosX += mVelX;
    mPosX=max(mPosX, 40 );
    mPosX=min(mPosX, SCREEN_WIDTH/2 );
    //If the Karim went too far to the left or right
    

    //Move the Karim up or down
    mPosY += mVelY;

    //If the Karim went too far up or down
    if( ( mPosY < 0 ) || ( mPosY + Karim_HEIGHT > SCREEN_HEIGHT ) )
    {
        //Move back
        mPosY -= mVelY;
    }
    Karim_Rect.x=mPosX;
    Karim_Rect.y=mPosY;
}

void Karim::render(SDL_Rect* currentClip)
{
    gKarim.render(mPosX, mPosY, currentClip);
}
