#include "SDL/SDL.h"   /* All SDL App's need this */
#include <time.h>
#include <limits.h>
#include <math.h>
#include "oc2.h"

// handles map calculations and functions

extern short			SIZE_Y,BLOCKSIZE;
extern short			setSoundEffects;

extern char				b_name[BUILDINGS][255];
extern short			b_freq[BUILDINGS];		// frequency
extern unsigned long	lb_freq;

char					block_name[MAXBLOCKS][30];
short					block_penres[MAXBLOCKS];
short					block_end[MAXBLOCKS];
unsigned short			block_cost[MAXBLOCKS];

extern char				gametxt[MAXGAMETXT][52];

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
extern bool				boolMapgenerated;

extern int				m_button,m_x,m_y,m_down_x,m_down_y,m_up_x,m_up_y,m_event,m_button_up;

char					exp_selected=0;
extern unsigned char	ec_selected;

extern long				lTimer,lLastDetonation,lPlacehours,lExtraCosts;
long					lLastSound=0;

extern bool				boolExplode;

bool					bAddDamper;

long					lFallenBlocks;

//short					miny[MAPSIZE];
extern short			iPprogress,tmpmovex,tmpmovey;
extern short			customer_nr;

extern long				lDamageInflicted,lAllExplosiveCosts;
extern short			setDustClouds;

short					max_mapy[MAPSIZE];

long					lBlocksmoving;

// OC2Ed begins
extern short			plr_gametype;
extern char				b_localename[BUILDINGS][52];

extern short			ed_currentcity;
extern char				ed_customer[52];
extern char				ed_building[52];
extern char				ed_message[10][52];
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

void CrashSound(int block)
{
	short isound;

	if(setSoundEffects==1)
	{
		if(abs(lTimer-lLastSound)>8)
		{
		isound=0;

		if(block==18) isound=rand()%3+1;
		if(block>=33 && block<=39) isound=rand()%3+1;

		if(block>0 && block<=55)
			if(isound==0) isound=rand()%3+4;

		switch(isound)
			{
			case 1:PlayWav("media\\window1.wav");break;
			case 2:PlayWav("media\\window2.wav");break;
			case 3:PlayWav("media\\window3.wav");break;
			case 4:PlayWav("media\\crash1.wav");break;
			case 5:PlayWav("media\\crash2.wav");break;
			case 6:PlayWav("media\\crash3.wav");break;
			default:break;
			}

		if(isound>0)
		{
			lLastSound=lTimer;
			//printf("LTIMER %u LLASTSOUND %u\n",lTimer,lLastSound);
		}
		}

	}
}

void ReduceEnd(short ix, short iy, short iReduce)
{
	if(ix>=0 && ix<MAPSIZE && iy>=0 && iy<MAPSIZE)
	{
		if(mapend[ix][iy]>iReduce)
			mapend[ix][iy]-=iReduce;
		else
		{
			map[ix][iy]=0;
			mapend[ix][iy]=0;
		}
	}
}


void HandleBlocks()
{
	short hbcounted[MAPSIZE][MAPSIZE];
	short ix,iy,side,addx,addy,idst,iside;
//	short iymin[MAPSIZE];
	bool bdone=false;
	bool badd; //,bmove;
	double dTempdam;
	long ldustinterval=100,lStoppedblocks=0;	// 10
char msg[30];

//	if(lTimer%5)
	Setmax_y();

	lBlocksmoving=0;
	lFallenBlocks=0;

	for(ix=0;ix<MAPSIZE;ix++)
	for(iy=0;iy<MAPSIZE;iy++)
		hbcounted[ix][iy]=0;

	blocksmoving=false;

//	do
	{
		bdone=true;
		for(ix=1;ix<MAPSIZE-1;ix++)
			{
			for(iy=1;iy<MAPSIZE-1;iy++)		// this MUST be MAPSIZE, not max_mapy
			{
				if(map[ix][iy]>0 && hbcounted[ix][iy]==0 && mapv[ix][iy]>0)	//mapsupport[ix][iy]<0
				{

					bdone=false;
					hbcounted[ix][iy]=2;
//					bmove=false;

					if(map[ix][iy]>0 && mapv[ix][iy]>0) // || mapvertv[ix][iy]!=0)
/*					{
						bmove=true;
					}
//					if(mapv[ix][iy]>0 && mapend[ix][iy]<block_end[map[ix][iy]]) bmove=true;

					if(bmove==true)*/
						{
						lBlocksmoving++;

	//					sprintf(msg,"MOVING %u %u",ix,iy);
	//					AddMessage(msg,1);

						tmpmovex=ix;
						tmpmovey=iy;

						blocksmoving=true;
						if(mapv[ix][iy]<5)
						{
							mapv[ix][iy]++;
						}
						else
						{
//						if(lTimer>50) printf("%u X%d Y%d BLOCK %d V%u \n",lTimer,ix,iy,map[ix][iy],mapv[ix][iy]);

						addy=-1;
						addx=0;

/*						if(mapend[ix][iy]<block_end[map[ix][iy]] && mapvertv[ix][iy]==0)
						{
						if(map[ix+1][iy-1]==0) mapvertv[ix][iy]=1;
						if(map[ix-1][iy-1]==0) mapvertv[ix][iy]=-1;
						}
*/
						if(MapBlock(ix,iy-1)>0)
							{
							side=rand()%100;

							if(side%2==0)
								addx=-1;
							else
								addx=1;

							if(map[ix][iy-1]>=26 && map[ix][iy-1]<=28) addx=-1;	// roofs etc. cause sliding to direction
							if(map[ix][iy-1]>=30 && map[ix][iy-1]<=32) addx=1;

							if(map[ix][iy-1]>=33 && map[ix][iy-1]<=35) addx=-1;
							if(map[ix][iy-1]>=36 && map[ix][iy-1]<=38) addx=1;

							if(map[ix][iy-1]>=57 && map[ix][iy-1]<=59) addx=-1;
							if(map[ix][iy-1]>=61 && map[ix][iy-1]<=63) addx=1;

							if(map[ix][iy-1]>=68 && map[ix][iy-1]<=70) addx=-1;
							if(map[ix][iy-1]>=72 && map[ix][iy-1]<=74) addx=1;

							if(map[ix][iy-1]>=85 && map[ix][iy-1]<=87) addx=-1;
							if(map[ix][iy-1]>=89 && map[ix][iy-1]<=91) addx=1;

//							if(mapvertv[ix][iy]>0) addx=1;
//							if(mapvertv[ix][iy]<0) addx=-1;
						}

						dTempdam=abs(mapend[ix][iy]);
						dTempdam/=30;
						dTempdam+=mapv[ix][iy];

						if(MapBlock(ix+addx,iy+addy)>0) // || iy+addy<=0)
							{
							if(map[ix+addx][iy]>0)
							{
								if(mapend[ix][iy]>mapend[ix][iy+addy]*2)
									ReduceEnd(ix,iy+addy,mapend[ix][iy+addy]);
								else
									ReduceEnd(ix,iy+addy,short(dTempdam));

								if(mapend[ix][iy]>mapend[ix+addx][iy]*2)
									ReduceEnd(ix+addx,iy,mapend[ix+addx][iy]);
								else
									ReduceEnd(ix+addx,iy,short(dTempdam));
								ReduceEnd(ix,iy,rand()%10+1);
							}
							if(mapv[ix][iy]>0) mapv[ix][iy]--;

//							if(iy+addy<0) mapv[ix][iy]=0;
							lStoppedblocks++;
							}
						else
							{
							if(mapend[ix][iy]>mapend[ix+addx][iy+addy-1]*2)
								ReduceEnd(ix+addx,iy+addy-1,mapend[ix+addx][iy+addy-1]);
							else
								ReduceEnd(ix+addx,iy+addy-1,short(dTempdam));

							ReduceEnd(ix,iy,rand()%10+1);

							map[ix+addx][iy+addy]=map[ix][iy];
							mapv[ix+addx][iy+addy]=mapv[ix][iy];
//							mapvertv[ix+addx][iy+addy]=mapvertv[ix][iy];
							mapend[ix+addx][iy+addy]=mapend[ix][iy];
							mapsupport[ix+addx][iy+addy]=mapsupport[ix][iy];
							hbcounted[ix+addx][iy+addy]=hbcounted[ix][iy];
							maptarget[ix+addx][iy+addy]=maptarget[ix][iy];

							if(setDustClouds==1)
							if(map[ix][iy+1]==0 && block_end[map[ix][iy]]>300)
							{
								for(idst=0;idst<5;idst++)
								{
									iside=rand()%100;
									if(iside%2==0)
										AddDustParticle(ix*20+rand()%20,iy*20+rand()%20,rand()%90,250+rand()%50);
									else
										AddDustParticle(ix*20+rand()%20,iy*20+rand()%20,269+rand()%90,250+rand()%50);
								}
							}
							mapend[ix][iy]=0;
							}

							lFallenBlocks++;
							if(lFallenBlocks%ldustinterval==0 && block_end[map[ix+addx][iy+addy]]>300) AddDust(ix+addx,iy+addy);
						}	// if mapv


						if(lStoppedblocks>20 || lLastSound-lTimer>5) CrashSound(map[ix][iy]);
						}
						else
						{
							badd=false;
							if(mapsupport[ix][iy]<0 && mapv[ix][iy]<10) badd=true;	//mapcounted[ix][iy]==0 &&
							if(mapv[ix][iy]>0) badd=true;

							if(badd==true)
								{
								mapv[ix][iy]++;
								}
						}		// bmove ends

						if(map[ix][iy-1]>0 && mapv[ix][iy-1]==0)
						{
							idst=3;
//							mapsupport[ix][iy]=1;
							if(map[ix][iy-1]>=26 && map[ix][iy-1]<=28) idst=2;	// roofs etc. slow less
							if(map[ix][iy-1]>=30 && map[ix][iy-1]<=32) idst=2;

							if(map[ix][iy-1]>=33 && map[ix][iy-1]<=35) idst=2;
							if(map[ix][iy-1]>=36 && map[ix][iy-1]<=38) idst=2;

							if(map[ix][iy-1]>=57 && map[ix][iy-1]<=59) idst=2;
							if(map[ix][iy-1]>=61 && map[ix][iy-1]<=63) idst=2;

							if(map[ix][iy-1]>=68 && map[ix][iy-1]<=70) idst=2;
							if(map[ix][iy-1]>=72 && map[ix][iy-1]<=74) idst=2;

							if(map[ix][iy-1]>=85 && map[ix][iy-1]<=87) idst=2;
							if(map[ix][iy-1]>=89 && map[ix][iy-1]<=91) idst=2;

							if(mapend[ix][iy-1]<block_end[map[ix][iy]-1]) idst=2;

							if(mapv[ix][iy]>=idst)//3)
								mapv[ix][iy]-=idst; //=3;
							else
								mapv[ix][iy]=0;

						lFallenBlocks++;
						if(lFallenBlocks%ldustinterval==0 || lLastSound-lTimer>5)
							{
							AddDust(ix+addx,iy+addy);
							CrashSound(map[ix][iy]);
							}
						}	// if map


				if(mapend[ix][iy]<=0 && block_end[map[ix][iy]]>0)
					{
					if(addx>0)	// right
						for(idst=0;idst<10;idst++) AddDustParticle((ix+1)*20+10,iy*20+rand()%20,75+rand()%60,rand()%200+200);

					if(addx<0)		// left
						for(idst=0;idst<10;idst++) AddDustParticle((ix-1)*20,iy*20+rand()%20,285-rand()%60,rand()%200+200);

					map[ix][iy]=0;
					mapv[ix][iy]=0;
//					mapvertv[ix][iy]=0;
					mapend[ix][iy]=block_end[0];
					mapsupport[ix][iy]=0;
					hbcounted[ix][iy]=0;
					maptarget[ix][iy]=false;

					}
				}// if map
			}	// for iy
		}	// for ix
	} //while(bdone==false);

	if(blocksmoving==true)
	{
		if(abs(lLastSound-lTimer)>10 || lLastSound==0) CrashSound(20);
		for(ix=0;ix<MAPSIZE;ix++) {mapv[ix][0]=0;mapvertv[ix][1]=0;}
	}

//	for(ix=0;ix<MAPSIZE;ix++) mapv[ix][0]=0;

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
	randseed=(unsigned int) dResult;

//	GenerateMap(1);
//	MapNrMessage();
//	boolMapgenerated=true;
//	iDrawSmallMap=1;
}

void ZeroMap(short iCode)
{
	short xc,yc;

	boolJobSuccess=false;
	lTimer=0;
	lLastDetonation=-1;
	lFallenBlocks=0;
	lPlacehours=0;
	lDamageInflicted=0;
	lAllExplosiveCosts=0;
	boolExplode=false;

	ZeroExplosives();
	ZeroDust();
	if(iCode==1) RandomCity();		// for random game only
	BeginRandomization();
	CountNeeded(0);
	lExtraCosts=0;

	sprintf(ed_building,"");
	for(xc=0;xc<10;xc++) sprintf(ed_message[xc],"");
	ed_currentcity=0;
	sprintf(ed_customer,"");

	for(xc=0;xc<MAPSIZE;xc++)
	for(yc=0;yc<MAPSIZE;yc++)
	{
		map[xc][yc]=0;
		mapend[xc][yc]=block_end[0];
		mapcounted[xc][yc]=0;
		mapsupport[xc][yc]=0;
		mapv[xc][yc]=0;
		mapvertv[xc][yc]=0;
		maptarget[xc][yc]=false;
	}
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
	for(xc=0;xc<MAPSIZE;xc++)
	for(yc=0;yc<MAPSIZE;yc++)
	{
		lbuilding+=map[xc][yc];
		if(maptarget[xc][yc]==true) lbtarget++;
	}

//	char msg[30];
//	sprintf(msg,"Sum: %u (%u)",lbuilding,lbtarget);
//	AddMessage(msg,1);
}


void SaveMapNr()	// precaution for crash and stdout.txt doesn't get written
{
   stream = fopen("mapnr.txt","a");
   fprintf(stream,"%u\n",randseed);
   fclose(stream);
}

void GenerateMap(short iCode)			// icode 0=no message
// changed in version 1.2
{
//	char msg[30];

	ZeroMap(plr_gametype);
	ZeroExplosives();
//	SaveMapNr();

iPprogress=0;
ShowParseProgress(gametxt[506]);	// please wait

mainbuild(randseed);
ShowParseProgress(gametxt[506]);

LoadMap("",0);

lTargetValue=TargetValue();
lOtherValue=OtherValue();

iDrawSmallMap=1;

if(iCode!=0)
	{
	MapNrMessage();


//	sprintf(msg,"map %u",randseed);
//	AddMessage(msg,1);

/*	sprintf(msg,"T:%u O:%u",lTargetValue,lOtherValue);
	AddMessage(msg,1);
	sprintf(msg,"P %u",lPayment);
	AddMessage(msg,1);
	*/
	}

}
