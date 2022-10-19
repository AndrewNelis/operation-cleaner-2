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
extern short			setSoundEffects,setDustClouds,setExplosions,setBits,setScroll,sBitSelected, setDifficulty;

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
extern long				plr_reputation,plr_money;
extern short			plr_difficulty;
extern unsigned long	plr_jobsdone,plr_daysinbusiness;
extern double			dp_money, dp_payment, dp_extracost, dp_monthlycost, dp_movingcost, dp_damperend, dp_reputation, dp_neighbour, dp_cleanup;

extern unsigned long	plrs_money[MAXPLAYERS][MAXSTATS];
extern unsigned char	plrs_statshow[MAXPLAYERS];
extern unsigned char	plrs_name[MAXPLAYERS];

extern short			iStatBeginMonth,iStatScale;

extern unsigned long	plr_month;
extern short			plr_monthcounter;

extern short			hs_reputation[MAXHISCORE];
extern char				hs_player[MAXHISCORE][30];
extern char				hs_date[MAXHISCORE][12];
extern unsigned long	hs_money[MAXHISCORE];
extern unsigned long	hs_daysinbusiness[MAXHISCORE];
extern short			hs_gametype[MAXHISCORE];
extern short			hs_diff[MAXHISCORE];

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
// OC2ED ends

void About()
{
	short i=0;
	DrawNormalText("v 1.2",xmiddle-220,ymiddle-120,iTextColor);
	DrawNormalText("1.2.2007 Jan Nyman",xmiddle+60,ymiddle-120,iTextColor);

	DrawTextRect(gametxt[370],xmiddle-220,ymiddle-100,xmiddle+220,ymiddle-80);	// credits
	while(strlen(gametxt[371+i+iCreditbegin])>0 && (371+i+iCreditbegin)<MAXGAMETXT && i<15)
	{
		DrawNormalText(gametxt[i+371+iCreditbegin],xmiddle-220,ymiddle-75+i*15,iTextColor);
		i++;
	}

	if(iCreditbegin>0) DrawBMPButton("",xmiddle+220,ymiddle-75,10);		// up
	if(strlen(gametxt[386+iCreditbegin])>0) DrawBMPButton("",xmiddle+220,ymiddle+140,11);		// down

	DrawBMPButton(gametxt[3],xmiddle-150,ymiddle+160,5);	// ok

}

void Themeselector()
{
short i;
char msg[100];
char *temp="";

	DrawFillRect(xmiddle-150,ymiddle-100,xmiddle+125,ymiddle,cBaseColours[0]-50,cBaseColours[1]-50,cBaseColours[2]-50);

	for(i=0;i<5;i++)
	{
	sprintf(temp,"%s",cButtonFileList[i+sButtonBegin]);
	if(strcmp(cButtonFile,temp)==0)
		sprintf(msg,"* %.32s",cButtonFile);
	else
		sprintf(msg,"%.34s",cButtonFileList[i+sButtonBegin]);

	if(sButtonSelected==i+sButtonBegin)
		DrawTextWBack(msg,xmiddle-150,ymiddle-96+i*20,1,cBaseColours[0]+50,cBaseColours[1]+50,cBaseColours[2]+50);
	else
		DrawNormalText(msg,xmiddle-150,ymiddle-96+i*20,iTextColor);
	}

	if(sButtonBegin>0) DrawBMPButton("",xmiddle+130,ymiddle-100,10);
	if(sButtonBegin<94)
		if(cButtonFileList[sButtonBegin+5][0]>0) DrawBMPButton("",xmiddle+130,ymiddle-20,11);

	for(i=0;i<5;i++)
		DrawNormalText(cButtonText[i],xmiddle-200,ymiddle+5+i*12,iTextColor);

	DrawNormalText(gametxt[86],xmiddle-200,ymiddle+70,iTextColor);		//author:
	DrawNormalText(cButtonAuthor,xmiddle-200,ymiddle+85,iTextColor);

	DrawNormalText(gametxt[87],xmiddle-200,ymiddle+105,iTextColor);		//Samples
	DrawRect(xmiddle-205,ymiddle+100,xmiddle+205,ymiddle+158,cBaseColours[0]+50,cBaseColours[1]+50,cBaseColours[2]+50);

	DrawBMPButton(gametxt[56],xmiddle-130,ymiddle+125,2);		// none
	DrawBMPButton("",xmiddle-40,ymiddle+105,6);
	DrawBMPButton("",xmiddle,ymiddle+105,7);
	DrawBMPButton("",xmiddle+40,ymiddle+105,8);
	DrawBMPButton("",xmiddle+100,ymiddle+105,9);

	DrawBMPButton("",xmiddle+120,ymiddle+105,10);
	DrawBMPButton("",xmiddle+120,ymiddle+135,11);

	DrawTextButton(gametxt[4],xmiddle-150,ymiddle+160,xmiddle+150,ymiddle+200);	// cancel

	sprintf(msg,"%s",cButtonFileList[sButtonSelected]);
	if(strlen(msg)>0) DrawBMPButton(gametxt[88],xmiddle-150,ymiddle+200,5);		//Change theme
}

void NewsPaper(short iCode)
{
short i,picnr,x,y,crep=0;;
short picy[5]={268,197,197,197,197};
char msg[200];
double dTemp;

	srand(5515);

	DrawFillRect(0,0,SIZE_X,SIZE_Y,220,220,220);
//	DrawFillRect(10,80,SIZE_X-20,SIZE_Y-20,200,200,200);

	for(i=0;i<400;i++)
	{
		picnr=rand()%5;
		x=rand()%(SIZE_X/50)*50;
		y=rand()%(SIZE_Y/50)*50;
		ShowBMP2(picnr*200,850,200,picy[picnr],x,y);
	}

	dTemp=plr_reputation*dp_reputation/100;
	while(dTemp>=rep_level[crep]) crep++;
	if(crep>0) crep--;

	switch(iCode)
	{
	case 1:	// begin career
		DrawFillRect(xmiddle-210,ymiddle-250,xmiddle+200,ymiddle-120,255,255,255);
	//	ShowBMP1(0,580,250,18,xmiddle-125,50);		// pine bluff courier

		sprintf(msg,"%s %s",plr_name,gametxt[291]);
		DrawNormalText(strupr(msg),xmiddle-200,ymiddle-240,1);	// moves in

		for(i=0;i<6;i++) DrawNormalText(gametxt[292+i],xmiddle-200,ymiddle-220+i*16,1);	// news text
		break;
	case 2:	// money ends, game over

	 	if(iHiscorerank==0) iHiscorerank=CheckHiscores();

		DrawFillRect(xmiddle-250,ymiddle-250,xmiddle+250,ymiddle-60,255,255,255);

		DrawNormalText(gametxt[350],xmiddle-240,ymiddle-240,1);	// demolition business news

		sprintf(msg,"%s %s",plr_name,gametxt[351]);	// goes bankrupt
		DrawNormalText(strupr(msg),xmiddle-240,ymiddle-220,1);

		sprintf(msg,"%s %d",gametxt[352],plr_daysinbusiness);	// A well-known demolition firm for
		DrawNormalText(msg,xmiddle-240,ymiddle-200,1);

		DrawNormalText(gametxt[353],xmiddle-240,ymiddle-180,1);	// has gone bankrupt.

		if(lDamageInflicted>1000)	// ruined other buildings (bigtime)
		{
		DrawNormalText(gametxt[354],xmiddle-240,ymiddle-160,1);	//  The reason was disaster in the demolition job.
		DrawNormalText(gametxt[355],xmiddle-240,ymiddle-140,1);	//  Buildings surrounding the site were ruined.

		sprintf(msg,"%s%u",gametxt[356],lDamageInflicted);			//The repairing cost will be as high as $
		DrawNormalText(msg,xmiddle-240,ymiddle-120,1);

		DrawNormalText(gametxt[357],xmiddle-240,ymiddle-100,1);	//  and the firm has no money for that.
		}
		else	// just ran out of money
			DrawNormalText(gametxt[358],xmiddle-240,ymiddle-160,1);	// The firm had already some monetary problems.

		sprintf(msg,"%s %d",gametxt[361],iHiscorerank);				// Rank in the high score list:
		if(iHiscorerank<100) DrawNormalText(msg,xmiddle-240,ymiddle-80,1);

		break;

	case 3:	// victorious career ends

	 	if(iHiscorerank==0) iHiscorerank=CheckHiscores();

		DrawFillRect(xmiddle-250,ymiddle-250,xmiddle+250,ymiddle-120,255,255,255);

		DrawNormalText(gametxt[360],xmiddle-240,ymiddle-240,1);	//You finished all the tasks!

		sprintf(msg,"%s $%u",gametxt[37],plr_money);			// money
		DrawNormalText(msg,xmiddle-240,ymiddle-220,1);

		sprintf(msg,"%s %.f (%s)",gametxt[39],dTemp,gametxt[40+crep]);			// reputation
		DrawNormalText(msg,xmiddle-240,ymiddle-200,1);

		sprintf(msg,"%s %d",gametxt[53],plr_daysinbusiness);
		DrawNormalText(msg,xmiddle-240,ymiddle-180,1);

		sprintf(msg,"%s %d",gametxt[52],plr_jobsdone);				// jobs done
		DrawNormalText(msg,xmiddle-240,ymiddle-160,1);

		sprintf(msg,"%s %d",gametxt[361],iHiscorerank);				// Rank in the high score list:
		if(iHiscorerank<100) DrawNormalText(msg,xmiddle-240,ymiddle-140,1);

		break;
	case 4:	// retire
	 	if(iHiscorerank==0) iHiscorerank=CheckHiscores();

		DrawFillRect(xmiddle-250,ymiddle-250,xmiddle+250,ymiddle-120,255,255,255);

		DrawNormalText(gametxt[363],xmiddle-240,ymiddle-240,1);	//You have finished your career succesfully.

		sprintf(msg,"%s $%u",gametxt[37],plr_money);			// money
		DrawNormalText(msg,xmiddle-240,ymiddle-220,1);

		sprintf(msg,"%s %.f (%s)",gametxt[39],dTemp,gametxt[40+crep]);			// reputation
		DrawNormalText(msg,xmiddle-240,ymiddle-200,1);

		sprintf(msg,"%s %d",gametxt[53],plr_daysinbusiness);
		DrawNormalText(msg,xmiddle-240,ymiddle-180,1);

		sprintf(msg,"%s %d",gametxt[52],plr_jobsdone);				// jobs done
		DrawNormalText(msg,xmiddle-240,ymiddle-160,1);

		sprintf(msg,"%s %d",gametxt[361],iHiscorerank);				// Rank in the high score list:
		if(iHiscorerank<100) DrawNormalText(msg,xmiddle-240,ymiddle-140,1);

		break;


	default:break;	// no idea when this would happen?
	}

	DrawBMPButton(gametxt[3],xmiddle-150,ymiddle+260,5);		// ok

	if(iCode>1)	// other than career begins
		DrawBMPButton(gametxt[543],xmiddle-150,ymiddle+220,5);		// statistics

}

void Statistics()
{
	short i,j,imc;
	unsigned int lMin,lMax,lTmp;
	double dsz,dsy,dpy,dpyold,dm,dy;
	unsigned char plrcol[MAXPLAYERS][3]={
		{200,0,0},
		{0,200,0},
		{0,0,200},
		{200,0,200}
	};
	char msg[50];
//		if(plr_monthcounter>12 && iStatBeginMonth<plr_monthcounter-12)
//			iStatBeginMonth=plr_monthcounter-12;

	dsz=(SIZE_X-100)/iStatScale;

	DrawFillRect(0,0,SIZE_X,SIZE_Y,220,220,220);

	lMin=4294967295;
	lMax=0;

	for(j=0;j<MAXPLAYERS-1;j++)
	for(i=0;i<MAXSTATS;i++)
	if(plrs_money[j][i]>0)
	{
		if(plrs_money[j][i]<lMin) lMin=plrs_money[j][i];
		if(plrs_money[j][i]>lMax) lMax=plrs_money[j][i];
	}

	if(plr_money==0) lMin=0;

	lTmp=lMax;
	lTmp/=1000;

	lTmp+=(lMax/10000)+1;

	lTmp*=1000;

	lMax=lTmp;

	lTmp=lMin;
	lTmp/=1000;

	if(lTmp>(lMax/10000)+1)
		lTmp-=(lMax/10000)+1;
	else
		lTmp=0;

	lTmp*=1000;
	lMin=lTmp;

//	sprintf(msg,"Min-max %u %u",lMin,lMax);
//	DrawNormalText(msg,SIZE_X-250,20,1);

	if(lMax>lMin)
	{
		dsy=lMax-lMin;
		dsy/=(SIZE_Y-100);
	}

	if(dsy<=0) dsy=1;

	for(i=0;i<iStatScale;i++)
	{
		if(plr_month>MAXSTATS)
			sprintf(msg,"%u",plr_month-MAXSTATS+iStatBeginMonth+i);
		else
			sprintf(msg,"%u",iStatBeginMonth+i);
//		else
//			sprintf(msg,"%u",i);

		DrawLine(100+(short) dsz*i,0,100+(short) dsz*i,SIZE_Y-100,cBaseColours[0],cBaseColours[1],cBaseColours[2]);

		imc=1;

		if(iStatScale>12)
		{
		if(strlen(msg)==3) imc=2;
		if(strlen(msg)>=4) imc=3;
		}

		if(i%imc==0) DrawNormalText(msg,90+(short) dsz*i,SIZE_Y-95,1);	// month nr (x)
	}

	DrawNormalText(gametxt[37],10,SIZE_Y-80,1);
	DrawNormalText(gametxt[544],SIZE_X-200,SIZE_Y-80,1);
	dm=lMax-lMin;
	dm/=10;

	for(i=0;i<11;i++)
	{
		dy=(SIZE_Y-100)-(i*(SIZE_Y-100)/10);
//		sprintf(msg,"%u",short(dy));
//		DrawNormalText(msg,100,20*i,1);

		sprintf(msg,"%.f",dm*i+lMin);
		DrawLine(100,(short) dy,100+(short) dsz*(iStatScale-1),(short) dy,cBaseColours[0],cBaseColours[1],cBaseColours[2]);

		DrawNormalText(msg,10,(short) dy,1);	// money amount (y)
	}

	for(j=0;j<MAXPLAYERS-1;j++)
	{
		i=plrs_statshow[j]*25-50;

		DrawFillRect(100,SIZE_Y-80+j*20,600,SIZE_Y-61+j*20,cBaseColours[0]+i,cBaseColours[1]+i,cBaseColours[2]+i);

//		DrawRect(100,SIZE_Y-80+j*20,600,SIZE_Y-61+j*20,cBaseColours[0]+i,cBaseColours[1]+i,cBaseColours[2]+i);
		ShowBMP1(550+j*20,570,20,20,100,(SIZE_Y-80)+j*20);

		if(j==0)
			DrawNormalText(plr_name,130,SIZE_Y-78+j*20,1);
		else
			DrawNormalText(gametxt[plrs_name[j]],130,SIZE_Y-78+j*20,1);

		dpy=plrs_money[j][iStatBeginMonth]-lMin;
		dpy/=dsy;

		if(plrs_statshow[j]>0)
		{
			ShowBMP1(550+j*20,570,20,20,90,(short) ((SIZE_Y-110)-dpy));
		}
	}


	for(j=0;j<MAXPLAYERS-1;j++)
	for(i=0;i<iStatScale;i++)
	if(plrs_money[j][iStatBeginMonth+i]>0) // || j==0)
	if(unsigned long (iStatBeginMonth+i)<=plr_month)
	{
		dpyold=plrs_money[j][iStatBeginMonth+i-1]-lMin;
		dpyold/=dsy;

		dpy=plrs_money[j][iStatBeginMonth+i]-lMin;
		dpy/=dsy;

		sprintf(msg,"%u",plrs_money[j][iStatBeginMonth+i]);

		if(plrs_statshow[j]>1)
			DrawTextWBack(msg,100+(short) (dsz*i),(short) ((SIZE_Y-125)-dpy),0,plrcol[j][0],plrcol[j][1],plrcol[j][2]);

//		sprintf(msg,"%u",plrs_money[j][ib+i-1]);
//		DrawTextWBack(msg,50+dsz*i,120+j*50,0,plrcol[j][0],plrcol[j][1],plrcol[j][2]);

		if(plrs_statshow[j]>0)
		{
			if(i>0) DrawLine(100+(short) (dsz*(i-1)),(short) ((SIZE_Y-100)-dpyold),(short) (100+dsz*i),(short) ((SIZE_Y-100)-dpy),plrcol[j][0],plrcol[j][1],plrcol[j][2]);

			ShowBMP1(550+j*20,570,20,20,(short) (90+dsz*i),(short) ((SIZE_Y-110)-dpy));
		}

//		sprintf(msg,"x %.f y %.f",50+dsz*i,dpy);
//		DrawNormalText(msg,i*100+50,50+j*20,1);
	}

	if(iStatScale==60)
		DrawBMPButton(gametxt[78],SIZE_X-240,SIZE_Y-30,3);
	else
		DrawBMPButton(gametxt[79],SIZE_X-240,SIZE_Y-30,3);

	if(iStatBeginMonth<MAXSTATS-iStatScale) DrawBMPButton(gametxt[5],SIZE_X-120,SIZE_Y-60,3);	// next
	if(iStatBeginMonth>0) DrawBMPButton(gametxt[6],SIZE_X-240,SIZE_Y-60,3);	// previous

	DrawBMPButton(gametxt[7],SIZE_X-120,SIZE_Y-30,3);	// back
//	Update();
}

void HighScores()
{
	short i;
	char msg[200];

	if(hs_reputation[0]==0)
	{
		GetHiscores();
//		SaveHiscores();	// only for testing hiscores
	}

	DrawFillRect(xmiddle-450,ymiddle-250,xmiddle-250,ymiddle+250,cBaseColours[0],cBaseColours[1],cBaseColours[2]);	// left side
	DrawFillRect(xmiddle+250,ymiddle-250,xmiddle+450,ymiddle+250,cBaseColours[0],cBaseColours[1],cBaseColours[2]);	// right side

	DrawTextRect(gametxt[108],xmiddle-440,ymiddle-110,xmiddle-151,ymiddle-90);	// high scores
	DrawTextRect(gametxt[39],xmiddle-150,ymiddle-110,xmiddle-51,ymiddle-90);	// reputation
	sprintf(msg,"%.11s",gametxt[545]);
	DrawTextRect(msg,xmiddle-50,ymiddle-110,xmiddle+49,ymiddle-90);		//Difficulty level
	DrawTextRect(gametxt[166],xmiddle+50,ymiddle-110,xmiddle+139,ymiddle-90);		// career game

	DrawTextRect(gametxt[37],xmiddle+140,ymiddle-110,xmiddle+249,ymiddle-90);	// money
	DrawTextRect(gametxt[53],xmiddle+250,ymiddle-110,xmiddle+440,ymiddle-90);	// days in business

/*	DrawNormalText(gametxt[108],xmiddle-290,ymiddle-110,iTextColor);	// high scores
	DrawNormalText(gametxt[39],xmiddle-50,ymiddle-110,iTextColor);	// reputation
	DrawNormalText(gametxt[37],xmiddle+60,ymiddle-110,iTextColor);			// money
	DrawNormalText(gametxt[53],xmiddle+110,ymiddle-110,iTextColor);	// days in business
*/
	for(i=0;i<MAXHISCORE-1;i++)
	{
		sprintf(msg,"%2d. %-30s        %5d        %2u       %-10s   %8u  %5d   %s",i+1,hs_player[i],hs_reputation[i],hs_diff[i],gametxt[hs_gametype[i]+1],hs_money[i],hs_daysinbusiness[i],hs_date[i]);
		DrawNormalText(msg,xmiddle-440,ymiddle-80+i*25,iTextColor);
	}

	DrawBMPButton(gametxt[3],xmiddle-150,ymiddle+200,5);		// ok
}

void DrawDifficultyDesc()
{
	char msg[110];

	DrawFillRect(xmiddle+250,ymiddle-35,xmiddle+550,ymiddle+150,cBaseColours[0],cBaseColours[1],cBaseColours[2]);

	DrawNormalText(gametxt[550],xmiddle+260,ymiddle-30,iTextColor);	// Difficulty level effects
	DrawNormalText(gametxt[552],xmiddle+260,ymiddle-10,iTextColor);	// start money

	sprintf(msg,"$%.f",dp_money);
	DrawNormalText(msg,xmiddle+450,ymiddle-10,iTextColor);

	DrawNormalText(gametxt[31],xmiddle+260,ymiddle+10,iTextColor);	// payment
	sprintf(msg,"%.f%%",dp_payment);
	DrawNormalText(msg,xmiddle+450,ymiddle+10,iTextColor);

	DrawNormalText(gametxt[555],xmiddle+260,ymiddle+30,iTextColor);	// damage costs
	sprintf(msg,"%.f%%",dp_neighbour);
	DrawNormalText(msg,xmiddle+450,ymiddle+30,iTextColor);

	DrawNormalText(gametxt[22],xmiddle+260,ymiddle+50,iTextColor);	// cleanup costs
	sprintf(msg,"%.f%%",dp_cleanup);
	DrawNormalText(msg,xmiddle+450,ymiddle+50,iTextColor);

	DrawNormalText(gametxt[157],xmiddle+260,ymiddle+70,iTextColor);	// extra costs
	sprintf(msg,"%.f%%",dp_extracost);
	DrawNormalText(msg,xmiddle+450,ymiddle+70,iTextColor);

	DrawNormalText(gametxt[551],xmiddle+260,ymiddle+90,iTextColor);	// moving costs
	sprintf(msg,"%.f%%",dp_movingcost);
	DrawNormalText(msg,xmiddle+450,ymiddle+90,iTextColor);

	DrawNormalText(gametxt[553],xmiddle+260,ymiddle+110,iTextColor);	// damper endurance
	sprintf(msg,"%.f%%",dp_damperend);
	DrawNormalText(msg,xmiddle+450,ymiddle+110,iTextColor);

	DrawNormalText(gametxt[39],xmiddle+260,ymiddle+130,iTextColor);	// reputation
	sprintf(msg,"%.f%%",dp_reputation);
	DrawNormalText(msg,xmiddle+450,ymiddle+130,iTextColor);

}

void DrawMenus()
{
short ix;
char msg[110];

//	if(iAccelerationTemp==0) iAccelerationTemp=setAccelerationMode;

	DrawFillRect(0,0,SIZE_X,SIZE_Y,0,0,0);

/*	DrawFillRect(10,10,20,20,ccounter,ccounter,ccounter);
	ccounter+=5;
	if(ccounter>240) ccounter=40;
*/
	DrawFillRect(xmiddle-250,ymiddle-250,xmiddle+250,ymiddle+250,cBaseColours[0],cBaseColours[1],cBaseColours[2]);

	ShowBMP1(450,450,300,100,xmiddle-150,ymiddle-220);

	switch(iMenulevel)
	{
	case 0:
		if(GameOn==0)
		{
			DrawBMPButton(gametxt[89],xmiddle-150,ymiddle-120,5);		//Quick game
			DrawBMPButton(gametxt[12],xmiddle-150,ymiddle-80,5);		// map nr/text
			DrawBMPButton(gametxt[533],xmiddle-150,ymiddle-40,5);		// Load map file
			DrawBMPButton(gametxt[90],xmiddle-150,ymiddle,5);			// begin career
			DrawBMPButton(gametxt[93],xmiddle-150,ymiddle+40,5);			//Load game
		}

		if(GameOn==2)
		{
			DrawBMPButton(gametxt[91],xmiddle-150,ymiddle-120,5);	//Return to game
			DrawBMPButton(gametxt[92],xmiddle-150,ymiddle-80,5);		//Cancel current game
			DrawBMPButton(gametxt[84],xmiddle-150,ymiddle+40,5);			//Load game/save game
		}

		DrawBMPButton(gametxt[108],xmiddle-150,ymiddle+80,5);		// high scores
		DrawBMPButton(gametxt[95],xmiddle-150,ymiddle+120,5);		//Game settings
		DrawBMPButton(gametxt[96],xmiddle-150,ymiddle+160,5);		// about
		DrawBMPButton(gametxt[97],xmiddle-150,ymiddle+200,5);		// quit
		break;
	case 3:	// career
		DrawNormalText(gametxt[98],xmiddle-150,ymiddle-120,iTextColor);			//Your name:
		DrawNormalText(plr_name,xmiddle-150,ymiddle-100,iTextColor);
		DrawBMPButton(gametxt[99],xmiddle-150,ymiddle-80,5);				//Change name

		sprintf(msg,"%s %u",gametxt[545],plr_difficulty);	// difficulty level
		DrawNormalText(msg,xmiddle-150,ymiddle-30,iTextColor);
		DrawRect(xmiddle-140,ymiddle-10,xmiddle+140,ymiddle,cBaseColours[0]+50,cBaseColours[1]+50,cBaseColours[2]+50);
		DrawBMPButton("",(xmiddle-173)+plr_difficulty*31,ymiddle-13,9);

		// difficulty level details
		DrawDifficultyDesc();

		DrawNormalText(gametxt[100],xmiddle-150,ymiddle+20,iTextColor);					//How careful you are?
		DrawRect(xmiddle-148,ymiddle+40,xmiddle+148,ymiddle+50,cBaseColours[0]+50,cBaseColours[1]+50,cBaseColours[2]+50);
		DrawBMPButton("",xmiddle+plr_careful-5,ymiddle+37,9);
//		DrawButton(xmiddle+plr_careful-5,ymiddle+75,xmiddle+plr_careful+5,ymiddle+95);
		DrawNormalText(gametxt[101],xmiddle-200,ymiddle+60,iTextColor);			//Not at all
		DrawNormalText(gametxt[102],xmiddle+100,ymiddle+60,iTextColor);		//Very careful

		if(bTutorial==true)
			DrawBMPButton("",xmiddle-150,ymiddle+80,6);
		else
			DrawBMPButton("",xmiddle-150,ymiddle+80,7);

		DrawNormalText(gametxt[400],xmiddle-100,ymiddle+90,iTextColor);			//tutorial

		DrawBMPButton(gametxt[5],xmiddle-150,ymiddle+120,5);		// next
		DrawBMPButton(gametxt[7],xmiddle-150,ymiddle+160,5);		// back
		break;
	case 4:	// load & save
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
	case 5:	// settings

		DrawNormalText(gametxt[80],xmiddle-200,ymiddle-110,iTextColor);		//Sounds
		DrawNormalText(gametxt[104],xmiddle+40,ymiddle-110,iTextColor);		//Show explosions
		DrawNormalText(gametxt[103],xmiddle-200,ymiddle-80,iTextColor);		//Show dust
//		DrawNormalText(gametxt[105],xmiddle+40,ymiddle-80,iTextColor);//Save all done maps

		if(setSoundEffects==0)
			DrawBMPButton("",xmiddle-240,ymiddle-120,7);
		else
			DrawBMPButton("",xmiddle-240,ymiddle-120,6);

		if(setExplosions==0)
			DrawBMPButton("",xmiddle,ymiddle-120,7);
		else
			DrawBMPButton("",xmiddle,ymiddle-120,6);


		if(setDustClouds==0)
			DrawBMPButton("",xmiddle-240,ymiddle-90,7);
		else
			DrawBMPButton("",xmiddle-240,ymiddle-90,6);

		sprintf(msg,"%2u",setDifficulty);
		DrawNormalText(msg,xmiddle+20,ymiddle-78,iTextColor);
		DrawNormalText(gametxt[545],xmiddle+70,ymiddle-80,iTextColor);	// difficulty level
		if(setDifficulty>1) DrawBMPButton("",xmiddle,ymiddle-80,11);	// down
		if(setDifficulty<10) DrawBMPButton("",xmiddle+40,ymiddle-80,10);	// up
		DrawDifficultyDesc();

/*		if(setSaveAllDoneMaps==0)
			DrawBMPButton("",xmiddle,ymiddle-90,7);
		else
			DrawBMPButton("",xmiddle,ymiddle-90,6);
*/
		DrawNormalText(gametxt[106],xmiddle-200,ymiddle-50,iTextColor);	//Resolution
		DrawFillRect(xmiddle-100,ymiddle-50,xmiddle,ymiddle+30,cBaseColours[0]-50,cBaseColours[1]-50,cBaseColours[2]-50);

		for(ix=0;ix<5;ix++)
		if(sResolutionX[sResolutionBegin+ix]>0)
		{
			if(sResolutionX[sResolutionBegin+ix]==SIZE_X && sResolutionY[sResolutionBegin+ix]==SIZE_Y)
				sprintf(msg,"* %4dx%4d",sResolutionX[ix+sResolutionBegin],sResolutionY[ix+sResolutionBegin]);
			else
				sprintf(msg,"%4dx%4d",sResolutionX[ix+sResolutionBegin],sResolutionY[ix+sResolutionBegin]);

			if(sResolutionSelected==ix+sResolutionBegin)
				DrawTextWBack(msg,xmiddle-95,ymiddle-50+ix*16,1,cBaseColours[0]+50,cBaseColours[1]+50,cBaseColours[2]+50);
			else
				DrawNormalText(msg,xmiddle-95,ymiddle-50+ix*16,iTextColor);
		}

		if(sResolutionBegin>0) DrawBMPButton("",xmiddle+10,ymiddle-50,10);		// up arrow
		if(sResolutionBegin<94)
			if(sResolutionX[sResolutionBegin+5]>0) DrawBMPButton("",xmiddle+10,ymiddle+10,11);	// down arrow

		DrawNormalText(gametxt[155],xmiddle+70,ymiddle-50,iTextColor);		// Color depth/bits

		if(sBitSelected==16)
		{
			DrawTextWBack("   16   ",xmiddle+70,ymiddle-30,1,cBaseColours[0]+50,cBaseColours[1]+50,cBaseColours[2]+50);
			DrawTextWBack("   32   ",xmiddle+70,ymiddle-10,iTextColor,cBaseColours[0]-50,cBaseColours[1]-50,cBaseColours[2]-50);
		}
		else
		{
			DrawTextWBack("   16   ",xmiddle+70,ymiddle-30,iTextColor,cBaseColours[0]-50,cBaseColours[1]-50,cBaseColours[2]-50);
			DrawTextWBack("   32   ",xmiddle+70,ymiddle-10,1,cBaseColours[0]+50,cBaseColours[1]+50,cBaseColours[2]+50);
		}

		if(setScroll==0)
			sprintf(msg,"%s: %s",gametxt[156],gametxt[2]);	 		// Scroll, no
		else
			sprintf(msg,"%s: %d",gametxt[156],setScroll);			// Scroll speed

		DrawNormalText(msg,xmiddle+70,ymiddle+40,iTextColor);

		DrawRect(xmiddle+70,ymiddle+60,xmiddle+180,ymiddle+70,cBaseColours[0]+50,cBaseColours[1]+50,cBaseColours[2]+50);
		DrawBMPButton("",xmiddle+65+setScroll*10,ymiddle+57,9);


		DrawNormalText(gametxt[147],xmiddle-200,ymiddle+40,iTextColor);	//Language
		DrawFillRect(xmiddle-100,ymiddle+40,xmiddle,ymiddle+110,cBaseColours[0]-50,cBaseColours[1]-50,cBaseColours[2]-50);

		for(ix=0;ix<4;ix++)
		{
			if(strcmp(cLangfile,cLanglist[ix+sLangBegin])==0)
				sprintf(msg,"* %s",cLanglist[ix+sLangBegin]);
			else
				sprintf(msg,"%s",cLanglist[ix+sLangBegin]);

			if(sLangSelected==ix+sLangBegin)
				DrawTextWBack(msg,xmiddle-90,ymiddle+40+ix*16,1,cBaseColours[0]+50,cBaseColours[1]+50,cBaseColours[2]+50);
			else
				DrawNormalText(msg,xmiddle-90,ymiddle+40+ix*16,iTextColor);
		}

		if(sLangBegin>0)
			DrawBMPButton("",xmiddle+10,ymiddle+40,10);			// up
		if(sLangBegin<95 && cLanglist[sLangBegin+4][0]>0)
			DrawBMPButton("",xmiddle+10,ymiddle+90,11);			// down



		DrawBMPButton(gametxt[109],xmiddle-150,ymiddle+120,5);		//Theme selector

		if(bSettingsChanged==true) DrawBMPButton(gametxt[110],xmiddle-150,ymiddle+160,5);//Save settings

		DrawBMPButton(gametxt[7],xmiddle-150,ymiddle+200,5);		// back

		break;
	case 6:	// about
		About();
		break;

	case 7:	// themes
		Themeselector();
		break;

	case 8:	// high scores
		HighScores();
		break;


	case 9:	//	load map file, version 1.1
		DrawFillRect(xmiddle-200,ymiddle-110,xmiddle+200,ymiddle+114,cBaseColours[0]-50,cBaseColours[1]-50,cBaseColours[2]-50);
		for(ix=0;ix<14;ix++)
		{
			if(ix+ed_filebegin==ed_mapfilenr)
				DrawTextWBack(ed_mapfile[ix+ed_filebegin],xmiddle-200,ymiddle-110+ix*16,1,cBaseColours[0]+50,cBaseColours[1]+50,cBaseColours[2]+50);
			else
				DrawNormalText(ed_mapfile[ix+ed_filebegin],xmiddle-200,ymiddle-110+ix*16,0);
		}

		DrawFillRect(xmiddle-250,ymiddle+250,xmiddle+250,ymiddle+400,cBaseColours[0],cBaseColours[1],cBaseColours[2]);	// down part

		DrawNormalText(gametxt[29],xmiddle-240,ymiddle+205,iTextColor); // Building name
		DrawNormalText(temp_ed_building,xmiddle-50,ymiddle+205,iTextColor);
		// messages

		DrawNormalText(gametxt[200],xmiddle-240,ymiddle+225,iTextColor); // messages

		DrawFillRect(xmiddle-245,ymiddle+242,xmiddle+245,ymiddle+380,cBaseColours[0]-50,cBaseColours[1]-50,cBaseColours[2]-50);
		for(ix=0;ix<10;ix++)
		{
			DrawNormalText(temp_ed_message[ix],xmiddle-240,ymiddle+245+ix*13,iTextColor);
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


		if(ed_filebegin>=0 && ed_mapfile[ed_mapfilenr][0]!=0) DrawBMPButton(gametxt[533],xmiddle-150,ymiddle+120,5);	// load map
		DrawBMPButton(gametxt[7],xmiddle-150,ymiddle+160,5);		// back

		break;

	case 10:	// begin career
			NewsPaper(1);
		break;

	case 11:	// money ends, game over
		NewsPaper(2);
		break;

	case 12:	// victorious career ends
		NewsPaper(3);
		break;

	case 13:	// retirement
		NewsPaper(4);
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
