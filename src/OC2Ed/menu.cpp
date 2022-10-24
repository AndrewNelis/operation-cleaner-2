#include <stdlib.h>
#include <string.h>
#include "SDL/SDL.h"   /* All SDL App's need this */
#include "oc2.h"

extern short			SIZE_X,SIZE_Y,xmiddle,ymiddle;
extern char				plr_name[40];
extern long				plr_careful;

extern unsigned int		randseed;
extern char				plr_slot;

extern short			plr_gametype;
extern char				GameOn;
extern short			setSoundEffects,setDustClouds,setExplosions,setBits,setScroll,sBitSelected;

short					iMenulevel;

extern char				cSaveGameName[MAXSAVEFILES][60];
extern unsigned char	cBaseColours[3];
extern char				iTextColor;

extern char				cButtonFile[50];
extern unsigned char	cButtonFileList[100][50];
short					sButtonBegin,sButtonSelected;
extern char				cButtonText[5][52];
extern char				cButtonAuthor[52];

extern char				gametxt[MAXGAMETXT][52];

extern short			sResolutionX[100],sResolutionY[100];
extern short			sResolutionSelected;
short					sResolutionBegin;
short					sSaveListBegin;

extern char				cLangfile[50];
extern char				cLanglist[100][50];
extern short			sLangBegin,sLangSelected;

extern bool				bSettingsChanged;
extern bool				bTutorial;

extern long				lDamageInflicted;

extern unsigned long	plr_daysinbusiness;
extern long				plr_money;
extern short			plr_reputation;
extern unsigned long	plr_jobsdone,plr_daysinbusiness;

extern short			hs_reputation[MAXHISCORE];
extern char				hs_player[MAXHISCORE][30];
extern char				hs_date[MAXHISCORE][12];
extern unsigned long	hs_money[MAXHISCORE];
extern unsigned long	hs_daysinbusiness[MAXHISCORE];
extern short			hs_gametype[MAXHISCORE];

extern char				cSaveSlotTmp;

short					iHiscorerank;

short					iCreditbegin=0;
//unsigned char			ccounter=0;

// OC2ED begins
extern short					ed_currentcity;
extern char						ed_filename[52];
extern long						ed_mapfilenr;
extern char						ed_customer[52];
extern char						ed_building[52];
extern char						ed_message[10][52];

extern short					ed_buildsel;

extern short					ed_filebegin;
extern char						ed_mapfile[MAXMAPFILES][100];

char							temp_ed_message[10][52];
char							temp_ed_building[52];
extern bool						ed_bsaveok;
// OC2ED ends

void About()
{
	short i=0;
	DrawNormalText("v 1.2",xmiddle-220,ymiddle-120,iTextColor);
	DrawNormalText("1.2.2007 Jan Nyman",xmiddle+60,ymiddle-120,iTextColor);

	DrawTextRect(gametxt[370],xmiddle-220,ymiddle-100,xmiddle+220,ymiddle-80);	// credits
//	while(strlen(gametxt[371+i+iCreditbegin])>0 && (371+i+iCreditbegin)<MAXGAMETXT && i<15)
	for(i=0;i<14;i++)
	{
		DrawNormalText(gametxt[i+371],xmiddle-220,ymiddle-75+i*15,iTextColor);
//		i++;
	}

//	if(iCreditbegin>0) DrawBMPButton("",xmiddle+220,ymiddle-75,10);		// up
//	if(strlen(gametxt[386+iCreditbegin])>0) DrawBMPButton("",xmiddle+220,ymiddle+140,11);		// down

	DrawBMPButton(gametxt[3],xmiddle-150,ymiddle+160,5);	// ok

}


void DrawMenus()
{
short ix;
//char msg[110];

//	if(iAccelerationTemp==0) iAccelerationTemp=setAccelerationMode;

	DrawFillRect(0,0,SIZE_X,SIZE_Y,0,0,0);

/*	DrawFillRect(10,10,20,20,ccounter,ccounter,ccounter);
	ccounter+=5;
	if(ccounter>240) ccounter=40;
*/
	DrawFillRect(xmiddle-250,ymiddle-250,xmiddle+250,ymiddle+250,cBaseColours[0],cBaseColours[1],cBaseColours[2]);

	ShowBMP1(450,450,300,100,xmiddle-150,ymiddle-240);	// logo
	ShowBMP1(450,550,62,18,xmiddle-31,ymiddle-145);		// editor text

	switch(iMenulevel)
	{
	case 0:

		if(GameOn==0)
		{
		DrawBMPButton(gametxt[82],xmiddle-150,ymiddle-120,5);		//Random map
		DrawBMPButton(gametxt[12],xmiddle-150,ymiddle-80,5);		// map nr/text
		DrawBMPButton(gametxt[532],xmiddle-150,ymiddle-40,5);		// Empty map
		}
		else
		{
		DrawBMPButton(gametxt[531],xmiddle-150,ymiddle-120,5);		//Return to editor
		DrawBMPButton(gametxt[534],xmiddle-150,ymiddle-80,5);		// cancel edit
		}

		DrawBMPButton(gametxt[533],xmiddle-150,ymiddle,5);		// Load map file
		DrawBMPButton(gametxt[96],xmiddle-150,ymiddle+160,5);		// about
		DrawBMPButton(gametxt[97],xmiddle-150,ymiddle+200,5);		// quit
		break;
	case 3:	// career
		break;
/*case 4:	// load & save
		DrawFillRect(xmiddle-200,ymiddle-110,xmiddle+200,ymiddle+114,cBaseColours[0]-50,cBaseColours[1]-50,cBaseColours[2]-50);
		for(ix=0;ix<14;ix++)
		{
			if(ix+sSaveListBegin==cSaveSlotTmp)
				DrawTextWBack(cSaveGameName[ix+sSaveListBegin],xmiddle-200,ymiddle-110+ix*16,1,cBaseColours[0]+50,cBaseColours[1]+50,cBaseColours[2]+50);
			else
				DrawNormalText(cSaveGameName[ix+sSaveListBegin],xmiddle-200,ymiddle-110+ix*16,0);
		}

		if(sSaveListBegin>0)
		{
			DrawBMPButton("",xmiddle+210,ymiddle-110,10);					// up

			DrawBMPButton("",xmiddle+210,ymiddle-80,10);
			DrawBMPButton("",xmiddle+210,ymiddle-60,10);

		}
		if(sSaveListBegin<MAXSAVEFILES-15)
		{
			DrawBMPButton("",xmiddle+210,ymiddle+90,11);		// down

			DrawBMPButton("",xmiddle+210,ymiddle+40,11);
			DrawBMPButton("",xmiddle+210,ymiddle+60,11);
		}

		if(cSaveSlotTmp>=0 && cSaveGameName[cSaveSlotTmp][36]!=0) DrawBMPButton(gametxt[93],xmiddle-150,ymiddle+120,5);	// load	// in 1.0.2
		if(cSaveSlotTmp>=0) DrawBMPButton(gametxt[94],xmiddle-150,ymiddle+160,5);	// save
		DrawBMPButton(gametxt[7],xmiddle-150,ymiddle+200,5);		// back
//		sprintf(msg,"tmp %d  plr %d ",cSaveSlotTmp,plr_slot);
//		DrawNormalText(msg,10,10,0);
		break;
		*/
	case 6:	// about
		About();
		break;

	case 4:	//	load map file, version 1.2
		DrawFillRect(xmiddle-200,ymiddle-110,xmiddle+200,ymiddle+114,cBaseColours[0]-50,cBaseColours[1]-50,cBaseColours[2]-50);
		for(ix=0;ix<14;ix++)
		{
			if(ix+ed_filebegin==ed_mapfilenr)
				DrawTextWBack(ed_mapfile[ix+ed_filebegin],xmiddle-200,ymiddle-110+ix*16,1,cBaseColours[0]+50,cBaseColours[1]+50,cBaseColours[2]+50);
			else
				DrawNormalText(ed_mapfile[ix+ed_filebegin],xmiddle-200,ymiddle-110+ix*16,0);
		}

		DrawFillRect(xmiddle-250,ymiddle+250,xmiddle+250,ymiddle+390,cBaseColours[0],cBaseColours[1],cBaseColours[2]);	// down part


		DrawNormalText(gametxt[29],xmiddle-240,ymiddle+225,iTextColor); // Building name

		DrawFillRect(xmiddle-55,ymiddle+223,xmiddle+245,ymiddle+240,cBaseColours[0]-50,cBaseColours[1]-50,cBaseColours[2]-50);
		DrawNormalText(temp_ed_building,xmiddle-50,ymiddle+225,iTextColor);


/*		DrawNormalText(gametxt[27],xmiddle-240,ymiddle+205,iTextColor); // customer name

		DrawFillRect(xmiddle-55,ymiddle+203,xmiddle+245,ymiddle+220,cBaseColours[0]-50,cBaseColours[1]-50,cBaseColours[2]-50);
		DrawNormalText(ed_customer,xmiddle-50,ymiddle+205,iTextColor);
*/
		// messages

		DrawNormalText(gametxt[200],xmiddle-240,ymiddle+240,iTextColor); // messages
//		DrawTextWBack("Messages",xmiddle-240,ymiddle+226,iTextColor,cBaseColours[0]-50,cBaseColours[1]-50,cBaseColours[2]-50);

		DrawFillRect(xmiddle-245,ymiddle+255,xmiddle+245,ymiddle+380,cBaseColours[0]-50,cBaseColours[1]-50,cBaseColours[2]-50);
		for(ix=0;ix<10;ix++)
		{
			DrawNormalText(temp_ed_message[ix],xmiddle-240,ymiddle+257+ix*12,iTextColor);
		}

		if(ed_filebegin>0)
		{
			DrawBMPButton("",xmiddle+210,ymiddle-110,10);						// up

			DrawBMPButton("",xmiddle+210,ymiddle-80,10);
			DrawBMPButton("",xmiddle+210,ymiddle-60,10);
		}
		if(ed_filebegin<MAXMAPFILES-15 && ed_mapfile[ed_filebegin+14][0]!=0)
		{
			DrawBMPButton("",xmiddle+210,ymiddle+90,11);		// down

			DrawBMPButton("",xmiddle+210,ymiddle+40,11);
			DrawBMPButton("",xmiddle+210,ymiddle+60,11);
		}


		if(ed_filebegin>=0 && ed_mapfile[ed_mapfilenr][0]!=0) DrawBMPButton(gametxt[533],xmiddle-120,ymiddle+120,4);	// load

		if(GameOn==0)
			DrawBMPButton(gametxt[85],xmiddle-120,ymiddle+180,4);		// main menu
		else
		{
			if(ed_bsaveok==true)
				DrawBMPButton(gametxt[535],xmiddle-120,ymiddle+150,4);	// save as...
			else
				DrawNormalText(gametxt[540],xmiddle-120,ymiddle+155,iTextColor);	//Map test must be succesfull before saving
			DrawBMPButton(gametxt[541],xmiddle-120,ymiddle+180,4);		// Back to editor
		}

		break;

	default:break;
	}

}

void MapNrMessage()
{
	char msg[30];
	sprintf(msg,"%s %u",gametxt[28],randseed);
	AddMessage("=========================",0);
	AddMessage(msg,iTextColor);
	AddMessage("=========================",0);
}
