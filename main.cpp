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
string PlayerHandle = " ";
//double hello = 0;	

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

struct saveData{
	int score;
	string handle, healthP, powerP;
	bool saved;
	saveData(int score = 0, string handle = "SYSTEM", string healthP = "health/1h.png", string powerP = "power/0.png", bool saved = false) :
		score(score), handle(handle), healthP(healthP), powerP(powerP), saved(saved) {};
}SLOT[6];

void readSaveData() {
	ifstream readF("savegames");
	if(!readF.is_open()){
		cout << "File Nai" << endl;
		return;
	} 
	for(int i = 0; i < 6;i++){
		readF >> SLOT[i].saved >> SLOT[i].score >> SLOT[i].handle >> SLOT[i].healthP >> SLOT[i].powerP;
	}
	readF.close();
}

void writeSaveData() {
	FILE *writeF = fopen("savegames", "w");
	for(int i = 0; i < 6;i++){
		fprintf(writeF, "%d %d %s %s %s\n", SLOT[i].saved, SLOT[i].score, SLOT[i].handle.c_str(), SLOT[i].healthP.c_str(), SLOT[i].powerP.c_str());
	}
	fclose(writeF);
}
void LoadSaveData(int slot, int &score, string &PlayerHandle,string &health_path,string &power_path, int &Nhealth, int &NPower){
	readSaveData();
	if(SLOT[slot].saved == false){
		cout << "Save Nai" << endl;
		return;
	}
	where = CLASSIC;
	mainFont = sFont;
	score = SLOT[slot].score;
	PlayerHandle = SLOT[slot].handle;
	health_path = SLOT[slot].healthP;
	power_path = SLOT[slot].powerP;

	Nhealth = 100 - (health_path[7] - '0' - 1)*25;
	NPower = power_path[6] - '0';

	gHealthTexture.loadFromFile(health_path);
	gPowerTexture.loadFromFile(power_path);

}

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
    gMenuMusic = Mix_LoadMUS( "Sounds/Apparat.wav" );
    if( gMenuMusic == NULL )
    {
	printf( "Failed to load Menu music! SDL_mixer Error: %s\n", Mix_GetError() );
	success = false;
    }
    gPlayMusic = Mix_LoadMUS( "Sounds/thor.wav" );
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
	if(x >= 55 && x<=233 && y >= 47 && y<=102)
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
			//The player running through the screen
			Karim Karim;
			bool recall=0;
			Fireball fb[5];
			Obstacle barrier[10];
			Moving_Obstacle plane[10];
			bool renderText = false;
			//new addition below
			int score=0,counter=0;
			char health_char='1';
			string health_path="health/1h.png";
			gHealthTexture.loadFromFile(health_path);
			string power_path="power/0.png";
			gPowerTexture.loadFromFile(power_path);
			//new addition above
			SDL_Color textColor = { 255, 255, 255, 0xFF };
			double scrollingOffset = 0;
			std::string inputText = "";
			SDL_StartTextInput();

			while( !quit )
			{	
				

				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}

					//Toggle music
					if(e.key.keysym.sym == SDLK_F5)
					{
						musicOn = 0;

					}
					if(e.key.keysym.sym == SDLK_F6)
					{
						musicOn = 1;	
					}

					if(where == MENU)
					{
						if(recall == 1)
						{
							scrollingOffset = 0;
							startTime=0;
							hello=0;
							/*new add below
							readSaveData();
                            writeSaveData();
                            //mainFont = gFont;
							*///new add avove
							for(int i=0;i<10;i++)
							{
								barrier[i].init();
								plane[i].init();
							}

							//new addition below
							
                            score=0;
                            counter=0;
                            health_char='1';
                            health_path="health/1h.png";
							gHealthTexture.loadFromFile(health_path);
							power_path="power/0.png";
							gPowerTexture.loadFromFile(power_path);
                            Karim.Life = 100;
                            Karim.Power=0;
							//new addition above
							renderText = false;
							inputText = " ";
							Karim.init();
							SDL_StartTextInput();
							recall = 0; 
						}

						/*if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_F1){
							LoadSaveData(0, score, PlayerHandle, health_path, power_path, Karim.Life, Karim.Power);
							// cout << score << ' ' << PlayerHandle << health_path << ' ' << power_path << endl;
						}
						else if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_F2){
							LoadSaveData(1, score, PlayerHandle, health_path, power_path, Karim.Life, Karim.Power);
						}
						else if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_F3){
							LoadSaveData(2, score, PlayerHandle, health_path, power_path, Karim.Life, Karim.Power);
						}
						else if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_F4){
							LoadSaveData(3, score, PlayerHandle, health_path, power_path, Karim.Life, Karim.Power);
						}
						else if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_F5){
							LoadSaveData(4, score, PlayerHandle, health_path, power_path, Karim.Life, Karim.Power);
						}
						else if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_F6){
							LoadSaveData(5, score, PlayerHandle, health_path, power_path, Karim.Life, Karim.Power);
						}*/
						handleMenuEvent(e);
					}

					else if(where == USERNAME)
					{
						//Karim.init();
						if( e.type == SDL_QUIT )
						{
							quit = true;
						}

						else if( e.type == SDL_KEYDOWN )
						{
							//Handle backspace
							if( e.key.keysym.sym == SDLK_BACKSPACE && inputText.length() > 0 )
							{
								//lop off character
								inputText.pop_back();
								renderText = true;
							}
							//Handle copy
							else if( e.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL )
							{
								SDL_SetClipboardText( inputText.c_str() );
							}
							//Handle paste
							else if( e.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL )
							{
								inputText = SDL_GetClipboardText();
								renderText = true;
							}
						}
						//Special text input event
						else if( e.type == SDL_TEXTINPUT )
						{
							//Not copy or pasting
							if( !( SDL_GetModState() & KMOD_CTRL && ( e.text.text[ 0 ] == 'c' || e.text.text[ 0 ] == 'C' || e.text.text[ 0 ] == 'v' || e.text.text[ 0 ] == 'V' ) ) )
							{
								//Append character
								inputText += e.text.text;
								renderText = true;
							}
						}
					}
					else if(where == CLASSIC)
					{
						if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_F1){
							SLOT[0] = saveData(score, PlayerHandle, health_path, power_path, true);
						}
						else if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_F2){
							SLOT[1] = saveData(score, PlayerHandle, health_path, power_path, true);
						}
						else if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_F3){
							SLOT[2] = saveData(score, PlayerHandle, health_path, power_path, true);
						}
						else if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_F4){
							SLOT[3] = saveData(score, PlayerHandle, health_path, power_path, true);
						}
						else if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_F5){
							SLOT[4] = saveData(score, PlayerHandle, health_path, power_path, true);
						}
						else if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_F6){
							SLOT[5] = saveData(score, PlayerHandle, health_path, power_path, true);
						}
						Karim.handleEvent(e);
					}

					if(where == LEADERBOARD) {

						handleLeaderBoardEvents(e);
					}
					if(where == INSTRUCTION)
					{
						handleInstructionEvent(e);

					}

				}

				if(where == MENU) 
				{

					if(musicOn)
					{
						if( Mix_PlayingMusic() == 0 )
						{
							//Play the music
							Mix_PlayMusic( gMenuMusic, -1 );
							
						}	
					}
					else 
					{
						Mix_HaltMusic();

					}

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

					if(whereInMenu == DEFAULT){
                        SDL_SetRenderDrawColor( gRenderer, 255, 255, 255, 255 );
                        SDL_RenderClear( gRenderer );

                        //Render background
                        gMenuTexture.render( 0, 0 );
                        gMenuTexture.render(gMenuTexture.getWidth(), 0 );
                        SDL_RenderPresent( gRenderer );
                    }
				}

				else if(where == LEADERBOARD) 
				{
					//cout<< "dhuklam" << endl;
					if(scoreLoaded == false) loadScoreFromFile();
					//cout<< "file loads" << endl;
					int Y = 260, rank = 0;

					mainFont = gFont;
					//cout<< "font loads" << endl;
					SDL_SetRenderDrawColor( gRenderer, 255, 255, 255, 255 );
                    SDL_RenderClear( gRenderer );
                    
                    if(!whereInLeaderBoard){
                    	gLeaderBoard1.render( 0, 0 );
                    	gLeaderBoard1.render( 0 + gLeaderBoard1.getWidth(), 0 );
                    }
                    else {

                    	gLeaderBoard2.render( 0, 0 );
                    	gLeaderBoard2.render( 0 + gLeaderBoard2.getWidth(), 0 );	
                    }

					for(auto user : LeaderboardData) {
						const int total_width = 40;
			            const int s_width = strlen(user.handle);
			            const int field_width = (total_width - s_width) / 2 + s_width;
			            sprintf(leaderboardScore, "%02d%*s%*s%4d",++rank,field_width + (s_width % 2), user.handle, field_width - s_width - 1 ,"", user.score);
						
						// cout << leaderboardScore << endl;
						//sprintf(leaderboardScore,"  %2d\t\t%20"

						gLeaderBoardScores[rank - 1].loadFromRenderedText( leaderboardScore, textColor );
						gLeaderBoardScores[rank - 1].render( 221 , Y);

						Y += 37;
					}
					SDL_RenderPresent( gRenderer );
				}

				else if(where == PLAY)
				{
					

					SDL_SetRenderDrawColor( gRenderer, 255, 255, 255, 255 );
                    SDL_RenderClear( gRenderer );
                    gModeTexture.render( 0, 0 );
                    gModeTexture.render(gModeTexture.getWidth(), 0 ); 
                    SDL_RenderPresent( gRenderer );

					
					if(e.key.keysym.sym==SDLK_ESCAPE)
					{
						Mix_HaltMusic();
						cout<< "Back To Menu" << endl;
						where = MENU;
					}

					if(e.key.keysym.sym==SDLK_c)
					{
						cout<< "CLASSIC Mode" << endl;
						where = CLASSIC;
						startTime = SDL_GetTicks();

					}

					if(e.key.keysym.sym==SDLK_t)
					{
						cout<< "Treasure Hunt Mode" << endl;
						where = TH;
					}

					
				}

				else if(where == USERNAME)
				{
					Mix_HaltMusic();
					if(e.key.keysym.sym==SDLK_ESCAPE)
					{
						cout<< "Back To Menu" << endl;
						where = MENU;
					}
					else if(e.key.keysym.sym==SDLK_RETURN)
					{
						where = PLAY;
					}
					if( renderText )
					{
						//Text is not empty
						if( inputText != "" )
						{
							//Render new text
							gInputTextTexture.loadFromRenderedText( inputText.c_str(), textColor );
						}
						//Text is empty
						else
						{
							//Render space texture
							gInputTextTexture.loadFromRenderedText( " ", textColor );
						}
					}	

					//Clear screen
					SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
					SDL_RenderClear( gRenderer );
					gUBGTexture.render( 0, 0 );
                    gUBGTexture.render(gUBGTexture.getWidth(), 0 ); 
					//Render text textures
					//gPromptTextTexture.render( ( SCREEN_WIDTH - gPromptTextTexture.getWidth() ) / 2, 0 );
					gInputTextTexture.render( ( SCREEN_WIDTH - gInputTextTexture.getWidth() ) / 2, ( SCREEN_HEIGHT - gInputTextTexture.getHeight() )/2);

					//Update screen
					SDL_RenderPresent( gRenderer );
				}

				else if(where == CLASSIC)
				{	
					if(musicOn)
					{
						if( Mix_PlayingMusic() == 0 )
						{
							//Play the music
							Mix_PlayMusic( gPlayMusic, -1 );
						}	
					}
					else Mix_HaltMusic();

					if(e.key.keysym.sym==SDLK_ESCAPE)
					{
						Mix_HaltMusic();
						cout<< "Back To Menu" << endl;
						where = MENU;
						recall = 1;
					}
					
					if(PauseOn)
					{
						SDL_RenderClear(gRenderer);
						gBGTexture.render(scrollingOffset,0);
						gBGTexture.render( scrollingOffset + gBGTexture.getWidth(), 0 );
						SDL_Rect* currentClip = &Karimclips[1];
						Karim.render(currentClip);
						SDL_RenderPresent(gRenderer);
					}

					else
					{
						Karim.hitten--;
						Karim.hitten = max(0, Karim.hitten);
						for(int i=0;i<10;i++)
						{
							if(!barrier[i].flag_of_obstacle)
							{
								if(rand()%2==0)
								{
									barrier[i].flag_of_obstacle=1;
								}
							}
							if(!plane[i].flag_of_obstacle)
							{
								if(rand()%1080==0)
								{
									plane[i].flag_of_obstacle=1;
								}
							}
						}
						Karim.move();

						if(Karim.fireball_throwed)
						{
							for(int i=0;i<5;i++)
							{
								if(!fb[i].flag_of_fireball)
								{
									Karim.Power--;
									power_path[6]--;
									gPowerTexture.loadFromFile(power_path);
									fb[i].flag_of_fireball=1;
									fb[i].mPosX=Karim.mPosX+Karim.Karim_WIDTH;
									fb[i].mPosY=Karim.mPosY+Karim.Karim_HEIGHT/2-5;
									break;
								}
							}
							Karim.fireball_throwed=0;
						}

						//Scroll background
                				scrollingOffset-=(4+hello);
                				//cout << "What up" << endl;
                				//cout <<hello << endl;
                    				hello+= .0005;
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
                	
                	
						++frame;

						//Cycle animation
						if( frame / 8 >= RUNNING )
						{
							frame = 0;
						}

						//Render current frame
						SDL_Rect* currentClip = &Karimclips[ frame / 8 ];
						Karim.render( currentClip );

						for(int i=0;i<5;i++)
						{
							if(fb[i].flag_of_fireball)
							{
								fb[i].move();
								fb[i].render();
								if(fb[i].mPosX>SCREEN_WIDTH)
								{
									fb[i].flag_of_fireball=0;
								}
							}
						}
						//hey hey
						//score = score + 1;
						for(int i=0;i<10;i++)
						{
							if(barrier[i].flag_of_obstacle)
							{
								barrier[i].move(hello);

								if(checkCollision(Karim.Karim_Rect,barrier[i].Obstacle_rect) && !barrier[i].hitten)
								{
									barrier[i].hitten = 1;
									//new addition below
									Karim.Life-=25;
									health_path[7]++;
									gHealthTexture.loadFromFile(health_path);
									Karim.hitten=77;
									cout << "Life :" << Karim.Life << endl;
									//new addition above
									//cout<<"Pathore bari khailam"<<endl;
								}
							}

							if(barrier[i].mPosX<=-10)
							{
								barrier[i].mPosX=SCREEN_WIDTH+rand()%15000;
								barrier[i].flag_of_obstacle=0;
								barrier[i].hitten=0;
								score += 10;
							}
							
							barrier[i].render();
							
						}

						for(int i=0;i<10;i++)
						{
							if(plane[i].flag_of_obstacle)
							{
								plane[i].move(hello);

								if(checkCollision(Karim.Karim_Rect,plane[i].Moving_Obstacle_rect) && !plane[i].hitten)
								{
									plane[i].hitten = 1;
									//new addition below
									Karim.Life-=25;
									health_path[7]++;
									gHealthTexture.loadFromFile(health_path);
									Karim.hitten=77;
									cout << "Life :" << Karim.Life << endl;
									//new addition above
									//cout<<"Plane e bari khailam"<<endl;
								}

							}

							if(plane[i].mPosX<=-10)
							{
								plane[i].mPosX=SCREEN_WIDTH+rand()%15000;
								plane[i].flag_of_obstacle=0;
								plane[i].hitten=0;
								score += 10;
							}
							if(plane[i].mPosX<SCREEN_WIDTH)
							{
								for(int j=0;j<5;j++)
								{
									if(checkCollision(fb[j].Fireball_rect, plane[i].Moving_Obstacle_rect) && fb[j].flag_of_fireball)
									{
										score += 50;
										plane[i].mPosX=SCREEN_WIDTH+(rand()%10)*(rand()%15000);
										plane[i].flag_of_obstacle=0;
										fb[j].flag_of_fireball=0;
										//cout<<"Plane pidailam"<<endl;
									}
								}
							}
							plane[i].render();
						}

						//new addition below
						sprintf(getScore, "Score : %04d", score);
						gScore.loadFromRenderedText( getScore, textColor );
						gScore.render( ( SCREEN_WIDTH - gScore.getWidth() - gScore.getWidth()) + 50, 35);
						
						gHealthTexture.render(50,30);
						//gPowerTexture.render(50,50);

						//new addition above

						//Go to next frame
						//Update screen
                		

						//new add below
						if(Karim.Life<=0)
						{
							where = GAMEOVER;
							Mix_HaltMusic();
							SDL_Delay(2000);
						}
						SDL_RenderPresent( gRenderer );

						//new add above
					}
					
				}

				else if(where == TH)
				{
					if(e.key.keysym.sym==SDLK_ESCAPE)
					{
						Mix_HaltMusic();
						cout<< "Back To Menu" << endl;
						where = MENU;
					}
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

				else if(where == GAMEOVER)
				{

                    

                    sprintf(getScore, "Your Score : %d", score);
                    gScore.loadFromRenderedText( getScore, textColor );
					SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
					SDL_RenderClear( gRenderer );
					gGameOverTexture.render( 0, 0 );
                    gGameOverTexture.render(gGameOverTexture.getWidth(), 0 ); 
					//Render text textures
					//gPromptTextTexture.render( ( SCREEN_WIDTH - gPromptTextTexture.getWidth() ) / 2, 0 );
					gScore.render((SCREEN_WIDTH - gScore.getWidth() ) / 2, ( SCREEN_HEIGHT - gScore.getHeight() )/2 - 10);

					//Update screen
					SDL_RenderPresent( gRenderer );

                    where = MENU;
                    whereInMenu = DEFAULT;
                    recall=1;
                    cout << "recalled" <<endl;
					//quit=true;
					scoreData now;
					
					strcpy(now.handle, inputText.c_str());
					now.score = score;

					LeaderboardData.insert(now);

					updateScores();
					cout<<"works"<<endl;
					writeSaveData();

					SDL_Delay(5000);
				}

				else if(where==INSTRUCTION)
				{
					//handleInstructionEvent(e);
					SDL_SetRenderDrawColor( gRenderer, 255, 255, 255, 255 );
                    SDL_RenderClear( gRenderer );
					if(!whereInInstructions){
                        
                        gInsTexture.render( 0, 0 );
                        gInsTexture.render(gInsTexture.getWidth(), 0 );
                        
                    }
                    else{

                        gIns1Texture.render( 0, 0 );
                        gIns1Texture.render(gIns1Texture.getWidth(), 0 );
                    }
                    SDL_RenderPresent( gRenderer );
                   
				}


                else if(where == EXIT) 
				{
                    quit = true;
                }
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}
