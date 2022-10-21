#include <limits.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
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

extern int				exp_needed[MAXEXPLOSIVETYPES];
extern int				acc_needed[MAXEXPLOSIVETYPES];

extern short			ec_x[MAXCHARGES];
extern short			ec_y[MAXCHARGES];
extern short			ec_dir[MAXCHARGES];
extern unsigned char	ec_type[MAXCHARGES];
extern short			ec_amount[MAXCHARGES];
extern long				ec_timer[MAXCHARGES];

extern short			p_buildbegin;
extern short			p_buildwidth;

extern short			tmpbuildbegin,tmpbuildwidth;
//extern int				iTargetx,iTargetendx;
//short					TargetAreaBegin,TargetAreaEnd;
extern long				lTargetValue,lOtherValue,lPayment;
extern bool				blocksmoving,boolJobDone,boolJobSuccess;

extern int				m_x,m_y,m_event;

short					oldm_x[100];
short					oldm_y[100];
short					oldm_event[100];
char					oldmcounter=0;
short					iLastActionMsec, iGroundSupport, iSteelSupport;
double					dLastActionTime=0,dPrevActionTime=0;
short					randvalues[100];
char					crandvalue=0;
char					cmouse=0;

extern long				plr_money,lTotalcost,lPlacehours,plr_taxpaid,lExtraCosts;
extern unsigned long	plr_jobsdone,plr_daysinbusiness;
extern short			plr_crew[4];

extern long				plr_currentcity,plr_careful,plr_reputation;
extern char				plr_advdecisions[4];
extern char				plr_usedmaps[MAXCAREERMAPS];

extern unsigned long	plrs_money[MAXPLAYERS][MAXSTATS];
extern unsigned long	plr_month;
extern short			plr_monthcounter;

extern double			dp_money, dp_payment, dp_cleanup, dp_dailycost, dp_extracost, dp_monthlycost, dp_movingcost, dp_damperend, dp_reputation, dp_neighbour;

extern short			sel_customer;

extern unsigned char	city_price[MAXCITIES];
extern unsigned char	city_insurance[MAXCITIES];
extern unsigned char	city_payment[MAXCITIES];

extern char				gametxt[MAXGAMETXT][52];

extern short			SIZE_X, SIZE_Y,BLOCKSIZE;
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

long CountDampers()
{
	short ix,iy;
	long ldamper=0;

	for(ix=0;ix<MAPSIZE;ix++)
	for(iy=0;iy<MAPSIZE;iy++)
		if(map[ix][iy]==56) ldamper++;

	lPlacehours+=ldamper/6;		// in 1.0.1
	acc_needed[3]=ldamper;
	return ldamper*acc_cost[3];
}


bool CountOverload()
{
	long ltemp=0;
	short ix,nta;
	double dc;
	char msg[200];

	nta=0;

	for(ix=0;ix<MAXCHARGES;ix++)
		if(ec_amount[ix]>0)
		{
			ltemp+=ec_amount[ix]*exp_velocity[ec_type[ix]];
			if(ec_x[ix]>0 && maptarget[ec_x[ix]/20][ec_y[ix]/20]==false)	// non-target area explosives
			{
//				sprintf(msg,"nta %u %u",ec_x[ix]/20,ec_y[ix]/20);
//				AddMessage(msg,1);
				nta++;
			}
		}

	dc=150+plr_careful;		// plr_careful -> 0-300
	dc/=5;
/*
	if(lTotalcost>plr_money)
	{
	sprintf(msg,"%s %s",gametxt[161],gametxt[162]);	//We don't have enough money for this detonation plan. Are you absolutely sure?
	YesNoBox(gametxt[152],msg,2);
	return true;
	}
*/
	if(ltemp>lPayment*(250-short(dc)))
	{
		sprintf(msg,"%s %s",gametxt[153],gametxt[154]);	//This is way too large amount of explosives for this site. Are you absolutely sure?
		YesNoBox(gametxt[152],msg,2);
		return true;
	}

	if(nta>0)
	{
		sprintf(msg,"%s %s",gametxt[164],gametxt[165]);	//Explosives set on non-target building! Do you want to proceed?
		YesNoBox(gametxt[152],msg,2);
		return true;
	}
	return false;
}

short CountNeeded(short icode)
{
	short ix=0,iy=0,ic=0,groundy=0,itemp=0,itimers=0, itempcrew=0;
	long lcost=0,lhours=0;

	lcost=0;
	lhours=0;
	itimers=0;
	lExpParticles=0;

	for(ix=0;ix<MAXEXPLOSIVETYPES;ix++) exp_needed[ix]=0;
	for(ix=0;ix<MAXACCESSORIES;ix++)	acc_needed[ix]=0;

	for(ic=0;ic<MAXCHARGES;ic++)
	if(ec_x[ic]>0)
	{
	groundy=0;

	lExpParticles+=200+ec_amount[ic]*50;

	ix=ec_x[ic]/20;
	for(iy=0;iy<MAPSIZE;iy++)
		{
		if(map[ix][iy]>=57 && map[ix][iy]<=92 && groundy==0)
			groundy=iy;
		}

	exp_needed[ec_type[ic]]+=ec_amount[ic];
	lhours++;
	if(ec_amount[ic]>10) (lhours)+=ec_amount[ic]/4;	// in 1.0.1, prev. +=2

	if(ec_dir[ic]>=0)
		{
		lcost+=(ec_amount[ic]*exp_cost[ec_type[ic]]);	// directed explosions are 4x price		// in 1.1 x1
		lhours++;
		itemp=ec_amount[ic]/5;
		lhours+=itemp/2;			// larger explosives take longer time	; in 1.1 /2
		if(ec_amount[ic]>20) lhours+=itemp/2;	// in 1.1 /2
		}

	if(ec_timer[ic]>0) itimers++;

	iy=ec_y[ic]/20;

	acc_needed[0]++;			// blast cap

	if(groundy>iy)
		{
		lcost+=(iy-groundy)*50;		// putting explosives underground is expensive
		lhours+=(iy-groundy);
		acc_needed[2]+=iy-groundy+2;		// fuse wire
		}
	else
		{

		do 	{
			if(mapend[ix][iy]<block_end[map[ix][iy]])
			{
				lcost+=20;	// damaged blocks bring more cost
				lhours++;
			}

			acc_needed[2]++;		// fuse wire
			iy--;

			if(iy%5==0) lhours++;

			} while(map[ix][iy]<57); // && iy>=MAPSIZE-2);

		}
	}

	lhours+=itimers/4;

	acc_needed[2]+=50;
	acc_needed[1]++;		// detonator
	lhours++;

	lTotalcost=lcost;
	for(ix=0;ix<MAXEXPLOSIVETYPES;ix++) lTotalcost+=(exp_needed[ix]*(exp_cost[ix]*city_price[plr_currentcity]/100));
	for(ix=0;ix<MAXACCESSORIES;ix++)	lTotalcost+=(acc_needed[ix]*(acc_cost[ix]*city_price[plr_currentcity]/100));

	itempcrew=(plr_crew[0]/2)+1;// in 1.0.1
	if(plr_crew[0]>10) itempcrew=10+(plr_crew[0]/4)+1;// in 1.0.1
	if(plr_crew[0]>50) itempcrew=50+(plr_crew[0]/5)+1;// in 1.0.1

	lPlacehours=(lhours/itempcrew);	// in 1.0.1, prev. =lhours
	lTotalcost+=CountDampers();
	if(lPlacehours<=0) lPlacehours=1;	// in 1.0.1

	lTotalcost+=(lhours*50);
	lTotalcost+=(lhours*(plr_crew[3]*crw_cost[3]*city_price[plr_currentcity]/100))/24;	// guards

	if(bTutorial==true && cTutorial==2 && exp_needed[1]>=35) MoveToNextTutorial(2);	// for tutorial

	if(lTotalcost>=plr_money)
	{
		if(icode==1) PlayWav("media\\beep.wav");
		AddMessage(gametxt[161],2);	//We don't have enough money
		AddMessage(gametxt[162],2);	//for this detonation plan.

		return 0;
	}
	else
		return 1;
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

	tmpbuildbegin=p_buildbegin;
	tmpbuildwidth=p_buildwidth;

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
			if(map[ix][iy]==17 || map[ix][iy]==2) lPayment+=block_cost[map[p_buildbegin+ix][iy]]/5;
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

	if(lPayment<1000) lPayment+=1000;
	lPayment*=long (dp_payment);	// for difficulty
	lPayment/=100;

//	sprintf(msg,"P %u",lPayment);
//	AddMessage(msg,1);

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
				lbuildingvalue=(mapend[ix][iy]*block_cost[map[ix][iy]]/block_end[map[ix][iy]])/2;	// in 1.1
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

		if(icode==1)	// only check end game if map ends
		{
		if(lAllCosts>0)	plr_usedmaps[sel_customer]=1;	// only set played ok if some costs

			if(CheckCareerCustomers()==true)	// victorious game over
			{
				PlayWav("media\\cheer.wav");
				GameOn=0;
				iMenulevel=12;
			}
			else
			{		// game continues normally

			if(lAllCosts>0)
				{
				ix=rand()%8+1;
				sprintf(msg,"media\\endjob%d.wav",ix);
				PlayWav(msg);
				}
			}
		}
	}

}

void StoreStats()
{
	short i,ip;
	long lMin,lMax,lDiff;
//	char msg[30];

	lDiff=0;
	lMin=4294967295;
	lMax=0;

	if(plr_monthcounter>=MAXSTATS-1)
		{
		for(ip=0;ip<MAXPLAYERS;ip++)
		{
			for(i=0;i<MAXSTATS-1;i++)
			{
				plrs_money[ip][i]=plrs_money[ip][i+1];
			}

			plrs_money[ip][MAXSTATS-1]=0;
		}

//		plrs_money[0][MAXSTATS-1]=plr_money;
	}
	else
		plr_monthcounter++;
	plr_month++;

	for(ip=0;ip<MAXPLAYERS;ip++)
	for(i=1;i<MAXSTATS-2;i++)
	if(plrs_money[ip][i]>0)
	{
		if(abs((long) (plrs_money[ip][i]-plrs_money[ip][i-1]))<lMin) {
			lMin = abs((long) (plrs_money[ip][i]-plrs_money[ip][i-1]));
		}
		if(abs((long) (plrs_money[ip][i]-plrs_money[ip][i-1]))>lMax) {
			lMax = abs((long) (plrs_money[ip][i]-plrs_money[ip][i-1]));
		}
	}

	if(lMin<lMax)
		lDiff=lMax-lMin;

	plrs_money[0][plr_monthcounter]=plr_money;

	if(lDiff<5000)
	{
		if(lMax>100000)
			lDiff=(rand()%120+20)*250;
		else
			lDiff=(rand()%70+10)*100;
	}

//	sprintf(msg,"ldiff %u %u ",lDiff,plr_monthcounter);
//	AddMessage(msg,1);
	for(i=1;i<MAXPLAYERS;i++)
		if(plrs_money[i][plr_monthcounter-1]>0)	// if 0, out of business!
		plrs_money[i][plr_monthcounter]=plrs_money[i][plr_monthcounter-1]+rand()%lDiff;
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
//		cost+=plr_advdecisions[3]*city_price[plr_currentcity];				// sabotage etc.

		sprintf(msg,"%s $%u",gametxt[14],cost);	// monthly cost
		AddMessage("*****",iTextColor);
		AddMessage(msg,iTextColor);

		if(plr_money>=cost)
		{
			plr_money-=cost;
		}
		else
		{
			plr_money=0;
		}

		sprintf(msg,"%s $%u",gametxt[37],plr_money);	// money
		AddMessage(msg,iTextColor);
		AddMessage("*****",iTextColor);
		StoreStats();
	}

	if(plr_advdecisions[3]>0) plr_advdecisions[3]--;
	plr_daysinbusiness++;
	lAddDays--;
	} while(lAddDays>0 && plr_money>0);

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
	double dPayment=0,dTax,dDamage,dDays,dTemp,dTemp2;

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

	dTemp2=lTargetValue;			// version 1.1.2
	dTemp2*=(100-dp_cleanup/2);
	dTemp2/=100;

	lTempTargetValue=TargetValue();

	if(cCode==1)	// end map (completed)
	{
//			sprintf(msg,"temp %u tv%u",lTempTargetValue,lTargetValue);
			//AddMessage(msg,1);
//		lTempTargetValue=TargetValue();
		if(lTempTargetValue<=long (dTemp2))	//lTargetValue)	// demolished
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

				if(plr_reputation>1000000/dp_reputation) plr_reputation=long (1000000/dp_reputation);

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

				lAllCosts*=long (dp_cleanup);	// for difficulty
				lAllCosts/=100;

				sprintf(msg,"%s $%u",gametxt[22],lAllCosts);	// cleanup costs
				AddMessage(msg,iTextColor);
			}		// ltemptargetvalue<ltargetvalue
		}

			else
//			if(lTempTargetValue==lTargetValue)		// cancel job
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

			dDamage*=dp_neighbour;		// for difficulty
			dDamage/=100;

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

				if(plr_reputation<-100000/dp_reputation) plr_reputation=long (-100000/dp_reputation);
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

			if(lTempTargetValue<lTargetValue) plr_jobsdone++;
		}

		CheckIfGameEnds(lAllCosts,lDamage,cCode);
		AddDays(long(dDays));
//	}

}

double AttachWeight(short bx,short by, short iCounted)
{
	short ix,iy,iDone,iOk,iTimes=0;
	double dSum=0;

	iDone=0;

	do
	{
		iDone=0;

		for(ix=1;ix<MAPSIZE-1;ix++)
		for(iy=1;iy<MAPSIZE-1;iy++)
		if(mapcounted[ix][iy]==0) // && map[ix][iy]>0) // && mapsupport[ix][iy]<0)	//  && mapv[ix][iy]==0 // &&
		{
			iOk=0;

			if(ix<MAPSIZE-2 && mapcounted[ix+1][iy]==iCounted && mapsupport[ix+1][iy]<0) iOk=1;	//  MapBlock(ix+1,iy)>0 && && mapv[ix+1][iy]==0
			if(ix>2 && mapcounted[ix-1][iy]==iCounted && mapsupport[ix-1][iy]<0) iOk=1;// mapv[ix-1][iy]==0 &&
			if(iy<MAPSIZE-2 && mapcounted[ix][iy+1]==iCounted && mapsupport[ix][iy+1]<0) iOk=1;// MapBlock(ix,iy+1)>0 &&mapv[ix][iy+1]==0 &&
			if(iy>2 && mapcounted[ix][iy-1]==iCounted && mapsupport[ix][iy-1]<0) iOk=1;// MapBlock(ix,iy-1)>0 && mapv[ix][iy-1]==0 &&

			if(iOk==1)
			{
				iDone=1;
				dSum+=mapsupport[ix][iy];
				mapcounted[ix][iy]=iCounted;
			}
		}

		if(iOk==0) iDone=2;

	} while(iDone==1);

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


bool IsTarget(short bx, short by, short iCounted)
{
	bool b;
	b=false;
	if(MapBlock(bx,by)>0)
		if(mapcounted[bx][by]==iCounted) b=true;

//	if(iCounted==194) printf("%u %d %d %d\n",lTimer,bx,by,b);
	return b;
}

void DropSupport(short iCounted)
{
	short ix,iy;

		for(ix=1;ix<MAPSIZE-1;ix++)
		for(iy=1;iy<MAPSIZE-1;iy++)	//max_mapy[ix]
		if(mapcounted[ix][iy]==iCounted && mapv[ix][iy]==0)
		{
			mapv[ix][iy]=1;
		}
}

void MapCount()	// version 1.2
{
	short ix,iy,iDone=1,iok; //iok2,ix2,iy2,iok_a;
	int iCount=1;
	double dSupport=0,dAdd,dWeight=0;
//	char msg[30];

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

						dAdd/=10;

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
//				sprintf(msg,"C%d S%.f W%.f G%u S%u",iCount,dSupport,dWeight,iGroundSupport, iSteelSupport);
//				if(lTimer<3) AddMessage(msg,1);
				if(fabs(dWeight)>fabs(dSupport)) DropSupport(iCount);

				iCount++;
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
	// XXX: This is just for seeding LongRandom() - which we can likely just replace with system
	//      rng func calls.
	struct timeval tv;

    gettimeofday(&tv,NULL);
    double milliseconds = (tv.tv_sec * 1000)+(tv.tv_usec/1000);
	double seconds = tv.tv_sec;

	iLastActionMsec=milliseconds;
	dPrevActionTime=dLastActionTime;
	dLastActionTime=seconds * 1000 + iLastActionMsec;
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

			dresult += (unsigned long)dsqrt % 10 * dfold;
			dfold*=10;
		}

		ctimes++;
		cmouse++;
		if(cmouse>=100) cmouse=0;
	}

	while(dresult>4294967295) dresult-=4294967295;

//	printf("dresult %f\n",dresult);
//	printf("Longrandom ends\n");
	return (unsigned long) dresult;
}
