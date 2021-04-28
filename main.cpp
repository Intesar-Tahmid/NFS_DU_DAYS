#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include "texturelib.h"
#include <SDL2/SDL_mixer.h>
#include <bits/stdc++.h>
#include <SDL2/SDL_ttf.h>
using namespace std;

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

int where = MENU, whereInMenu;



bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "NFS - DU Days", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
			}
		}
	}
	if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
	{
		printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}
	if( TTF_Init() == -1 )
	{
		printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}

	return success;
}

bool loadMedia()
{	
	//Loading success flag
	bool success = true;

	/*//Load Fonts
	if( !U_BG.loadFromFile( "test.png" ) )
	{
		printf( "Failed to load MENU texture!\n" );
		success = false;
	}
	//Open the font
	gFont = TTF_OpenFont( "lazy.ttf", 28 );
	sFont = TTF_OpenFont( "score.ttf", 28 );
	mainFont = gFont;
	if( gFont == NULL )
	{
		printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}
	else
	{	
		//Render the prompt
		TTF_SetFontStyle(mainFont, TTF_STYLE_BOLD);
		//TTF_SetFontStyle(sFont, TTF_STYLE_BOLD);
		SDL_Color textColor = { 255, 255, 255, 0xFF };
		string T = "Enter Handle : ";
		if( !gPromptTextTexture.loadFromRenderedText( T.c_str(), textColor ) )
		{
			printf( "Failed to render prompt text!\n" );
			success = false;
		}
	}*/

	//Load Menu

    if( !gMenuTexture.loadFromFile( "Menu/Menu.png" ) )
	{
		printf( "Failed to load MENU texture!\n" );
		success = false;
	}

    if( !gMenuPlayTexture.loadFromFile( "Menu/Menu_Play.png" ) )
    {
        printf( "Failed to load MENU texture!\n" );
        success = false;
    }
    if( !gMenuInsTexture.loadFromFile( "Menu/Menu_Ins.png" ) )
    {
        printf( "Failed to load MENU texture!\n" );
        success = false;
    }
    if( !gMenuLeadTexture.loadFromFile( "Menu/Menu_Lead.png" ) )
    {
        printf( "Failed to load MENU texture!\n" );
        success = false;
    }
    if( !gMenuExitTexture.loadFromFile( "Menu/Menu_Exit.png" ) )
    {
        printf( "Failed to load MENU texture!\n" );
        success = false;
    }
    if( !gBGTexture.loadFromFile( "Background/BG_Scroll.png" ) )
    {
        printf( "Failed to load Background texture!\n" );
        success = false;
    }

	return success;
}
void changeFromMenu(int x, int y) {

	if(x>=885 && x<=1151){
        if(whereInMenu == EXIT){
            where = EXIT;
            return;
        }
        if(whereInMenu == LEADERBOARD){
        	where = LEADERBOARD;
        	return;
        }
		if(whereInMenu == PLAY){
        	where = PLAY;
        	return;
        }
        if(whereInMenu == INSTRUCTION){
        	where = INSTRUCTION;
        	return;
        }
	}
	return;
}

void changeMenuAnimation(int x, int y)
{
	if(x>=885 && x<=1151)
	{
		if(y>=235 && y<=309)
		{
			whereInMenu = PLAY;
			return;
		}
		if(y>=309 && y<=383)
		{
			whereInMenu = INSTRUCTION;
			return;
		}
		if(y>=383 && y<=457)
		{
			whereInMenu =LEADERBOARD;
			return;
		}
		if(y>=457 && y<=531)
		{
			whereInMenu = EXIT;
			return;
		}
	}
	whereInMenu = 0;
}

void handleMenuEvent(SDL_Event& e)
{
	int mx,my,wx,wy;
	if(e.type == SDL_MOUSEMOTION){

        SDL_GetGlobalMouseState(&mx, &my);
        SDL_GetWindowPosition(gWindow, &wx, &wy);
        changeMenuAnimation(mx - wx, my - wy);
    }

	if(e.type == SDL_MOUSEBUTTONUP) {

		SDL_GetGlobalMouseState(&mx, &my);
		SDL_GetWindowPosition(gWindow, &wx, &wy);
		changeFromMenu(mx - wx, my - wy);
	}
}

void close()
{
	//Free loaded images
	gBGTexture.free();

	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

int main( int argc, char* args[] )
{
	//Start up SDL and create window
	srand(time(0));
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{	
			bool quit = false;

			//Event handler
			SDL_Event e;

			int scrollingOffset = 0;

			while( !quit )
			{	
				

				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}
					handleMenuEvent(e);
				}

				if(where == MENU) {

                    if(whereInMenu == PLAY){
                        SDL_SetRenderDrawColor( gRenderer, 255, 255, 255, 255 );
                        SDL_RenderClear( gRenderer );

                        //Render background
                        gMenuPlayTexture.render( 0, 0 );
                        gMenuPlayTexture.render( 0 + gMenuPlayTexture.getWidth(), 0 );
                        SDL_RenderPresent( gRenderer );
                    }
                    if(whereInMenu == INSTRUCTION){
                        SDL_SetRenderDrawColor( gRenderer, 255, 255, 255, 255 );
                        SDL_RenderClear( gRenderer );

                        //Render background
                        gMenuInsTexture.render( 0, 0 );
                        gMenuInsTexture.render( 0 + gMenuInsTexture.getWidth(), 0 );
                        SDL_RenderPresent( gRenderer );
                    }
                    if(whereInMenu == LEADERBOARD){
                        SDL_SetRenderDrawColor( gRenderer, 255, 255, 255, 255 );
                        SDL_RenderClear( gRenderer );

                        //Render background
                        gMenuLeadTexture.render( 0, 0 );
                        gMenuLeadTexture.render( 0 + gMenuLeadTexture.getWidth(), 0 );
                        SDL_RenderPresent( gRenderer );
                    }
                    if(whereInMenu == EXIT){
                        SDL_SetRenderDrawColor( gRenderer, 255, 255, 255, 255 );
                        SDL_RenderClear( gRenderer );

                        //Render background
                        gMenuExitTexture.render( 0, 0 );
                        gMenuExitTexture.render( 0 + gMenuExitTexture.getWidth(), 0 );
                        SDL_RenderPresent( gRenderer );
                    }

					if(whereInMenu == 0){
                        SDL_SetRenderDrawColor( gRenderer, 255, 255, 255, 255 );
                        SDL_RenderClear( gRenderer );

                        //Render background
                        gMenuTexture.render( 0, 0 );
                        gMenuTexture.render(gMenuTexture.getWidth(), 0 );
                        SDL_RenderPresent( gRenderer );
                    }
				}

				else if(where == PLAY)
				{
					
					//Scroll background
                	scrollingOffset-=4;
                	if( scrollingOffset < -gBGTexture.getWidth() )
                	{	
                    	scrollingOffset = 0;
                	}

                	//Clear screen
                	SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                	SDL_RenderClear( gRenderer );

                	//Render background
                	gBGTexture.render( scrollingOffset, 0 );
                	gBGTexture.render( scrollingOffset + gBGTexture.getWidth(), 0 );


                	//Update screen
                	SDL_RenderPresent( gRenderer );
				}

                else if(where == EXIT) {
                    quit = true;
                }
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}