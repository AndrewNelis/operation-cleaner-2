#include "SDL/SDL.h"   /* All SDL App's need this */

#include <math.h>
#include <stdlib.h>

#include "oc2.h"

extern unsigned char			city_price[MAXCITIES];

extern unsigned long			plr_daysinbusiness,plr_lastspecial;
extern short					plr_crew[4],plr_reputation;

extern char						plr_advdecisions[4];//{"Pay taxes","Accountant","Insurances","Sabotage competitors"};
extern long						plr_currentcity,plr_money,plr_taxpaid,lExtraCosts;

extern char						cOKBoxText[10][50];

extern char						gametxt[MAXGAMETXT][52];

short SpecialOccasions()
{
	short ix,idelay=0,ixbegin,irows=0,ix2;
	long lCostperday=0;
	double repay,dOthercost,dOtherIncome=0;
	char msg[30];
	bool bOkBoxShown=false;

	for(ix=0;ix<4;ix++)
	{
		lCostperday+=plr_crew[ix]*crw_cost[ix]*city_price[plr_currentcity]/100;
	}


	if(plr_daysinbusiness>plr_lastspecial+20+rand()%10)	//plr_daysinbusiness>0 &&
	{

		for(ix=0;ix<10;ix++) sprintf(cOKBoxText[ix],"");
		ix=rand()%10000-(plr_crew[3]/2);	// guards affect on some special occasions
//		ix=rand()%200+9800;	// for testing

		if(ix>9900 && ix<=9910)		// Your crew is on strike for one day
		{
			idelay=1;
			ixbegin=329;
			irows=4;
		}

		if(ix>9910 && ix<=9920)		// angry conservationists
		{
			idelay=rand()%10-plr_crew[3];
			if(idelay<1) idelay=1;
			ixbegin=300;
			irows=5;
		}

		if(ix>9920 && ix<=9930)		// tenants didn't leave
		{
			idelay=rand()%5-plr_crew[3];
			if(idelay<1) idelay=1;
			ixbegin=305;
			irows=3;
		}


		if(ix>9930 && ix<=9940)		// city wants to examine
		{
			idelay=rand()%2+1;
			ixbegin=308;
			irows=4;
		}

		if(ix>9940 && ix<=9950)		// minor fault
		{
			idelay=rand()%2+1;
			ixbegin=312;
			irows=3;
		}

		if(ix>9950 && ix<=9960)		// major fault
		{
			idelay=rand()%7+3;
			ixbegin=315;
			irows=4;
		}

		if(ix>9960 && ix<=9970)		// charges placed incorrectly
		{
			idelay=rand()%3+1;
			ixbegin=319;
			irows=3;
		}

		if(ix>9970 && ix<=9980)		// equipment stolen
		{
			idelay=rand()%2+1;

			ixbegin=322;
			irows=4;
		}

		if(ix>9990 && ix<=10000)	// hot air ballooner
		{
			idelay=rand()%1+1;

			ixbegin=326;
			irows=3;
		}

		if(idelay>0)
		{
		for(ix=0;ix<irows;ix++) sprintf(cOKBoxText[ix],"%s",gametxt[ix+ixbegin]);
		sprintf(cOKBoxText[6],"%s %u",gametxt[150],idelay); // days of delay
		sprintf(cOKBoxText[7],"%s $%u",gametxt[151],lCostperday*idelay); // cost of delay

		// repay (insurance)
		repay=plr_advdecisions[2];
		repay*=lCostperday*idelay;
		repay/=100;

		if(repay>lCostperday*idelay) repay=lCostperday*idelay;

		// insurance ends

		lExtraCosts+=lCostperday*idelay;
/*		if(plr_money>=lCostperday*idelay)
			plr_money-=lCostperday*idelay;
		else
			plr_money=0;
*/
//		plr_lastspecial=plr_daysinbusiness;
//		AddDays(idelay);

		if(repay>0)
			{
//			plr_money+=long(repay);
				lExtraCosts-=long(repay);
				sprintf(cOKBoxText[8],"%s%u",gametxt[333],long(repay));	// your insurance repays $
			}

//		if(plr_money>lExtraCosts)
		OKBox(gametxt[107],50+irows*15);	// delay in job
		bOkBoxShown=true;
		}


//		dOthercost=0;
		// taxes
		ix=rand()%100;
		dOthercost=plr_money*city_price[plr_currentcity]/500;

		if(irows==0 && ix>50 && long(dOthercost)-plr_taxpaid>1000)		// taxes
		{
		// if has not paid enough -> punishment
			dOthercost=plr_money*city_price[plr_currentcity];
			dOthercost/=400;
			dOthercost+=1000;
			dOthercost-=plr_taxpaid;

			if(dOthercost>10000)
				{
				if(plr_money>100000) dOthercost+=9000;
				if(plr_money>1000000) dOthercost+=199000;
				if(plr_money>10000000) dOthercost+=1999000;
				}

			plr_taxpaid+=long(dOthercost);

			sprintf(cOKBoxText[0],"%s",gametxt[335]);					//You haven't paid enough tax.
			sprintf(cOKBoxText[1],"%s",gametxt[336]);		//Additional taxes are
			sprintf(cOKBoxText[2],"$%u.",long(dOthercost));
//			AddMessage(cOKBoxText[1],2);
//			AddMessage(cOKBoxText[2],2);
			irows=5;
		}
		else
			dOthercost=0;
			//tax ends

		// accountant
		ix=rand()%90;
		ix2=rand()%100;
		if(irows==0 && dOthercost==0 && ix>plr_advdecisions[1] && ix2>50)
		{
		// if not good accountant -> punishment later

			dOthercost=(ix-plr_advdecisions[1])*50;
			dOthercost+=plr_money/100*(rand()%5+1);

			sprintf(cOKBoxText[0],"%s",gametxt[338]);		//Because of poor bookkeeping you have
			sprintf(cOKBoxText[1],"%s",gametxt[339]);	//to pay additional
			sprintf(cOKBoxText[2],"%s",gametxt[340]);	//Fixing the bookkeeping costs $
			sprintf(cOKBoxText[3],"$%u.",long(dOthercost));
//			AddMessage(cOKBoxText[2],2);
//			AddMessage(cOKBoxText[3],2);
			irows=6;
		}	// accountant ends


		// sabotage
		ix=rand()%110;
		if(irows==0 && dOthercost==0 && plr_daysinbusiness>60 && ix<plr_advdecisions[3]+10)
		{
		// if sabotages enemies -> possibly money (30 %), possibly punishment (70 %)

			if(plr_advdecisions[3]>17)
			{
				irows=5;
				ix=rand()%100;
				sprintf(cOKBoxText[0],"%s",gametxt[342]);		//You sabotaged your competitors:

				if(ix<30)
				{	// money
					dOtherIncome=(plr_advdecisions[3]*city_price[plr_currentcity])*(rand()%5+1);
					sprintf(cOKBoxText[1],"%s",gametxt[343]);	// The sabotage brought you
					sprintf(cOKBoxText[2],"$%u.",long(dOtherIncome));	// The sabotage brought you $
					AddMessage(cOKBoxText[1],3);
					AddMessage(cOKBoxText[2],3);
				}
				else
				{	// punishment
					dOthercost=1000+(plr_advdecisions[3]*city_price[plr_currentcity])*(rand()%3+1);
					sprintf(cOKBoxText[1],"%s",gametxt[344]);				//The sabotage failed and you get caught.
					sprintf(cOKBoxText[2],"%s",gametxt[345]);				//Pay punishment of $
					sprintf(cOKBoxText[3],"$%u.",long(dOthercost));
//					AddMessage(cOKBoxText[2],2);
//					AddMessage(cOKBoxText[3],2);
				}
			}
			else
			{
				ix=rand()%18;
				if(ix>plr_advdecisions[3])
				{
				idelay=1+rand()%(ix-plr_advdecisions[3]);
				sprintf(cOKBoxText[0],"%s",gametxt[346]);	// You didn't find out competitors' plans
				sprintf(cOKBoxText[1],"%s",gametxt[347]);	// and they got a valuable contract you
				sprintf(cOKBoxText[2],"%s",gametxt[348]);	// were also pursuing.
				sprintf(cOKBoxText[3],"%s %u",gametxt[150],idelay); // days of delay
				sprintf(cOKBoxText[4],"%s %u",gametxt[151],lCostperday*idelay); // days of delay

				lExtraCosts+=lCostperday*idelay;
/*				if(plr_money>=lCostperday*idelay)
					plr_money-=lCostperday*idelay;
				else
					plr_money=0;
*/
//				plr_lastspecial=plr_daysinbusiness;
//				AddDays(idelay);
				}

			}
		}

		if(idelay==0)
		if(dOthercost!=0 || dOtherIncome!=0)
		{
		lExtraCosts+=long(dOthercost);

/*		if(plr_money>=long(dOthercost))
			plr_money-=long(dOthercost);
		else
			plr_money=0;
*/
//		if(plr_money>lExtraCosts)
		OKBox("",50+irows*15);
		bOkBoxShown=true;
		plr_lastspecial=plr_daysinbusiness;
		if(dOtherIncome>0) plr_money+=(long) dOtherIncome;

		idelay=1;
		}

	}

	if(lExtraCosts>0 || idelay>0)
	{

		if(bOkBoxShown==true && lExtraCosts>0)
		{
		PlayWav("media\\siren.wav");
		sprintf(msg,"%s $%u",gametxt[157],lExtraCosts);		// extra cost
		AddMessage(msg,2);

		if(plr_money>=lExtraCosts)
			plr_money-=lExtraCosts;
		else
			plr_money=0;

		AddDays(idelay);
		plr_lastspecial=plr_daysinbusiness;
		}
		else
			idelay=0;
	}
	else
	{
	ix=rand()%100+5;

	if(ix<plr_reputation && plr_daysinbusiness>plr_lastspecial+50+rand()%10)
		{
		ix=rand()%50+1;
		ix*=100;
		plr_money+=ix;

		sprintf(cOKBoxText[0],"%s",gametxt[365]);	// You have received a reward of excellence in
		sprintf(cOKBoxText[1],"%s$%u.",gametxt[366],ix);	// demolition business. The reward is

		OKBox("",120);
		bOkBoxShown=true;

		sprintf(msg,"%s $%u",gametxt[367],ix);		//Reward of excellence:
		AddMessage(msg,3);
		plr_lastspecial=plr_daysinbusiness;
		idelay=1;
		}
	}

	lExtraCosts=0;
	return idelay;

}
