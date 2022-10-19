#include "SDL/SDL.h"   /* All SDL App's need this */
#include "oc2.h"

char					cAdvFinancial=0;

extern short			SIZE_X,SIZE_Y,xmiddle,ymiddle;
extern char				plr_name[40];

extern long				plr_money;
extern short			plr_crew[4];
extern short			plr_reputation,plr_gametype;
extern long				plr_careful,plr_currentcity,plr_taxpaid;
extern unsigned long	plr_jobsdone,plr_daysinbusiness;
extern char				plr_advdecisions[4];

extern char				city_name[MAXCITIES][20];
extern unsigned char	city_price[MAXCITIES];
extern unsigned char	city_insurance[MAXCITIES];

extern char				gametxt[MAXGAMETXT][52];
extern char				iTextColor;

void StandardMenu()
{
	char msg[50];
//	char rep_txt[12][35]={"You stink - bigtime","The BAD exploder","Mountebank of detonators","Unreliable","No good, no bad",
//	"Pretty good","Reliable","Very good","Very reliable","Extremely good","Extremely reliable","The master detonator"};
	char crep;

	short ix,xc,yc;
	unsigned long lCostperday=0;

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
	while(plr_reputation>=rep_level[crep]) crep++;
	if(crep>0) crep--;

	DrawNormalText(gametxt[39],xmiddle-240,ymiddle+30,iTextColor);//Reputation
	sprintf(msg,"%s (%d)",gametxt[40+crep],plr_reputation);
	DrawNormalText(msg,xmiddle,ymiddle+30,iTextColor);

	DrawNormalText(gametxt[52],xmiddle-240,ymiddle+50,iTextColor);//Jobs done
	sprintf(msg,"%u",plr_jobsdone);
	DrawNormalText(msg,xmiddle,ymiddle+50,iTextColor);

	DrawNormalText(gametxt[53],xmiddle-240,ymiddle+70,iTextColor);//Days in business
	sprintf(msg,"%u",plr_daysinbusiness);
	DrawNormalText(msg,xmiddle,ymiddle+70,iTextColor);

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

	DrawBMPButton(gametxt[54],xmiddle-150,ymiddle+140,5);	// adv. management
	DrawBMPButton(gametxt[55],xmiddle-150,ymiddle+180,5);	// continue
}

void AdvancedMenu()
{

	short ix,yc,sdesc;
	unsigned long cost;

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

	for(ix=0;ix<4;ix++)
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

	cost=plr_advdecisions[3]*city_price[plr_currentcity];
	sprintf(msg,"%s $%d",gametxt[57],cost);
	DrawNormalText(msg,xmiddle-50,ymiddle+70,iTextColor);


	DrawBMPButton(gametxt[7],xmiddle-150,ymiddle+150,5);		// back


}

void DrawFinancialMenu()
{
	switch(cAdvFinancial)
	{
	case 0:StandardMenu();break;
	case 1:AdvancedMenu();break;
	}

}
