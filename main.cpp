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
		//loadmedia function loads all the media files that are used in the code
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

			bool recall=0; // Recall is used to get back to the menu with already saved information

			Fireball fb[5]; //Refers to the fireball that will be shot
			Obstacle barrier[10]; //Refers to the obstacles that have to avoided
			Moving_Obstacle plane[10]; //Refers to the obstacles that have to avoided or can be shot to earn points
			bool renderText = false;
		
			int score=0; //variable that will store the score of the gamer

			char health_char='1';
			string health_path="health/1h.png"; //path for calling the images that represent health
			gHealthTexture.loadFromFile(health_path);
			
			SDL_Color textColor = { 255, 255, 255, 0xFF };
			double scrollingOffset = 0;

			std::string inputText = ""; //string variable that will store the "Username"
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
					//Press F5 to turn the music off and F6 for turning on again
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
							scrollingOffset = 0; //Making the scrolling speed zero for starting over with this speed, not that of when the game was over
							startTime=0; //Flag to count the time spent from beginning again

							hello=0; 

							level = 1;
							//initializes obstacles
							for(int i=0;i<10;i++)
							{
								barrier[i].init(); 
								plane[i].init();
							}
							
                            score=0;

                            health_char='1';
                            health_path="health/1h.png";
							gHealthTexture.loadFromFile(health_path);

                            Karim.Life = 100; // Providing full life when restarting the game 

							renderText = false;
							inputText = " ";
							Karim.init(); //Initializing the running character again

							SDL_StartTextInput();
							recall = 0; 
						}

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
						Mix_HaltMusic(); //This library function stops the music

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
					if(scoreLoaded == false) loadScoreFromFile();
				
					int Y = 260, rank = 0;

					mainFont = gFont;
				
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
						const int total_width = 55;
			            const int s_width = strlen(user.handle); //Determining the length of the handle given by the user
			            const int field_width = (total_width - s_width) / 2 + s_width;
			            sprintf(leaderboardScore, "%02d%*s%*s%4d",++rank,field_width + (s_width % 2), user.handle, field_width - s_width - 5 ,"", user.score);

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

					//Pressing the "Esc" key takes one back to the menu
					if(e.key.keysym.sym==SDLK_ESCAPE)
					{
						Mix_HaltMusic();
						cout<< "Back To Menu" << endl;
						where = MENU;
					}
					//Press "C" key on keyboard to enter the Classic Mode
					if(e.key.keysym.sym==SDLK_c)
					{
						cout<< "CLASSIC Mode" << endl;
						where = CLASSIC;
						startTime = SDL_GetTicks();

					}
					//Press "T" key on keyboard to enter the Treasure Hunt Mode
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
						//Karim.hitten--;
						//Karim.hitten = max(0, Karim.hitten);
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

									fb[i].flag_of_fireball=1;
									fb[i].mPosX=Karim.mPosX+Karim.Karim_WIDTH;
									fb[i].mPosY=Karim.mPosY+Karim.Karim_HEIGHT/2-5;
									break;
								}
							}
							Karim.fireball_throwed=0;
						}

								//Scroll background
                				scrollingOffset-=(4+hello); //Determinant of the scrolling speed
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
						
						//score = score + 1;
						for(int i=0;i<10;i++)
						{
							if(barrier[i].flag_of_obstacle)
							{
								barrier[i].move(hello);

								//collision detection 
								if(checkCollision(Karim.Karim_Rect,barrier[i].Obstacle_rect) && !barrier[i].hitten)
								{
									barrier[i].hitten = 1;
									
									Karim.Life-=25; //Reduces life in each event of collision

									health_path[7]++;
									gHealthTexture.loadFromFile(health_path);

									Karim.hitten=77;
									cout << "Life :" << Karim.Life << endl; //In each loss of life, the remaining lives will be showed in the terminal
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
									
									Karim.Life-=25;  //Reduces life in each event of collision

									health_path[7]++;
									gHealthTexture.loadFromFile(health_path);

									Karim.hitten=77;
									cout << "Life :" << Karim.Life << endl; //In each loss of life, the remaining lives will be showed in the terminal
									
								}

							}

							if(plane[i].mPosX<=-10)
							{
								plane[i].mPosX=SCREEN_WIDTH+rand()%15000;
								plane[i].flag_of_obstacle=0;
								plane[i].hitten=0;
								score += 10; //Adds 10 to score in each event of avoidance
							}
							if(plane[i].mPosX<SCREEN_WIDTH)
							{
								for(int j=0;j<5;j++)
								{
									if(checkCollision(fb[j].Fireball_rect, plane[i].Moving_Obstacle_rect) && fb[j].flag_of_fireball)
									{
										score += 50; //Adds 50 to the score in each event of destroying a plane

										plane[i].mPosX=SCREEN_WIDTH+(rand()%10)*(rand()%15000);
										plane[i].flag_of_obstacle=0;

										fb[j].flag_of_fireball=0;
										
									}
								}
							}
							plane[i].render();
						}

						sprintf(getScore, "Score : %04d", score);
						gScore.loadFromRenderedText( getScore, textColor );
						gScore.render( ( SCREEN_WIDTH - gScore.getWidth() - gScore.getWidth()) + 50, 35);
						
						gHealthTexture.render(50,30);
					

						//Go to next frame
						//Update screen
                		
						if(Karim.Life<=0)
						{
							where = GAMEOVER;
							Mix_HaltMusic();
							SDL_Delay(2000);
						}
						SDL_RenderPresent( gRenderer );

					}
					
				}
				//Levels in the Treasure Hunt Mode
				else if(where == LEVELS)
				{
					Mix_HaltMusic();
					SDL_SetRenderDrawColor( gRenderer, 255, 255, 255, 255 );
                   	SDL_RenderClear( gRenderer );
					switch(level)
					{
						case 1: //Level 1
						{
                   			gR1Texture.render( 0, 0 );
              				gR1Texture.render(gR1Texture.getWidth(), 0 ); 
              				SDL_RenderPresent( gRenderer );

							//Mousemotion has been prohibitted in all the levels.
							//In case of answering the riddles only keyboard event is applicable
                    		SDL_WaitEvent(&e);
                    		if(e.type != SDL_MOUSEMOTION)
                    		{
                    			if(e.key.keysym.sym==SDLK_2)
								{
									level++;
										
									lAns = CORRECT; //lAns is the flag to store whether the answer is correct or not
								}
								else
								{
									lAns = WRONG;
									
								}
								lFlag=1;
							}
							
                    	}break;

						case 2: //Level 2
						{
                    		gR2Texture.render( 0, 0 );
                    		gR2Texture.render(gR2Texture.getWidth(), 0 ); 
                    		
                    		SDL_RenderPresent( gRenderer );
               				
                    		SDL_WaitEvent(&e);

                    		if(e.type != SDL_MOUSEMOTION)
                    		{
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
							}
							
						}break;

						case 3: // Level 3
						{
                    		gR3Texture.render( 0, 0 );
                    		gR3Texture.render(gR3Texture.getWidth(), 0 ); 
                    		
                    		SDL_RenderPresent( gRenderer );
               				
                    		SDL_WaitEvent(&e);

                    		if(e.type != SDL_MOUSEMOTION)
                    		{
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
							}
						}break;

						case 4: //Level 4
						{
                    		gR4Texture.render( 0, 0 );
                    		gR4Texture.render(gR4Texture.getWidth(), 0 ); 

                    		SDL_RenderPresent( gRenderer );
               				
                    		SDL_WaitEvent(&e);
                    		
                    		if(e.type != SDL_MOUSEMOTION)
                    		{
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
							}
						}break;

						case 5: //Level 5
						{
                   			gR5Texture.render( 0, 0 );
              				gR5Texture.render(gR5Texture.getWidth(), 0 ); 
              				SDL_RenderPresent( gRenderer );
               				
                    		SDL_WaitEvent(&e);
                    		if(e.type != SDL_MOUSEMOTION)
                    		{
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
							}
							//cout<<hello<<endl;
							//where = TH;
                    	}break;

						case 6: //Level 6
						{
                   			gR6Texture.render( 0, 0 );
              				gR6Texture.render(gR6Texture.getWidth(), 0 ); 
              				SDL_RenderPresent( gRenderer );
               				
                    		SDL_WaitEvent(&e);
                    		if(e.type != SDL_MOUSEMOTION)
                    		{
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
							}
							
                    	}break;

						case 7: //Level 7
						{
                   			gR7Texture.render( 0, 0 );
              				gR7Texture.render(gR7Texture.getWidth(), 0 ); 
              				SDL_RenderPresent( gRenderer );
               				
                    		SDL_WaitEvent(&e);
                    		if(e.type != SDL_MOUSEMOTION)
                    		{
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
							}
						
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
							lAns = WRONG;
							prevhello = hello;
							if(level>7)
							{
								gCongratsTexture.render( 0, 0 );
              				 	gCongratsTexture.render(gCongratsTexture.getWidth(), 0 ); //Shows the congratulation message of game completion if all the levels are completed
              				 	where = MENU;
                   				whereInMenu = DEFAULT;
                    			recall=1;
                    			cout << "recalled" <<endl;

							}
							else
							{
								gRightTexture.render( 0, 0 );
              					gRightTexture.render(gRightTexture.getWidth(), 0 ); //Shows user the Right answer message
              					where = LEVELS;
							}

						}
						else
						{
							hello = prevhello;
							gWrongTexture.render( 0, 0 );
              				gWrongTexture.render(gWrongTexture.getWidth(), 0 ); //Shows user the wrong answer message and asks to replay
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
						
						for(int i=0;i<10;i++)
						{
							if(barrier[i].flag_of_obstacle)
							{
								barrier[i].move(hello);

								if(checkCollision(Karim.Karim_Rect,barrier[i].Obstacle_rect) && !barrier[i].hitten)
								{
									barrier[i].hitten = 1;
									
									Karim.Life-=25;
									health_path[7]++;
									gHealthTexture.loadFromFile(health_path);
									Karim.hitten=77;
									cout << "Life :" << Karim.Life << endl;
									
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
									
									Karim.Life-=25;
									health_path[7]++;
									gHealthTexture.loadFromFile(health_path);
									Karim.hitten=77;
									cout << "Life :" << Karim.Life << endl;
									
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
										
									}
								}
							}
							plane[i].render();
						}

					
						sprintf(getScore, "Score : %04d", score);
						gScore.loadFromRenderedText( getScore, textColor );
						gScore.render( ( SCREEN_WIDTH - gScore.getWidth() - gScore.getWidth()) + 50, 35);
						
						gHealthTexture.render(50,30);
					


						//Go to next frame
						//Update screen
                		
						SDL_RenderPresent( gRenderer );
						
						if(Karim.Life<=0)
						{
							where = GAMEOVER2;
							Mix_HaltMusic();
							SDL_Delay(1000);
						}

						if(score>=350)
						{
							score = 0;
							lFlag = 0;
							Karim.Life = 100;
							health_char='1';
                            health_path="health/1h.png";
							//cout<<Karim.Life<<endl;
							
							SDL_Delay(1000);
							//PauseOn = 1;
							//where = LEVELS;

						}

						

					
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
