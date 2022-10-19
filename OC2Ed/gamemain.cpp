#include "SDL.h"   /* All SDL App's need this */
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include "OC2.h"

extern int				m_x,m_y;
extern char				iDrawSmallMap;
extern unsigned int		randseed;
extern short			iMenulevel;
extern char				cButtonFile[50];

short					setSoundEffects,setDustClouds,setExplosions,setBits,setScroll;
extern short			tmpScroll;

char					GameOn;
long					plr_money,lTotalcost,lPlacehours,plr_taxpaid,lExtraCosts;
short					plr_crew[4];
short					plr_reputation,plr_gametype,plr_currentcustomer;
long					plr_careful,plr_currentcity;
unsigned long			plr_jobsdone,plr_daysinbusiness,plr_lastspecial;
char					plr_name[40]="Player";
char					plr_usedmaps[MAXCAREERMAPS];			// career game maps
char					plr_advdecisions[4];

char					plr_slot;
extern char				cSaveLoadFile;

extern char				exp_selected;
long					lTimer;

char					cLangfile[50];
char					cLanglist[100][50];
short					sLangBegin,sLangSelected;

char 					gametxt[MAXGAMETXT][52];

bool					boolMapgenerated;
extern bool				bTutorial,boolJobDone,boolJobSuccess;

extern short			sCityBegin;

extern char				cDraw;
extern short			iProgress;

short					hs_reputation[MAXHISCORE];
char					hs_player[MAXHISCORE][30];
char					hs_date[MAXHISCORE][12];
unsigned long			hs_money[MAXHISCORE];
unsigned long			hs_daysinbusiness[MAXHISCORE];
short					hs_gametype[MAXHISCORE];
extern short			iHiscorerank;
extern char				cSaveSlotTmp;
extern short			sel_customer;

extern char				cTutorialsdone[MAXTUTORIALS];
extern char				cTutorial;

extern short			messagelines, messagebegin;
extern short			helpbegin;

short					setMaxFramerate;
double					dFps;

// OC2ED begins
short					ed_currentcity;
char					ed_filename[52];
long					ed_mapfilenr;
short					ed_filebegin;
char					ed_customer[52];
char					ed_building[52];
char					ed_message[10][52];
short					ed_beginx,ed_beginy,ed_endx,ed_endy;

bool					ed_bsingle,ed_bfill,ed_bassist,ed_bbuilding,ed_brectangle,ed_bfrectangle;
bool					ed_btarget,ed_insert,ed_bcopy,ed_bsaveok,ed_bshowdropping;

short					ed_scut;

short					ed_block;

short					ed_buildsel;

char					ed_mapfile[MAXMAPFILES][100];
// OC2ED ends

bool					bAddRandseed,boolFps;

void ZeroPlayer()
{
	short i;

	sprintf(plr_name,gametxt[146]);
	plr_money=10000;
	plr_taxpaid=0;
	plr_crew[0]=5;
	plr_crew[1]=1;
	plr_crew[2]=1;
	plr_crew[3]=0;
	plr_reputation=0;
	plr_careful=0;
	plr_jobsdone=0;
	plr_daysinbusiness=0;
	plr_lastspecial=0;
	plr_gametype=0;

	lTotalcost=0;
	lExtraCosts=0;

	plr_advdecisions[0]=85;
	plr_advdecisions[1]=90;
	plr_advdecisions[2]=60;
	plr_advdecisions[3]=0;
	exp_selected=0;

	lTimer=0;
	iHiscorerank=0;
	plr_slot=-1;
	cSaveLoadFile=-1;
	cSaveSlotTmp=0;
	bTutorial=false;
	EmptyMessages();
	EmptyHelp();

	boolJobSuccess=false;
	boolJobDone=false;
	boolMapgenerated=false;

	for(i=0;i<MAXCAREERMAPS;i++) plr_usedmaps[i]=0;
	for(i=0;i<MAXTUTORIALS;i++) cTutorialsdone[i]=1;
	cTutorial=0;

	sCityBegin=4;
	sel_customer=0;
/*	setSounds=1;
	setDust=1;
	setExplosions=1;
	setSaveAllDoneMaps=1;*/
}

int FilterEvents(const SDL_Event *event) 
{
    if ( event->type == SDL_MOUSEMOTION ) 
	{
		m_x=event->motion.x;
		m_y=event->motion.y;
		return(0);
	}

    return(1);
}

short CheckHiscores()
{
	short i,j;
	char cdate[20];
	struct tm *newtime;
	time_t aclock;

	if(hs_reputation[0]==0) GetHiscores();

	time( &aclock );                 
	newtime = localtime( &aclock );  	
	strftime(cdate,20,"%Y-%m-%d",newtime);

	for(i=0;i<MAXHISCORE;i++)
	{
		if(plr_reputation>hs_reputation[i])
		{
			for(j=MAXHISCORE-1;j>i;j--)
			{
				strcpy(hs_player[j],hs_player[j-1]);
				strcpy(hs_date[j],hs_date[j-1]);
				hs_reputation[j]=hs_reputation[j-1];
				hs_money[j]=hs_money[j-1];
				hs_daysinbusiness[j]=hs_daysinbusiness[j-1];
			}

			strcpy(hs_player[i],plr_name);

			strcpy(hs_date[i],cdate);
			hs_reputation[i]=plr_reputation;
			hs_money[i]=plr_money;
			hs_daysinbusiness[i]=plr_daysinbusiness;

			SaveHiscores();
			return i+1;
		}

	}
	return 100;
}

double CountFrameRate(double dF, DWORD t1, DWORD t2)
{
	if(t2>t1)
		return dF/((t2-t1))*1000;
	else
		return 0;
}

void Mainstructure()
{
unsigned char ccounter;
DWORD time1, time2;
double dFrames;
unsigned int iSlow;
short idone,iFast=0;
char msg[30]="";

iMenulevel=0;
iDrawSmallMap=1;
GameOn=0;
iSlow=0;

//printf("%u\n",GenerateRandSeed());
GenerateRandSeed();
randseed=0;
boolMapgenerated=false;

//GenerateMap();
ZeroPlayer();

SDL_SetEventFilter(FilterEvents);

ccounter=100;

SDL_ShowCursor(SDL_ENABLE);

time1=GetTickCount();
dFrames=0;
idone=0;
cDraw=1;			// the draw 'flag'
iProgress=0;		// show the progress meter when starting
bTutorial=false;
messagebegin=MAXMESSAGES-messagelines;

sprintf(ed_building,"%c",0);
sprintf(ed_filename,"%c",0);
ed_mapfilenr=-1;
ed_filebegin=0;
ed_bsingle=true;
ed_block=0;
ed_scut=0;
ed_bcopy=false;
ed_bassist=true;
ed_bshowdropping=true;
dFps=setMaxFramerate;

do
{

//	ccounter++;
	if(GameOn==1) dFrames++;
	RecordMouse();
/*	if(ccounter>255)
	{
		ccounter=0;
	}
*/
//	LongRandom();
	

	DrawAll();
/*
	if(boolFps==true && GameOn==1)
	{
	sprintf(msg,"%3.f",dFps);// REMOVE FROM FINAL (?), FPS
	DrawTextWBack(msg,10,30,1,255,255,255);
//	if(ccounter%10==0) 
//	SDL_UpdateRect(screen, 10,30,48,15);
//	Update();
	}
	*/
	Update();

	if(dFrames>100)
	{
		if(dFps>setMaxFramerate)	// if framerate is too high, let's slow down
		{
			if(iSlow<UINT_MAX) iSlow++;
			SDL_Delay(iSlow);
		}
		else
		{	// if framerate drops, let's get faster
			if(iSlow>0) iSlow--;
		}
	}

	dFps=CountFrameRate(dFrames,time1,GetTickCount());

	if(dFrames>15000)	// approx. in every 6-10 minutes reset timer
		{
/*		dFps=CountFrameRate(dFrames,time1,GetTickCount());
		sprintf(msg,"%.f",dFps);// REMOVE FROM FINAL, FPS
		AddMessage(msg,1);
		cDraw=1;*/
		time1=GetTickCount();
		dFrames=1;
		}


	idone=CheckEvents();
	if(setScroll!=tmpScroll) setScroll=tmpScroll;

/*	randseed++;
//	printf("RS %d\n",randseed);
	GenerateMap();*/
} while(idone==0);

	time2=GetTickCount();

	dFps=dFrames/((time2-time1))*1000;
	printf("FRAME RATE %.2f frames/sec.\n",CountFrameRate(dFrames,time1,time2));


}

int LoadNeededFiles()
{
	char cTemp[70];

	LoadBMP("media\\001.bmp",1);
//	printf("001 ok\n");
	LoadGameTexts();
//	printf("loadgametxt ok\n");

	ShowInitProgress();

	LoadBMP("media\\002.bmp",2);
//	printf("002 ok\n");
	ShowInitProgress();

	sprintf(cTemp,"themes\\%s.bmp",cButtonFile);
	LoadBMP(cTemp,3);
	ShowInitProgress();
//	printf("themes ok\n");

	LoadButtonThemes();
	ShowInitProgress();
//	printf("button ok\n");

	GetBuildings(0);
	ShowInitProgress();
//	printf("buildings ok\n");

	GetBlockData(0);
	ShowInitProgress();
//	printf("blockdata ok\n");

	GetCityData();
	ShowInitProgress();
//	printf("citydata ok\n");

	GetMapData();
	ShowInitProgress();
//	printf("mapdata ok\n");

	LoadSaveGameHeaders();
	ShowInitProgress();
//	printf("savegameheaders ok\n");

	GetCustomers();
	ShowInitProgress();
//	printf("customers ok\n");

	return 0;

}

