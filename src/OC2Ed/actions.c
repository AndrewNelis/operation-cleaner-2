#include <errno.h>
#include <process.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//#include <windows.h>
#include "SDL/SDL.h"   /* All SDL App's need this */
#include "oc2.h"

extern int				intRunning;
extern char				GameOn;
extern short			iMenulevel;
extern bool				boolMapgenerated;

extern short			setDustClouds,setSoundEffects,setExplosions,setBits,setScroll;
short					tmpScroll;

extern short			sResolutionX[100],sResolutionY[100];
extern short			sResolutionSelected;
extern short			sResolutionBegin;
short					sBitSelected;

extern int				m_button,m_button_up,m_x,m_y,m_down_x,m_down_y,m_up_x,m_up_y,m_event;
extern int				mapbegin_x,mapbegin_y;
extern char				iDrawSmallMap;
extern unsigned char	map[MAPSIZE][MAPSIZE];
extern unsigned char	mapv[MAPSIZE][MAPSIZE];

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
extern bool				boolJobDone,boolJobSuccess;
extern long				lTimer,lLastSound;
extern char				exp_selected;

extern unsigned long	plr_daysinbusiness;
extern short			plr_gametype,plr_reputation;
extern long				plr_careful,plr_currentcity;
extern char				plr_name[40];
extern short			plr_crew[4];
extern char				plr_advdecisions[4];
extern char				plr_slot;
extern long				plr_money;
extern char				plr_usedmaps[MAXCAREERMAPS];			// career game maps, 1=done

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
extern bool						ed_bsingle,ed_bfill,ed_bassist,ed_bbuilding,ed_brectangle,ed_bfrectangle,ed_btarget,ed_insert,ed_bcopy, ed_bshowdropping;
extern short					ed_block,ed_scut;

extern short					block_penres[MAXBLOCKS];


extern char						ed_filename[52];
extern long						ed_mapfilenr;
//extern char						ed_customer[52];

extern char						ed_building[52];
extern short					ed_buildsel;
extern short					ed_currentcity;
extern char						ed_message[10][52];
extern char						ed_customer[52];

extern short					ed_filebegin;
extern char						ed_mapfile[MAXMAPFILES][100];

extern short					ed_beginx,ed_beginy,ed_endx,ed_endy;


extern short					block_end[MAXBLOCKS];
extern short					mapend[MAPSIZE][MAPSIZE];

extern bool						maptarget[MAPSIZE][MAPSIZE];
unsigned char					tmpmap[MAPSIZE][MAPSIZE];

extern short					p_buildbegin, p_buildwidth;

extern char						b_name[BUILDINGS][255];
extern char						b_localename[BUILDINGS][52];
extern short					b_freq[BUILDINGS];		// frequency

bool							ed_bBuildingList;
extern bool						ed_bsaveok;
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
	char msg[50];
	short tempsize,ix;

	tempsize=cInputLimit*4;
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

	if(cType==5)	// building
	{
		strcpy(cInputText,ed_building);
		cInputCursor=strlen(cInputText);
	}

	if(cType==6)	// customer
	{
		strcpy(cInputText,ed_customer);
		cInputCursor=strlen(cInputText);
	}

	if(cType==7)	// save map as...
	{
		if(strlen(ed_filename)>0)
			strcpy(cInputText,ed_filename);
		else
			strcpy(cInputText,ed_mapfile[ed_mapfilenr]);

		cInputCursor=strlen(cInputText);
	}

	if(cType>=10 && cType<20)
	{
		strcpy(cInputText,ed_message[cType-10]);
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


void DoFill(short bx, short by, short newbnr)
{
bool bOk=true,bContinue=true;
short bnr,ix,iy; //isz=0;
//char msg[30];

bnr=MapBlock(bx,by);
PutMap(bx,by,250);

do
{
	bContinue=false;
	for(ix=1;ix<MAPSIZE-2;ix++)
	for(iy=1;iy<MAPSIZE-2;iy++)
	{
		if(map[ix][iy]==bnr)
		{
			bOk=false;
			if(map[ix+1][iy]==250) bOk=true;
			if(map[ix-1][iy]==250) bOk=true;
			if(map[ix][iy+1]==250) bOk=true;
			if(map[ix][iy-1]==250) bOk=true;

			if(bOk==true)
			{
				bContinue=true;
				map[ix][iy]=250;
			}
		}
	}
} while(bContinue==true);

for(ix=0;ix<MAPSIZE;ix++)
for(iy=0;iy<MAPSIZE;iy++)
	if(map[ix][iy]==250) PutMap(ix,iy,newbnr);
}

void DoRect(short bx,short by,short ex,short ey, short bl)
{
	short ix, iy, change;

	if(bx>ex) {change=ex;ex=bx;bx=change;}
	if(by>ey) {change=ey;ey=by;by=change;}

	for(ix=bx;ix<=ex;ix++)
	{
		PutMap(ix,by,bl);
		PutMap(ix,ey,bl);
	}

	for(iy=by;iy<=ey;iy++)
	{
		PutMap(bx,iy,bl);
		PutMap(ex,iy,bl);
	}
}

void DoFillRect(short bx,short by,short ex,short ey, short bl)
{
	short ix, iy, change;

	if(bx>ex) {change=ex;ex=bx;bx=change;}
	if(by>ey) {change=ey;ey=by;by=change;}

	for(ix=bx;ix<=ex;ix++)
	for(iy=by;iy<=ey;iy++)
	{
		PutMap(ix,iy,bl);
	}
}

void SetTargetArea(short bx,short by,short ex,short ey)
{
	short ix, iy, change;
	bool bOk;

	if(bx>ex) {change=ex;ex=bx;bx=change;}
	if(by>ey) {change=ey;ey=by;by=change;}

	for(ix=0;ix<MAPSIZE;ix++)
	for(iy=0;iy<MAPSIZE;iy++)
		maptarget[ix][iy]=false;

	p_buildbegin=MAPSIZE;
	p_buildwidth=0;
	for(ix=bx;ix<=ex;ix++)
	for(iy=by;iy<=ey;iy++)
	{
		bOk=false;

		if(map[ix][iy]>0 && map[ix][iy]<40) bOk=true;
		if(map[ix][iy]>50 && map[ix][iy]<56) bOk=true;
		if(map[ix][iy]>=133 && map[ix][iy]<=138) bOk=true;
		if(bOk==true)
		{
			if(ix<p_buildbegin) p_buildbegin=ix;
			if(ix>p_buildwidth) p_buildwidth=(ix-p_buildbegin)+1;
			maptarget[ix][iy]=true;
		}
	}
	ed_bsaveok=false;
}

void EmptyPipes()
{
	short ix,iy,ir;

	for(ix=0;ix<MAPSIZE;ix++)
	for(iy=0;iy<MAPSIZE;iy++)
	{
		ir=0;
		if(MapBlock(ix,iy)>=40 && MapBlock(ix,iy)<=50) ir=1;	// pipes away
		if(MapBlock(ix,iy)==40 && MapBlock(ix,iy+1)==5) ir=0;	// pipe connected to basement stays
		if(MapBlock(ix,iy)==40 && MapBlock(ix,iy+1)==17) ir=0;	// pipe connected to steel stays
		if(ir==1) PutMap(ix,iy,71);
	}
}

short TestMap()
{
	short ix,iy,iLoose,iret,iok,itargetok;
	char msg[30];

	AddMessage("----------------------------",1);

	iret=0;
	iLoose=0;
	itargetok=0;

	for(ix=1;ix<MAPSIZE-1;ix++)
	for(iy=1;iy<MAPSIZE-1;iy++)
		mapv[ix][iy]=0;

	for(ix=1;ix<MAPSIZE-1;ix++)
	for(iy=1;iy<MAPSIZE-1;iy++)
	{
		if(MapBlock(ix,iy)>0 && MapBlock(ix+1,iy)==0 && MapBlock(ix-1,iy)==0 && MapBlock(ix,iy+1)==0 && MapBlock(ix,iy-1)==0)
		{
			iret++;
			sprintf(msg,"-%s: %3u %3u",gametxt[502],ix,iy);	// Loose block
			AddMessage(msg,2);
			iLoose++;
		}

		if(MapBlock(ix,iy)>=40 && MapBlock(ix,iy)<=50)	// pipes
		{
			iok=0;
			if(MapBlock(ix+1,iy)>=40 && MapBlock(ix+1,iy)<=50) iok=1;
			if(MapBlock(ix-1,iy)>=40 && MapBlock(ix-1,iy)<=50) iok=1;
			if(MapBlock(ix,iy+1)>=40 && MapBlock(ix,iy+1)<=50) iok=1;
			if(MapBlock(ix,iy-1)>=40 && MapBlock(ix,iy-1)<=50) iok=1;

			if(iok==0)
			{
				iret++;
				sprintf(msg,"-%s: %3u %3u",gametxt[503],ix,iy);	// loose pipe
				AddMessage(msg,2);
			}
		}

		if(maptarget[ix][iy]==true)
		{
			iok=0;

			if(maptarget[ix+1][iy]==false)
			{
				if(map[ix+1][iy]>0 && map[ix+1][iy]<40) iok=1;	// pipes are not normally targetarea
				if(map[ix+1][iy]>50 && map[ix+1][iy]<56) iok=1;			// water post - satellite dish
				if(map[ix+1][iy]>=133 && map[ix+1][iy]<=138) iok=1;		// signs
				if(iok==1)
				{
					sprintf(msg,"-%s: %3u %3u",gametxt[525],ix+1,iy);		// target area
					iret++;
					AddMessage(msg,2);
				}
			}

			if(maptarget[ix][iy+1]==false)
			{
				if(map[ix][iy+1]>0 && map[ix][iy+1]<40) iok=1;	// pipes are not normally targetarea
				if(map[ix][iy+1]>50 && map[ix][iy+1]<56) iok=1;			// water post - satellite dish
				if(map[ix][iy+1]>=133 && map[ix][iy+1]<=138) iok=1;		// signs
				if(iok==1)
				{
					sprintf(msg,"-Targetarea:  %3u %3u",ix,iy+1);
					iret++;
					AddMessage(msg,2);
				}
			}

			if(maptarget[ix-1][iy]==false)
			{
				if(map[ix-1][iy]>0 && map[ix-1][iy]<40) iok=1;	// pipes are not normally targetarea
				if(map[ix-1][iy]>50 && map[ix-1][iy]<56) iok=1;			// water post - satellite dish
				if(map[ix-1][iy]>=133 && map[ix-1][iy]<=138) iok=1;		// signs
				if(iok==1)
				{
					sprintf(msg,"-Targetarea:  %3u %3u",ix-1,iy);
					iret++;
					AddMessage(msg,2);
				}
			}

			if(maptarget[ix][iy-1]==false)
			{
				if(map[ix][iy-1]>0 && map[ix][iy-1]<40) iok=1;	// pipes are not normally targetarea
				if(map[ix][iy-1]>50 && map[ix][iy-1]<56) iok=1;			// water post - satellite dish
				if(map[ix][iy-1]>=133 && map[ix][iy-1]<=138) iok=1;		// signs
				if(iok==1)
				{
					sprintf(msg,"-Targetarea:  %3u %3u",ix,iy-1);
					iret++;
					AddMessage(msg,2);
				}
			}

			itargetok++;

		}
	}

/*	if(iLoose>0)
	{
		sprintf(msg,"%u loose blocks",iLoose);
		AddMessage(msg,2);
	}
*/
	MapCount();	// dropping blocks

	if(strlen(ed_customer)==0)
	{
		iret++;
		AddMessage(gametxt[547],2); //Customer name missing
	}


	if(strlen(ed_building)==0)
	{
		iret++;
		AddMessage(gametxt[548],2); //Building name missing
	}

	if(itargetok==0)
	{
		iret++;
		AddMessage(gametxt[549],2); //Targetarea missing
	}

return iret;
}

void CheckActions()
{
	short max_y=SIZE_Y/BLOCKSIZE,ix,iy,ic,itx,ity,xs,ys;
	char fn[300];


	if(GameOn==1 && m_x<SIZE_X-260)
	{
		cDraw=1;
		ix=mapbegin_x+(m_x/BLOCKSIZE);
		iy=(mapbegin_y+max_y-1)-(m_y/BLOCKSIZE);

		if(m_button==2)	// version 1.2
		{
			ed_block=MapBlock(ix,iy);
		}


		if(m_button==1)
		{

			if(ed_bsingle==true)
			{
				if(m_event==1)
				{
					PutMap(ix,iy,(unsigned char) ed_block);
//					map[ix][iy]=(unsigned char) ed_block;
//					mapend[ix][iy]=block_end[ed_block];
					iDrawSmallMap=1;
				}

				if(ed_bassist==true)
				{
					switch(ed_block)
					{
					case 26:PutMap(ix+1,iy,ed_block+1);break;	// roof
					case 32:PutMap(ix-1,iy,ed_block-1);break;

					case 33:PutMap(ix+1,iy,ed_block+1);break;	// glass roof
					case 39:PutMap(ix-1,iy,ed_block-1);break;

					case 57:PutMap(ix+1,iy,ed_block+1);break;	// asphalt
					case 63:PutMap(ix-1,iy,ed_block-1);break;

					case 68:PutMap(ix+1,iy,ed_block+1);break;	// ground
					case 74:PutMap(ix-1,iy,ed_block-1);break;

					case 78:PutMap(ix+1,iy,ed_block+1);break;	// sand
					case 84:PutMap(ix-1,iy,ed_block-1);break;

					case 85:PutMap(ix+1,iy,ed_block+1);break;	// grass
					case 91:PutMap(ix-1,iy,ed_block-1);break;

					case 92:PutMap(ix,iy+1,93);break;	// roots --> trunk

					case 127:PutMap(ix+1,iy,ed_block+1);break;	// car 1
					case 128:PutMap(ix-1,iy,ed_block-1);break;

					case 129:PutMap(ix+1,iy,ed_block+1);break;	// car 2
					case 130:PutMap(ix-1,iy,ed_block-1);break;

					case 131:PutMap(ix+1,iy,ed_block+1);break;	// car 3
					case 132:PutMap(ix-1,iy,ed_block-1);break;

					case 133:PutMap(ix+1,iy,ed_block+1);break;	// hotel

					case 136:PutMap(ix+1,iy,ed_block+1);PutMap(ix+2,iy,ed_block+2);break;	// hospital
					default:break;
					}

					if(ed_block>=140 && ed_block<147)	// traffic signs + lights
					{
						if(MapBlock(ix,iy-1)==0)
						{
							ic=1;
							do
							{
								PutMap(ix,iy-ic,139);
								ic++;
							} while(MapBlock(ix,iy-ic)==0);
						}
					}
				}	// bassist

				if(m_event==2)
				{
					m_event=0;
					m_button=0;
				}
			}	// ed_bsingle

			if(ed_brectangle==true || ed_bfrectangle==true || ed_btarget==true || ed_bbuilding==true || ed_scut==1)
			{
				if(m_event==1)
				{
				ed_insert=true;
				ed_beginx=ix;
				ed_beginy=iy;
				m_event=0;
				}
			}

		}	// mbutton 1

			if(m_button_up==1 && m_event==2 && m_up_x<SIZE_X-260)
			{
				m_event=0;
				cDraw=1;
				iDrawSmallMap=1;
				ix=mapbegin_x+(m_x/BLOCKSIZE);
				iy=(mapbegin_y+max_y-1)-(m_y/BLOCKSIZE);

				m_event=0;
				m_button_up=0;

				if(ed_bfill==true)
				{
					DoFill(ix,iy,ed_block);
				}

				if(ed_brectangle==true)
				{
					DoRect(ed_beginx,ed_beginy,ix,iy,ed_block);
					ed_insert=false;
				}

				if(ed_bfrectangle==true)
				{
					DoFillRect(ed_beginx,ed_beginy,ix,iy,ed_block);
					ed_insert=false;
				}

				if(ed_bbuilding==true)
				{
					if(abs(ix-ed_beginx)>5 || abs(iy-ed_beginy)>5)

					{
					sprintf(fn,"buildings\\%s.map",b_name[ed_buildsel]);
					LoadMapCode(fn);
//	printf("%s [%d] %d\n",fn,b_freq[xc],xc);

//	yc=FindPlace(0,MAPSIZE);

					ParseMapCode(ed_beginx,abs(ix-ed_beginx)+1);
//					AddMessage("Building added",1);
					ed_insert=false;
//					ed_bbuilding=false,
//					ed_bsingle=true;
					}
					else
					{
						ed_insert=false;
					}
				}

				if(ed_btarget==true)
				{
					SetTargetArea(ed_beginx,ed_beginy,ix,iy);
					ed_insert=false;
				}

				if(ed_scut==1)
				{
					ed_endx=ix;
					ed_endy=iy;

					for(ix=0;ix<MAPSIZE;ix++)
					for(iy=0;iy<MAPSIZE;iy++)
						tmpmap[ix][iy]=0;

					if(ed_beginx>ed_endx) {ix=ed_beginx;ed_beginx=ed_endx;ed_endx=ix;}
					if(ed_beginy>ed_endy) {ix=ed_beginy;ed_beginy=ed_endy;ed_endy=ix;}

					xs=ed_endx-ed_beginx;
					ys=ed_endy-ed_beginy;

					for(ix=0;ix<=xs;ix++)	// copy to "clipboard"
					for(iy=0;iy<=ys;iy++)
					{
						tmpmap[ed_beginx+ix][ed_beginy+iy]=(unsigned char) MapBlock(ed_beginx+ix,ed_beginy+iy);
					}

					ed_scut=2;
				}

			}	// m_event2

		}	// gameon 1
//	DrawFillRect(SIZE_X-255,135,SIZE_X-5,405,cBaseColours[0]-50,cBaseColours[1]-50,cBaseColours[2]-50);

	if(ed_scut==2 && m_button==1 && m_event==1)
	{
		if(m_x<SIZE_X-260)
		{
			itx=mapbegin_x+(m_x/BLOCKSIZE);
			ity=(mapbegin_y+max_y-1)-(m_y/BLOCKSIZE);

			xs=ed_endx-ed_beginx;
			ys=ed_endy-ed_beginy;

			if(ed_bcopy==false)
			{
			for(ix=0;ix<=xs;ix++)
			for(iy=0;iy<=ys;iy++)
				PutMap(ed_beginx+ix,ed_endy-iy,0);

			ed_scut=0;
			ed_bsingle=true;
			}
			else
				ed_scut=2;

			for(ix=0;ix<=xs;ix++)
			for(iy=0;iy<=ys;iy++)
			{
				PutMap(itx+ix,ity-iy,tmpmap[ed_beginx+ix][ed_endy-iy]);
			}

			m_button=0;
			m_event=0;
			ed_insert=false;
			cDraw=1;
			iDrawSmallMap=1;
		}

	}


	// block selector
	if(m_x>SIZE_X-260 && m_x<SIZE_X-10 && m_y>140 && m_y<400 && m_button==1)
	{
		m_button=0;
		cDraw=1;

		ix=(m_y-140)/20*12+(m_x-(SIZE_X-250))/20;

		if(block_penres[ix]>0)
		{
			ed_block=ix;

			if(ed_bbuilding==true || ed_btarget==true || ed_scut>0)
			{
				m_button=1;
				m_x=SIZE_X-245;
				m_y=410;
			}
		}
	}

	if(m_button==1 && m_x>SIZE_X-250)
	{
	m_button=0;
	cDraw=1;

	if(m_x>SIZE_X-250 && m_x<SIZE_X-10)								// version 1.1
		if(m_y>SIZE_Y-70 && m_y<SIZE_Y-40)	//load map file
		{
			LoadMaplist(1);
			iMenulevel=4;
			GameOn=2;
			// TO DO!!!!
//			AddMessage("Load map file",1);
		}

	if(m_y>405 && m_y<435)
	{
		if(m_x>SIZE_X-255 && m_x<SIZE_X-45)		// select build tools
		{
			ed_bsingle=false;
			ed_bfill=false;
			ed_brectangle=false;
			ed_bfrectangle=false;
			ed_bbuilding=false;
			ed_btarget=false;
			ed_scut=0;
			ed_insert=false;

			ix=(m_x-(SIZE_X-255))/30;

			switch(ix)
			{
			case 0:ed_bsingle=true;break;
			case 1:ed_bfill=true;break;
			case 2:ed_brectangle=true;break;
			case 3:ed_bfrectangle=true;break;
			case 4:ed_bbuilding=true;break;
			case 5:ed_btarget=true;break;
			case 6:ed_scut=1;break;
			default:break;
			}


		}

		if(m_x>SIZE_X-45 && m_x<SIZE_X-15)
		{
			switch(ed_bassist)
			{
			case true:ed_bassist=false;AddMessage(gametxt[501],1);break;		// assist off
			case false:ed_bassist=true;AddMessage(gametxt[500],1);break;				// on
			}
		}
	}


	if(ed_scut>0 && m_y>460 && m_y<490)
	{
		if(m_x>SIZE_X-250 && m_x<SIZE_X-130) ed_bcopy=true;
		if(m_x>SIZE_X-130 && m_x<SIZE_X-10) ed_bcopy=false;
	}


	if(ed_bbuilding==true && m_y>460 && m_y<490)
	{
		if(m_x>SIZE_X-250 && m_x<SIZE_X-10) GameOn=4;
	}

	if(m_y>510 && m_y<540)
	{
		if(m_x>SIZE_X-250 && m_x<SIZE_X-130)		// zoom
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
		}
		if(m_x>SIZE_X-130 && m_x<SIZE_X-10)
		{
			GameOn=3;		// map settings
		}
	}

	if(m_y>460 && m_y<490)
	{
		if(m_x>SIZE_X-250 && m_x<SIZE_X-10)		// show dropping
		if(ed_scut==0 && ed_bbuilding==false)
			switch(ed_bshowdropping)
			{
			case true:ed_bshowdropping=false;break;
			case false:ed_bshowdropping=true;break;
			}
	}

	if(m_y>540 && m_y<570)
	{
		if(m_x>SIZE_X-250 && m_x<SIZE_X-130)
		{
			EmptyPipes();
			BuildPipes();
			AddMessage(gametxt[504],1);	// pipes remade
		}

		if(m_x>SIZE_X-130 && m_x<SIZE_X-10 && ed_bsaveok==false)
		{
			if(TestMap()==0)
			{
				AddMessage(gametxt[509],3);	// map test ok
				ed_bsaveok=true;
			}
			else
			{
				AddMessage(gametxt[510],2);	// map test failed
				ed_bsaveok=false;
			}
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

void LoadMapMessage(char * fn)
{
	char *msg="";
	AddMessage("=========================",0);
	AddMessage(gametxt[514],iTextColor);	//Loaded map file
	sprintf(msg,"%.29s",fn);
	AddMessage(msg,iTextColor);	//Loaded map file
	AddMessage("=========================",0);
}


void CheckMenuActions()
{
	short selection,ix,iy;
	char *msg="";
	char i;

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
				case 1:						// random map

					if(GameOn==0)
					{
					randseed=LongRandom();
					GenerateMap(1);
//					sprintf(msg,"%s %u",gametxt[28],randseed);
//					AddMessage(msg,iTextColor);
					}

					GameOn=1;
					cDraw=1;
					iDrawSmallMap=1;
					break;
				case 2:						// seed

					if(GameOn==0)
					{
						InputBox(gametxt[12],50,1);	// map nr
					}
					else
					{
						GameOn=0;
						ed_bsaveok=false;
					}

					cDraw=1;
					break;

				case 3:					// empty map

					if(GameOn==0)
					{
					ZeroMap(0);
					GenerateGround();
					GenerateTrees();
					ed_bsaveok=false;
					for(ix=0;ix<MAPSIZE;ix++)
						for(iy=0;iy<MAPSIZE;iy++) mapend[ix][iy]=block_end[map[ix][iy]];
					GameOn=1;
					cDraw=1;
					iDrawSmallMap=1;
					}

					break;
				case 4:		// load map file
					m_button_up=0;
					m_event=0;
					LoadMaplist(0);
					iMenulevel=4;
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
		case 6: // about
			if(m_up_y>ymiddle+160 && m_up_y<ymiddle+200)
			{
				iMenulevel=0;
			}
			break;

		default:break;
		}

	}


/*	if(iMenulevel==6 && m_button_up==1 && m_event==2)		// credit list scroll
			if(m_up_x>xmiddle+220 && m_up_x<xmiddle+240)
			{
				cDraw=1;
				if(m_up_y>ymiddle-75 && m_up_y<ymiddle-55 && iCreditbegin>0) iCreditbegin--;
				if(m_up_y>ymiddle+140 && m_up_y<ymiddle+160 && (iCreditbegin+386)<400 && strlen(gametxt[386+iCreditbegin])>0)
					iCreditbegin++;	// down
			}
*/
	if(iMenulevel==4 && m_button_up==1 && m_event==2)		// // load map file, version 1.2
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

			if(m_up_x>xmiddle-120 && m_up_x<xmiddle+120)
			{
				if(m_up_y>ymiddle+120 && m_up_y<ymiddle+150)	// load map
				{
					ZeroMap(0);
					if(LoadMap(ed_mapfile[ed_mapfilenr],1)==0)
					{
						if(GameOn==0)
						{
//							ZeroPlayer();
//							plr_gametype=1;
							ed_bsaveok=true;
						}
						sprintf(ed_filename,"%s",ed_mapfile[ed_mapfilenr]);
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

				if(m_up_y>ymiddle+150 && m_up_y<ymiddle+180)	// save map
				{
					if(ed_bsaveok==true) InputBox(gametxt[512],100,7);
				}

				if(m_up_y>ymiddle+180 && m_up_y<ymiddle+210)	// main menu
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
	char *f="";
	short i;
	unsigned int ltmp,lb,lb2,lbfr;
//	char msg[100];

	if(finishtype==1)
	{
	GameOn=1;
	cDraw=1;
	if(cSearchType==1)
	{
		SeedGenerator(cInputText);

		if(ed_bBuildingList==true)
		{
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
			lbfr=rand()%b_freq[ed_buildsel];
			randseed=ltmp+lb2+lbfr;
		}
		GenerateMap(1);

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

/*	if(cSearchType==3)	// change name in career beginning
	{
		GameOn=0;
		sprintf(plr_name,"%-s\0",cInputText);
	}

	if(cSearchType==4)	// change name during game
	{
		sprintf(plr_name,"%-s\0",cInputText);
		GameOn=3;
	}
*/
	if(cSearchType==5)	// building
	{
		sprintf(ed_building,"%-s\0",cInputText);
		if(strlen(ed_filename)==0) sprintf(ed_filename,"%-s\0",cInputText);
		GameOn=3;
	}

	if(cSearchType==6)	// customer
	{
		sprintf(ed_customer,"%-s\0",cInputText);
		GameOn=3;
	}

	if(cSearchType==7)	// save map
	{
		for(i=0;i<52;i++) ed_filename[i]=0;
		sprintf(ed_filename,"%-s%c",cInputText,0);
//		sprintf(ed_mapfile[ed_mapfilenr],"%-s\0",cInputText);
//		sprintf(f,"%-s\0",cInputText);
		if(SaveMap(ed_filename)==0)
		{
//		sprintf(ed_customer,"%-s\0",cInputText);
			AddMessage("Map saved",1);
			LoadMaplist(1);
		}
		else
		{
			AddMessage("Save file failed.",2);
		}
		GameOn=1;
		cDraw=1;
		iDrawSmallMap=1;
	}

	if(cSearchType>=10 && cSearchType<20)	// message
	{
		sprintf(ed_message[cSearchType-10],"%-s\0",cInputText);
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

	if(cSearchType==1 && m_button_up==1 && m_event==2)
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

void CheckMapSettingActions()
{
	short i;
	char msg[100];

	if(m_button_up==1 && m_event==2)
	{

		if(m_up_y>ymiddle-200 && m_up_y<ymiddle-180)
		{
			if(m_up_x>xmiddle-100 && m_up_x<xmiddle-80 && ed_currentcity>0) ed_currentcity--;
			if(m_up_x>xmiddle-80 && m_up_x<xmiddle-60 && strlen(city_name[ed_currentcity+1])>1 && ed_currentcity<MAXCITIES-1) ed_currentcity++;
			cDraw=1;
		}

		if(m_up_x>xmiddle-100 && m_up_x<xmiddle+240)
		{
			if(m_up_y>ymiddle-240 && m_up_y<ymiddle-220) InputBox(gametxt[29],50,5);	// building name
			if(m_up_y>ymiddle-220 && m_up_y<ymiddle-200) InputBox(gametxt[27],50,6);	// customer name
		}

		if(m_up_x>xmiddle-200 && m_up_x<xmiddle+200 && m_up_y>ymiddle-160 && m_up_y<ymiddle-10)
		{
			i=(m_up_y-(ymiddle-160))/15;
			sprintf(msg,"%s (%u/10)",gametxt[200],i+1);
			InputBox(msg,50,10+i);	// message line
		}
		/*
	DrawNormalText(gametxt[29],xmiddle-240,ymiddle-240,iTextColor);	// building
	DrawNormalText(ed_building,xmiddle-100,ymiddle-240,iTextColor);


	DrawNormalText(gametxt[27],xmiddle-240,ymiddle-220,iTextColor);	// customer
	DrawNormalText(ed_customer,xmiddle-100,ymiddle-220,iTextColor);

	DrawNormalText(gametxt[200],xmiddle-240,ymiddle-180,iTextColor);	// messages

	DrawFillRect(xmiddle-205,ymiddle-163,xmiddle+205,ymiddle-10,cBaseColours[0]-50,cBaseColours[1]-50,cBaseColours[2]-50);

*/
		if(m_up_x>xmiddle-150 && m_up_x<xmiddle+150)
		{

			if(m_up_y>ymiddle+100 && m_up_y<ymiddle+180)
			{
				for(i=0;i<10;i++) sprintf(ed_message[i],"%c",0);

				if(m_up_y>ymiddle+140 && m_up_y<ymiddle+180)
				{
					sprintf(ed_building,"%c",0);
					sprintf(ed_customer,"%c",0);
					ed_currentcity=0;
				}
			}


			if(m_up_y>ymiddle+180 && m_up_y<ymiddle+220)
				GameOn=1;
			}

		cDraw=1;
		m_button=0;
		m_event=0;

//	DrawBMPButton(gametxt[55],xmiddle-150,ymiddle+180,5);	// continue
	}
}

void CheckBuildSelActions()
{
//		DrawBMPButton(gametxt[3],xmiddle-150,ymiddle+320,5);	// ok
	short ix;

	if(m_button_up==1 && m_event==2)
	{
		if(m_up_y>ymiddle+330 && m_up_y<ymiddle+360 && m_up_x>xmiddle-120 && m_up_x<xmiddle+120) GameOn=1;

/*
	DrawFillRect(xmiddle-240,ymiddle-360,xmiddle+240,ymiddle+320,cBaseColours[0]-50,cBaseColours[1]-50,cBaseColours[2]-50);

	for(ix=0;ix<BUILDINGS;ix++)
	if(b_localename[ix][0]!=0)
	{
		if(ed_buildsel==ix)
			DrawTextWBack(b_localename[ix],xmiddle-230,ymiddle-360+ix*15,iTextColor,cBaseColours[0]+20,cBaseColours[1]+20,cBaseColours[2]+20);

  */

		if(m_up_y>ymiddle-360 && m_up_y<ymiddle+320 && m_up_x>xmiddle-230 && m_up_x<xmiddle+230)
		{
			ix=(m_up_y-(ymiddle-360))/15;
			if(strlen(b_localename[ix])>0) ed_buildsel=ix;
		}
		cDraw=1;
		m_button=0;
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
			CheckMapSettingActions();
			break;
		case 4:
			CheckBuildSelActions();
			break;
		default:break;
		}

	}
}
