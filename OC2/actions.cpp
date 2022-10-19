#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "SDL/SDL.h"   /* All SDL App's need this */
#include "oc2.h"

extern int				intRunning;
extern char				GameOn;
extern short			iMenulevel;
extern bool				boolMapgenerated;

extern short			setDustClouds,setSoundEffects,setExplosions,setBits,setScroll,setDifficulty;
short					tmpScroll;

extern short			sResolutionX[100],sResolutionY[100];
extern short			sResolutionSelected;
extern short			sResolutionBegin;
short					sBitSelected;

extern int				m_button,m_button_up,m_x,m_y,m_down_x,m_down_y,m_up_x,m_up_y,m_event;
extern int				mapbegin_x,mapbegin_y;
extern char				iDrawSmallMap;
extern unsigned char	map[MAPSIZE][MAPSIZE];

bool					bInputBox;
char					cInputText[55];
char					cInputBoxHeader[50];
unsigned char			cInputCursor;
unsigned char			cInputLimit;
bool					bShowKeyboard;

bool					bOKBox;
short					sOKBoxYsize;
char					cOKBoxHeader[50];
char					cOKBoxText[10][50];

bool					bYesNoBox;
char					cYesNoCode;
char					cYesNoBoxHeader[50];
char					cYesNoBoxText[100];
char					cYesNoAnswer;

char					messagewindow[MAXMESSAGES][30];
char					messagecolor[MAXMESSAGES];
short					messagebegin;

char					helpwindow[MAXMESSAGES][30];
char					helpcolor[MAXMESSAGES];
short					helpbegin;

char					cSaveSlotTmp,cSaveLoadFile;
extern char				cSaveGameName[MAXSAVEFILES][60];

short					messagelines=0;
extern short			SIZE_X,SIZE_Y,xmiddle,ymiddle;
extern unsigned char	cSearchType;
extern short			BLOCKSIZE;
extern bool				boolExplode,boolJobDone,boolJobSuccess;
extern long				lTimer,lLastSound;
extern char				exp_selected;

extern short			ec_x[MAXCHARGES];
extern short			ec_y[MAXCHARGES];
extern unsigned char	ec_selected;
extern unsigned char	ec_type[MAXCHARGES];
extern short			ec_dir[MAXCHARGES];
extern bool				ec_primed[MAXCHARGES];
extern short			ec_amount[MAXCHARGES];
extern long				ec_timer[MAXCHARGES];

extern bool				bAddDamper;
extern short			iDampermode,iDamperbeginx,iDamperbeginy;

extern unsigned long	plr_daysinbusiness;
extern short			plr_gametype;
extern long				plr_careful,plr_currentcity,plr_reputation;
extern char				plr_name[40];
extern short			plr_crew[4];
extern char				plr_advdecisions[4];
extern char				plr_slot;
extern long				plr_money;
extern char				plr_usedmaps[MAXCAREERMAPS];			// career game maps, 1=done
extern short			plr_monthcounter,plr_difficulty;

extern short			iSTarget, iSAction;

extern unsigned long	plrs_money[MAXPLAYERS][MAXSTATS];
extern unsigned char	plrs_statshow[MAXPLAYERS];
extern unsigned char	plrs_name[MAXPLAYERS];

extern unsigned long	plr_month;

extern double			dp_money;

extern short			iStatBeginMonth,iStatScale;

extern char				city_name[MAXCITIES][20];
extern unsigned char	city_price[MAXCITIES];
extern short			sCityBegin;

extern char				cAdvFinancial;

extern char				sel_city;

extern bool				boolprev,boolnext,boolcustomercounted;

//extern unsigned long	map_nr[MAXCAREERMAPS];
extern char				map_nrc[MAXCAREERMAPS][52];
extern short			sel_customer,plr_currentcustomer;
extern char				map_city[MAXCAREERMAPS];

bool					bAddCharge=true;
bool					bSettingsChanged=false,bThemeChanged=false,bLangChanged=false,bBitsChanged=false,bScrollChanged=false;

extern char				cButtonFile[50];
extern unsigned char	cButtonFileList[100][50];
extern short			sButtonBegin,sButtonSelected;
extern unsigned char	cBaseColours[3];
extern char				iTextColor;

extern char				cLangfile[50];
extern char				cLanglist[100][50];
extern short			sLangBegin,sLangSelected;
extern short			sSaveListBegin;

extern char				gametxt[MAXGAMETXT][52];

extern unsigned int		randseed;

extern char				b_localename[BUILDINGS][52];
extern short			b_freq[BUILDINGS];		// frequency


extern char				cDraw;
extern bool				bShowHelp;
extern short			iHiscorerank;

extern bool				bTutorial;
extern char				cTutorialsdone[MAXTUTORIALS];
extern char				cTutorial;

short					iFindTargetBuilding;
extern short			iTargetbuilding;

extern short			iCreditbegin;

short					mapdestx,mapdesty;

extern short			iFirstMapInCity;

// OC2ED begins
//extern short					ed_currentcity;
extern char						ed_filename[52];
extern long						ed_mapfilenr;
//extern char						ed_customer[52];
//extern char						ed_message[10][52];

extern char						ed_building[52];
extern short					ed_buildsel;
bool							ed_bBuildingList;

extern short					ed_filebegin;
extern char						ed_mapfile[MAXMAPFILES][100];

//extern long						lTargetValue,lOtherValue;
// OC2ED ends

void CheckActions();

void MoveMap()
{
//	int ix,iy,ib;
	short max_x,max_y;
//	char msg[20];

	max_x=(SIZE_X-260)/BLOCKSIZE;
	max_y=SIZE_Y/BLOCKSIZE;

	if(m_button==3)
	if(m_down_x<=SIZE_X-260)
	{
		mapdestx=mapbegin_x+m_x/BLOCKSIZE-max_x/2;
		mapdesty=mapbegin_y+(max_y-1)-m_y/BLOCKSIZE-max_y/2;

		iDrawSmallMap=1;
		cDraw=1;

		m_button=0;
	}

	if(mapbegin_x>mapdestx)
	{
		if(mapbegin_x-mapdestx>setScroll)
			mapbegin_x-=setScroll;
		else
			mapbegin_x=mapdestx;

		iDrawSmallMap=1;
		cDraw=1;
	}

	if(mapbegin_x<mapdestx)
	{
		if(mapdestx-mapbegin_x>setScroll)
			mapbegin_x+=setScroll;
		else
			mapbegin_x=mapdestx;

		iDrawSmallMap=1;
		cDraw=1;
	}


	if(mapbegin_y>mapdesty)
	{
		if(mapbegin_y-mapdesty>setScroll)
			mapbegin_y-=setScroll;
		else
			mapbegin_y=mapdesty;

		iDrawSmallMap=1;
		cDraw=1;
	}

	if(mapbegin_y<mapdesty)
	{
		if(mapdesty-mapbegin_y>setScroll)
			mapbegin_y+=setScroll;
		else
			mapbegin_y=mapdesty;

		iDrawSmallMap=1;
		cDraw=1;
	}

	if(setScroll==0)
	{
		mapbegin_x=mapdestx;
		mapbegin_y=mapdesty;
	}

	if(m_button>0 && m_down_x>SIZE_X-255 && m_down_x<SIZE_X-255+MAPSIZE/2 && m_down_y>5 && m_down_y<5+MAPSIZE/2)
	{
		mapbegin_x=(m_down_x-(SIZE_X-255))*2-max_x/2;
		mapdestx=mapbegin_x;
		mapbegin_y=(130-m_down_y)*2-max_y/2;
		mapdesty=mapbegin_y;
		iDrawSmallMap=1;
		cDraw=1;
	}

	if(mapbegin_x<0) {mapbegin_x=0;mapdestx=0;}
	if(mapbegin_y<0) {mapbegin_y=0;mapdesty=0;}
	if(mapbegin_x>MAPSIZE-max_x) {mapbegin_x=MAPSIZE-max_x;mapdestx=mapbegin_x;}
	if(mapbegin_y>MAPSIZE-max_y-1) {mapbegin_y=MAPSIZE-max_y-1;mapdesty=mapbegin_y;}

	if(m_x>=SIZE_X-270 && m_y<140) iDrawSmallMap=1;

}

void YesNoBox(char *sHeader,char *sText,char cCode)
{
	cYesNoAnswer=0;
	strcpy(cYesNoBoxHeader,sHeader);
	strcpy(cYesNoBoxText,sText);
	cYesNoCode=cCode;

	bYesNoBox=true;
}

void ShowYesNoBox()
{
	short x=strlen(cYesNoBoxText)*4+10;
	if(x*2>SIZE_X) x=SIZE_X/2-10;
	DrawButton(SIZE_X/2-x,SIZE_Y/2-50,SIZE_X/2+x,SIZE_Y/2+50);

	DrawFillRect(SIZE_X/2-x+6,SIZE_Y/2-45,SIZE_X/2+x-4,SIZE_Y/2-25,cBaseColours[0]-20,cBaseColours[1]-20,cBaseColours[2]-20);
	DrawNormalText(cYesNoBoxHeader,SIZE_X/2-x+10,SIZE_Y/2-45,iTextColor);

	DrawNormalText(cYesNoBoxText,SIZE_X/2-x+10,SIZE_Y/2-13,iTextColor);

	DrawBMPButton(gametxt[1],SIZE_X/2-100,SIZE_Y/2+10,2);	// yes
	DrawBMPButton(gametxt[2],SIZE_X/2+5,SIZE_Y/2+10,2);		// no
}

void CheckYesNoBoxActions()
{
	if(m_button==1 && m_down_y>SIZE_Y/2+10 && m_down_y<SIZE_Y/2+40)
	{
		if(m_down_x>SIZE_X/2-100 && m_down_x<SIZE_X/2-5)	// yes
		{
			cYesNoAnswer=1;
			if(cYesNoCode==1)		// cancel job
			{
			CountExpenses(1);
			boolJobSuccess=false;
			boolJobDone=true;

				boolMapgenerated=false;

				if(plr_gametype==0)	// career
				{
//					GameOn=5;	 (?)
				}
			}

			if(cYesNoCode==2)
			{				// detonate explosives
				m_button=1;
				m_x=SIZE_X-200;
				m_y=450;
				CheckActions();
			}

			bYesNoBox=false;
		}
		if(m_down_x>SIZE_X/2+5 && m_down_x<SIZE_X/2+100)	// no
		{
			cYesNoAnswer=2;
			bYesNoBox=false;
		}
	}

	if(cYesNoAnswer>0) cDraw=1;
	m_button_up=0;
	m_button=0;
	m_down_x=0;
	m_down_y=0;
	m_event=0;
}

void pastetext()
{
	char * strFromClipboard;
	short i;
	char tmp[50];

	if (OpenClipboard(NULL))
	{
	HANDLE hClipboardData = GetClipboardData(CF_TEXT);
	char *pchData = (char*)GlobalLock(hClipboardData);
	strFromClipboard = pchData;
	GlobalUnlock(hClipboardData);
	CloseClipboard();
//	printf("from clipboard: %s\n",strFromClipboard);
	sprintf(tmp,"%.10s",strFromClipboard);

	if(strcmp(tmp,"(null)")!=0)
	if(strlen(strFromClipboard)>0)
	{
		if(strlen(strFromClipboard)<unsigned int (cInputLimit-1))
			strcpy(cInputText,strFromClipboard);
		else
			for(i=0;i<cInputLimit-1;i++) cInputText[i]=strFromClipboard[i];

		cInputCursor=strlen(cInputText);
	}

	}

}

void ShowInputBox()
{
	char msg[60];
	short tempsize,ix;

	tempsize=cInputLimit*4;
	if(tempsize<200) tempsize=200;
	DrawButton(SIZE_X/2-tempsize-20,SIZE_Y/2-50,SIZE_X/2+tempsize+20,SIZE_Y/2+50);

	DrawFillRect(SIZE_X/2-tempsize-4,SIZE_Y/2-16,SIZE_X/2+tempsize+4,SIZE_Y/2,cBaseColours[0]-50,cBaseColours[1]-50,cBaseColours[2]-50);

	DrawFillRect(SIZE_X/2-tempsize-14,SIZE_Y/2-45,SIZE_X/2+tempsize+16,SIZE_Y/2-25,cBaseColours[0]-20,cBaseColours[1]-20,cBaseColours[2]-20);

	DrawNormalText(cInputBoxHeader,SIZE_X/2-tempsize,SIZE_Y/2-45,iTextColor);

	sprintf(msg,"%s_ ",cInputText);
	DrawNormalText(msg,SIZE_X/2-tempsize-4,SIZE_Y/2-16,iTextColor);

	if(cSearchType==1)
	{
		DrawButton(0,ymiddle-370,260,ymiddle+370);
		DrawFillRect(10,ymiddle-360,250,ymiddle+320,cBaseColours[0]-50,cBaseColours[1]-50,cBaseColours[2]-50);

		for(ix=0;ix<BUILDINGS;ix++)
		if(b_localename[ix][0]!=0)
		{
			sprintf(msg,"%.30s",b_localename[ix]);

			if(ed_buildsel==ix)
				DrawTextWBack(msg,10,ymiddle-360+ix*15,iTextColor,cBaseColours[0]+20,cBaseColours[1]+20,cBaseColours[2]+20);
			else
				DrawNormalText(msg,10,ymiddle-360+ix*15,iTextColor);
		}

		DrawNormalText("Set building",50,ymiddle+335,iTextColor);

		if(ed_bBuildingList==true)
			DrawBMPButton("",10,ymiddle+330,6);
		else
			DrawBMPButton("",10,ymiddle+330,7);
	}

	DrawBMPButton(gametxt[3],SIZE_X/2-200,SIZE_Y/2+10,2);	// ok
	DrawBMPButton(gametxt[4],SIZE_X/2-100,SIZE_Y/2+10,2);		// cancel
	DrawBMPButton(gametxt[546],SIZE_X/2,SIZE_Y/2+10,2);	// paste

	DrawNormalText("Abc",SIZE_X/2+110,SIZE_Y/2+10,iTextColor);
	DrawBMPButton("",SIZE_X/2+90,SIZE_Y/2+10,9);		// yes, show extra characters


	if(bShowKeyboard==true)
	{
		DrawButton(SIZE_X/2-400,SIZE_Y/2+60,SIZE_X/2+400,SIZE_Y/2+140);
		ShowBMP1(0,iTextColor*48,760,48,SIZE_X/2-390,SIZE_Y/2+70);
	}
}

void InputBox(char *sHeader,unsigned char cLimit,unsigned char cType)
{
	short ix;

	for(ix=0;ix<cLimit;ix++) cInputText[ix]=0;

	if(cType==3 || cType==4)
	{
		strcpy(cInputText,plr_name); //sprintf(cInputText,"%-s\0",plr_name);
		cInputCursor=strlen(cInputText);
	}

	strcpy(cInputBoxHeader,sHeader);

	bInputBox=true;
	cInputLimit=cLimit;
	cSearchType=cType;
}

void OKBox(char *sHeader,short iYsize)
{
	strcpy(cOKBoxHeader,sHeader);
	sOKBoxYsize=iYsize;

	bOKBox=true;
}

void ShowOKBox()
{
	short ix;
	DrawButton(SIZE_X/2-220,SIZE_Y/2-sOKBoxYsize,SIZE_X/2+220,SIZE_Y/2+sOKBoxYsize);

//	DrawFillRect(SIZE_X/2-216,605-sOKBoxYsize,SIZE_X/2+214,625-sOKBoxYsize,cBaseColours[0]-20,cBaseColours[1]-20,cBaseColours[2]-20);
	DrawFillRect(SIZE_X/2-214,SIZE_Y/2-sOKBoxYsize+5,SIZE_X/2+216,SIZE_Y/2-sOKBoxYsize+25,cBaseColours[0]-20,cBaseColours[1]-20,cBaseColours[2]-20);
	DrawNormalText(cOKBoxHeader,SIZE_X/2-210,SIZE_Y/2-sOKBoxYsize+7,iTextColor);

	for(ix=0;ix<10;ix++)
	{
		DrawNormalText(cOKBoxText[ix],SIZE_X/2-210,SIZE_Y/2-sOKBoxYsize+30+ix*12,iTextColor);
	}

	DrawBMPButton(gametxt[3],SIZE_X/2-60,SIZE_Y/2+sOKBoxYsize-40,3);	// ok
}

void CheckOKBoxActions()
{
	if(m_button_up==1 && m_event==2 && m_up_x>SIZE_X/2-60 && m_up_x<SIZE_X/2+60 && m_up_y>SIZE_Y/2+sOKBoxYsize-40 && m_up_y<SIZE_Y/2+sOKBoxYsize-10)
	{
		bOKBox=false;
		cDraw=1;
		if(plr_money==0)			// end game
		{
			GameOn=0;
			iMenulevel=11;
		}
		m_button_up=0;
		m_button=0;
		m_down_x=0;
		m_down_y=0;
		m_event=0;
		m_up_x=0;
		m_up_y=0;
	}
//	if(m_button_up==1 && m_event==2 && m_up_x>xmiddle-150 && m_up_x<xmiddle+150)

}


void FindCharge()
{
	int ix,icy,max_y,icx;
	max_y=SIZE_Y/BLOCKSIZE;

	for(ix=0;ix<MAXCHARGES;ix++)
	if(ec_x[ix]>0)
	{
		icx=ec_x[ix]-mapbegin_x*20;
		icy=(mapbegin_y+max_y)*20-(ec_y[ix]);

		if(abs((m_x*20/BLOCKSIZE)-icx)<BLOCKSIZE && abs((m_y*20/BLOCKSIZE)-icy)<BLOCKSIZE)
		{
			ec_selected=ix;
			exp_selected=ec_type[ix];
			bAddCharge=true;
			PlayClick();
		}
	}

}

void PrimeCharges(int code)
{
	int ix;

	for(ix=0;ix<MAXCHARGES;ix++)
	if(ec_amount[ix]>0)
	{
		ec_primed[ix]=false;

		if(code==1000)
		{
			if(ec_x[ix]>0) ec_primed[ix]=true;
		}
		else
		{
		if(code==ix) ec_primed[ix]=true;
		}
	}
	lLastSound=0;
}

void CheckActions()
{
	signed short max_y=SIZE_Y/BLOCKSIZE,ix,iex,iy;


	if(GameOn==1 && m_event>0 && m_down_x<SIZE_X-260 && m_up_x<SIZE_X-260 && lTimer==0 && boolMapgenerated==true)
	{
//		cDraw=2;

		if(bAddDamper==false)
		{
			cDraw=1;

			if(m_event==2 && m_button_up==1)
			{
				m_event=0;

				switch(bAddCharge)
				{
				case true:
					if(CountNeeded(1)==1) AddExplosive();
					cDraw=1;
					break;
				case false:
					FindCharge();
					cDraw=1;
					break;
				}
			}
		}

		if(bAddDamper==true)
		{
			if(m_event==1 && m_button==1)
			{
				cDraw=1;
				iDampermode=1;

				iDamperbeginx=mapbegin_x+(m_down_x/BLOCKSIZE);
//				iDamperbeginy= MAPSIZE-mapbegin_y+(m_down_y/BLOCKSIZE);
				iDamperbeginy=(mapbegin_y+max_y-1)-(m_down_y/BLOCKSIZE);
//				mapy=mapbegin_y+(max_y-1)-m_y/BLOCKSIZE;

				if(CheckDampers()==true)
				{
					m_button=0;
					iDampermode=0;
					PlayClick();
					CountNeeded(0);
				}
			}
			if(m_event==2 && iDampermode==1 && m_button_up==1)
			{
				m_event=0;

				if(CountNeeded(1)==1)
				{
					AddDampers();
					CountNeeded(0);
					PlayClick();
				}

			iDampermode=0;
			cDraw=1;
			}
		}
	}

	if(m_button==1 && m_x>SIZE_X-250)
	{
	m_button=0;
	cDraw=1;

		if(lTimer==0)
		{
			if(boolMapgenerated==true)
			{

				if(m_y>140 && m_y<230)					// explosive type
				{
					exp_selected=(m_y-140)/16;
					bAddDamper=false;
					bAddCharge=true;

					if(m_x>SIZE_X-30)				// explosive help text
					{
						EmptyHelp();
						iex=exp_selected;
						if(iex<3) iex=0; else iex-=2;
						bShowHelp=true;
						iex*=20;
						iex+=210;

						for(ix=0;ix<20;ix++)
							if(iex<MAXGAMETXT) AddHelp(gametxt[iex+ix],iTextColor);
					}
				}

				if(m_y>=235 && m_y<=260)				// buttons
				{
					if(m_x>=SIZE_X-250 && m_x<SIZE_X-170)			// add/select charge
					{
						if(ec_x[ec_selected]>0)
						{
							do
							{
								ec_selected++;
							} while(ec_x[ec_selected]>0 && ec_selected<MAXCHARGES-1);
							m_x=0;
							bAddCharge=true;
							bAddDamper=false;
						}
						else
						{
						switch(bAddCharge)
						{
						case true:bAddCharge=false;break;
						case false:bAddCharge=true;break;
						}

						}

					if(bTutorial==true && cTutorial==3)
					{
						MoveToNextTutorial(3);	// for tutorial
					}

					}
					if(m_x>=SIZE_X-170 && m_x<SIZE_X-90)		// remove charge
					{
						if(ec_x[ec_selected]==0 && ec_selected>0)
						{
							do
							{
								ec_selected--;
							} while(ec_x[ec_selected]==0 && ec_selected>0);
						}

						if(ec_x[ec_selected]>0) PlayClick();

						ec_x[ec_selected]=0;
						ec_y[ec_selected]=0;
//						ec_selected--;

						if(ec_x[ec_selected]==0)
						{
							while(ec_x[ec_selected]==0 && ec_selected>0) ec_selected--;
						}

						CountNeeded(0);
					}
					if(m_x>=SIZE_X-90 && m_x<SIZE_X-10)		// detonate selected
					{
						if(ec_x[ec_selected]>0 && ec_amount[ec_selected]>0)
						{
						PrimeCharges(ec_selected);
						boolExplode=true;
						if(ec_selected>0) ec_selected--;
						}
						else
						{
							PlayWav("media\\beep.wav");
							AddMessage(gametxt[8],2);		//expl not set
						}
					}
				}

				if(m_x>SIZE_X-120 && m_x<SIZE_X && m_y>260 && m_y<275)		// charge size
				{
					ec_amount[ec_selected]=(m_x-(SIZE_X-115))/4;
					if(ec_amount[ec_selected]>25) ec_amount[ec_selected]=25;
					if(ec_amount[ec_selected]<1) ec_amount[ec_selected]=1;
					m_button=1;
				}

				if(m_x>SIZE_X-120 && m_x<SIZE_X && m_y>280 && m_y<295)		// charge timer
				{
					ec_timer[ec_selected]=(m_x-(SIZE_X-110))/2;

					if(m_x>SIZE_X-60)
					{
						ix=(m_x-(SIZE_X-60))*5;
						ec_timer[ec_selected]=25+ix; //*ix/4;
					}
//					if(m_x>=SIZE_X-30) ec_timer[ec_selected]+=(m_x-(SIZE_X-30))*2;

					if(ec_timer[ec_selected]>250) ec_timer[ec_selected]=250;
					if(ec_timer[ec_selected]<0) ec_timer[ec_selected]=0;
					m_button=1;
				}

				if(m_x>=SIZE_X-130 && m_x<=SIZE_X-80 && m_y>=300 && m_y<=350)	// direction
				{
					ec_dir[ec_selected]=CountAngle(m_x,325,SIZE_X-104,m_y);
					m_button=1;
				}

				if(ec_dir[ec_selected]>=0 && m_x>=SIZE_X-250 && m_x<=SIZE_X-170 && m_y>=320 && m_y<=346)	// reset dir
				{
					ec_dir[ec_selected]=-1;
				}


				if(m_x>SIZE_X-30 && m_y>355 && m_y<406)
				{
					EmptyHelp();
					bShowHelp=true;

					if(m_y>355 && m_y<370) {AddHelp(gametxt[177],iTextColor);AddHelp(gametxt[178],iTextColor);}	// blast caps help
					if(m_y>370 && m_y<382) {AddHelp(gametxt[179],iTextColor);AddHelp(gametxt[180],iTextColor);}	// detonator help
					if(m_y>382 && m_y<394) {AddHelp(gametxt[181],iTextColor);AddHelp(gametxt[182],iTextColor);AddHelp(gametxt[183],iTextColor);}	// fuse wire help
					if(m_y>394 && m_y<406) {AddHelp(gametxt[184],iTextColor);AddHelp(gametxt[185],iTextColor);AddHelp(gametxt[186],iTextColor);}	// damper help
				}


				if(m_y>=450 && m_y<=480)	// buttons
				{
					if(m_x>=SIZE_X-250 && m_x<SIZE_X-120)		// detonate all
					{
						if(CountExplosives()==0)
						{
								PlayWav("media\\beep.wav");
								AddMessage(gametxt[9],2);	// no charges set
						}
						else
						{
							if(SpecialOccasions()==0)
							{
								if(cYesNoAnswer==1 || CountOverload()==false)
									if(CountExplosives()>0)
									{
										if(CountNeeded(1)==1)
										{
											PrimeCharges(1000);
											CountExpenses(0);

											boolExplode=true;
											ec_selected=0;
											bAddDamper=false;
											bAddCharge=true;
											if(bTutorial==true) MoveToNextTutorial(5);
										}
										else
										{
											PlayWav("media\\beep.wav");
											AddMessage(gametxt[161],2);	//We don't have enough money
											AddMessage(gametxt[162],2);	//for this detonation plan.

										}
									}

									cYesNoAnswer=0;
							}
						}
					}

					if(m_x>=SIZE_X-120 && m_x<SIZE_X-10)		// place dampers
					{
						switch(bAddDamper)
						{
						case true:bAddDamper=false;break;
						case false:bAddDamper=true;break;
						}
					}
				}
			}	// 		boolMapgenerated

/*		if(m_x>SIZE_X-10 && m_y>540 && m_y<570)
					{
							iFindTargetBuilding=0;
							iTargetbuilding=0;
							InputBox(gametxt[30],50,2);	// enter building type
//							randseed=LongRandom();
//							GenerateMap();
							boolMapgenerated=true;
							boolJobSuccess=false;
							boolJobDone=false;
					}
*/
		if(boolJobDone==true)
		{

/*		DrawBMPButton(gametxt[82],SIZE_X-250,330,4);		// random map
		DrawBMPButton(gametxt[28],SIZE_X-250,360,4);			//Map nr/code
		DrawBMPButton("Load map file",SIZE_X-250,390,4);			//from file
*/
			if(m_x>SIZE_X-250 && m_x<SIZE_X-10 && plr_gametype==1)								// version 1.1
			{	// only in random game
				if(m_y>330 && m_y<360)
				{
							randseed=LongRandom();		// random map
							GenerateMap(1);

							boolMapgenerated=true;
							boolJobDone=false;
							m_x=0;
							m_y=0;
				}
				if(m_y>360 && m_y<390)//Map nr/code
				{
						InputBox(gametxt[12],50,1);		// map nr/text
				}
				if(m_y>390 && m_y<420)	//load map file
				{
					LoadMaplist(1);
					iMenulevel=9;
					GameOn=2;
//					AddMessage("Load map file",1);
				}
			}								// version 1.1 ends
			// customer & change city
			if(m_y>540 && m_y<570)
				{
					if(m_x>SIZE_X-250 && m_x<SIZE_X-120)
					{
						if(plr_gametype==0)			// customers
						{
//							ix=sel_customer;
							ix=1;
							if(plr_usedmaps[sel_customer]==1) ix=FindFirstCustomer();

							if(ix>=0)
							{

								boolJobDone=true;
								boolJobSuccess=false;
								boolMapgenerated=false;
								GameOn=5;
								cDraw=2;
								if(boolcustomercounted==false || sel_customer==0) GenerateCustomerData(1);
 							}
							else	// hint to the player that all maps in current city are done
							{
								sel_city=(char) plr_currentcity;
								GameOn=4;
							}
						}
/*	version 1.1					else
						{
							randseed=LongRandom();		// random map
							GenerateMap();
							MapNrMessage();

							boolMapgenerated=true;
							boolJobDone=false;
							m_x=0;
							m_y=0;
						}*/
					}

					if(m_x>SIZE_X-120 && m_x<SIZE_X-10)
					{
						if(plr_gametype==0)
						{// change city
							sel_city=char(plr_currentcity);
							GameOn=4;
						}
//						else								// version 1.1
//						InputBox(gametxt[12],50,1);		// map nr/text
					}
				}

		}	// booljobdone

	if(m_x>SIZE_X-250 && m_y>540 && m_x<SIZE_X-10 && m_y<570)
	{
	if(boolJobSuccess==false && boolJobDone==false && boolMapgenerated==true) YesNoBox(gametxt[10],gametxt[11],1);	// cancel, do you really want to cancel
	if(boolJobSuccess==true)	// finish job
		{
		boolMapgenerated=false;
		CountExpenses(1);
		if(plr_money>0 && bTutorial==true) MoveToNextTutorial(6);
		}
	}


	}		// lTimer=0

	if(m_y>510 && m_y<540)
	{
		if(m_x>SIZE_X-250 && m_x<SIZE_X-120)		// zoom
		{
			ix=((SIZE_X/10)-(SIZE_X/20))/2;
			iy=((SIZE_Y/10)-(SIZE_Y/20))/2;

			switch(BLOCKSIZE)
			{
			case 10:BLOCKSIZE=20;mapdestx+=ix;mapdesty+=iy;break;
			case 20:BLOCKSIZE=10;mapdestx-=ix;mapdesty-=iy;break;
			default:BLOCKSIZE=20;break;
			}
		MoveMap();
//		cDraw=2;
		}
		if(m_x>SIZE_X-120 && m_x<SIZE_X-10)
		{
			switch(setSoundEffects)
			{
			case 1:setSoundEffects=0;break;
			case 0:setSoundEffects=1;break;
			}
			// sounds
		}
	}

	if(m_y>=575 && m_y<=595)	// show messages/help
	{
		if(m_x>=SIZE_X-250 && m_x<SIZE_X-150) bShowHelp=false;
		if(m_x>=SIZE_X-110) bShowHelp=true;

		// scroll messages
			if(m_x>SIZE_X-150 && m_x<SIZE_X-130)
				{
				if(bShowHelp==false)
				{
					if(messagebegin>0 && messagewindow[messagebegin][0]>0) {messagebegin--;SDL_Delay(10);}
				}
				else
					if(helpbegin>0 && helpwindow[helpbegin][0]>0) {helpbegin--;SDL_Delay(10);}
				m_button=1;
				}
			if(m_x>SIZE_X-130 && m_x<SIZE_X-110)
				{
				if(bShowHelp==false)
				{
				if(messagebegin<MAXMESSAGES-messagelines)
				{messagebegin++;SDL_Delay(10);}
				}
				else
					if(helpbegin<MAXMESSAGES-messagelines) {helpbegin++;SDL_Delay(10);}
				m_button=1;
				}

	}


	if(m_x>=SIZE_X-250 && m_x<=SIZE_X-10)
	{

		if(m_y>480 && m_y<510 && lTimer==0)		// financial decisions
		{
			GameOn=3;
		}

		if(m_y>=SIZE_Y-70 && m_y<=SIZE_Y-40 && lTimer==0)	// save & load
		{
			GameOn=2;

			LoadSaveGameHeaders();
			cSaveSlotTmp=plr_slot;
			iMenulevel=4;
			m_y=0;
			m_x=0;
			m_up_y=0;
			m_up_x=0;
		}
		if(m_y>SIZE_Y-40 && m_y<=SIZE_Y-10)	// main menu
		{
			GameOn=2;
			iMenulevel=0;
		}
	}

	}	// m_button=1

	if(m_button==3)		// clear
	if(m_y>=575 && m_y<=595)	// messages/help
	{
		if(m_x>=SIZE_X-250 && m_x<SIZE_X-130)
			{
			if(messagewindow[MAXMESSAGES-1][0]>0) PlayClick();
			bShowHelp=false;
			EmptyMessages();
			}
		if(m_x>=SIZE_X-130)
			{
			if(helpwindow[MAXMESSAGES-1][0]>0) PlayClick();
			bShowHelp=true;
			EmptyHelp();
			}
	}
}

void StatisticsBegin()
{
	iStatScale=12;
	if(iStatBeginMonth==0 && plr_month>11)
		iStatBeginMonth=short (plr_monthcounter-11);
}

void LoadMapMessage(char * fn)
{
	char *msg="";
	AddMessage("=========================",0);
	AddMessage(gametxt[514],iTextColor);	//Loaded map file
	sprintf(msg,"%.29s",fn);
	AddMessage(msg,iTextColor);	//Loaded map file
	AddMessage("=========================",0);
	CountMapSum();
}


void CheckMenuActions()
{
	short selection;
	double dTemp;
	bool bOk;
	char *msg="";
	char i,j;

	if(m_button_up==1 && m_event==2 && m_up_x>xmiddle-150 && m_up_x<xmiddle+150)
	{
		m_button=0;
		cDraw=1;

		switch(iMenulevel)
		{
		case 0:
			selection=(m_up_y-(ymiddle-160))/40;

			switch(selection)
				{
				case 1:						// quick game
//					printf("Gameon %d\n",GameOn);
					if(GameOn==0)
					{
					ZeroPlayer();
					ZeroMap(plr_gametype);
					plr_gametype=1;
///					randseed=512294286; // for testing only
					randseed=LongRandom();	// THIS IS THE REAL ONE!!!!

					GenerateMap(1);
//					MapNrMessage();

					CountNeeded(0);
//					GenerateGround();
//					GenerateTrees();

					boolMapgenerated=true;
					boolcustomercounted=true;
					boolJobDone=false;
					}

					GameOn=1;
					cDraw=1;
					iDrawSmallMap=1;
					break;
				case 2:						// seed
					if(GameOn==2)	// cancel game
					{
						GameOn=0;

						if(plr_daysinbusiness>0) // && plr_reputation>0)
							iMenulevel=13;	// career ends
						else	// if no jobs done, just quit
						{
							ZeroPlayer();
							ZeroMap(plr_gametype);
							iMenulevel=0;
						}
					}
					else
					{
					boolcustomercounted=false;
					ZeroPlayer();
					ZeroMap(plr_gametype);
					plr_currentcity=rand()%14;
					plr_gametype=1;
					InputBox(gametxt[12],50,1);	// map nr
					boolMapgenerated=true;
					boolJobDone=false;
					cDraw=1;
					}
					break;
				case 3:		// load map file
					m_button_up=0;
					m_event=0;
					LoadMaplist(0);
					iMenulevel=9;
					break;
				case 4:						// career
					bOk=false;
					if(GameOn==0) bOk=true;
//					if(GameOn==2 && plr_gametype==1) bOk=true;

					if(bOk==true)
					{
					boolcustomercounted=false;
					ZeroPlayer();
					ZeroMap(plr_gametype);
					plr_gametype=0;
					iMenulevel=3;

					m_button_up=0;
					m_up_x=0;
					m_up_y=0;
					}
					break;
				case 5:	// load & save
					m_button_up=0;
					m_event=0;
					LoadSaveGameHeaders();
					iMenulevel=4;
					break;
				case 6:	// high scores
					iMenulevel=8;
					break;
				case 7:	// settings
					LoadLanguages();
					SetDifficultyVar(setDifficulty);
					bLangChanged=false;
					bSettingsChanged=false;
					bBitsChanged=false;
					bScrollChanged=false;
					sBitSelected=setBits;
					iMenulevel=5;
					m_button_up=0;
					m_up_y=0;
					m_up_x=0;
				break;

				case 8:
					iCreditbegin=0;
					iMenulevel=6;
					break;	// about
				case 9:						// quit
					intRunning=0;
					ShowQuitProgress();
				break;
				default:break;
				}

			break;
		case 3:	// career
			if(m_up_y>ymiddle-80 && m_up_y<ymiddle-40)		// change name
			{
				InputBox(gametxt[13],30,3);		// insert name
			}

			if(m_up_y>ymiddle-15 && m_up_y<ymiddle+10)
			{
				plr_difficulty=(m_up_x-(xmiddle-172))/31;
				if(plr_difficulty<1) plr_difficulty=1;
				if(plr_difficulty>10) plr_difficulty=10;
				SetDifficultyVar(plr_difficulty);
			}

			if(m_up_y>ymiddle+35 && m_up_y<ymiddle+60)
			{
				plr_careful=m_up_x-xmiddle;
			}

			if(m_up_y>ymiddle+80 && m_up_y<ymiddle+110)	// switch tutorial on/off
			{
				switch(bTutorial)
				{
				case true:bTutorial=false;break;
				case false:bTutorial=true;break;
				}
			}

			if(m_up_y>ymiddle+120 && m_up_y<ymiddle+160)
			{
/*				if(strcmp(plr_name,gametxt[146])==0)
					InputBox(gametxt[13],30,3);	// player, insert name
				else*/
					SetDifficultyVar(plr_difficulty);
					plr_money=(long) dp_money;
					for(j=0;j<MAXPLAYERS;j++) plrs_money[j][0]=(long) dp_money;	// all "players" begin at same amount
					plr_currentcity=11;		// pine bluff
					MoveToNextTutorial(-1);
					iMenulevel=10;
			}
			if(m_up_y>ymiddle+160 && m_up_y<ymiddle+200)
			{
				iMenulevel=0;
			}

			break;
		case 5:	// settings

			if(m_up_y>ymiddle+120 && m_up_y<ymiddle+160)
			{
				bThemeChanged=false;
				iMenulevel=7;
			}

			if(m_up_y>ymiddle+160 && m_up_y<ymiddle+200)
			{
				if(bSettingsChanged==true)	// don't save unless changes are made
				{
					if(SIZE_X!=sResolutionX[sResolutionSelected] || SIZE_Y!=sResolutionY[sResolutionSelected] || setBits!=sBitSelected)
					{
					SIZE_X=sResolutionX[sResolutionSelected];
					SIZE_Y=sResolutionY[sResolutionSelected];
					setBits=sBitSelected;
					QuitSDL();
					InitSDL();
					SetAudioFormat();
					SDL_SetEventFilter(FilterEvents);

					messagelines=0;		// empty & redefine message lines because height might change
					EmptyMessages();
					}
					if(bThemeChanged==true) sprintf(cButtonFile,"%s",cButtonFileList[sButtonSelected]);
					if(bLangChanged==true)
					{
						sprintf(cLangfile,"%s",cLanglist[sLangSelected]);
						LoadGameTexts();
//						GetBuildings();
						GetBlockData(1);
						GetCityData();
						GetBuildings(1);
					}

					SaveSettings();
					bSettingsChanged=false;
					bThemeChanged=false;
					bLangChanged=false;
					bBitsChanged=false;
					bScrollChanged=false;
					iMenulevel=0;
					SetDifficultyVar(plr_difficulty);
			}
			}

			if(m_up_y>ymiddle+200 && m_up_y<ymiddle+240)
			{
				if(bThemeChanged==true)
				{
				LoadTheme(cButtonFile);
				sprintf(msg,"themes\\%s.bmp",cButtonFile);
				LoadBMP(msg,3);
				}

				m_button_up=0;
				m_up_y=0;
				m_up_x=0;
				iMenulevel=0;
			}


			break;

		case 6: // about
			if(m_up_y>ymiddle+160 && m_up_y<ymiddle+200)
			{
				iMenulevel=0;
			}
			break;

		case 7: // theme selector
			if(m_up_x>xmiddle+130 && m_up_x<xmiddle+150)
			{
				if(m_up_y>ymiddle-100 && m_up_y<ymiddle-80 && sButtonBegin>0) sButtonBegin--;

				if(m_up_y>ymiddle-20 && m_up_y<ymiddle)
				if(sButtonBegin<94)
					if(cButtonFileList[sButtonBegin+5][0]>0) sButtonBegin++;

			}
			if(m_up_x<xmiddle+130 && m_up_y>ymiddle-100 && m_up_y<ymiddle)
			{
				sButtonSelected=sButtonBegin+(m_up_y-(ymiddle-100))/20;
				sprintf(msg,"%s",cButtonFileList[sButtonSelected]);
				if(strlen(msg)>0)
					{
					LoadTheme(msg);
					sprintf(msg,"themes\\%s.bmp",cButtonFileList[sButtonSelected]);
					LoadBMP(msg,3);
					bThemeChanged=true;
					}
			}

			if(m_up_y>ymiddle+160 && m_up_y<ymiddle+200)
			{
				if(bThemeChanged==true)
				{
//				sprintf(msg,"%s",cButtonFile);
				LoadTheme(cButtonFile);
				sprintf(msg,"themes\\%s.bmp",cButtonFile);
				LoadBMP(msg,3);
				}

				iMenulevel=5;
			}
			if(m_up_y>ymiddle+200 && m_up_y<ymiddle+240)
			{
				sprintf(msg,"%s",cButtonFileList[sButtonSelected]);
				if(strlen(msg)>0)
				{
				bSettingsChanged=true;
				iMenulevel=5;
				}
			}
			break;

		case 8:		// high scores
			if(m_up_y>ymiddle+200 && m_up_y<ymiddle+240)
				iMenulevel=0;
			break;

		case 10:		// newspaper, career begins
			if(m_up_y>ymiddle+260 && m_up_y<ymiddle+300)
			{
//				randseed=FindCityMap(char(plr_currentcity));
//				GenerateMap();
				boolJobDone=true;

				j=0;
				for(i=1;i<MAXPLAYERS;i++)	// in case of same name for player and competitor
				{
//					sprintf(msg,"%u %u ",i,strcmp(plr_name,gametxt[189+i]));
//					AddMessage(msg,1);
					if(strcmp(plr_name,gametxt[189+i])==0)
					{
						j++;
					}
					plrs_name[i]=189+i+j;
				}
				FindFirstCustomer();

				iMenulevel=0;
				GameOn=1;
				iDrawSmallMap=1;
				cDraw=1;
			}

			break;
		case 11:		// game over, money ends
			if(m_up_y>ymiddle+260 && m_up_y<ymiddle+300)
			{
				if(iHiscorerank<100)
				{
					DeleteCurrentSaveFile();
					iMenulevel=8;
				}
				else
					iMenulevel=0;
				ZeroPlayer();
			}
			break;

		case 12:		// victorious game over
			if(m_up_y>ymiddle+260 && m_up_y<ymiddle+300)
			{
				if(iHiscorerank<100)
				{
					iMenulevel=8;
					DeleteCurrentSaveFile();
				}
				else
					iMenulevel=0;
				ZeroPlayer();
			}
			break;

		case 13:		// retirement
			if(m_up_y>ymiddle+260 && m_up_y<ymiddle+300)
			{
				if(iHiscorerank<100)
				{
					DeleteCurrentSaveFile();
					iMenulevel=8;
				}
				else
					iMenulevel=0;
				ZeroPlayer();
			}
			break;

		default:break;
		}

	}

	if(iMenulevel>=11 && iMenulevel<=13)
	{
		if(m_up_y>ymiddle+220 && m_up_y<ymiddle+260)
		{
		StatisticsBegin();
		GameOn=6;		// statistics
		}
	}

	if(iMenulevel==4 && m_button_up==1 && m_event==2)		// save & load
	{
		cDraw=1;

		if(m_up_x>xmiddle-200 && m_up_x<xmiddle+200)
		{

			if(m_up_y>ymiddle-110 && m_up_y<ymiddle+114)
			{
				cSaveSlotTmp=(m_up_y-(ymiddle-110))/16+sSaveListBegin;

				if(cSaveSlotTmp<0) cSaveSlotTmp=0;
				if(cSaveSlotTmp>MAXSAVEFILES-1) cSaveSlotTmp=MAXSAVEFILES-1;
			}

			if(m_up_y>ymiddle+120 && m_up_y<ymiddle+160)	// load
			{
				if(cSaveSlotTmp>=0 && cSaveGameName[cSaveSlotTmp][36]!=0)// in 1.0.2
//				if(cSaveSlotTmp>=0)	// prev.
				{
					i=cSaveSlotTmp;
					ZeroPlayer();
					plr_slot=i;
					cSaveLoadFile=plr_slot;
					if(LoadGame()==true)
					{
						SetDifficultyVar(plr_difficulty);
						boolcustomercounted=false;
						ZeroMap(plr_gametype);
						EmptyMessages();
						EmptyHelp();
						boolJobDone=true;
						boolJobSuccess=false;
						boolMapgenerated=false;

						if(plr_gametype==0)
							FindFirstCustomer();
						else
						{
							randseed=LongRandom();
							GenerateMap(1);
//							MapNrMessage();
							boolMapgenerated=true;
							boolJobSuccess=false;
							boolJobDone=false;
						}

						GameOn=2;
						m_up_y=ymiddle+210;
					}
					else
					{
						plr_gametype=1;
						ZeroPlayer();
						iMenulevel=0;
						GameOn=0;
					}
				}

			}

			if(m_up_y>ymiddle+160 && m_up_y<ymiddle+200)	// save
			{
				if(cSaveSlotTmp>=0)
				{
				plr_slot=cSaveSlotTmp;
				cSaveLoadFile=plr_slot;
				SaveGame();
				m_up_y=ymiddle+210;
				}
			}

			if(m_up_y>ymiddle+200 && m_up_y<ymiddle+240)
			{
				if(GameOn==2)
				{
					GameOn=1;
					cDraw=1;
					iDrawSmallMap=1;
				}
				cSaveSlotTmp=plr_slot;
				cSaveLoadFile=plr_slot;
				iMenulevel=0;

			}
		}

		if(m_up_x>xmiddle+210 && m_up_x<xmiddle+230)
		{
		if(m_up_y>ymiddle-110 && m_up_y<ymiddle-90) sSaveListBegin--;
		if(m_up_y>ymiddle-80 && m_up_y<ymiddle-40) sSaveListBegin-=14;
		if(m_up_y>ymiddle+90 && m_up_y<ymiddle+110) sSaveListBegin++;
		if(m_up_y>ymiddle+40 && m_up_y<ymiddle+80) sSaveListBegin+=14;

		if(sSaveListBegin<0) sSaveListBegin=0;
		if(sSaveListBegin>MAXSAVEFILES-14) sSaveListBegin=MAXSAVEFILES-14;
		}
	}

	if(iMenulevel==5 && m_button_up==1 && m_event==2)		// settings
	{

		if(m_up_x>xmiddle-240 && m_up_x<xmiddle)
		{
			if(m_up_y>ymiddle-120 && m_up_y<ymiddle-90)
			{
				bSettingsChanged=true;
				switch(setSoundEffects)
				{
				case 0:setSoundEffects=1;break;
				case 1:setSoundEffects=0;break;
				default:break;
				}
			}

			if(m_up_y>ymiddle-90 && m_up_y<ymiddle-70)
			{
				bSettingsChanged=true;
				switch(setDustClouds)
				{
				case 0:setDustClouds=1;break;
				case 1:setDustClouds=0;break;
				default:break;
				}
			}
		}

		if(m_up_x>xmiddle && m_up_x<xmiddle+240)
		{

			if(m_up_y>ymiddle-120 && m_up_y<ymiddle-90)
			{
				bSettingsChanged=true;
				switch(setExplosions)
				{
				case 0:setExplosions=1;break;
				case 1:setExplosions=0;break;
				default:break;
				}
			}

/*			if(m_up_y>ymiddle-90 && m_up_y<ymiddle-70)	// not in use
			{
				bSettingsChanged=true;
//				switch(setSaveAllDoneMaps)
				{
//				case 0:setSaveAllDoneMaps=1;break;
//				case 1:setSaveAllDoneMaps=0;break;
				default:break;
				}
			}*/
		}

		if(m_up_y>ymiddle-80 && m_up_y<ymiddle-60)	// difficulty level
		{
			if(m_up_x>xmiddle && m_up_x<xmiddle+20 && setDifficulty>1)
			{
				setDifficulty--;
				SetDifficultyVar(setDifficulty);
				bSettingsChanged=true;
			}
			if(m_up_x>xmiddle+40 && m_up_x<xmiddle+60 && setDifficulty<10)
			{
				setDifficulty++;
				SetDifficultyVar(setDifficulty);
				bSettingsChanged=true;
			}
		}


/*		if(m_up_y>ymiddle+20 && m_up_y<ymiddle+50)	// acceleration
		{
			if(m_up_x>xmiddle-240 && m_up_x<xmiddle)
			{
				iAccelerationTemp=1;
				bSettingsChanged=true;
			}
			if(m_up_x>xmiddle && m_up_x<xmiddle+240)
			{
				iAccelerationTemp=2;
				bSettingsChanged=true;
			}
		}
*/
		if(m_up_y>ymiddle-50 && m_up_y<ymiddle+30 && m_up_x>xmiddle-100 && m_up_x<xmiddle)	// resolution
		{
			if(sResolutionX[sResolutionBegin+(m_up_y-(ymiddle-50))/16]>0)
			{
			sResolutionSelected=sResolutionBegin+(m_up_y-(ymiddle-50))/16;
			bSettingsChanged=true;
			}
		}

		if(m_up_x>xmiddle+10 && m_up_x<xmiddle+30)
		{
			if(m_up_y>ymiddle-50 && m_up_y<ymiddle-30 && sResolutionBegin>0) sResolutionBegin--;
			if(m_up_y>ymiddle+10 && m_up_y<ymiddle+30)
				if(sResolutionBegin<94)
					if(sResolutionX[sResolutionBegin+5]>0) sResolutionBegin++;
		}

		if(m_up_x>xmiddle+70 && m_up_x<xmiddle+140)
		{	// change color depth
			if(m_up_y>ymiddle-30 && m_up_y<ymiddle-10) {sBitSelected=16;bSettingsChanged=true;bBitsChanged=true;}
			if(m_up_y>ymiddle-10 && m_up_y<ymiddle+10) {sBitSelected=32;bSettingsChanged=true;bBitsChanged=true;}
		}


		if(m_up_x>xmiddle+60 && m_up_x<xmiddle+200 && m_up_y>ymiddle+55 && m_up_y<ymiddle+80)	// scroll setting
		{
			dTemp=m_up_x-(xmiddle+70);
			dTemp/=10;
//			printf("Changing scroll: %.2f\n",dTemp);
			setScroll=short(dTemp);
			if(setScroll<0) setScroll=0;
			if(setScroll>10) setScroll=10;
			tmpScroll=setScroll;
			bSettingsChanged=true;
			bScrollChanged=true;
		}


		if(m_up_y>ymiddle+40 && m_up_y<ymiddle+110 && m_up_x>xmiddle-100 && m_up_x<xmiddle)	// languages
		{
			if(cLanglist[sLangBegin+(m_up_y-(ymiddle+40))/16][0]!=0)
			{
				sLangSelected=sLangBegin+(m_up_y-(ymiddle+40))/16;
				bSettingsChanged=true;
				bLangChanged=true;
			}
		}

		if(m_up_x>xmiddle+10 && m_up_x<xmiddle+30)
		{
			if(m_up_y>ymiddle+40 && m_up_y<ymiddle+60 && sLangBegin>0) sLangBegin--;
			if(m_up_y>ymiddle+90 && m_up_y<ymiddle+110)
				if(sLangBegin<95)
					if(cLanglist[sLangBegin+4][0]>0) sLangBegin++;
		}

	}

	if(iMenulevel==6 && m_button_up==1 && m_event==2)		// credit list scroll
			if(m_up_x>xmiddle+220 && m_up_x<xmiddle+240)
			{
				cDraw=1;
				if(m_up_y>ymiddle-75 && m_up_y<ymiddle-55 && iCreditbegin>0) iCreditbegin--;
				if(m_up_y>ymiddle+140 && m_up_y<ymiddle+160 && (iCreditbegin+386)<400 && strlen(gametxt[386+iCreditbegin])>0)
					iCreditbegin++;	// down
			}

	if(iMenulevel==9 && m_button_up==1 && m_event==2)		// // load map file, version 1.1
	{
/*		DrawFillRect(xmiddle-200,ymiddle-110,xmiddle+200,ymiddle+114,100,100,100);
		if(ed_filebegin>=0 && ed_filename[ed_mapfilenr][0]!=0) DrawBMPButton(gametxt[93],xmiddle-150,ymiddle+160,5);	// load
		DrawBMPButton(gametxt[7],xmiddle-150,ymiddle+200,5);		// back
*/
			if(m_up_x>xmiddle-200 && m_up_x<xmiddle+200 && m_up_y>ymiddle-110 && m_up_y<ymiddle+114)
			{
				i=ed_filebegin+(m_up_y-(ymiddle-110))/16;
				if(ed_mapfile[i][0]!=0)
				{
					ed_mapfilenr=i;
					if(LoadMapTemp(ed_mapfile[ed_mapfilenr])!=0)
						OKBox(gametxt[202],40);	// error loading file
				}
				cDraw=1;
			}

			if(m_up_x>xmiddle+210 && m_up_x<xmiddle+230)
			{
			cDraw=1;
			if(ed_filebegin>0)
			{
				if(m_up_y>ymiddle-110 && m_up_y<ymiddle-90) ed_filebegin--;
				if(m_up_y>ymiddle-80 && m_up_y<ymiddle-40) ed_filebegin-=14;
			}
			if(ed_mapfile[ed_filebegin+14][0]!=0)
			{
				if(m_up_y>ymiddle+90 && m_up_y<ymiddle+110) ed_filebegin++;
				if(m_up_y>ymiddle+40 && m_up_y<ymiddle+80) ed_filebegin+=14;
			}

			if(ed_filebegin<0) ed_filebegin=0;
			if(ed_filebegin>MAXMAPFILES-14) ed_filebegin=MAXMAPFILES-14;
			}

			if(m_up_x>xmiddle-150 && m_up_x<xmiddle+150)
			{
				if(m_up_y>ymiddle+120 && m_up_y<ymiddle+160)
				{
					ZeroMap(1);	// for random game only
					if(LoadMap(ed_mapfile[ed_mapfilenr],1)==0)
					{
						if(GameOn==0)
						{
							ZeroPlayer();
							plr_gametype=1;
						}
						GameOn=1;
						iDrawSmallMap=1;
						cDraw=1;

						LoadMapMessage(ed_mapfile[ed_mapfilenr]);

						boolcustomercounted=true;

						boolJobDone=false;
						boolJobSuccess=false;
						boolMapgenerated=true;

//						lTargetValue=TargetValue();
//						lOtherValue=OtherValue();
					}
				}

				if(m_up_y>ymiddle+160 && m_up_y<ymiddle+200)
				{
					if(GameOn==2)
					{
						GameOn=1;
						iDrawSmallMap=1;
						cDraw=1;
					}
					else
						iMenulevel=0;
				}
			}

	}

	m_button_up=0;
	m_button=0;
	m_up_x=0;
	m_up_y=0;
	m_event=0;
	if(bSettingsChanged==true || bLangChanged==true) cDraw=1;

}

void EmptyMessages()
{
	char ix,iy;

	if(messagelines==0) messagelines=(SIZE_Y-665)/16;
	if(messagelines>=100) messagelines=100;

	for(iy=0;iy<MAXMESSAGES;iy++)
	{
	for(ix=0;ix<30;ix++)
	{
		messagewindow[iy][ix]=0;
		helpwindow[iy][ix]=0;
	}
	messagecolor[iy]=0;
	helpcolor[iy]=0;
	}

	cDraw=1;
}

void EmptyHelp()
{
	char ix,iy;

	for(iy=0;iy<messagelines;iy++)
	{
	for(ix=0;ix<30;ix++)
	{
		helpwindow[iy][ix]=0;
	}
	helpcolor[iy]=0;
	}
	cDraw=1;
}

// 0 white, 1 black, 2 red, 3 green
void AddMessage(char addtxt[30],char msgcolor)
{
	char iy,templen;

	templen=strlen(addtxt);

	for(iy=0;iy<MAXMESSAGES-1;iy++)
	{
		strcpy(messagewindow[iy],messagewindow[iy+1]);
		messagecolor[iy]=messagecolor[iy+1];
	}

	strcpy(messagewindow[MAXMESSAGES-1],addtxt);
	messagecolor[MAXMESSAGES-1]=msgcolor;

	messagebegin=MAXMESSAGES-messagelines;

	if(bShowHelp==true && bTutorial==false) bShowHelp=false;
}

void AddHelp(char addtxt[30],char hlpcolor)	// 0 white, 1 black, 2 red, 3 green
{
	char iy,templen;

	templen=strlen(addtxt);

	for(iy=0;iy<MAXMESSAGES-1;iy++)
	{
		strcpy(helpwindow[iy],helpwindow[iy+1]);
		helpcolor[iy]=helpcolor[iy+1];
	}

	strcpy(helpwindow[MAXMESSAGES-1],addtxt);
	helpcolor[MAXMESSAGES-1]=hlpcolor;

	helpbegin=MAXMESSAGES-messagelines;
}

void FinishInput(char finishtype)
{
//	char msg[100];
	unsigned int ltmp,lb,lb2,lbfr;
	short i;

	if(finishtype==1)
	{
	GameOn=1;
	cDraw=1;
	if(cSearchType==1)
	{
		SeedGenerator(cInputText);
		if(ed_bBuildingList==true)
		{
//			sprintf(msg,"randseed %u", randseed);
//			AddMessage(msg,iTextColor);
			lb=0;
			lb2=0;

			for(i=0;i<BUILDINGS;i++)
			{
				lb+=b_freq[i];
		 		if(i<ed_buildsel) lb2+=b_freq[i];
			}

			ltmp=randseed;
			while(ltmp%lb!=0)
			{
				ltmp++;
				if(ltmp==4294967295) ltmp=0;
			}
//			sprintf(msg,"ltmp %u %u",ltmp, ltmp%lb);
//			AddMessage(msg,iTextColor);
			lbfr=rand()%b_freq[ed_buildsel];
			randseed=ltmp+lb2+lbfr;
/*			sprintf(msg,"ltmp %u", ltmp);
			AddMessage(msg,iTextColor);

			sprintf(msg,"rnds %u", randseed);
			AddMessage(msg,iTextColor);

			sprintf(msg,"l+bf %u", ltmp+b_freq[ed_buildsel+1]);
			AddMessage(msg,iTextColor);

/*			sprintf(msg,"ltmp %u ",ltmp);
			AddMessage(msg,iTextColor);

			ltmp*=lb;
			ltmp+=lb2;

			ltmp+=rand()%b_freq[ed_buildsel];


			randseed=ltmp+lb2;*/

//			sprintf(msg,"bld %s",b_localename[ed_buildsel]);
//			AddMessage(msg,iTextColor);
		}

		GenerateMap(1);
//		sprintf(msg,"%s %u",gametxt[28],randseed);
//		AddMessage(msg,iTextColor);

		iDrawSmallMap=1;

		boolJobDone=false;
		boolMapgenerated=true;
		boolJobSuccess=false;
	}
	if(cSearchType==2)
	{
//		SearchBuilding(cInputText);

		iFindTargetBuilding=0;
		if(cInputText[1]>=48 && cInputText[1]<=57)
		{
			iFindTargetBuilding=(cInputText[0]-48)*10+(cInputText[1]-48);
		}
		else
			iFindTargetBuilding=(cInputText[0]-48);

//		printf("TRY TO FIND %d\n",iFindTargetBuilding);
		GameOn=6;
		boolJobDone=false;
		boolMapgenerated=true;
		boolJobSuccess=false;
	}

	if(cSearchType==3)	// change name in career beginning
	{
		GameOn=0;
		sprintf(plr_name,"%-s\0",cInputText);
	}

	if(cSearchType==4)	// change name during game
	{
		sprintf(plr_name,"%-s\0",cInputText);
		GameOn=3;
	}

	}
	bInputBox=false;
	cInputCursor=0;
	cInputText[0]='\0';
}

void CheckInputBoxActions()
{
	short sc,y1,x1;

	if(m_button_up==1 && m_event==2 && m_up_y>SIZE_Y/2+10 && m_up_y<SIZE_Y/2+36)
	{
		if(m_up_x>SIZE_X/2-200 && m_up_x<SIZE_X/2-120) FinishInput(1);
		if(m_up_x>SIZE_X/2-100 && m_up_x<SIZE_X/2-20) FinishInput(0);
		if(m_up_x>SIZE_X/2 && m_up_x<SIZE_X/2+80) pastetext();

		if(m_up_x>SIZE_X/2+90 && m_up_x<SIZE_X/2+120)
		{
			switch(bShowKeyboard)
			{
			case false:bShowKeyboard=true;break;
			case true:bShowKeyboard=false;break;
			}
		}

		cDraw=1;
	}

	if(bShowKeyboard==true)
	{
		if(m_button_up==1 && m_event==2)
		if(m_up_x>SIZE_X/2-390 && m_up_x<SIZE_X/2+370 && m_up_y>SIZE_Y/2+70 && m_up_y<SIZE_Y/2+120)
		{
			x1=(m_up_x-(SIZE_X/2-390))/8;
			y1=(m_up_y-(SIZE_Y/2+70))/16*95;
			sc=32+x1+y1;
//			printf("SC: %d x%d y%d\n",sc,x1,y1);
			if(sc>=32 && sc<=255 && cInputCursor<cInputLimit-1)
			{
				cInputText[cInputCursor]=unsigned char(sc);
				cInputCursor++;
			}
		}

	}


	if(cSearchType==1 && m_button_up==1 && m_event==2)	// building list
	{
		if(m_up_x>10 && m_up_x<250 && m_up_y>ymiddle-360 && m_up_y<ymiddle+360)
		{
			y1=(m_up_y-(ymiddle-360))/15;
			if(strlen(b_localename[y1])>0)
			{
				ed_buildsel=y1;
				if(ed_bBuildingList==false) ed_bBuildingList=true;
			}
		}

		if(m_up_x>10 && m_up_x<40 && m_up_y>ymiddle+330 && m_up_y<ymiddle+360)
			switch(ed_bBuildingList)
			{
			case true:ed_bBuildingList=false;break;
			case false:ed_bBuildingList=true;break;
			}
	}

	m_button_up=0;
	m_button=0;
	m_down_x=0;
	m_down_y=0;
	m_event=0;
	m_up_x=0;
	m_up_y=0;
	m_event=0;
}

void CheckFinancialActions()
{
//	char msg[30];
	short crwtemp,acttemp,advvalue,advtemp,hlptemp,mincrew;

	if(m_button>0)
	{
	m_button=0;
	cDraw=1;

		switch(cAdvFinancial)
		{
		case 0:

			if(m_down_y>ymiddle-140 && m_down_y<ymiddle-120 && m_down_x>xmiddle-20 && m_down_x<xmiddle)	// change name
			{
				InputBox(gametxt[13],30,4);		// change name
			}

			if(m_down_y>ymiddle-80 && m_down_y<ymiddle-5 && m_down_x>xmiddle+60 && m_down_x<xmiddle+220)
			{
				crwtemp=(m_down_y-(ymiddle-80))/20;
				acttemp=(m_down_x-(xmiddle+60))/40;

				switch(acttemp)
				{
				case 0:plr_crew[crwtemp]-=10;break;
				case 1:plr_crew[crwtemp]--;break;
				case 2:plr_crew[crwtemp]++;break;
				case 3:plr_crew[crwtemp]+=10;break;
				}

				if(plr_crew[crwtemp]<0) plr_crew[crwtemp]=0;
				if(plr_crew[crwtemp]>100) plr_crew[crwtemp]=100;
				mincrew=plr_crew[1];
				if(mincrew<plr_crew[2]) mincrew=plr_crew[2];
				if(mincrew<1) mincrew=1;
				if(plr_crew[0]<mincrew) plr_crew[0]=mincrew;
//				m_button=1;
				CountNeeded(0);
			}

			if(m_down_x>xmiddle+220 && m_down_x<xmiddle+240)
			{
				hlptemp=0;

				if(m_down_y>ymiddle-80 && m_down_y<ymiddle-5) hlptemp=(m_down_y-(ymiddle-100))/20;

				if(hlptemp>0)
				{
				bShowHelp=true;
				EmptyHelp();
				if(hlptemp==1) {AddHelp(gametxt[170],iTextColor);AddHelp(gametxt[171],iTextColor);}	// workers help
				if(hlptemp==2 || hlptemp==3) {AddHelp(gametxt[172],iTextColor);AddHelp(gametxt[173],iTextColor);}	// bulldozers+lorries help
				if(hlptemp==4) {AddHelp(gametxt[174],iTextColor);AddHelp(gametxt[175],iTextColor);;AddHelp(gametxt[176],iTextColor);}	// guard help
				}
			}
			//		DrawTextRect("?",xc+162,yc-5,xc+182,yc+14);	// question mark next to item

			if(m_down_x>xmiddle-150 && m_down_x<xmiddle+150)
			{
				if(m_down_y>ymiddle+100 && m_down_y<ymiddle+140)	// statistics
				{
					StatisticsBegin();
					GameOn=6;
				}

				if(m_down_y>ymiddle+140 && m_down_y<ymiddle+180)	// adv. management
				{
					m_x=0;
					m_y=0;
					m_down_x=0;
					m_down_y=0;
					m_event=0;
					cAdvFinancial=1;
				}
			}

			if(m_down_x>xmiddle-150 && m_down_x<xmiddle+150 && m_down_y>ymiddle+180 && m_down_y<ymiddle+220)	// continue game
				{
				GameOn=1;
				m_down_x=0;
				m_down_y=0;
				m_event=0;
				if(bTutorial==true) MoveToNextTutorial(1);
				}

		break;

		case 1:

			if(m_y>ymiddle-120 && m_y<ymiddle+50 && m_x>xmiddle-80 && m_x<xmiddle+180)	// set adv. financial settings
			{
				advtemp=(m_y-(ymiddle-120))/50;
				advvalue=(m_x-(xmiddle-50))/2;
				if(advvalue>100) advvalue=100;
				if(advvalue<0) advvalue=0;

				if(advtemp>=0 && advtemp<3)
				{
					plr_advdecisions[advtemp]=(char) advvalue;
				}

				m_button=1;
			}

			if(m_y>ymiddle+70 && m_y<ymiddle+90)
			{
				if(m_x>xmiddle-50 && m_x<xmiddle-30 && iSTarget>0) iSTarget--;
				if(m_x>xmiddle-30 && m_x<xmiddle-10 && iSTarget<2) iSTarget++;
			}

			if(m_y>ymiddle+90 && m_y<ymiddle+110)
			{
				if(m_x>xmiddle-50 && m_x<xmiddle-30 && iSAction>0) iSAction--;
				if(m_x>xmiddle-30 && m_x<xmiddle-10 && iSAction<3) iSAction++;
			}

			if(m_y>ymiddle+110 && m_y<ymiddle+140 && m_x>xmiddle+110 && m_x<xmiddle+230)
			{
				// proceed!
				if(plr_advdecisions[3]==0 && plrs_money[iSTarget+1][plr_monthcounter]>0)
				{
//					AddMessage("Sabotaged!",1);
					Sabotage(iSTarget+1,iSAction,SabotageCost());
				}
			}


			/*
	DrawBMPButton("",xmiddle-50,ymiddle+70,10);
	DrawBMPButton("",xmiddle-30,ymiddle+70,11);
	sprintf(msg,"%s",gametxt[plrs_name[iSTarget+1]]);	// competitor firm's name
	DrawNormalText(msg,xmiddle,ymiddle+70,iTextColor);

	DrawNormalText(gametxt[167],xmiddle-230,ymiddle+90,iTextColor);	//	action
	DrawBMPButton("",xmiddle-50,ymiddle+90,10);
	DrawBMPButton("",xmiddle-30,ymiddle+90,11);

  DrawBMPButton(gametxt[168],xmiddle+110,ymiddle+110,3);	// proceed

  */
			if(m_down_x>xmiddle-150 && m_down_x<xmiddle+150 && m_down_y>ymiddle+190 && m_down_y<ymiddle+230)	// return
			{
				m_button_up=0;
				m_up_x=0;
				m_up_y=0;
//				cDraw=1;
				cAdvFinancial=0;
			}
		break;

		default:break;
		}

//	cDraw=2;


}

}

void CheckCityActions()
{

short inr=0,ix;
for(ix=0;ix<MAXCAREERMAPS;ix++) if(map_city[ix]==sel_city && plr_usedmaps[ix]==0 && map_nrc[ix][0]!=0) inr++;

	if(m_event==2)
	{
		cDraw=1;
		if(m_up_x>xmiddle-150 && m_up_x<xmiddle+150)
		{
			if(m_up_y>ymiddle-120 && m_up_y<ymiddle+50)
			{
				sel_city=sCityBegin+(m_up_y-(ymiddle-120))/16;
				if(sel_city<0) sel_city=0;
				if(sel_city>13) sel_city=13;
			}
			if(m_up_y>ymiddle+160 && m_up_y<ymiddle+200)	// change city
			{

				if(plr_currentcity!=sel_city && plr_money>MovingCosts(false) && inr>0)
				{
					MovingCosts(true);
//					GenerateCustomerData(1);
					m_up_y=ymiddle+210;	// close window
				}
			}
			if(m_up_y>ymiddle+200 && m_up_y<ymiddle+240)	// continue game
			{
			GameOn=1;
			cDraw=1;
			}
		}

		if(m_up_x>xmiddle+160 && m_up_x<xmiddle+180)
		{
			if(m_up_y>ymiddle-120 && m_up_y<ymiddle-100 && sCityBegin>0) sCityBegin--;					// up
			if(m_up_y>ymiddle+30 && m_up_y<ymiddle+50 && city_price[sCityBegin+10]>0 && sCityBegin+10<MAXCITIES) sCityBegin++;	// down
		}

	m_button_up=0;
	m_up_x=0;
	m_up_y=0;
	m_event=0;
	}

}

void CheckCustomerActions()
{
	short ix;
	char * fn="";
	if(m_event==2)
	{
//		cDraw=2;
/*		if(m_up_y>ymiddle+80 && m_up_y<ymiddle+120)
		{
		if(m_up_x>xmiddle-150 && m_up_x<xmiddle)	// prev
			{
			FindPrevCustomer();
			}
		if(m_up_x>xmiddle && m_up_x<xmiddle+150)	// next
			{
			FindNextCustomer();
			}
		}
*/
		if(m_up_x>xmiddle-200 && m_up_x<xmiddle+200)
		{
			if(m_up_y>ymiddle+40 && m_up_y<ymiddle+115)
			{
				ix=(m_up_y-(ymiddle+40))/25*5+(m_up_x-(xmiddle-200))/80;
				if(plr_usedmaps[ix+iFirstMapInCity]==0 && sel_customer!=iFirstMapInCity+ix && plr_currentcity==map_city[iFirstMapInCity+ix])
				{
					sel_customer=iFirstMapInCity+ix;
					GenerateCustomerData(0);
					cDraw=1;
					m_up_x=0;
					m_event=0;
				}
			}
		}

		if(m_up_x>xmiddle-150 && m_up_x<xmiddle+150)
		{

/*			if(m_up_y>ymiddle && m_up_y<ymiddle+40 && boolnext==true)	// next
				{
				FindNextCustomer(); //==false) GameOn=1;
				cDraw=1;
				}

			if(m_up_y>ymiddle+40 && m_up_y<ymiddle+80 && boolprev==true)	// prev
				{
				FindPrevCustomer(); //==false) GameOn=1;
				cDraw=1;
				}
*/
			if(m_up_y>ymiddle+160 && m_up_y<ymiddle+200 && plr_usedmaps[sel_customer]==0) //sel_customer!=plr_currentcustomer)	// preview
			{
				boolJobDone=true;
				GameOn=1;
				cDraw=1;
				AddDays(1);
			}
			if(m_up_y>ymiddle+200 && m_up_y<ymiddle+240 && plr_usedmaps[sel_customer]==0 && plr_currentcity==map_city[sel_customer]) //sel_customer!=plr_currentcustomer)	// accept
			{
				boolMapgenerated=true;
				boolJobDone=false;
				plr_currentcustomer=sel_customer;
				GameOn=1;
				cDraw=1;
//				MapNrMessage();

//				if(ed_building[0]!=0)

//					MapNrMessage();
//				else
				{
					for(ix=0;ix<(short) strlen(map_nrc[sel_customer]);ix++) fn[ix]=map_nrc[sel_customer][ix];
					LoadMapMessage(fn);
//					MapNrMessage();
				}

				if(bTutorial==true) MoveToNextTutorial(0);
			}

			if(m_up_y>ymiddle+160 && m_up_y<ymiddle+200)	// cancel
			{
				GameOn=1;
				cDraw=1;
			}
		}
	m_button=0;
	m_event=0;
	}

}

void CheckStatisticsActions()
{
	short i;
//	char msg[30];

	if(m_event==2)
	{

		//		DrawRect(100,SIZE_Y-80+j*20,600,SIZE_Y-61+j*20,cBaseColours[0]+i,cBaseColours[1]+i,cBaseColours[2]+i);

		if(m_up_x>100 && m_up_x<600 && m_up_y>SIZE_Y-80 && m_up_y<SIZE_Y)
		{
			i=(m_up_y-(SIZE_Y-80))/20;
			plrs_statshow[i]++;
			if(plrs_statshow[i]>2) plrs_statshow[i]=0;
//			sprintf(msg,"stat %u=%u",i,plrs_statshow[i]);
//			AddMessage(msg,1);
		}

		if(m_up_y>SIZE_Y-60 && m_up_y<SIZE_Y-30)
		{
		if(m_up_x>SIZE_X-240 && m_up_x<SIZE_X-120 && iStatScale<60)
		{
			if(m_button_up==1 && iStatBeginMonth>0) iStatBeginMonth--;
			if(m_button_up==3)
			{
				if(iStatBeginMonth>iStatScale)
					iStatBeginMonth-=iStatScale;
				else
					iStatBeginMonth=0;
			}

		}
		if(m_up_x>SIZE_X-120 && m_up_x<SIZE_X && iStatScale<60)
		{
			if(m_button_up==1 && iStatBeginMonth<(short) MAXSTATS-iStatScale) iStatBeginMonth++;
			if(m_button_up==3)
			{
				if(iStatBeginMonth<MAXSTATS-12)
					iStatBeginMonth+=iStatScale;
				else
					iStatBeginMonth=MAXSTATS-iStatScale;
			}

			if(iStatBeginMonth>MAXSTATS-iStatScale) iStatBeginMonth=MAXSTATS-iStatScale;
		}
		}


		if(m_up_y>SIZE_Y-30 && m_up_y<SIZE_Y)
		{
			if(m_up_x>SIZE_X-240 && m_up_x<SIZE_X-120)
			{
				switch(iStatScale)
				{
				case 12:iStatScale=60;iStatBeginMonth=0;break;
				case 60:iStatScale=12;StatisticsBegin();break;
				default:iStatScale=12;break;
				}
			}

			if(m_up_x>SIZE_X-120 && m_up_x<SIZE_X)
			{
				iDrawSmallMap=2;
				GameOn=3;

				if(iMenulevel>=11 && iMenulevel<=13) GameOn=0;	// return to game
			}
		}

		cDraw=2;
		m_event=0;
	}
}

void Actions()
{
	bool bBox;

	bBox=false;

	if(bInputBox==true)
	{
		ShowInputBox();
		CheckInputBoxActions();
		bBox=true;
	}

	if(bYesNoBox==true)
	{
		ShowYesNoBox();
		CheckYesNoBoxActions();
		bBox=true;
	}

	if(bOKBox==true)
	{
		ShowOKBox();
		CheckOKBoxActions();
		bBox=true;
	}


	if(bBox==false)
	{
		switch(GameOn)
		{
		case 0:
			CheckMenuActions();
			break;
		case 1:
			MoveMap();
			CheckActions();
			break;
		case 2:
			CheckMenuActions();
			break;
		case 3:
			CheckFinancialActions();
			break;
		case 4:
			CheckCityActions();
			break;
		case 5:
			CheckCustomerActions();
			break;

		case 6:
			CheckStatisticsActions();
			break;

			/*		case 6:
			if(iFindTargetBuilding==iTargetbuilding)
			{
				cDraw=1;
				GameOn=1;
			}
			else
			{
				randseed++;
//				GenerateMap();
			}
			break;*/
		default:break;
		}

	}
}
