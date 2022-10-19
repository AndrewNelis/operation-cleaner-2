#include <limits.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/timeb.h>
#include "SDL/SDL.h"   /* All SDL App's need this */
#include "oc2.h"

extern short			block_penres[MAXBLOCKS];
extern short			block_end[MAXBLOCKS];
extern unsigned short	block_cost[MAXBLOCKS];

extern unsigned char	map[MAPSIZE][MAPSIZE];
extern unsigned char	mapv[MAPSIZE][MAPSIZE];
extern short			mapend[MAPSIZE][MAPSIZE];
extern int				mapsupport[MAPSIZE][MAPSIZE];
extern short			mapcounted[MAPSIZE][MAPSIZE];
extern bool				maptarget[MAPSIZE][MAPSIZE];

extern short			p_buildbegin;
extern short			p_buildwidth;

//extern int				iTargetx,iTargetendx;
//short					TargetAreaBegin,TargetAreaEnd;
extern long				lTargetValue,lOtherValue,lPayment;
extern bool				blocksmoving,boolJobDone,boolJobSuccess;

extern int				m_x,m_y,m_event;

short					oldm_x[100];
short					oldm_y[100];
short					oldm_event[100];
char					oldmcounter=0;
short					iLastActionMsec;
double					dLastActionTime=0,dPrevActionTime=0;
short					randvalues[100];
char					crandvalue=0;
char					cmouse=0;
short					iGroundSupport, iSteelSupport;

extern long				plr_money,lTotalcost,lPlacehours,plr_taxpaid,lExtraCosts;
extern unsigned long	plr_jobsdone,plr_daysinbusiness;
extern short			plr_crew[4];
extern short			plr_reputation;
extern long				plr_currentcity,plr_careful;
extern char				plr_advdecisions[4];
extern char				plr_usedmaps[MAXCAREERMAPS];

extern short			sel_customer;

extern unsigned char	city_price[MAXCITIES];
extern unsigned char	city_insurance[MAXCITIES];
extern unsigned char	city_payment[MAXCITIES];

extern char				gametxt[MAXGAMETXT][52];

extern short			SIZE_X,SIZE_Y,BLOCKSIZE;
extern int				mapbegin_x,mapbegin_y;
extern short			mapdestx,mapdesty;

char					cMapredraw[MAPSIZE][MAPSIZE];
long					lDamageInflicted,lAllExplosiveCosts;

extern char				GameOn;
extern short			iMenulevel;

extern char				iTextColor;
extern bool				bTutorial;
extern char				cTutorial;

extern char				cOKBoxText[10][50];

extern long				lLastSound,lExpParticles;

extern short			max_mapy[MAPSIZE];

extern long				lTimer;

extern bool				ed_bsaveok;

void RecordMouse()
{
	if(oldm_x[oldmcounter]!=m_x || oldm_y[oldmcounter]!=m_y)
	{
	oldmcounter++;
	if(oldmcounter>=100) oldmcounter=0;
	oldm_x[oldmcounter]=m_x;
	oldm_y[oldmcounter]=m_y;
	oldm_event[oldmcounter]=m_event+1;

	cMapredraw[mapbegin_x+m_x/BLOCKSIZE][mapbegin_y+m_y/BLOCKSIZE]=2;
	}
}

short CountAngle(short x1,short y1,short x2,short y2)
{
	double temp;

	temp=atan2(y2-y1,x2-x1)/rad-90;
//	temp=atan2(y2-y1,x2-x1)/rad-90;

	if(temp<0) temp+=360;

	return short(temp);
}

void DefineTargetArea()
{
	short ix,iy,ybegin=0,miny=MAPSIZE,imaxt,ixsize;
	bool ok;
//	char msg[30];

	lPayment=0;
	imaxt=0;

//	printf(" %d %d\n",p_buildbegin,p_buildwidth);

//	tmpbuildbegin=p_buildbegin;
//	tmpbuildwidth=p_buildwidth;

	for(ix=0;ix<p_buildwidth;ix++)
	for(iy=0;iy<MAPSIZE;iy++)
	{
		ok=false;
		if(map[p_buildbegin+ix][iy]>0 && map[p_buildbegin+ix][iy]<40) ok=true;	// pipes are not normally targetarea
		if(map[p_buildbegin+ix][iy]>50 && map[p_buildbegin+ix][iy]<56) ok=true;			// water post - satellite dish
		if(map[p_buildbegin+ix][iy]>=133 && map[p_buildbegin+ix][iy]<=138) ok=true;		// signs

/*		if(map[p_buildbegin+ix][iy]>=40 && map[p_buildbegin+ix][iy]<=50) 		// pipes?
		{
			if(iy>0 && map[p_buildbegin+ix][iy-1]>0 && map[p_buildbegin+ix][iy-1]<40) ok=true;
			if(iy<MAPSIZE-1 && map[p_buildbegin+ix][iy+1]>0 && map[p_buildbegin+ix][iy+1]<40) ok=true;
			if(p_buildbegin+ix>0 && map[p_buildbegin+ix-1][iy]>0 && map[p_buildbegin+ix-1][iy]<40) ok=true;
			if(p_buildbegin+ix<MAPSIZE-1 && map[p_buildbegin+ix+1][iy]>0 && map[p_buildbegin+ix+1][iy]<40) ok=true;
		}
*/
		if(ok==true)
		{
			if(iy>ybegin) ybegin=iy;
			maptarget[p_buildbegin+ix][iy]=true;
			lPayment+=(block_cost[map[p_buildbegin+ix][iy]]*city_payment[plr_currentcity]);	// payment
			if(iy<miny) miny=iy;
			if(ix>imaxt) imaxt=ix;
		}
	}

	for(ix=0;ix<p_buildwidth;ix++)	// search for target pipe, another target in buildpipe routine
	for(iy=miny-1;iy<MAPSIZE;iy++)
		if(map[ix+p_buildbegin][iy]==40) maptarget[ix+p_buildbegin][iy]=true;


	lPayment/=1000;

	if(lPayment<=50000) lPayment/=9;
	if(lPayment>50000 && lPayment<=100000) lPayment/=10;
	if(lPayment>100000 && lPayment<=200000) lPayment/=12;
	if(lPayment>200000 && lPayment<=300000) lPayment/=14;
	if(lPayment>300000)					// in 1.0.1, prev. /=8
			lPayment/=16;

	// center x
	ixsize=(SIZE_X-260)/BLOCKSIZE;

	if(p_buildwidth<ixsize)
		mapbegin_x=p_buildbegin-(ixsize-p_buildwidth)/2;
	else
		mapbegin_x=p_buildbegin-2;

	mapdestx=mapbegin_x;

	mapbegin_y=ybegin-(SIZE_Y/BLOCKSIZE)+4;
	mapdesty=mapbegin_y;
	MoveMap();

}

/*
void DefineTargetArea()
{
	short ix,iy,ybegin=0,miny=MAPSIZE;
	bool ok;
//	char msg[30];

	lPayment=0;

//	printf(" %d %d\n",p_buildbegin,p_buildwidth);
//	sprintf(msg," %d %d",p_buildbegin,p_buildwidth+p_buildbegin);
//	AddMessage(msg,1);

	for(ix=0;ix<p_buildwidth;ix++)
	for(iy=0;iy<MAPSIZE;iy++)
	{
		ok=false;
		if(map[p_buildbegin+ix][iy]>0 && map[p_buildbegin+ix][iy]<40) ok=true;	// pipes are not normally targetarea
		if(map[p_buildbegin+ix][iy]>50 && map[p_buildbegin+ix][iy]<56) ok=true;			// water post - satellite dish
		if(map[p_buildbegin+ix][iy]>=133 && map[p_buildbegin+ix][iy]<=138) ok=true;		// signs

/*		if(map[p_buildbegin+ix][iy]>=40 && map[p_buildbegin+ix][iy]<=50) 		// pipes?
		{
			if(iy>0 && map[p_buildbegin+ix][iy-1]>0 && map[p_buildbegin+ix][iy-1]<40) ok=true;
			if(iy<MAPSIZE-1 && map[p_buildbegin+ix][iy+1]>0 && map[p_buildbegin+ix][iy+1]<40) ok=true;
			if(p_buildbegin+ix>0 && map[p_buildbegin+ix-1][iy]>0 && map[p_buildbegin+ix-1][iy]<40) ok=true;
			if(p_buildbegin+ix<MAPSIZE-1 && map[p_buildbegin+ix+1][iy]>0 && map[p_buildbegin+ix+1][iy]<40) ok=true;
		}
//
		if(ok==true)
		{
			if(iy>ybegin) ybegin=iy;
			maptarget[p_buildbegin+ix][iy]=true;
			lPayment+=(block_cost[map[p_buildbegin+ix][iy]]*city_payment[plr_currentcity]);	// payment
			if(iy<miny) miny=iy;
		}
	}

	for(ix=0;ix<p_buildwidth;ix++)	// search for target pipe, another target in buildpipe routine
	for(iy=miny-1;iy<MAPSIZE;iy++)
		if(map[ix+p_buildbegin][iy]==40) maptarget[ix+p_buildbegin][iy]=true;


	lPayment/=1000;
	if(lPayment<=50000) lPayment/=8;
	if(lPayment>50000 && lPayment<=100000) lPayment/=10;
	if(lPayment>100000 && lPayment<=200000) lPayment/=12;
	if(lPayment>200000 && lPayment<=300000) lPayment/=14;
	if(lPayment>300000)					// in 1.0.1, prev. /=8
			lPayment/=16;

	if(lPayment<500) lPayment+=500;



	mapbegin_x=p_buildbegin-2;
	mapdestx=mapbegin_x;
	mapbegin_y=ybegin-(SIZE_Y/BLOCKSIZE)+2;
	mapdesty=mapbegin_y;
	MoveMap();

}
*/
long TargetValue()
{
	short ix,iy;
	long lbuilding;

	lbuilding=0;

	for(ix=0;ix<MAPSIZE;ix++)
	for(iy=0;iy<MAPSIZE;iy++)
		if(maptarget[ix][iy]==true && mapend[ix][iy]>0 && map[ix][iy]>0)
			lbuilding+=mapend[ix][iy];

	return lbuilding;
}

long OtherValue()
{
	short ix,iy,ok;
	long lbuilding=0,lbuildingvalue;

	for(ix=0;ix<MAPSIZE;ix++)
	for(iy=0;iy<MAPSIZE;iy++)
	{
		ok=0;
		if(maptarget[ix][iy]==false)
		{
		if(map[ix][iy]>0 && map[ix][iy]<56) ok=1;	// buildings etc
		if(map[ix][iy]>=127) ok=1;					// cars etc
		}

		if(ok==1)
		{
			if(block_end[map[ix][iy]]>0)
				lbuildingvalue=mapend[ix][iy]*block_cost[map[ix][iy]]/block_end[map[ix][iy]];
			else
				lbuildingvalue=0;

			lbuilding+=lbuildingvalue;
		}
	}

	return lbuilding;
}

void CheckIfGameEnds(long lAllCosts, long lDamage, short icode)
{
	short ix;
	char msg[30];

	if(plr_money<lAllCosts || plr_money==0)
	{
		lLastSound=-100;
		CrashSound(rand()%100);
		plr_money=0;	// game over
		if(lDamage<1000)	// note if only little damage
		{
		for(ix=0;ix<10;ix++) sprintf(cOKBoxText[ix],"");
		strcpy(cOKBoxText[0],gametxt[158]);	// firm ran out of money
		OKBox("",50);
//		AddMessage("MONEY ENDS!!!",3);
		}
		else
		{
			GameOn=0;
			iMenulevel=11;
		}
	}
	else
	{
		plr_money-=lAllCosts;
		if(lAllCosts>0)	plr_usedmaps[sel_customer]=1;	// only set played ok if some costs

		if(CheckCareerCustomers()==true)	// victorious game over
		{
			PlayWav("media\\cheer.wav");
			GameOn=0;
			iMenulevel=12;
		}
		else
		{		// game continues normally
		// here some other noises as well??? in final version?

		if(lAllCosts>0 && icode==1)	// only play sound if map ends
			{
			ix=rand()%8+1;
			sprintf(msg,"media\\endjob%d.wav",ix);
			PlayWav(msg);
			}
		}
	}

}

void AddDays(long lAddDays)
{
	long cost;
	char msg[30];

	do
	{


	if(plr_daysinbusiness%30==0 && plr_daysinbusiness>0)	// monthly costs
	{
		cost=plr_advdecisions[1]*city_price[plr_currentcity]/4;				// accountant
		cost+=plr_advdecisions[2]*city_insurance[plr_currentcity]/2;		// insurances
		cost+=plr_advdecisions[3]*city_price[plr_currentcity];				// sabotage etc.

		sprintf(msg,"%s $%u",gametxt[14],cost);	// monthly cost
		AddMessage("*****",iTextColor);
		AddMessage(msg,iTextColor);
		AddMessage("*****",iTextColor);

		if(plr_money>=cost)
		{
			plr_money-=cost;
		}
		else
		{
			plr_money=0;
		}

	}

	plr_daysinbusiness++;
	lAddDays--;
	} while(lAddDays>0);

/*
	cost=plr_advdecisions[1]*city_price[plr_currentcity]/4;
	sprintf(msg,"$%d",cost);
	DrawNormalText(msg,xmiddle-50,ymiddle-30,0);

	cost=plr_advdecisions[2]*city_insurance[plr_currentcity]/2;
	sprintf(msg,"$%d",cost);
	DrawNormalText(msg,xmiddle-50,ymiddle+20,0);

	cost=plr_advdecisions[3]*city_price[plr_currentcity];
	sprintf(msg,"$%d",cost);
	DrawNormalText(msg,xmiddle-50,ymiddle+70,0);
*/

	CheckIfGameEnds(0,0,0);
}


void CountExpenses(char cCode)	// 0=count expenses, 1=count exp & payment
{

	long lTempOtherValue,lDamage,lAllCosts,lTempTargetValue,lCleanupTime,lTransportTime,lMaxTime;
	short ix;
	char msg[30];
	double dPayment=0,dTax,dDamage,dDays,dTemp;

	lAllCosts=0;

	if(cCode==0)
	{
	lAllCosts=lTotalcost;
	lAllExplosiveCosts+=lTotalcost;
	CheckIfGameEnds(lAllCosts,0,cCode);
	sprintf(msg,"%s $%u",gametxt[15],lTotalcost);	// expl costs
	AddMessage(msg,iTextColor);
	lAllCosts=0;
	lTotalcost=0;
	}

	lTempTargetValue=TargetValue();
	if(cCode==1)	// end map
	{
//		lTempTargetValue=TargetValue();
		if(lTempTargetValue<=lTargetValue)	// demolished
		{
			boolJobSuccess=false;
			boolJobDone=true;

			if(lTempTargetValue<lTargetValue)
			{
				dTax=lPayment*25*plr_advdecisions[0];
				dTax/=10000;
				dPayment=lPayment-dTax;
				plr_taxpaid+=long(dTax);

				dTemp=plr_money;
				dTemp+=dPayment;

				if(dTemp>ULONG_MAX)
					plr_money=ULONG_MAX;
				else
					plr_money+=long(dPayment);

				AddMessage("-------------------------",1);
				sprintf(msg,"%s $%u",gametxt[31],lPayment);	// payment
				AddMessage(msg,3);

				sprintf(msg,"%s $%.f",gametxt[19],dTax);	// taxes
				AddMessage(msg,iTextColor);

//				lCleanupTime=2+lTempTargetValue/(plr_crew[1]*15000+plr_crew[0]*400);
//				lTransportTime=2+lTempTargetValue/(plr_crew[2]*20000+plr_crew[1]*2000+plr_crew[0]*50);

				lCleanupTime=2+lTempTargetValue/(plr_crew[1]*20000+plr_crew[0]*500);
				if(plr_crew[0]*10<plr_crew[1]) lCleanupTime=2+lTempTargetValue/(plr_crew[1]*2000+plr_crew[0]*500);	// in 1.0.1

				lTransportTime=2+lTempTargetValue/(plr_crew[2]*25000+plr_crew[1]*3000+plr_crew[0]*70);
				if(plr_crew[0]*10<plr_crew[2]) lTransportTime=2+lTempTargetValue/(plr_crew[2]*2500+plr_crew[1]*2000+plr_crew[0]*70);	// in 1.0.1

				dTemp=lTargetValue;

				if(lTempTargetValue>0)
					dTemp/=lTempTargetValue;
				else
					dTemp=10;

				if(dTemp<2) plr_reputation++;
				if(dTemp>=2 && dTemp<5) plr_reputation+=3;
				if(dTemp>=5) plr_reputation+=5;

				if(plr_reputation>10000) plr_reputation=10000;

				sprintf(msg,"%s $%u",gametxt[15],lAllExplosiveCosts);	// expl costs
				AddMessage(msg,iTextColor);

				dTemp=lCleanupTime;
				dTemp/=12;
				sprintf(msg,"%s %uh (%.1fd)",gametxt[20],lCleanupTime,dTemp);	// cleanup
				AddMessage(msg,iTextColor);
				dTemp=lTransportTime;
				dTemp/=12;
				sprintf(msg,"%s %uh (%.1fd)",gametxt[21],lTransportTime,dTemp);	// transport
				AddMessage(msg,iTextColor);

				if(lCleanupTime>lTransportTime)
					lMaxTime=lCleanupTime;
					else
					lMaxTime=lTransportTime;

				for(ix=0;ix<3;ix++)
				{
					lAllCosts+=lMaxTime*(plr_crew[ix]*crw_cost[ix]*city_price[plr_currentcity]/100)/12;		 // they're doing 12-hour days
				}
				lAllCosts+=2*lMaxTime*(plr_crew[3]*crw_cost[3]*city_price[plr_currentcity]/100)/12;			// except guards

				sprintf(msg,"%s $%u",gametxt[22],lAllCosts);	// cleanup costs
				AddMessage(msg,iTextColor);
			}		// ltemptargetvalue<ltargetvalue

			if(lTempTargetValue==lTargetValue)		// cancel job
			{
				dTemp=lPayment;
				dTemp*=city_price[plr_currentcity];
				dTemp/=1000;
				lAllCosts+=long(dTemp);
				sprintf(msg,"%s $%u",gametxt[23],long(dTemp));	// cancel job fine
				AddMessage(msg,2);
				lMaxTime=1;
			}

			// damage counting begings
			lTempOtherValue=OtherValue();
			dDamage=(lOtherValue-lTempOtherValue)*city_insurance[plr_currentcity];
			dDamage*=(110-plr_advdecisions[2]);
			dDamage/=10000;
			lDamage=long(dDamage);
			lOtherValue=lTempOtherValue;

			if(lDamage>0)
			{
				lDamageInflicted=lDamage;
				lAllCosts+=lDamage;
				sprintf(msg,"%s $%u",gametxt[16],lDamage);	// damage inflicted
				AddMessage(msg,2);

				if(lDamage>city_price[plr_currentcity]*10) plr_reputation-=3;
				if(lDamage>=city_price[plr_currentcity]*20) plr_reputation-=10;
				if(lDamage>=city_price[plr_currentcity]*50) plr_reputation-=20;
				if(lDamage>=city_price[plr_currentcity]*80) plr_reputation-=30;

				if(plr_reputation<-1000) plr_reputation=-1000;
			}

//			lAllCosts+=lTotalcost;		// add explosion costs
			lAllCosts+=lExtraCosts;		// possible extra costs

			dPayment-=lAllCosts;
			dPayment-=lAllExplosiveCosts;

			if(dPayment>0)
			{
				sprintf(msg,"%s $%.f",gametxt[17],dPayment);	// profit
				AddMessage(msg,3);
			}
			else
			{
				sprintf(msg,"%s $%.f",gametxt[18],dPayment);	// loss
				AddMessage(msg,2);
			}
			AddMessage("-------------------------",1);

			dDays=lMaxTime;
			dDays/=12;
			AddDays(long(dDays));

			if(lTempTargetValue<lTargetValue) plr_jobsdone++;
		}
	}

if(cCode==1)		// when map ends, check conditions for endgame
{
	CheckIfGameEnds(lAllCosts,lDamage,cCode);
}

}

bool IsTarget(short bx, short by, short iCounted)
{
	bool b;
	b=false;
	if(MapBlock(bx,by)>0)
		if(mapcounted[bx][by]==iCounted) b=true;

//	if(iCounted==194) printf("%u %d %d %d\n",lTimer,bx,by,b);
	return b;
}

double CountSupport(short bx, short by, short iCounted)
{
	short ix,idist, icont,tx,ty;	//iy,
	double dSum;

	idist=1;

	do
	{
	icont=0;
	for(ix=0;ix<=idist*2;ix++)
	{
		tx=bx-idist+ix;
		ty=by-idist+1;
		if(MapBlock(tx,ty-1)>0 && mapcounted[tx][ty]==iCounted) {icont=1;mapcounted[tx][ty-1]=iCounted;dSum+=mapsupport[tx][ty-1];}

		tx=bx+idist-1;
		ty=by-idist+ix;
		if(MapBlock(tx+1,ty)>0 && mapcounted[tx][ty]==iCounted) {icont=1;mapcounted[tx+1][ty]=iCounted;dSum+=mapsupport[tx+1][ty];}

//		if(IsTarget(bx+idist-1,by-idist+ix,iCounted)==true) icont=1;

		tx=bx-idist+ix;
		ty=by+idist-1;
		if(MapBlock(tx,ty+1)>0 && mapcounted[tx][ty]==iCounted) {icont=1;mapcounted[tx][ty+1]=iCounted;dSum+=mapsupport[tx][ty+1];}

//		if(IsTarget(bx-idist+ix,by+idist-1,iCounted)==true) icont=1;

		tx=bx-idist+1;
		ty=by-idist+ix;
		if(MapBlock(tx-1,ty)>0 && mapcounted[tx][ty]==iCounted) {icont=1;mapcounted[tx-1][ty]=iCounted;dSum+=mapsupport[tx-1][ty];}

//		if(IsTarget(bx-idist+1,by-idist+ix,iCounted)==true) icont=1;
	}

	if(icont>0)
		{
//		mapcounted[bx][by]=iCounted;
//		dSum+=mapsupport[bx][by];
		idist++;
		}
	} while(icont>0);

	return dSum;
}

double CountSupportRight(short iCounted)
{
	short ix,iy,iOk;
	double dSum=0,dTemp;

	for(ix=1;ix<MAPSIZE-1;ix++)
	for(iy=1;iy<MAPSIZE-1;iy++)		//max_mapy[ix]
	if(mapcounted[ix][iy]==iCounted && mapend[ix][iy]==block_end[map[ix][iy]] && mapv[ix][iy]==0)
	{
		iOk=0;
		dTemp=0;

		if(mapsupport[ix+1][iy]>0 && mapv[ix+1][iy]==0 && mapend[ix+1][iy]==block_end[map[ix+1][iy]]) {iOk=1;dTemp=mapsupport[ix+1][iy];}
		if(mapsupport[ix-1][iy]>0 && mapv[ix-1][iy]==0 && mapend[ix-1][iy]==block_end[map[ix-1][iy]]) {iOk=1;dTemp=mapsupport[ix-1][iy];}
		if(mapsupport[ix][iy+1]>0 && mapv[ix][iy+1]==0 && mapend[ix][iy+1]==block_end[map[ix][iy+1]]) {iOk=1;dTemp=mapsupport[ix][iy+1];}
		if(mapsupport[ix][iy-1]>0 && mapv[ix][iy-1]==0 && mapend[ix][iy-1]==block_end[map[ix][iy-1]]) {iOk=1;dTemp=mapsupport[ix][iy-1];}

		if(mapv[ix][iy+1]==0 && mapend[ix][iy+1]==block_end[map[ix][iy+1]])	// version 1.2
		{
		if(MapBlock(ix,iy+1)>=40 && MapBlock(ix,iy+1)<=84) iGroundSupport++;
		if(MapBlock(ix,iy+1)==17 || MapBlock(ix,iy+1)==2) iSteelSupport++;
		}

		if(iOk==1)
		{
			dSum+=dTemp; //mapsupport[ix][iy];
		}
	}

return dSum;
}

double CountSupportLeft(short iCounted)
{
	short ix,iy,iOk;
	double dSum=0,dTemp;

//	for(ix=MAPSIZE-1;ix>1;ix--)
	for(ix=1;ix<MAPSIZE-1;ix++)
	for(iy=MAPSIZE-1;iy>1;iy--)	//max_mapy[ix]
	if(mapcounted[ix][iy]==iCounted && mapend[ix][iy]==block_end[map[ix][iy]] && mapv[ix][iy]==0)
	{
		iOk=0;
		dTemp=0;

		if(mapsupport[ix+1][iy]>0 && mapv[ix+1][iy]==0 && mapend[ix+1][iy]==block_end[map[ix+1][iy]]) {iOk=1;dTemp=mapsupport[ix+1][iy];}
		if(mapsupport[ix-1][iy]>0 && mapv[ix-1][iy]==0 && mapend[ix-1][iy]==block_end[map[ix-1][iy]]) {iOk=1;dTemp=mapsupport[ix-1][iy];}
		if(mapsupport[ix][iy+1]>0 && mapv[ix][iy+1]==0 && mapend[ix][iy+1]==block_end[map[ix][iy+1]]) {iOk=1;dTemp=mapsupport[ix][iy+1];}
		if(mapsupport[ix][iy-1]>0 && mapv[ix][iy-1]==0 && mapend[ix][iy-1]==block_end[map[ix][iy-1]]) {iOk=1;dTemp=mapsupport[ix][iy-1];}

		if(mapv[ix][iy+1]==0 && mapend[ix][iy+1]==block_end[map[ix][iy+1]])	// version 1.2
		{
		if(MapBlock(ix,iy+1)>=40 && MapBlock(ix,iy+1)<=84) iGroundSupport++;
		if(MapBlock(ix,iy+1)==17 || MapBlock(ix,iy+1)==2) iSteelSupport++;
		}

		if(iOk==1)
		{
			if(map[ix][iy]>0) dSum+=dTemp; //mapsupport[ix][iy];
		}
	}

return dSum;
}

double AttachSupport(short bx,short by, short iCounted)
{
	short ix2,iy2,iDone,iOk,iTimes=0;
	double dSum=0;

	iDone=0;

	do
	{
		iDone=0;

		for(ix2=1;ix2<MAPSIZE-1;ix2++)
		for(iy2=1;iy2<MAPSIZE-1;iy2++)

		if(mapcounted[ix2][iy2]==0 && mapsupport[ix2][iy2]<0)	//  && mapv[ix2][iy2]==0 // &&
		{
			iOk=0;

			if(ix2<MAPSIZE-2 && mapcounted[ix2+1][iy2]==iCounted && mapsupport[ix2+1][iy2]<0) iOk=1;	//  MapBlock(ix2+1,iy2)>0 && && mapv[ix2+1][iy2]==0
			if(ix2>2 && mapcounted[ix2-1][iy2]==iCounted && mapsupport[ix2-1][iy2]<0) iOk=1;// mapv[ix2-1][iy2]==0 &&
			if(iy2<MAPSIZE-2 && mapcounted[ix2][iy2+1]==iCounted && mapsupport[ix2][iy2+1]<0) iOk=1;// MapBlock(ix2,iy2+1)>0 &&mapv[ix2][iy2+1]==0 &&
			if(iy2>2 && mapcounted[ix2][iy2-1]==iCounted && mapsupport[ix2][iy2-1]<0) iOk=1;// MapBlock(ix2,iy2-1)>0 && mapv[ix2][iy2-1]==0 &&

			if(iOk==1)
			{
				iDone=1;

				if(MapBlock(ix2,iy2)==17 || MapBlock(ix2,iy2)==2)
					dSum+=(mapsupport[ix2][iy2]/5);
				else
					dSum+=mapsupport[ix2][iy2];

				mapcounted[ix2][iy2]=iCounted;
			}
		}

		if(iOk==0) iDone=2;

	} while(iDone==1);

	return dSum;
}


int DropSupport(short iCounted)
{
	short ix,iy;

	int iDrop=0;

		for(ix=1;ix<MAPSIZE-1;ix++)
		for(iy=1;iy<MAPSIZE-1;iy++)	//max_mapy[ix]
		if(mapcounted[ix][iy]==iCounted && mapv[ix][iy]==0)
		{
			mapv[ix][iy]=1;
			iDrop++;
		}

		return iDrop;
}

void MapCount()	// from version 1.0.3
{
	short ix,iy,iDone=1,iok; //iok2,ix2,iy2,iok_a;
	int iCount=1;
	double dSupport=0,dAdd,dWeight=0;
	long lDropped=0;
	char msg[30];

	for(ix=0;ix<MAPSIZE;ix++)
	for(iy=0;iy<MAPSIZE;iy++)
	{
		mapsupport[ix][iy]=0;
//		mapv[ix][iy]=0;
		mapcounted[ix][iy]=0;
	}

// phase 1
	for(ix=0;ix<MAPSIZE;ix++)
	{
		iy=1;

		while(iy<MAPSIZE)
		{
			if(MapBlock(ix,iy)>0)
			{
				if(MapBlock(ix,iy-1)==0)
				{
					while(MapBlock(ix,iy)>0 && iy<MAPSIZE)
					{
						dAdd=block_end[map[ix][iy]];

//						if(MapBlock(ix,iy)==17 || MapBlock(ix,iy)==1)
//							dAdd/=50;
//						else
							dAdd/=10;	//10

						if(dAdd<1) dAdd=1;

						mapsupport[ix][iy]=-int(dAdd);
						iy++;
					}
				}
				else
				{
					iok=0;

					if(MapBlock(ix-1,iy)==0 && MapBlock(ix+1,iy)==0) iok=50;	// less support for thin structures
					if(MapBlock(ix,iy)<=17 && mapend[ix][iy]<block_end[map[ix][iy]])
						if(MapBlock(ix-1,iy)==0 || MapBlock(ix+1,iy)==0 || MapBlock(ix-1,iy)>17 || MapBlock(ix+1,iy)>17) iok=50;	// ground doesn't support well either

					if(MapBlock(ix,iy)>=92 && MapBlock(ix,iy)<=113)
					{
						iok=100;
						if(MapBlock(ix-1,iy)>=92 && MapBlock(ix-1,iy)<=113) iok=0;
						if(MapBlock(ix+1,iy)>=92 && MapBlock(ix+1,iy)<=113) iok=0;
					}

					if(MapBlock(ix,iy)<=17 && mapend[ix][iy]<block_end[map[ix][iy]])
						if(MapBlock(ix-1,iy)>17 || MapBlock(ix+1,iy)>17) iok=50;	// ground doesn't support well either
						//MapBlock(ix-1,iy)==0 || MapBlock(ix+1,iy)==0 ||

					if(MapBlock(ix,iy)>=92 && MapBlock(ix,iy)<=113)	// trees etc.
					{
						iok=100;
						if(MapBlock(ix-1,iy)>=92 && MapBlock(ix-1,iy)<=113) iok=0;
						if(MapBlock(ix+1,iy)>=92 && MapBlock(ix+1,iy)<=113) iok=0;
					}

					if(iok>0)
						mapsupport[ix][iy]=block_end[map[ix][iy]]/iok;
					else
						mapsupport[ix][iy]=block_end[map[ix][iy]];
				}
			}
			iy++;
		}
	}

//AddMessage("Phase 1 done",1);
// phases 2 - 4

		iDone=0;

		for(ix=1;ix<MAPSIZE-1;ix++)
		for(iy=1;iy<MAPSIZE-1;iy++)
			if(mapcounted[ix][iy]==0 && mapsupport[ix][iy]<0) // && mapv[ix][iy]==0 && mapend[ix][iy]==block_end[map[ix][iy]])
			{
				iDone=1;
				iGroundSupport=0;
				iSteelSupport=0;
				mapcounted[ix][iy]=iCount;
				dWeight=mapsupport[ix][iy];
				dWeight+=AttachSupport(ix,iy,iCount);
//				sprintf(msg,"%d",iCount);
//				AddMessage(msg,1);
				dSupport=CountSupportRight(iCount)+CountSupportLeft(iCount);

				if(iGroundSupport<5 && iSteelSupport==0) dSupport/=(5-iGroundSupport); // version 1.2
//				sprintf(msg,"C%d S%.f W%.f G%u",iCount,dSupport,dWeight,iGroundSupport);
//				if(lTimer<3) AddMessage(msg,1);
				if(fabs(dWeight)>fabs(dSupport)) lDropped+=DropSupport(iCount);

				iCount++;
			}


if(lDropped>0)
{
	ed_bsaveok=false;
	sprintf(msg,"%s %u",gametxt[505],lDropped);
	AddMessage(msg,2);
}
}

unsigned int CharToInt(char * cText)
{
	unsigned long i;
	char * stop;

	i=strtol(cText,&stop,10);
	return (i);
}

void LastActionTime()
{
struct _timeb tstruct;
time_t ltime;

_ftime( &tstruct );
time(&ltime);

iLastActionMsec=tstruct.millitm;
dPrevActionTime=dLastActionTime;
dLastActionTime=ltime*1000+iLastActionMsec;

}

void GenerateRandSeed()
{
	cmouse=0;
	oldmcounter=0;
	srand( (unsigned)time( NULL ));
	for(crandvalue=0;crandvalue<100;crandvalue++)
	{
		randvalues[crandvalue]=rand()%10+1;
		oldm_x[crandvalue]=rand()%1000;
		oldm_y[crandvalue]=rand()%1000;
	}
}

short GetRandValue()
{
	crandvalue++;
	if(crandvalue>=100) crandvalue=0;
	return randvalues[crandvalue];
}

unsigned long LongRandom()	// 0-4294967295
{
	double ddifference=0;
	double dresult,dfold,dsqrt,dtime;
	char ctimes=0;
	short maxtimes;

	struct tm *newtime;
	time_t aclock;

//	printf("Longrandom begins\n");
	time( &aclock );
	newtime = localtime( &aclock );

	maxtimes=GetRandValue();

	if(dLastActionTime>dPrevActionTime)
	ddifference=dLastActionTime-dPrevActionTime;
//	printf("ddifference %f ",ddifference);

	dfold=1;
	dresult=ddifference;

	while(ctimes<=maxtimes)
	{
		if(oldm_x[cmouse]>0 && oldm_y[cmouse]>0)
		{
			if(cmouse%2==0)
			{
				dtime=newtime->tm_min;
				dtime*=newtime->tm_sec;
				dsqrt=sqrt(oldm_x[cmouse]+oldm_y[cmouse])*dtime;
			}
			else
			{
				dtime=newtime->tm_min+newtime->tm_sec;
				dsqrt=sqrt(oldm_x[cmouse]*oldm_y[cmouse])+dtime;
			}

			dresult+=unsigned long(dsqrt)%10*dfold;
			dfold*=10;
		}

		ctimes++;
		cmouse++;
		if(cmouse>=100) cmouse=0;
	}

	while(dresult>4294967295) dresult-=4294967295;

//	printf("dresult %f\n",dresult);
//	printf("Longrandom ends\n");
	return unsigned long(dresult);
}
