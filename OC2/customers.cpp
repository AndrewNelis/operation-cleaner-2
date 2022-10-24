#include "SDL/SDL.h"   /* All SDL App's need this */
#include "oc2.h"
#include <math.h>
#include <stdlib.h>
//#include <stdio.h>
//#include <windows.h>

extern short			SIZE_X,SIZE_Y,xmiddle,ymiddle;

extern unsigned int		randseed;
extern long				lPayment;
//extern int				iTarget;

extern char				b_name[BUILDINGS][255];
extern char				b_localename[BUILDINGS][52];

char					map_nrc[MAXCAREERMAPS][52];
//extern unsigned long			map_nr[MAXCAREERMAPS];
char					map_city[MAXCAREERMAPS];

char					city_name[MAXCITIES][20];
char					city_description[MAXCITIES][5][52];
unsigned char			city_price[MAXCITIES];
unsigned char			city_insurance[MAXCITIES];
unsigned char			city_payment[MAXCITIES];
short					city_x[MAXCITIES];
short					city_y[MAXCITIES];

short					sCityBegin;

short					customer_nr;
char					customer_name[MAXCUSTOMERS][12];

char					sel_city;

extern short			plr_crew[4];
extern unsigned long	plr_daysinbusiness;
extern long				plr_money;
extern short			plr_currentcustomer,plr_gametype;
extern long				plr_currentcity;
extern char				plr_usedmaps[MAXCAREERMAPS];			// career game maps, 1=done

extern char				gametxt[MAXGAMETXT][52];
extern unsigned char	cBaseColours[3];

bool					boolFirstCustomer;
short					sel_customer;

short					customer_building;
unsigned long			customer_mapnr;
unsigned long			customer_payment;
bool					boolcustomercounted;

bool					boolprev,boolnext;
extern short			iTargetbuilding;
extern char				iTextColor;

short					iFirstMapInCity;
extern double			dp_movingcost,dp_payment;

// OC2ED begins
extern char				ed_mapfile[MAXMAPFILES][100];

extern long				ed_mapfilenr;
extern char				ed_customer[52];
extern char				ed_building[52];
extern char				ed_message[10][52];

// OC2ED ends

signed short FindFirstCustomer()
{
	short ix,iresult;

	iresult=-1;

	for(ix=0;ix<MAXCAREERMAPS;ix++)
	if(map_city[ix]>=0)
	{
		if(map_city[ix]==plr_currentcity && plr_usedmaps[ix]==0 && map_nrc[ix][0]!=0 && iresult==-1) iresult=ix;
	}

	if(iresult>=0) sel_customer=iresult;

	boolcustomercounted=false;
//	plr_currentcustomer=sel_customer;
	//boolprev=true;
	boolnext=true;
	return iresult;
}

void FindFirstCustomerInCity()
{
	iFirstMapInCity=1;
	while(map_city[iFirstMapInCity]!=plr_currentcity && iFirstMapInCity<MAXCAREERMAPS) iFirstMapInCity++; // && plr_usedmaps[iFirstMapInCity]!=0 ) iFirstMapInCity++;
	sel_customer=iFirstMapInCity;
	while(plr_usedmaps[sel_customer]==1 && map_nrc[sel_customer+1][0]!=0) sel_customer++;
}

long MovingCosts(bool bmoving)
{
long lcost,lmoving;
short xdiff,ydiff,ix;
short movecost[4]={100,500,300,100};
char msg[50];

	lcost=plr_daysinbusiness/100;
	lcost*=1000;
	if(lcost>20000) lcost=20000;

	xdiff=abs(city_x[sel_city]-city_x[plr_currentcity]);
	ydiff=abs(city_y[sel_city]-city_y[plr_currentcity]);

	lmoving=long(sqrt(xdiff*xdiff+ydiff*ydiff))*5;
	lcost=lmoving;

	for(ix=0;ix<4;ix++)
	{
		lcost+=(plr_crew[ix]*movecost[ix]);
	}

	lcost*=(long) dp_movingcost;		// for difficulty
	lcost/=100;

	if(bmoving==true && plr_money>lcost)
	{
		plr_money-=lcost;
		AddDays(5+lmoving/500);
		plr_currentcity=sel_city;
//		FindFirstCustomer();
		sprintf(msg,"*** %s",gametxt[24]);	// moving to
		AddMessage(msg,3);

		sprintf(msg,"*** %s ($%u)",city_name[plr_currentcity],lcost);
		AddMessage(msg,3);

		FindFirstCustomer();
	}

return lcost;
}

void DrawCityMenu()
{
short ix,ic,inr;
char msg[30];
long tmpCost;

	DrawButton(xmiddle-250,ymiddle-250,xmiddle+250,ymiddle+250);
	ShowBMP1(450,450,300,100,xmiddle-150,ymiddle-240);

	DrawFillRect(xmiddle-150,ymiddle-125,xmiddle+150,ymiddle+45,100,100,100);	// list's background

	for(ix=0;ix<10;ix++)
	{
		inr=0;
		ic=iTextColor;
		for(inr=0;inr<MAXCAREERMAPS;inr++) if(map_city[inr]==ix+sCityBegin && plr_usedmaps[inr]==0) ic=3;

		sprintf(msg,"%-20s",city_name[ix+sCityBegin]);
		if(ix+sCityBegin==plr_currentcity) sprintf(msg,"* %-20s",city_name[ix+sCityBegin]);

		if(ix+sCityBegin==sel_city)
		{
			DrawTextWBack(msg,xmiddle-140,ymiddle-120+ix*16,ic,cBaseColours[0],cBaseColours[1],cBaseColours[2]);
		}
		else
		{
			DrawNormalText(msg,xmiddle-140,ymiddle-120+ix*16,ic);
		}
	}

	if(sCityBegin>0) DrawBMPButton("",xmiddle+160,ymiddle-120,10);	// up
	if(city_price[sCityBegin+10]>0) DrawBMPButton("",xmiddle+160,ymiddle+30,11);	// down

//	DrawFillRect(xmiddle-200,ymiddle+95,xmiddle+200,ymiddle+155,100,100,100);
	for(ix=0;ix<5;ix++)
	{
		DrawNormalText(city_description[sel_city][ix],xmiddle-195,ymiddle+52+ix*14,iTextColor);
	}

	tmpCost=MovingCosts(false);
//	tmpCost*=long (dp_movingcost);	// for difficulty
//	tmpCost/=100;

	ic=iTextColor;

	if(sel_city!=plr_currentcity)
	{
		if(tmpCost>plr_money) ic=2;
		sprintf(msg,"%s $%u",gametxt[25],tmpCost);	// moving would cost
		DrawNormalText(msg,xmiddle-150,ymiddle+125,ic);
	}

	inr=0;
	for(ix=0;ix<MAXCAREERMAPS;ix++) if(map_city[ix]==sel_city && plr_usedmaps[ix]==0 && map_nrc[ix][0]!=0) inr++;

	sprintf(msg,"%s: %d",gametxt[81],inr);
	DrawNormalText(msg,xmiddle-150,ymiddle+140,iTextColor);	// number of customers left

	if(plr_money>MovingCosts(false) && sel_city!=plr_currentcity && inr>0)
		DrawBMPButton(gametxt[26],xmiddle-150,ymiddle+160,5);	// change city

	DrawBMPButton(gametxt[4],xmiddle-150,ymiddle+200,5);	// cancel

}

void GenerateCustomerData(short iCode)
{
	char fn[52];
	unsigned int ix;
	bool bOk=false;

	if(iCode==1) FindFirstCustomerInCity();

	ed_customer[0]=0;
	ed_building[0]=0;
	for(ix=0;ix<10;ix++) sprintf(ed_message[ix],"%c",0);

	if(map_nrc[sel_customer][1]==58)	// :		version 1.1 begins
		if(map_nrc[sel_customer][0]==102 || map_nrc[sel_customer][0]==70)	// f or F
			bOk=true;

	sprintf(fn,"");
	if(bOk==true)
	{
		for(ix=2;ix<52;ix++)
		if(map_nrc[sel_customer][ix]>31)
			fn[ix-2]=map_nrc[sel_customer][ix];

		if(LoadMap(fn,1)!=0)
			OKBox(gametxt[202],40);	// error loading map
	}
	else
	{	// 		version 1.1 ends

	customer_mapnr=CharToInt(map_nrc[sel_customer]);
	randseed=customer_mapnr;
	GenerateMap(0);
//	customer_building=iTarget;
	customer_payment=lPayment; //*dp_payment);
//	customer_payment/=100;
	}

	boolcustomercounted=true;
}

bool CheckCareerCustomers()
{
	short ix,imatch,imaps;
//	char msg[30];

	imatch=0;
	imaps=0;

	for(ix=0;ix<MAXCAREERMAPS;ix++)
	{
		if(map_nrc[ix][0]!=0) imaps++;
		if(plr_usedmaps[ix]==1) imatch++;
	}

//	sprintf(msg,"maps %u match %u",imaps,imatch);
//	AddMessage(msg,1);

	if(imatch==imaps)		// current nr of maps
		return true;
	else
		return false;
}

void DrawCustomerMenu()
{
char msg[50];
char * fn="";
unsigned char cBld;
short ix,iy,nr,ic;
bool bOk=false;

//	if(boolcustomercounted==false) GenerateCustomerData();

	DrawButton(xmiddle-250,ymiddle-350,xmiddle+250,ymiddle+250);
	ShowBMP1(450,450,300,100,xmiddle-150,ymiddle-340);

	DrawNormalText(gametxt[27],xmiddle-240,ymiddle-220,iTextColor);	// customer name
	DrawNormalText(gametxt[29],xmiddle-240,ymiddle-180,iTextColor);	// building name
	DrawNormalText(gametxt[31],xmiddle-240,ymiddle-160,iTextColor);	// payment

	sprintf(msg,"%s",customer_name[customer_nr]);

	if(ed_customer[0]!=0 && strcmp(ed_customer,"dll")!=0) sprintf(msg,"%s",ed_customer);	// version 1.1
	DrawNormalText(msg,xmiddle-100,ymiddle-220,iTextColor);


	if(map_nrc[sel_customer][1]==58)	// :		version 1.1 begins
		if(map_nrc[sel_customer][0]==102 || map_nrc[sel_customer][0]==70)	// f or F
			bOk=true;

		if(bOk==true)
		{
		DrawNormalText(gametxt[542],xmiddle-240,ymiddle-200,iTextColor);	// map file

		for(ix=2;ix<52;ix++)
		if(map_nrc[sel_customer][ix]>31)
			fn[ix-2]=map_nrc[sel_customer][ix];
		sprintf(msg,"%s",fn);
		}
		else
		{
			DrawNormalText(gametxt[28],xmiddle-240,ymiddle-200,iTextColor);	// map nr
			sprintf(msg,"%u",customer_mapnr);
		}

		DrawNormalText(msg,xmiddle-100,ymiddle-200,iTextColor);

//	sprintf(msg,"%-s",b_name[customer_building]);
	cBld = *b_name[iTargetbuilding];

			//	sprintf(msg,"%-s %d %d",b_localename[iTargetbuilding],cBld,iTargetbuilding);
	sprintf(msg,"%-s",b_localename[iTargetbuilding]);
	if(ed_building[0]!=0) sprintf(msg,"%s",ed_building);	// version 1.1

	DrawNormalText(msg,xmiddle-100,ymiddle-180,iTextColor);

	sprintf(msg,"$%u",customer_payment);
	DrawNormalText(msg,xmiddle-100,ymiddle-160,iTextColor);
/*
// messages
	DrawFillRect(xmiddle-240,ymiddle-122,xmiddle+240,ymiddle+30,cBaseColours[0]-20,cBaseColours[1]-20,cBaseColours[2]-20);
	iy=0;
	for(ix=0;ix<10;ix++)
	{
		DrawNormalText(ed_message[ix],xmiddle-235,ymiddle-120+ix*15,iTextColor);
		if(ed_message[ix][iy]!=0) iy=1;
	}

	if(iy==1) DrawNormalText(gametxt[200],xmiddle-240,ymiddle-140,iTextColor);	// messages
	*/
//	DrawFillRect(xmiddle-220,ymiddle-120,xmiddle+220,ymiddle,cBaseColours[0],cBaseColours[1],cBaseColours[2]);

	for(ix=0;ix<5;ix++)
	for(iy=0;iy<3;iy++)
	{
		nr=iy*5+ix;
		if(plr_currentcity==map_city[iFirstMapInCity+nr])
		{
		sprintf(msg,"%2u",nr+1);

		ic=-20;

		if(plr_usedmaps[iFirstMapInCity+nr]==0)
			ic=20;

//		ic=iTextColor;
		if(sel_customer==iFirstMapInCity+nr)
		{
//			DrawRect(xmiddle-200+ix*80,ymiddle-20+iy*25,xmiddle-122+ix*80,ymiddle+3+iy*25,cBaseColours[0]+50,cBaseColours[1]+50,cBaseColours[2]+50);
//			DrawRect(xmiddle-199+ix*80,ymiddle-19+iy*25,xmiddle-123+ix*80,ymiddle+2+iy*25,cBaseColours[0]+50,cBaseColours[1]+50,cBaseColours[2]+50);
			ic=50;
		}

		DrawFillRect(xmiddle-200+ix*80,ymiddle+40+iy*25,xmiddle-121+ix*80,ymiddle+64+iy*25,cBaseColours[0]+ic,cBaseColours[1]+ic,cBaseColours[2]+ic);
		DrawNormalText(msg,xmiddle-170+ix*80,ymiddle+45+iy*25,iTextColor);
		}
	}

//	if(boolnext==true) DrawBMPButton(gametxt[5],xmiddle-150,ymiddle,5);	// next
//	if(boolprev==true) DrawBMPButton(gametxt[6],xmiddle-150,ymiddle+40,5);	// prev

	if(plr_usedmaps[sel_customer]==0 && map_city[sel_customer]==plr_currentcity)// sel_customer!=plr_currentcustomer)
	{
		DrawBMPButton(gametxt[32],xmiddle-150,ymiddle+160,5);	// preview
		DrawBMPButton(gametxt[33],xmiddle-150,ymiddle+200,5);	// accept
	}

}

void RandomCity()
{
	double dtemp=-1;

	if(plr_gametype==1) // different city for each map
	{
		while(dtemp<0 || dtemp>13)
		{
		dtemp=GetRandValue()+GetRandValue();
		dtemp/=1.286;
		}

		plr_currentcity=char(dtemp);
	}
}
