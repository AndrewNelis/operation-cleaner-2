#include <math.h>
#include "SDL/SDL.h"   /* All SDL App's need this */
#include "oc2.h"

char					cAdvFinancial=0;

extern unsigned char	cBaseColours[3];

extern short			SIZE_X,SIZE_Y,xmiddle,ymiddle;
extern char				plr_name[40];

extern long				plr_money;
extern short			plr_crew[4];
extern short			plr_gametype,plr_difficulty;
extern long				plr_reputation,plr_careful,plr_currentcity,plr_taxpaid;
extern unsigned long	plr_jobsdone,plr_daysinbusiness;
extern char				plr_advdecisions[4];

extern double			dp_dailycost, dp_reputation;

extern char				city_name[MAXCITIES][20];
extern unsigned char	city_price[MAXCITIES];
extern unsigned char	city_insurance[MAXCITIES];

extern char				gametxt[MAXGAMETXT][52];
extern char				iTextColor;

extern unsigned char	plrs_name[MAXPLAYERS];
extern unsigned long	plrs_money[MAXPLAYERS][MAXSTATS];
extern short			plr_monthcounter;

short					iSTarget, iSAction;
long					lSMoney;

void StandardMenu()
{
	char msg[50];
//	char rep_txt[12][35]={"You stink - bigtime","The BAD exploder","Mountebank of detonators","Unreliable","No good, no bad",
//	"Pretty good","Reliable","Very good","Very reliable","Extremely good","Extremely reliable","The master detonator"};
	char crep;
	double dTemp;

	short ix,xc,yc;
	unsigned long lCostperday=0;

	dTemp=plr_reputation*dp_reputation/100;

	DrawButton(xmiddle-250,ymiddle-250,xmiddle+250,ymiddle+250);
	ShowBMP1(450,450,300,100,xmiddle-150,ymiddle-240);

	DrawNormalText(gametxt[98],xmiddle-240,ymiddle-140,iTextColor);	// your name
	DrawNormalText(plr_name,xmiddle,ymiddle-140,iTextColor);

	DrawBMPButton("",xmiddle-20,ymiddle-140,9);				//Change name

	DrawNormalText(gametxt[36],xmiddle-240,ymiddle-120,iTextColor);	// current city
	DrawNormalText(city_name[plr_currentcity],xmiddle,ymiddle-120,iTextColor);

	DrawNormalText(gametxt[37],xmiddle-240,ymiddle-100,iTextColor);		// money
	sprintf(msg,"$%u",plr_money);
	DrawNormalText(msg,xmiddle,ymiddle-100,iTextColor);

	for(ix=0;ix<4;ix++)
	{
		yc=ymiddle-80+ix*20;
		DrawNormalText(gametxt[142+ix],xmiddle-240,yc,iTextColor);

		sprintf(msg,"%u",plr_crew[ix]);
		DrawNormalText(msg,xmiddle,yc,iTextColor);

		sprintf(msg,"$%u/d",(crw_cost[ix]*city_price[plr_currentcity])/100);
		DrawNormalText(msg,xmiddle-80,yc,iTextColor);

		xc=xmiddle+60;
		DrawBMPButton("-10",xc,yc-5,1);
		DrawBMPButton("-1",xc+40,yc-5,1);
		DrawBMPButton("+1",xc+80,yc-5,1);
		DrawBMPButton("+10",xc+120,yc-5,1);

		DrawTextRect("?",xc+162,yc-5,xc+182,yc+14);	// question mark next to item

		lCostperday+=plr_crew[ix]*crw_cost[ix]*city_price[plr_currentcity]/100;
	}

	DrawNormalText(gametxt[38],xmiddle-240,ymiddle,iTextColor);	// cost/day
	sprintf(msg,"$%u",lCostperday);
	DrawNormalText(msg,xmiddle,ymiddle,iTextColor);

	crep=0;
	while(dTemp>=rep_level[crep]) crep++;
	if(crep>0) crep--;

	DrawNormalText(gametxt[39],xmiddle-240,ymiddle+25,iTextColor);//Reputation
	sprintf(msg,"%s (%.f)",gametxt[40+crep],dTemp);
	DrawNormalText(msg,xmiddle,ymiddle+25,iTextColor);

	DrawNormalText(gametxt[52],xmiddle-240,ymiddle+45,iTextColor);//Jobs done
	sprintf(msg,"%u",plr_jobsdone);
	DrawNormalText(msg,xmiddle,ymiddle+45,iTextColor);

	DrawNormalText(gametxt[53],xmiddle-240,ymiddle+65,iTextColor);//Days in business
	sprintf(msg,"%u",plr_daysinbusiness);
	DrawNormalText(msg,xmiddle,ymiddle+65,iTextColor);

	DrawNormalText(gametxt[545],xmiddle-240,ymiddle+85,iTextColor);	// Difficulty level
	sprintf(msg,"%u",plr_difficulty);
	DrawNormalText(msg,xmiddle,ymiddle+85,iTextColor);


//	sprintf(msg,"%u",plr_taxpaid);
//	DrawNormalText(msg,xmiddle,ymiddle+90,iTextColor);

	/*
  DrawNormalText("Window penalty",xmiddle-240,ymiddle+90,0);
	sprintf(msg,"0");
	DrawNormalText(msg,xmiddle,ymiddle+90,0);

	DrawNormalText("Wall penalty",xmiddle-240,ymiddle+110,0);
	sprintf(msg,"0");
	DrawNormalText(msg,xmiddle,ymiddle+110,0);
*/

	DrawBMPButton(gametxt[543],xmiddle-150,ymiddle+100,5);	// Statistics

	DrawBMPButton(gametxt[54],xmiddle-150,ymiddle+140,5);	// adv. management
	DrawBMPButton(gametxt[55],xmiddle-150,ymiddle+180,5);	// continue
}

long SabotageCost()
{
	long cost,lAdd;

///	plr_money=110000;
	lAdd=0;
	if(plr_money>100000) lAdd=plr_money/50000;

	cost=(long) (city_price[plr_currentcity]*pow((iSAction+1)*3,2)/100);
	cost*=100;
	cost+=cost*lAdd/10;
	return cost;
}

void AdvancedMenu()
{

	short ix,yc,sdesc;
	long cost;

	char msg[100];

/*	char txtdesc[4][4][25]={
		{"Almost nothing","Little","Almost all","Surely enough"},
		{"Incompetent","Cheap","Mediocre","Authenticated"},
		{"Almost nothing","Minimum","Required","All possible"},
		{"Hire private eye","Ruin reputation","Hire house squatters","Hire saboteurs"}
	};
*/
	DrawButton(xmiddle-250,ymiddle-250,xmiddle+250,ymiddle+250);
	ShowBMP1(450,450,300,100,xmiddle-150,ymiddle-240);

//	sprintf(msg,"%u",plr_money);
//	AddMessage(msg,1);

	for(ix=0;ix<3;ix++)
	{
		yc=ymiddle-100+ix*50;

		DrawNormalText(gametxt[111+ix*5],xmiddle-240,yc,iTextColor);

		DrawRect(xmiddle-50,yc+5,xmiddle+150,yc+10,200,200,200);
//		DrawButton(xmiddle+(plr_advdecisions[ix]*2)-55,yc,xmiddle+(plr_advdecisions[ix]*2)-45,yc+15);
		DrawBMPButton("",xmiddle+(plr_advdecisions[ix]*2)-55,yc,9);
		sdesc=(plr_advdecisions[ix]+15)/33;

		if(plr_advdecisions[ix]==0)
			sprintf(msg,"%s",gametxt[56]);		// none
		else
			sprintf(msg,"%s",gametxt[112+ix*5+sdesc]);	// description

		DrawNormalText(msg,xmiddle-230,yc+20,iTextColor);
	}

//	DrawNormalText(gametxt[57],xmiddle-50,ymiddle-120,iTextColor);	// cost/month

	cost=plr_advdecisions[0]*25/100;
	sprintf(msg,"%2d%% %s",cost,gametxt[131]);			// of incomes
	DrawNormalText(msg,xmiddle-50,ymiddle-80,iTextColor);

	cost=plr_advdecisions[1]*city_price[plr_currentcity]/4;
	sprintf(msg,"%s $%d",gametxt[57],cost);
	DrawNormalText(msg,xmiddle-50,ymiddle-30,iTextColor);

	cost=plr_advdecisions[2]*city_insurance[plr_currentcity]/2;
	sprintf(msg,"%s $%d",gametxt[57],cost);
	DrawNormalText(msg,xmiddle-50,ymiddle+20,iTextColor);


	if(plr_advdecisions[3]==0)
	{
		DrawRect(xmiddle-242,ymiddle+45,xmiddle+242,ymiddle+150,cBaseColours[0]+25,cBaseColours[1]+25,cBaseColours[2]+25);
		DrawNormalText(gametxt[126],xmiddle-240,ymiddle+50,iTextColor);	// competitors

		DrawNormalText(gametxt[98],xmiddle-230,ymiddle+70,iTextColor);	//Company name
		if(iSTarget>0) DrawBMPButton("",xmiddle-50,ymiddle+70,11);
		if(iSTarget<2) DrawBMPButton("",xmiddle-30,ymiddle+70,10);
		sprintf(msg,"%s",gametxt[plrs_name[iSTarget+1]]);	// competitor firm's name
		DrawNormalText(msg,xmiddle,ymiddle+70,iTextColor);

		DrawNormalText(gametxt[167],xmiddle-230,ymiddle+90,iTextColor);	//	action
		if(iSAction>0) DrawBMPButton("",xmiddle-50,ymiddle+90,11);
		if(iSAction<3) DrawBMPButton("",xmiddle-30,ymiddle+90,10);
		DrawNormalText(gametxt[127+iSAction],xmiddle,ymiddle+90,iTextColor);

		cost=SabotageCost();

		sprintf(msg,"%s $%u",gametxt[517],cost);	// cost
		DrawNormalText(msg,xmiddle-230,ymiddle+110,iTextColor);

		if(cost<plr_money && plrs_money[iSTarget+1][plr_monthcounter]>0) DrawBMPButton(gametxt[168],xmiddle+110,ymiddle+110,3);	// proceed
	}

	DrawBMPButton(gametxt[7],xmiddle-150,ymiddle+190,5);		// back

}

void DrawFinancialMenu()
{
	switch(cAdvFinancial)
	{
	case 0:StandardMenu();break;
	case 1:AdvancedMenu();break;
	}

}
