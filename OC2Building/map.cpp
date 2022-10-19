//#include "SDL.h"   /* All SDL App's need this */
#include <stdio.h>
#include <time.h>
#include <limits.h>
#include <math.h>
// #include <windows.h>

#include "OC2b.h"
// handles map calculations and functions

extern short			SIZE_Y,BLOCKSIZE;

extern char				b_name[BUILDINGS][255];
extern short			b_freq[BUILDINGS];		// frequency
extern unsigned long	lb_freq;

char					block_name[MAXBLOCKS][30];
short					block_penres[MAXBLOCKS];
short					block_end[MAXBLOCKS];
unsigned short			block_cost[MAXBLOCKS];

//extern char				gametxt[MAXGAMETXT][52];

int						mapbegin_x,mapbegin_y;
unsigned char			map[MAPSIZE][MAPSIZE];
short					mapend[MAPSIZE][MAPSIZE];
int						mapsupport[MAPSIZE][MAPSIZE];
unsigned char			mapv[MAPSIZE][MAPSIZE];	// velocity of block (downwards only)
short					mapvertv[MAPSIZE][MAPSIZE];	// velocity of block (vertical only)
short		 			mapcounted[MAPSIZE][MAPSIZE];
extern char				iDrawSmallMap;
bool					maptarget[MAPSIZE][MAPSIZE];
short					iTargetbuilding;

short					dp_x[MAXDUSTPARTICLES];	// dust particles
short					dp_y[MAXDUSTPARTICLES];
short					dp_v[MAXDUSTPARTICLES];	// velocity
double					dp_dir[MAXDUSTPARTICLES];

long					lTargetValue,lOtherValue,lPayment;

bool					blocksmoving,boolJobDone,boolJobSuccess;

unsigned int			randseed;
unsigned char			cSearchType;

extern short			p_buildbegin;		//p for parse
extern short			p_buildwidth;

//extern bool				boolMapgenerated;

//extern int				m_button,m_x,m_y,m_down_x,m_down_y,m_up_x,m_up_y,m_event,m_button_up;

//char					exp_selected=0;
//extern unsigned char	ec_selected;

extern long				lTimer,lLastDetonation,lPlacehours,lExtraCosts;
long					lLastSound=0;

extern bool				boolExplode;

bool					bAddDamper;

long					lFallenBlocks;

//short					miny[MAPSIZE];
//extern short			iPprogress,tmpmovex,tmpmovey;
extern char				customer_name[MAXCUSTOMERS][12];
short					customer_nr;

extern long				lDamageInflicted,lAllExplosiveCosts;
extern short			setDustClouds;

short					max_mapy[MAPSIZE];

long					lBlocksmoving;

extern short			tmpbuildbegin,tmpbuildwidth;

extern char				b_localename[BUILDINGS][52];

// OC2Ed begins
/*extern short			plr_gametype;

extern short			ed_currentcity;
extern char				ed_customer[52];
extern char				ed_building[52];*/
char						ed_message[10][52];

short						ed_currentcity;
char						ed_filename[52];
long						ed_mapfilenr;
char						ed_customer[52];
char						ed_building[52];

// OC2Ed ends

short PutMap(short mapx,short mapy,short material)
{
	if(mapx>=0 && mapx<MAPSIZE)
	{
		if(mapy>=0 && mapy<MAPSIZE)
		{
			map[mapx][mapy]=(unsigned char) material;
			mapv[mapx][mapy]=0;
			mapend[mapx][mapy]=block_end[map[mapx][mapy]];
			return 0;
		}
		else
		{
//			printf("Illegal map coordinates: %d %d %d\n",mapx,mapy,material);
			return -1;
		}
	}
	else
	{
//		printf("Illegal map coordinates: %d %d %d\n",mapx,mapy,material);
		return -1;
	}
}

short MapBlock(short mapx, short mapy)
{
	if(mapx>=0 && mapx<MAPSIZE)
	if(mapy>=0 && mapy<MAPSIZE)
		return map[mapx][mapy];

	return 0;
}

void Setmax_y()
{
	short ix,iy;

	for(ix=0;ix<MAPSIZE;ix++)
	{
		iy=MAPSIZE-1;
		while(map[ix][iy]==0 && iy>0) iy--;
		max_mapy[ix]=iy+1;
	}

}

void DefineTargetArea()
{
	short ix,iy,ybegin=0,miny=MAPSIZE;
	bool ok;
	char msg[30];

	lPayment=0;

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
//			lPayment+=(block_cost[map[p_buildbegin+ix][iy]]*city_payment[plr_currentcity]);	// payment
			if(iy<miny) miny=iy;
		}
	}

	for(ix=0;ix<p_buildwidth;ix++)	// search for target pipe, another target in buildpipe routine
	for(iy=miny-1;iy<MAPSIZE;iy++)
		if(map[ix+p_buildbegin][iy]==40) maptarget[ix+p_buildbegin][iy]=true;


	lPayment/=1000;
	sprintf(msg,"P %u",lPayment);
//	AddMessage(msg,1);
	if(lPayment<=50000) lPayment/=8;
	if(lPayment>50000 && lPayment<=100000) lPayment/=10;
	if(lPayment>100000 && lPayment<=200000) lPayment/=12;
	if(lPayment>200000 && lPayment<=300000) lPayment/=14;
	if(lPayment>300000)					// in 1.0.1, prev. /=8
			lPayment/=16;

	if(lPayment<500) lPayment+=500;



//	mapbegin_x=p_buildbegin-2;
//	mapdestx=mapbegin_x;
//	mapbegin_y=ybegin-(SIZE_Y/BLOCKSIZE)+2;
//	mapdesty=mapbegin_y;
//	MoveMap();

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
				lbuildingvalue=mapend[ix][iy]*block_cost[map[ix][iy]]/block_end[map[ix][iy]];
			else
				lbuildingvalue=0;

			lbuilding+=lbuildingvalue;
		}
	}

	return lbuilding;
}

void ReduceEnd(short ix, short iy, short iReduce)
{
	if(mapend[ix][iy]>iReduce)
		mapend[ix][iy]-=iReduce;
	else
	{
		map[ix][iy]=0;
		mapend[ix][iy]=0;
	}
}



void BeginRandomization()
{
	int ix,iy;

	for(ix=0;ix<MAPSIZE;ix++)
	for(iy=0;iy<MAPSIZE;iy++)
		map[ix][iy]=0;

//	randseed=174;
	srand(randseed);		// unsigned int values 0-4294967295

}
/*
bool CheckGround(short iBlock1)
{
	bool bOk=false;

	if(iBlock1==60 || iBlock1==71 || iBlock1==81 || iBlock1==88 || iBlock1==92)	// ground
		bOk=true;

	if(iBlock1>=64 && iBlock1<=67) bOk=true;

	return bOk;
}

void SmoothGround()
{
	short ix=0,iy,ibeginx=0,ibeginy;
	double ddiff,dydiff;

	do
	{
		iy=MAPSIZE-1;
		while(MapBlock(ix,iy)==0) iy--;

		if(CheckGround(MapBlock(ix,iy))==true && ibeginx==0)
		{
			ibeginy=iy;
			ibeginx=ix;
			do {ix++;} while(CheckGround(MapBlock(ix,iy))==true && MapBlock(ix,iy+1)==0);
			iy=MAPSIZE;
			ix--;
			while(CheckGround(MapBlock(ix,iy))==true) iy--;

			ddiff=abs(ibeginx-ix);
			dydiff=abs(ibeginy-iy);

//			printf("ddiff %f %f\n",ddiff,dydiff);

			if(ddiff>0 && dydiff>0)
			{
			ddiff/=dydiff;
			if(ddiff<1) ddiff=1;

			do
			{
				ibeginx+=short (ddiff);

				if(ibeginy<iy)	// up
				{
					ibeginy++;
					PutMap(ibeginx,ibeginy,70);
				}
				else		// down
				{
					ibeginy--;
					PutMap(ibeginx,ibeginy,72);
				}
			} while(ibeginx<ix);

			ibeginx=0;
			}
		}

		ix++;
	} while(ix<MAPSIZE);
}
*/
void GenerateGround()
{
	int iLength,ix,iy,iMapBumpiness,iChange,iground,icounter,iSurface,iRand,iBushGap,iBlocks,mapx,mapy,icounter2,iTries,iblock,isteepness;
	int iGrounds[4]={30,150,150,300};
	int iGroundsRand[4];
	int iGroundBlock[4]={64,65,66,67};
//	int iTries;
	double dBlockx,dBlocky;

	for(ix=0;ix<MAPSIZE;ix++)
	for(iy=0;iy<MAPSIZE;iy++) map[ix][iy]=0;


//	for(ix=0;ix<MAPSIZE;ix++)
//		for(iy=200;iy<MAPSIZE;iy++) map[ix][iy]=71;


	iMapBumpiness=rand()%15+1;

	iy=rand()%20+30;
	ix=0;

	while(ix<MAPSIZE)
	{

		iBushGap=0;
		iRand=rand()%100;
		if(iRand>50) iBushGap=rand()%10;

		iRand=rand()%150;

		iSurface=60;
		if(iRand>=50 && iRand<55) iSurface=65;
		if(iRand>=55 && iRand<60) iSurface=66;
		if(iRand>=60 && iRand<70) iSurface=67;
		if(iRand>=70 && iRand<85) iSurface=71;
		if(iRand>=85) iSurface=88;

		do
		{
		iLength=rand() % 50-iMapBumpiness;
		} while(iLength<3);

		while(iLength>0 && ix<MAPSIZE)
		{
		iLength=iLength--;
		map[ix][iy]=iSurface;
		if(iBushGap>0 && ix%iBushGap==0 && iSurface==88)
			{
			map[ix][iy]=92;
			map[ix][iy+1]=rand()%3+107;
			}
		ix++;
		}

		iTries=0;
		do
		{
		iChange=(rand()%iMapBumpiness)-iMapBumpiness/2+iy;
//		printf("%d %d iChange %d \n",randseed,iy,iChange);
		iTries++;
		if(iTries>10) iChange=10;
		} while(iChange<10);

		while(iy!=iChange && ix<MAPSIZE)
		{
			isteepness=rand()%100;

			if(iy>iChange)
			{
				iblock=72;						// soil
				if(iSurface==60) iblock=61;	// asphalt
				if(iSurface==81) iblock=82;	// sand
				if(iSurface==88) iblock=89;	// grass

				if(isteepness<30)
					PutMap(ix,iy,iblock);
				else		// less steep
				{
					PutMap(ix,iy,iblock+1);
					ix++;
					PutMap(ix,iy,iblock+2);
				}
				iy--;
				ix++;
			}

			if(iy<iChange)
			{
				iblock=70;
				if(iSurface==60) iblock=59;
				if(iSurface==81) iblock=80;
				if(iSurface==88) iblock=87;
				if(isteepness<30)
					PutMap(ix,iy+1,iblock);
				else		// less steep
				{
					PutMap(ix,iy+1,iblock-2);
					ix++;
					PutMap(ix,iy+1,iblock-1);
				}

				iy++;
				ix++;
			}

		}
	}


	for(ix=0;ix<MAPSIZE;ix++)
	{
		iy=0;

		while(map[ix][iy]==0)
		{
			map[ix][iy]=71;
			iy++;
		}
	}

	for(iground=0;iground<3;iground++)
	{
		icounter =0;
		iGroundsRand[iground]=rand()%iGrounds[iground];

		while(icounter<iGroundsRand[iground])
		{
			do
			{
				ix=rand()%MAPSIZE;
				iy=rand()%MAPSIZE;
			} while(map[ix][iy]!=71);

			map[ix][iy]=iGroundBlock[iground];
			icounter++;

			iBlocks=rand()%80+70;
			dBlockx=0;
			dBlocky=0;

			for(icounter2=0;icounter2<iBlocks;icounter2+=2)
			{
				dBlockx+=0.05; //rand()%10/100;
				dBlocky+=0.05; //rand()%10/100;
				mapx=ix+int(dBlockx*cos(icounter2));
				mapy=iy+int(dBlocky*sin(icounter2));

				if(mapx>0 && mapx<MAPSIZE && mapy>0 && mapy<MAPSIZE)
				if(map[mapx][mapy]==71 || map[mapx][mapy]==67 || iground==0)
					map[mapx][mapy]=iGroundBlock[iground];
			}
		}

	}

/*	for(ix=0;ix<MAPSIZE;ix++)
//		if(map[ix][1]>0) printf("%d ERROR\n",randseed);
//*/
}

short LeafTree(int month)
{
int ix,iy,size_x,size_y,iCounter,iObstacles,iGroundy,iBlock,iBranch,iTop,mapx,mapy,iEmpty,iLeaves,iLeavebegin;
double dLeaves;
char ok;

	month++;
	iLeavebegin=98;
	if(month<4 || month>11) iLeavebegin=104;	// winter
	if(month>9 && month<=11) iLeavebegin=101;	// autumn

	size_x=rand()%10+3;
	ix=rand()%(MAPSIZE-size_x-8)+size_x/2+4;
	size_y=rand()%20+5;

	iGroundy=MAPSIZE;
	ok=0;

	do
	{
	iGroundy--;
	if(map[ix+size_x/2][iGroundy]>=64 && map[ix+size_x/2][iGroundy]<=67) ok=1;
	if(map[ix+size_x/2][iGroundy]==71) ok=1;
	if(map[ix+size_x/2][iGroundy]==81) ok=1;
	if(map[ix+size_x/2][iGroundy]==88) ok=1;
	if(map[ix+size_x/2][iGroundy]==92) ok=2;
	if(map[ix+size_x/2][iGroundy]>=57 && map[ix+size_x/2][iGroundy]<=63) ok=2;
	} while(map[ix+size_x/2][iGroundy]==0);

	iObstacles=0;
	for(iCounter=0;iCounter<size_x;iCounter++)
	for(iy=0;iy<size_y;iy++)
	{
		if(map[ix+iCounter][iGroundy+iy]>0 && map[ix+iCounter][iGroundy+iy]<56) iObstacles++;
		if(map[ix+iCounter][iGroundy+iy]>=93) iObstacles++;
	}

	if(iObstacles==0 && ok==1)	// let's make a tree
	{
		map[ix+size_x/2][iGroundy]=92;

		for(iy=1;iy<size_y*2/3;iy++)
		{
		iBlock=93;
		if(iy>size_y/3) iBlock+=rand()%4;
		map[ix+size_x/2][iGroundy+iy]=iBlock;
		iBranch=rand()%size_x/2+1;

		switch(iBlock)
			{
			case 94:
				for(iCounter=1;iCounter<iBranch;iCounter++) PutMap(ix+size_x/2-iCounter,iGroundy+iy,rand()%3+iLeavebegin);
				break;
			case 95:
				for(iCounter=1;iCounter<iBranch;iCounter++) PutMap(ix+size_x/2+iCounter,iGroundy+iy,rand()%3+iLeavebegin);
				break;
			case 96:
				for(iCounter=1;iCounter<iBranch;iCounter++)
				{
						PutMap(ix+size_x/2-iCounter,iGroundy+iy,rand()%3+iLeavebegin);
						PutMap(ix+size_x/2+iCounter,iGroundy+iy,rand()%3+iLeavebegin);
				}
			default:break;
			}

		}

		PutMap(ix+size_x/2,iGroundy+iy,97);
		iTop=iGroundy+iy;

		dLeaves=0;
		iLeaves=int(pow(size_x,2));
		if(iLeaves<100) iLeaves=100;

		for(iy=0;iy<iLeaves;iy+=2)
		{
			dLeaves+=0.05;

			mapx=ix+size_x/2+int(dLeaves*cos(iy));
			mapy=iTop+int(dLeaves*sin(iy)-1);

//			if(mapx>0 && mapx<MAPSIZE-1 && mapy>=0 && mapy<MAPSIZE)
			if(map[mapx][mapy]==0) PutMap(mapx,mapy,rand()%3+iLeavebegin);

		}

	for(iCounter=0;iCounter<size_x;iCounter++)	// purge non-attached leaves
	for(iy=0;iy<size_y;iy++)
	{
		mapx=ix-size_x/2+iCounter;
		mapy=iGroundy-iy;
		iEmpty=0;
		if(map[mapx][mapy]>=98 && map[mapx][mapy]<=106)
		{
			if(map[mapx-1][mapy]==0) iEmpty++;
			if(map[mapx+1][mapy]==0) iEmpty++;
			if(map[mapx][mapy-1]==0) iEmpty++;
			if(map[mapx][mapy+1]==0) iEmpty++;
			if(iEmpty==4) PutMap(mapx,mapy,0);
		}
	}

	return 1;
	}
	else
		return 0;

}

short PineTree()
{
int ix,iy,size_x,size_y,iCounter,iObstacles,iGroundy,iBlock,iBranch,iTop,mapx,mapy,iEmpty,iLeaves;
double dLeaves;
char ok;

	size_x=rand()%10+3;
	ix=rand()%(MAPSIZE-size_x-4)+size_x/2+2;
	size_y=rand()%30+10;

	iGroundy=MAPSIZE;
	ok=0;

	do
	{
	iGroundy--;
	if(map[ix+size_x/2][iGroundy]>=64 && map[ix+size_x/2][iGroundy]<=67) ok=1;
	if(map[ix+size_x/2][iGroundy]==71) ok=1;
	if(map[ix+size_x/2][iGroundy]==81) ok=1;
	if(map[ix+size_x/2][iGroundy]==88) ok=1;
	if(map[ix+size_x/2][iGroundy]==92) ok=2;
	if(map[ix+size_x/2][iGroundy]>=57 && map[ix+size_x/2][iGroundy]<=63) ok=2;
	} while(map[ix+size_x/2][iGroundy]==0);

	iObstacles=0;
	for(iCounter=0;iCounter<size_x;iCounter++)
	for(iy=0;iy<size_y;iy++)
	{
		if(map[ix+iCounter][iGroundy+iy]>0 && map[ix+iCounter][iGroundy+iy]<56) iObstacles++;
		if(map[ix+iCounter][iGroundy+iy]>=93) iObstacles++;
	}

	if(iObstacles==0 && ok==1)	// let's make a pine
	{
		map[ix+size_x/2][iGroundy]=92;

		for(iy=1;iy<size_y*2/3;iy++)
		{
		iBlock=93;
		if(iy>size_y/3) iBlock+=rand()%4;
		PutMap(ix+size_x/2,iGroundy+iy,iBlock);
		iBranch=rand()%size_x/2+1;
		if(iBranch>3) iBranch=3;

		switch(iBlock)
			{
			case 94:
				for(iCounter=1;iCounter<iBranch;iCounter++) PutMap(ix+size_x/2-iCounter,iGroundy+iy,rand()%2+112);
				break;
			case 95:
				for(iCounter=1;iCounter<iBranch;iCounter++) PutMap(ix+size_x/2+iCounter,iGroundy+iy,rand()%2+112);
				break;
			case 96:
				for(iCounter=1;iCounter<iBranch;iCounter++)
				{
						PutMap(ix+size_x/2-iCounter,iGroundy+iy,rand()%2+112);
						PutMap(ix+size_x/2+iCounter,iGroundy+iy,rand()%2+112);
				}
			default:break;
			}

		}

		PutMap(ix+size_x/2,iGroundy+iy,97);
		iTop=iGroundy+iy;

		dLeaves=0;
		iLeaves=int(pow(size_x,2));
		if(iLeaves<100) iLeaves=100;

		for(iy=0;iy<iLeaves;iy+=2)
		{
			dLeaves+=0.05;

			mapx=ix+size_x/2+int(dLeaves*cos(iy));
			mapy=iTop+int(dLeaves*sin(iy)-1);

//			if(mapx>0 && mapx<MAPSIZE-1 && mapy>=0 && mapy<MAPSIZE)
			if(map[mapx][mapy]==0) PutMap(mapx,mapy,rand()%2+112);

		}

	for(iCounter=0;iCounter<size_x;iCounter++)	// purge non-attached leaves
	for(iy=0;iy<size_y;iy++)
	{
		mapx=ix-size_x/2+iCounter;
		mapy=iGroundy-iy;
		iEmpty=0;
		if(map[mapx][mapy]>=112 && map[mapx][mapy]<=113)
		{
			if(map[mapx-1][mapy]==0) iEmpty++;
			if(map[mapx+1][mapy]==0) iEmpty++;
			if(map[mapx][mapy-1]==0) iEmpty++;
			if(map[mapx][mapy+1]==0) iEmpty++;
			if(iEmpty==4) PutMap(mapx,mapy,0);
		}
	}

	return 1;
	}
	else
		return 0;
}

short ConiferTree()	// = havupuu
{
	short ix,iy,size_y,iGroundy,code;

	ix=rand()%(MAPSIZE-4)+2;
	size_y=rand()%5+3;

	iGroundy=MAPSIZE;
	code=0;

	do
	{
	iGroundy--;
	if(map[ix][iGroundy]>=64 && map[ix][iGroundy]<=67) code=1;
	if(map[ix][iGroundy]==71) code=1;
	if(map[ix][iGroundy]==81) code=1;
	if(map[ix][iGroundy]==88) code=1;
	} while(map[ix][iGroundy]==0);

	if(code==1)
	{
	PutMap(ix,iGroundy,92);
	for(iy=1;iy<size_y;iy++) PutMap(ix,iGroundy+iy,110);
	PutMap(ix,iGroundy+size_y,111);
	return 1;
	}
	else
		return 0;
}

void GenerateTrees()
{
	short r,iTimes,iMaxTrees,iTries=0;
	iMaxTrees=rand()%15+5;
	iTimes=0;

	struct tm *newtime;
	time_t aclock;

	time( &aclock );
	newtime = localtime( &aclock );

	do
	{

		r=rand()%100;

		if(r<50) iTimes+=LeafTree(newtime->tm_mon);
		if(r>=50 && r<80) iTimes+=ConiferTree();
		if(r>=80) iTimes+=PineTree();

		iTries++;
		if(iTries>10)
		{
			iTimes++;
			iTries=0;
		}
	} while(iTimes<iMaxTrees);

}

void SeedGenerator(char *cText)
{
	double			dResult;
	unsigned char	ch;
	bool			bNumber;
	int				ifold,i,max;
	unsigned char 	cTemp[50];

	dResult=0;
	bNumber=true;
	ifold=1;
	for(i=0;i<50;i++) cTemp[50]=0;
	i=0;

	while(*cText)
	{
		ch=*cText;
		dResult+=i*ifold*int(ch-32);
		ifold*=10;

		if(ch<48 || ch>57)
		{
			bNumber=false;
		}
		else
//			if(ch!=204)
			cTemp[i]=ch-48;

		cText++;
		i++;
	}

	if (bNumber==true)
	{
		dResult=0;
		i=0;

		randseed=int (cTemp);
		for(i=0;i<50;i++)
		if(cTemp[i]>=0 && cTemp[i]<=9) max=i;

		ifold=1;
		for(i=max;i>=0;i--)
		{
			dResult+=cTemp[i]*ifold;
			ifold*=10;
		}
	}

	while(dResult>UINT_MAX) dResult-=UINT_MAX;
	randseed = (unsigned int) dResult;

	GenerateMap();
//	MapNrMessage();
//	boolMapgenerated=true;
//	iDrawSmallMap=1;
}


//	lTargetValue=TargetValue();
//	lOtherValue=OtherValue();

/*
void SetTargetArea()
{
	short ix,iy,ybegin=0;
	long lblocks=0;
	bool ok;

//	printf("Target area: %d-%d\n",p_buildbegin,p_buildbegin+p_buildwidth);
//	lPayment=0;

	for(ix=0;ix<p_buildwidth;ix++)
	for(iy=0;iy<MAPSIZE;iy++)
	{
		ok=false;
		if(map[p_buildbegin+ix][iy]>0 && map[p_buildbegin+ix][iy]<40) ok=true;		// building blocks
		if(map[p_buildbegin+ix][iy]==54 || map[p_buildbegin+ix][iy]==55) ok=true;	// satellite dishes

		if(ok==true)
		{
			if(ybegin==0) ybegin=iy;
			maptarget[p_buildbegin+ix][iy]=true;
//			lPayment+=(block_cost[map[ix][iy]]*city_payment[plr_currentcity]);	// payment
			lblocks++;
		}
	}

//		printf("%d blocks in target\n",lblocks);
}
*/

void CountMapSum()
{
	long lbuilding=0;
	long lbtarget=0;
	short xc,yc;
//	char msg[30];
	for(xc=0;xc<MAPSIZE;xc++)
	for(yc=0;yc<MAPSIZE;yc++)
	{
		lbuilding+=map[xc][yc];
		if(maptarget[xc][yc]==true) lbtarget++;
	}

//	sprintf(msg,"Sum: %u (%u)",lbuilding,lbtarget);
//	AddMessage(msg,1);
}


void SaveMapNr()	// precaution for crash and stdout.txt doesn't get written
{
/*   stream = fopen("mapnr.txt","a");
   fprintf(stream,"%u\n",randseed);
   fclose(stream);
   */
}

short GenerateMap()			// changed in version 1.1
{
	short xc,yc,i,iTimes=0;
	bool bfirst,bOk;
	unsigned int lbuilding,lcount,lb,lb2,lc;
	char fn[300];
	short iret;

//	char msg[30];
	bOk=false;
	lb=0;
	lb2=0;

	for(i=0;i<BUILDINGS;i++)
	{
		lb+=b_freq[i];
		if(b_freq[i]>0) lb2++;
//		if(i<ed_buildsel) lb2+=b_freq[i];
	}


	while(iTimes<10)
	{

//printf("***********************************\nBegin %u\n",randseed);
	SaveMapNr();
//	SDL_Delay(100);

//	iPprogress=0;

//	boolJobDone=true;
//	ShowParseProgress("Begin");
//	ZeroMap(0);

//	ShowParseProgress("GenerateGround");
	GenerateGround();

/*	for(xc=0;xc<MAPSIZE;xc++)
	for(yc=0;yc<MAPSIZE;yc++)
		maptarget[xc][yc]=false;
*/
//	ShowParseProgress("LoadMapCode");
	yc=0;
	i=0;		// just a counter to prevent lock-up
	bfirst=true;

//	for(xc=0;xc<43;xc++)
//		printf("%s %u %u %s\n",b_localename[xc],xc,b_freq[xc],b_name[xc]);

	do
	{
	i++;

	if(bfirst==true)
		lbuilding=abs(int(randseed));
	else
		lbuilding=rand()%lb_freq;

//	sprintf(msg,"rs %u %u",randseed,lb_freq);
//	AddMessage(msg,1);


	//	printf("lbuilding: %d/%d\n",lbuilding,lb_freq);
/*	xc=1;
	while(lbuilding>0) // && b_freq[xc]>0)
	{
//		printf("lbuilding: %s %d/%d\n",b_localename[xc],lbuilding,b_freq[xc]);	// AVOID USING THIS --> big stdout file if big number
		lbuilding-=b_freq[xc];
		xc++;
		if(b_freq[xc]<=0) xc=0;
	}
	if(xc>0) xc--;
*/

	lcount=0;
	lc=0;
	xc=0;
	while(lcount<lbuilding)
	{
		lc++;
		lcount+=b_freq[xc];
		xc++;
		if(b_freq[xc]<=0) xc=0;
	}

	while(lc>lb2) lc-=lb2;
	xc=lc;
	if(xc>0) xc--;
/*
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
*/
//	if(bfirst==true) printf("SEL lbuilding: (%s) %d\n",b_name[xc],xc);

	sprintf(fn,"buildings\\%s.map",b_name[xc]);
//	printf("%s\n",fn);
	LoadMapCode(fn);
//	printf("%s [%d] %d\n",fn,b_freq[xc],xc);

//	yc=FindPlace(0,MAPSIZE);

	yc=ParseMapCode(bfirst);
	if(yc>0 && bfirst==true)
		{

//		SetTargetArea();
		DefineTargetArea();
		bfirst=false;
		iTargetbuilding=xc;
		sprintf(ed_building,"%s",b_localename[iTargetbuilding]);

		/*
		if(short(b_name[xc][1])>=48 && short(b_name[xc][1])<=57)		// numbers
			iTargetbuilding=(short(b_name[xc][0])-48)*10+short(b_name[xc][1])-48;
		else
			iTargetbuilding=short(b_name[xc][0]-48);
			*/
//		iTargetbuilding=xc;
		}
	} while(yc>0 && i<100); //FindPlace(0,MAPSIZE)==0);

//	ShowParseProgress("SmoothGround");
//	SmoothGround();

//	ShowParseProgress("GenerateTrees");
	GenerateTrees();

//	ShowParseProgress("AddExtras");
	AddExtras();
	BuildPipes();

	for(xc=0;xc<MAPSIZE;xc++)
	for(yc=0;yc<MAPSIZE;yc++)
	{
		mapend[xc][yc]=block_end[map[xc][yc]];
		mapv[xc][yc]=0;
//		mapvertv[xc][yc]=0;
	}

	lTargetValue=TargetValue();	// NEEDED!!!!
	lOtherValue=OtherValue();

	do
	{
	customer_nr=rand()%MAXCUSTOMERS;
	} while(customer_name[customer_nr][0]==0);

	srand( (unsigned)time( NULL ));

//	EmptyMessages();

	// check that there is target area, remove from final (?)
	bfirst=false;
	for(xc=0;xc<MAPSIZE;xc++)
	for(yc=0;yc<MAPSIZE;yc++)
	{
		if(maptarget[xc][yc]==true)
		{
			bfirst=true;
//			printf("randseed %u targetarea %u %u\n",randseed,xc,yc);
			bOk=true;
		}
	}

	for(xc=0;xc<MAPSIZE;xc++)	// no empty blocks on bottom
		if(map[xc][0]==0) PutMap(xc,0,71);

	if(bOk==false)
	{
		// FIX THIS, DO SOMETHING ABOUT IT WHEN NO TARGETBUILDING!!!!
//		printf("NO TARGETBUILDING!!!\n");
/*		AddMessage("*************************",2);
		AddMessage(gametxt[203],2);	//Error creating map.
		sprintf(fn,"%u",randseed);
		AddMessage(fn,2);
		AddMessage("*************************",2);
		iTimes++;
		randseed=LongRandom();		// random map
		*/
		iTimes++;
	}
	else
	{
		iret=0;
		iTimes=30000;
	}

	}

	if(iTimes>=10 && iTimes!=30000)
	{
		iret=1;
		printf("Error creating map %u\n",randseed);
//		sprintf(fn,"%s",gametxt[203]);	//Error creating map.
//		OKBox(fn,40);
	}
//	printf("End %u\n\n\n",randseed);

//	FindTargetBuilding();

/*	// Count the checksum of the map, REMOVE FROM FINAL
	lbuilding=0;
	for(xc=0;xc<MAPSIZE;xc++)
	for(yc=0;yc<MAPSIZE;yc++)
		lbuilding+=map[xc][yc];

	sprintf(fn,"Sum: %u",lbuilding);
	AddMessage(fn,1);*/
	CountMapSum();

	return iret;
}
