#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include "texturelib.h"
#include "karim.h"
#include <SDL2/SDL_mixer.h>
#include <bits/stdc++.h>
#include <SDL2/SDL_ttf.h>
#include <sstream>
using namespace std;

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

int where = MENU, whereInMenu, musicOn = 1;

Mix_Music *gMenuMusic = NULL;
Mix_Music *gPlayMusic = NULL;

int frame = 0;
int startTime = 0;
int level = 1,lFlag=0,lAns=WRONG;
string PlayerHandle = " ";
double prevhello = 0;	

//new addition below

char getScore[100], leaderboardScore[200];

bool scoreLoaded;
struct scoreData{
    char handle[100];
    int score;
    bool operator < (const scoreData &p) const {
        if(score == p.score) return strcmp(handle, p.handle) < 0;
        return score > p.score;
    }
};
std::set< scoreData > LeaderboardData;



void loadScoreFromFile() {

	scoreLoaded = true;

	FILE *readF = fopen("leaderboard", "r");

    if(readF == NULL) {
        cout << "No File Found" << endl;
    }
    else {
        scoreData temp;

        int rank;
        while(fscanf(readF, "%d %s %d", &rank, temp.handle, &temp.score) != EOF) LeaderboardData.insert( temp );
        //cout<<"hoy"<<endl;
    }
    fclose(readF);
}

void updateScores() {
	loadScoreFromFile();
	FILE *writeF = fopen("leaderboard", "w");
	bool scoreLoaded = false;
    int rank = 0;
    for(auto user : LeaderboardData) {
        fprintf(writeF, "%d %s %d\n",++rank, user.handle, user.score);
        if(rank == 10) break;
    }
    fclose(writeF);
}

//new addition above
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

	//Load Fonts
	if(!gUBGTexture.loadFromFile("Mode/Username.png"))
	{
		printf("Failer to load Username!\n");
		success = false;
	}
	gFont = TTF_OpenFont("Fonts/spyagency.ttf", 28);
	sFont = TTF_OpenFont("Fonts/score.ttf", 28);
	mainFont = gFont;
	if(gFont == NULL)
	{
		printf("Failed to load spyagency font! SDL_ttf Error: %s\n", TTF_GetError());
	}

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
    if( !gInsTexture.loadFromFile( "Instructions/Ins.png" ) )
    {
        printf( "Failed to load Instructions texture!\n" );
        success = false;
    }
    if( !gIns1Texture.loadFromFile( "Instructions/Ins_1.png" ) )
    {
        printf( "Failed to load Instructions1 texture!\n" );
        success = false;
    }
    if( !gModeTexture.loadFromFile( "Mode/Mode.png" ) )
    {
        printf( "Failed to load Mode texture!\n" );
        success = false;
    }
    gMenuMusic = Mix_LoadMUS( "Sounds/S1.wav" );
    if( gMenuMusic == NULL )
    {
	printf( "Failed to load Menu music! SDL_mixer Error: %s\n", Mix_GetError() );
	success = false;
    }
    gPlayMusic = Mix_LoadMUS( "Sounds/S1.wav" );
    if( gPlayMusic == NULL )
    {
	printf( "Failed to load Play music! SDL_mixer Error: %s\n", Mix_GetError() );
	success = false;
    }
    if( !gKarim.loadFromFile( "karim.png" ) )
	{
		printf( "Failed to load walking animation texture!\n" );
		success = false;
	}	
	else
	{
		//Set Birdy clips
	
		int ex = 0;
		for(int hp = 0; hp <4; hp++)
		{
			Karimclips[ hp ].x = ex;
			Karimclips[ hp ].y = 0;
			Karimclips[ hp ].w = 63;
			Karimclips[ hp ].h = 85;
			ex += 63;
		}
	
		ex = 0;
		for(int hp = 4; hp <8; hp++)
		{
			Karimclips[ hp ].x = ex;
			Karimclips[ hp ].y = 85;
			Karimclips[ hp ].w = 63;
			Karimclips[ hp ].h = 85;
			ex += 63;
		}  
	}

	if( !gObstacleTexture.loadFromFile( "obstacle.png" ) )
    {
        printf( "Failed to load obstacle texture!\n" );
        success = false;
    }
    
    if( !gPlaneTexture.loadFromFile( "plane.png" ) )
    {
        printf( "Failed to load plane texture!\n" );
        success = false;
    }  

    if( !gFireballTexture.loadFromFile( "fireball.png" ) )
    {
        printf( "Failed to load fireball texture!\n" );
        success = false;
    } 

	//new add below
	if( !gLeaderBoard1.loadFromFile( "Leaderboard/Leaderboard1.png" ) )
	{
		printf( "Failed to load Leads texture!\n" );
		success = false;
	}
	if( !gLeaderBoard2.loadFromFile( "Leaderboard/Leaderboard2.png" ) )
	{
		printf( "Failed to load Leads texture!\n" );
		success = false;
	}

	//new add above

	if( !gHealthTexture.loadFromFile( "health/1h.png" ) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}
	if( !gPowerTexture.loadFromFile( "power/0.png" ) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	} 

	if( !gGameOverTexture.loadFromFile( "GameOver.png" ) )
	{
		printf( "Failed to load GameOver texture!\n" );
		success = false;
	} 
	if( !gR1Texture.loadFromFile( "Riddles/R1.png" ) )
	{
		printf( "Failed to load iddle texture!\n" );
		success = false;
	} 
	if( !gR2Texture.loadFromFile( "Riddles/R2.png" ) )
	{
		printf( "Failed to load iddle texture!\n" );
		success = false;
	} 
	if( !gR3Texture.loadFromFile( "Riddles/R3.png" ) )
	{
		printf( "Failed to load iddle texture!\n" );
		success = false;
	} 

	if( !gR4Texture.loadFromFile( "Riddles/R4.png" ) )
	{
		printf( "Failed to load iddle texture!\n" );
		success = false;
	} 
	if( !gR5Texture.loadFromFile( "Riddles/R5.png" ) )
	{
		printf( "Failed to load iddle texture!\n" );
		success = false;
	} 

	if( !gR6Texture.loadFromFile( "Riddles/R6.png" ) )
	{
		printf( "Failed to load iddle texture!\n" );
		success = false;
	} 

	if( !gR7Texture.loadFromFile( "Riddles/R7.png" ) )
	{
		printf( "Failed to load iddle texture!\n" );
		success = false;
	} 
	if( !gWrongTexture.loadFromFile( "Wrong.png" ) )
	{
		printf( "Failed to load wrongtexture!\n" );
		success = false;
	} 
	if( !gRightTexture.loadFromFile( "Right.png" ) )
	{
		printf( "Failed to load right texture!\n" );
		success = false;
	} 

	if( !gCongratsTexture.loadFromFile( "Congrats.png" ) )
	{
		printf( "Failed to load congrats texture!\n" );
		success = false;
	} 

	if( !gGO2Texture.loadFromFile( "GameOver2.png" ) )
	{
		printf( "Failed to load GameOver2 texture!\n" );
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
        	where = USERNAME;
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
	if(e.type == SDL_MOUSEMOTION)
	{

        SDL_GetGlobalMouseState(&mx, &my);
        SDL_GetWindowPosition(gWindow, &wx, &wy);
        changeMenuAnimation(mx - wx, my - wy);
    }

	if(e.type == SDL_MOUSEBUTTONUP) 
	{

		SDL_GetGlobalMouseState(&mx, &my);
		SDL_GetWindowPosition(gWindow, &wx, &wy);
		changeFromMenu(mx - wx, my - wy);
	}
}

int whereInInstructions;

void changeFromInstructions(int x, int y)
{
	if(whereInInstructions == BACK)
	{
		where = MENU;
		whereInInstructions = 0;
		return;
	}
}

void changeInstructionsAnimation(int x, int y)
{
	if(x >= 37 && x<=223 && y >= 31 && y<=97)
	{
		whereInInstructions = BACK;
		return;
	}
	whereInInstructions = 0;
}

void handleInstructionEvent(SDL_Event &e)
{
	int mx, my, wx, wy;
	if(e.type == SDL_MOUSEMOTION){

        SDL_GetGlobalMouseState(&mx, &my);
        SDL_GetWindowPosition(gWindow, &wx, &wy);
        changeInstructionsAnimation(mx - wx, my - wy);
    }

	if(e.type == SDL_MOUSEBUTTONUP) {

		SDL_GetGlobalMouseState(&mx, &my);
		SDL_GetWindowPosition(gWindow, &wx, &wy);
		changeFromInstructions(mx - wx, my - wy);
	}

}

//new addition below
int whereInLeaderBoard;

void changeFromLeaderBoard(int x, int y) {

	if(whereInLeaderBoard == BACK) {
		where = MENU;
		whereInLeaderBoard = 0;
		return;
	}
}

void changeLeaderBoardAnimation(int x, int y){

	if(x >= 37 && x <= 223 && y >= 28 and y <= 95) {
		whereInLeaderBoard = BACK;
		return; 
	}
	whereInLeaderBoard = 0;
}

void handleLeaderBoardEvents(SDL_Event& e) {

	int mx, my, wx, wy;

	if(e.type == SDL_MOUSEMOTION){

        SDL_GetGlobalMouseState(&mx, &my);
        SDL_GetWindowPosition(gWindow, &wx, &wy);
        // cout << mx - wx << ' ' << my  - wy << endl;
        changeLeaderBoardAnimation(mx - wx, my - wy);
    }

	if(e.type == SDL_MOUSEBUTTONUP) {

		SDL_GetGlobalMouseState(&mx, &my);
		SDL_GetWindowPosition(gWindow, &wx, &wy);
		// cout << mx - wx << ' ' << my  - wy << endl;
		changeFromLeaderBoard(mx - wx, my - wy);
	}
}
//new addition above

void close()
{
	//Free loaded images
	gBGTexture.free();
	gUBGTexture.free();
	gMenuPlayTexture.free();
	gMenuInsTexture.free();
	gMenuLeadTexture.free();
	gMenuExitTexture.free();
	gInsTexture.free();
	gIns1Texture.free();
	gModeTexture.free();
	gKarim.free();
	gObstacleTexture.free();
	gPlaneTexture.free();
	gFireballTexture.free();
	gLeaderBoard1.free();
	gLeaderBoard2.free();
	gHealthTexture.free();
	gGameOverTexture.free();
	gR1Texture.free();
	gR2Texture.free();
	gR3Texture.free();
	gR4Texture.free();
	gR5Texture.free();
	gR6Texture.free();
	gR7Texture.free();
	gWrongTexture.free();
	gRightTexture.free();
	gCongratsTexture.free();
	gGO2Texture.free();
	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;
	gHealthTexture.free();
	gPowerTexture.free();

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}
