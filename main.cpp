#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include "funcs.h"
#include <SDL2/SDL_mixer.h>
#include <bits/stdc++.h>
#include <SDL2/SDL_ttf.h>
#include <sstream>
using namespace std;

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
							//power_path="power/0.png";
							//gPowerTexture.loadFromFile(power_path);
                            Karim.Life = 100;
                            //Karim.Power=0;
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
						/*if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_F1){
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
						}*/
						Karim.handleEvent(e);
					}

					else if(where == TH)
					{
						Karim.handleEvent(e);
					}

					else if(where == LEADERBOARD) {

						handleLeaderBoardEvents(e);
					}
					else if(where == INSTRUCTION)
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
						where = LEVELS;
						lFlag = 0;
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
							for(int i=0;i<2;i++)
							{
								if(!fb[i].flag_of_fireball)
								{
									//Karim.Power--;
									//power_path[6]--;
									//gPowerTexture.loadFromFile(power_path);
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

				else if(where == LEVELS)
				{
					Mix_HaltMusic();
					SDL_SetRenderDrawColor( gRenderer, 255, 255, 255, 255 );
                   	SDL_RenderClear( gRenderer );
					switch(level)
					{
						case 1:
						{
                   			gR1Texture.render( 0, 0 );
              				gR1Texture.render(gR1Texture.getWidth(), 0 ); 
              				SDL_RenderPresent( gRenderer );
               				
                    		SDL_WaitEvent(&e);
                    		if(e.key.keysym.sym==SDLK_2)
							{
								level++;
										
								lAns = CORRECT;

							}
							else
							{
								lAns = WRONG;
							}
							lFlag=1;
							//cout<<hello<<endl;
							//where = TH;
                    	}break;

						case 2:
						{
                    		gR2Texture.render( 0, 0 );
                    		gR2Texture.render(gR2Texture.getWidth(), 0 ); 
                    		
                    		SDL_RenderPresent( gRenderer );
               				
                    		SDL_WaitEvent(&e);

                    		if(e.key.keysym.sym==SDLK_1)
							{
								level++;
										
								lAns = CORRECT;
							}
							else
							{
								lAns = WRONG;
							}
							lFlag=1;
							//cout<<hello<<endl;
						}break;

						case 3:
						{
                    		gR3Texture.render( 0, 0 );
                    		gR3Texture.render(gR3Texture.getWidth(), 0 ); 
                    		
                    		SDL_RenderPresent( gRenderer );
               				
                    		SDL_WaitEvent(&e);

                    		if(e.key.keysym.sym==SDLK_4)
							{
								level++;
										
								lAns = CORRECT;
							}
							else
							{
								lAns = WRONG;
							}
							lFlag=1;
						}break;

						case 4:
						{
                    		gR4Texture.render( 0, 0 );
                    		gR4Texture.render(gR4Texture.getWidth(), 0 ); 

                    		SDL_RenderPresent( gRenderer );
               				
                    		SDL_WaitEvent(&e);
                    		
                    		if(e.key.keysym.sym==SDLK_4)
							{
								level++;
										
								lAns = CORRECT;
								//lFlag = 0;
							}
							else
							{
								lAns = WRONG;
							}
							lFlag=1;
						}break;

						case 5:
						{
                   			gR5Texture.render( 0, 0 );
              				gR5Texture.render(gR5Texture.getWidth(), 0 ); 
              				SDL_RenderPresent( gRenderer );
               				
                    		SDL_WaitEvent(&e);
                    		if(e.key.keysym.sym==SDLK_1)
							{
								level++;
										
								lAns = CORRECT;

							}
							else
							{
								lAns = WRONG;
							}
							lFlag=1;
							//cout<<hello<<endl;
							//where = TH;
                    	}break;

						case 6:
						{
                   			gR6Texture.render( 0, 0 );
              				gR6Texture.render(gR6Texture.getWidth(), 0 ); 
              				SDL_RenderPresent( gRenderer );
               				
                    		SDL_WaitEvent(&e);
                    		if(e.key.keysym.sym==SDLK_4)
							{
								level++;
										
								lAns = CORRECT;

							}
							else
							{
								lAns = WRONG;
							}
							lFlag=1;
							//cout<<hello<<endl;
							//where = TH;
                    	}break;

						case 7:
						{
                   			gR7Texture.render( 0, 0 );
              				gR7Texture.render(gR7Texture.getWidth(), 0 ); 
              				SDL_RenderPresent( gRenderer );
               				
                    		SDL_WaitEvent(&e);
                    		if(e.key.keysym.sym==SDLK_1)
							{
								level++;
										
								lAns = CORRECT;

							}
							else
							{
								lAns = WRONG;
							}
							lFlag=1;
							//cout<<hello<<endl;
							//where = TH;
                    	}break;

						default:
						break;	

					}

					//lFlag = 1;
					
					if(lFlag){
						where = TH;
						//PauseOn = 0;
					}
				}

				else if(where == TH)
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
					
					if(!lFlag)
					{
						SDL_SetRenderDrawColor( gRenderer, 255, 255, 255, 255 );
                   		SDL_RenderClear( gRenderer );
						if(lAns == CORRECT)
						{
							prevhello = hello;
							if(level>7)
							{
								gCongratsTexture.render( 0, 0 );
              				 	gCongratsTexture.render(gCongratsTexture.getWidth(), 0 ); 
              				 	where = MENU;
              				 	recall = 1;

							}
							else
							{
								gRightTexture.render( 0, 0 );
              					gRightTexture.render(gRightTexture.getWidth(), 0 ); 
              					where = LEVELS;
							}

						}
						else
						{
							hello = prevhello;
							gWrongTexture.render( 0, 0 );
              				gWrongTexture.render(gWrongTexture.getWidth(), 0 ); 
              				where = LEVELS;
						}
						SDL_RenderPresent( gRenderer );
						SDL_Delay(3000);

						
					}

					else
					{
						gHealthTexture.loadFromFile(health_path);
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
							for(int i=0;i<2;i++)
							{
								if(!fb[i].flag_of_fireball)
								{
									//Karim.Power--;
									//power_path[6]--;
									//gPowerTexture.loadFromFile(power_path);
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
                		
						SDL_RenderPresent( gRenderer );
						//new add below
						if(Karim.Life<=0)
						{
							where = GAMEOVER2;
							Mix_HaltMusic();
							SDL_Delay(1000);
						}

						if(score>=100)
						{
							score = 0;
							lFlag = 0;
							Karim.Life = 100;
							health_char='1';
                            health_path="health/1h.png";
							cout<<Karim.Life<<endl;
							
							SDL_Delay(1000);
							//PauseOn = 1;
							//where = LEVELS;

						}

						

						//new add above
					}
					
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
					//writeSaveData();

					SDL_Delay(2000);
				}
				else if(where == GAMEOVER2)
				{

					SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
					SDL_RenderClear( gRenderer );
					gGO2Texture.render( 0, 0 );
                    gGO2Texture.render(gGO2Texture.getWidth(), 0 ); 
					//Update screen
					SDL_RenderPresent( gRenderer );

                    where = MENU;
                    whereInMenu = DEFAULT;
                    recall=1;
                    cout << "recalled" <<endl;
					//quit=true;

					SDL_Delay(2000);
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
