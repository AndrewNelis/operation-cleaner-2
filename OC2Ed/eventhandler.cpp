#include "SDL.h"   /* All SDL App's need this */
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "OC2.h"

extern short			SIZE_X,SIZE_Y,BLOCKSIZE;

int						m_button,m_button_up,m_x,m_y,m_down_x,m_down_y,m_up_x,m_up_y,m_event;
extern int				begin_x,begin_y;
int						intRunning=1;
extern unsigned int		randseed;
extern char				iDrawSmallMap;
extern bool				bInputBox;
extern unsigned char	cInputCursor,cInputLimit;
extern char				cInputText[55];
extern unsigned char	cSearchType;

extern short			iMenulevel;
extern char				GameOn;

extern char				cDraw;

extern int				mapbegin_x,mapbegin_y;
extern short			mapdestx,mapdesty;

extern bool				bAddRandseed,boolFps;

void GetKeys(SDL_KeyboardEvent *key)
{
char *keyinput;
int code;
//char msg[10];

keyinput=SDL_GetKeyName( key->keysym.sym);
code=key->keysym.unicode;

//printf("%s ",SDL_GetKeyName( key->keysym.sym));
//printf("%c (0x%04X) %d\n", key->keysym.unicode,key->keysym.unicode,key->keysym.unicode  );

if(bInputBox==true)
{
	if(code>=32 && code<255 && cInputCursor<cInputLimit-1)	// <127
	{
		cInputText[cInputCursor]=code;
		cInputCursor++;
	}
	if(code==8 && cInputCursor>0)
	{
		cInputCursor--;
		cInputText[cInputCursor]=0;
	}

	if(code==13)		// enter
	{
		FinishInput(1);
	}

}


}


int CheckEvents() 
{
	
SDL_Event event;
Uint8 *keys;
short half_x,half_y,iquit,ix,iy;

half_x=(SIZE_X-250)/BLOCKSIZE/2;
half_y=SIZE_Y/BLOCKSIZE/2;

	
	while(SDL_PollEvent(&event))
//SDL_WaitEvent(&event);

	{
		LastActionTime();		// aiding the LongRandom generator

		keys = SDL_GetKeyState(NULL);

        switch (event.type) 
		{

            case SDL_MOUSEBUTTONDOWN: 

				m_down_x=event.motion.x;
				m_down_y=event.motion.y;
				m_button=event.button.button;		// 1 LMB, 3 RMB, 2 MMB
				m_button_up=0;
				m_event=1;

				m_up_x=0;
				m_up_y=0;
				cDraw=1;

				return 0;
	            break;

			case SDL_MOUSEBUTTONUP: 

				m_up_x=event.motion.x;
				m_up_y=event.motion.y;
				m_button=0;
				m_button_up=event.button.button;
				m_event=2;
//				cDraw=1;

				return 0;
				break;

			case SDL_KEYDOWN:

				cDraw=1;
				GetKeys(&event.key);

                if ( keys[SDLK_ESCAPE] == SDL_PRESSED ) 
					{
						bAddRandseed=false;	// REMOVE FROM FINAL
						iquit=1;

						if(bInputBox==true) 
						{
							FinishInput(0);	// cancel input
							iquit=0;
						}

						if(GameOn==1) 
							{
								iMenulevel=0;
								GameOn=2;
								iquit=0;
							}

						if(GameOn==6) 
						{
							iquit=0;
							GameOn=1;
						}

						if(iquit==1)
						{
								intRunning=0;
								ShowQuitProgress();
						}

					}
				/// REMOVE FROM FINAL begins here
				if(keys[SDLK_TAB]==SDL_PRESSED)
				{
					randseed++;
					cDraw=1;
					iDrawSmallMap=1;
					GenerateMap(1);

					/*
					switch(bAddRandseed)
					{
					case true:bAddRandseed=false;break;
					case false:bAddRandseed=true;break;
					}*/
					return 0;
				}
				// REMOVE FROM FINAL ends here */
				if(keys[SDLK_f]==SDL_PRESSED)
				{
					switch(boolFps)
					{
					case true:boolFps=false;break;
					case false:boolFps=true;break;
					}
					return 0;
				}

				if(GameOn==1) 
				{

				if(keys[SDLK_UP]==SDL_PRESSED)
				{
					mapbegin_y+=half_y;
					mapdesty=mapbegin_y;
					iDrawSmallMap=1;
					MoveMap();
				}

				if(keys[SDLK_DOWN]==SDL_PRESSED)
				{
					mapbegin_y-=half_y;
					mapdesty=mapbegin_y;
					iDrawSmallMap=1;
					MoveMap();
				}

				if(keys[SDLK_LEFT]==SDL_PRESSED)
				{
					mapbegin_x-=half_x;
					mapdestx=mapbegin_x;
					iDrawSmallMap=1;
					MoveMap();
				}

				if(keys[SDLK_RIGHT]==SDL_PRESSED)
				{
					mapbegin_x+=half_x;
					mapdestx=mapbegin_x;
					iDrawSmallMap=1;
					MoveMap();
				}

				if(keys[SDLK_z]==SDL_PRESSED)	// zoom
				{
					ix=((SIZE_X/10)-(SIZE_X/20))/2;
					iy=((SIZE_Y/10)-(SIZE_Y/20))/2;

					switch(BLOCKSIZE)
					{
					case 10:BLOCKSIZE=20;mapdestx+=ix;mapdesty+=iy;break;
					case 20:BLOCKSIZE=10;mapdestx-=ix;mapdesty-=iy;break;
					default:BLOCKSIZE=20;break;
					}
					iDrawSmallMap=1;
					MoveMap();
				}
				
				}	// gameon==1

				break;


			default:
				break;
        }
    }

//printf("EVENT %d\n",m_event);

if(intRunning==0) return 1;
return 0;

}

