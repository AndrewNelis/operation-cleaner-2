#include "SDL/SDL.h"   /* All SDL App's need this */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
//#include <windows.h>
#include "oc2.h"

// handles almost all graphics in the game

extern long				lMapnr;

extern short			p_buildbegin;
extern short			p_buildwidth;

extern char				GameOn;
extern bool				boolMapgenerated;

extern char				block_name[MAXBLOCKS][30];
extern short			block_end[MAXBLOCKS];
extern unsigned short	block_cost[MAXBLOCKS];

extern int				mapbegin_x,mapbegin_y;
extern short			mapdestx,mapdesty;
extern unsigned char	map[MAPSIZE][MAPSIZE];
extern short			mapend[MAPSIZE][MAPSIZE];
extern unsigned char	mapv[MAPSIZE][MAPSIZE];
extern int				mapsupport[MAPSIZE][MAPSIZE];
extern bool				maptarget[MAPSIZE][MAPSIZE];
extern short			mapcounted[MAPSIZE][MAPSIZE];
//extern short			mapvertv[MAPSIZE][MAPSIZE];
extern short			iTargetbuilding;

extern short			SIZE_X,SIZE_Y,BLOCKSIZE,xmiddle,ymiddle;

extern short			ep_x[MAXEXPLOSIONPARTICLES];
extern short			ep_y[MAXEXPLOSIONPARTICLES];
extern short			ep_v[MAXEXPLOSIONPARTICLES];			// velocity
extern long				ep_e[MAXEXPLOSIONPARTICLES];

extern short			ec_counter;
extern unsigned char	ec_selected;

extern short			ec_x[MAXCHARGES];
extern short			ec_y[MAXCHARGES];
extern short			ec_dir[MAXCHARGES];
extern long				ec_timer[MAXCHARGES];
extern unsigned char	ec_type[MAXCHARGES];
extern short			ec_amount[MAXCHARGES];
extern bool				ec_primed[MAXCHARGES];

extern short			dp_x[MAXDUSTPARTICLES];	// dust particles
extern short			dp_y[MAXDUSTPARTICLES];
extern short			dp_v[MAXDUSTPARTICLES];
extern double			dp_dir[MAXDUSTPARTICLES];

extern char				exp_selected;
extern bool				boolExplode;

extern long				plr_currentcity;
extern char				city_name[MAXCITIES][20];

extern long				lTimer;

extern short			iDampermode,iDamperbeginx,iDamperbeginy;
extern int				m_button,m_x,m_y,m_down_x,m_down_y,m_up_x,m_up_y,m_event;

//extern int				iTargetBuildingType,iTarget,iTargetx,iTargetendx;

extern unsigned int		randseed;
//extern short			TargetAreaBegin,TargetAreaEnd;
extern char				b_name[BUILDINGS][255];
extern char				b_localename[BUILDINGS][52];
extern bool				blocksmoving,boolJobDone,boolJobSuccess;

int						exp_needed[MAXEXPLOSIVETYPES]={0,0,0,0,0,0};
int						acc_needed[MAXEXPLOSIVETYPES]={0,1,50,0};
extern long				lTotalcost,lPlacehours;

extern bool				bAddCharge;

extern long				plr_money,plr_careful;
extern short			plr_gametype;

extern bool				bAddDamper;

extern char				messagewindow[MAXMESSAGES][30];
extern char				messagecolor[MAXMESSAGES];
extern short			messagelines;
extern short			messagebegin;

extern char				helpwindow[MAXMESSAGES][30];
extern char				helpcolor[MAXMESSAGES];
extern short			helpbegin;

char					iDrawSmallMap;

short					dustx[MAXDUST];
short					dusty[MAXDUST];
char					dustframe[MAXDUST];

extern short			setSoundEffects,setDustClouds,setExplosions;

extern char				gametxt[MAXGAMETXT][52];

extern unsigned char	cBaseColours[3];
extern char				iTextColor,iColOffset;

extern char				cMapredraw[MAPSIZE][MAPSIZE];
short					iProgress;
short					iPprogress;

//bool					bFlip;
extern short			mapcodelines;

char					cDraw;
bool					bShowHelp;

extern bool				bTutorial;
extern char				cTutorial;

extern long				lBlocksmoving;

short					tmpmovex,tmpmovey;

// OC2ED begins
extern char				ed_building[52];
// OC2ED ends

void AddDust(short ix,short iy)
{
	short idust=0;

	while(dustx[idust]>0 && idust<MAXDUST-1)
	{
		idust++;
	}

	if(idust<MAXDUST-1)
	{
		dustx[idust]=ix*20;
		dusty[idust]=iy*20;
		dustframe[idust]=0;
	}

}

void CheckDust()
{
	short ix;

	for(ix=0;ix<MAXDUST;ix++)
	if(dustx[ix]>0)
	{
		dustframe[ix]++;
		if(dustframe[ix]>10) dustx[ix]=0;
	}
}

void ZeroDust()
{
	short ix;
	for(ix=0;ix<MAXDUST;ix++)
	{
		dustx[ix]=0;
		dustframe[ix]=0;
	}
	for(ix=0;ix<MAXDUSTPARTICLES;ix++)
	{
		dp_x[ix]=0;
		dp_y[ix]=0;
		dp_v[ix]=0;
	}
}

void DrawTutorialCharges()
{
	short xc,mapx,mapy,chargesize,max_x,max_y;
	short maxpixelx=SIZE_X-260, maxpixely=SIZE_Y;

	short chargex[9]={140,153,143,152};
	short chargey[9]={53,53,41,41};

	chargesize=BLOCKSIZE/2;

	max_x=(SIZE_X-260)/BLOCKSIZE;
	max_y=SIZE_Y/BLOCKSIZE;

	for(xc=0;xc<8;xc++)
	{
		mapx=(chargex[xc]*20-mapbegin_x*20)/(20/BLOCKSIZE)+chargesize;
		mapy=max_y*BLOCKSIZE-(chargey[xc]*20-mapbegin_y*20)/(20/BLOCKSIZE)-chargesize;

		if(mapx>0 && mapx<maxpixelx-10 && mapx>0 && mapy<maxpixely-10)
			DrawBMPButton("",mapx,mapy,12);
//			DrawLine(chargex+chargesize/2,chargey+chargesize/2,ix,iy,255,0,0);

	}

}

void DrawMap()
{
	short xc,yc;
	short src_bx,src_by,iframe,iframesize,iframeybegin,iframeperrow,damadd,chargex,chargey,tempx,tempy,maskx,masky;
	char msg[100];
	short max_x,max_y,add_y,mapx,mapy,ix,iy,ixend,iyend,change,maxpixelx,maxpixely,chargesize,xreduce,dstx;
	double tempdir,temprad;
	unsigned char r,g,b;

	max_x=(SIZE_X-260)/BLOCKSIZE;
	max_y=SIZE_Y/BLOCKSIZE;
	//printf("MAX_Y %d ",max_y);
	maskx=720;
	masky=260;

	if(BLOCKSIZE==10)
	{
		maskx=360;
		masky=470;
	}

	tempx=max_x*BLOCKSIZE;
	tempy=max_y*BLOCKSIZE;

	maxpixelx=max_x*BLOCKSIZE;// SIZE_X-260;
	maxpixely=max_y*BLOCKSIZE; //SIZE_Y;

	DrawFillRect(0,tempy,SIZE_X-260,SIZE_Y,0,0,0);	// bottom
	DrawFillRect(tempx,0,SIZE_X-260,SIZE_Y,0,0,0);		// between map and sidebar, needed at least in 1024x768 resolution
	DrawFillRect(0,0,maxpixelx,maxpixely,100,100,250);	// sky

//	SDL_FillRect(screen,NULL,0);

	xreduce=BLOCKSIZE*40;

	add_y=200;
	if(BLOCKSIZE==10) add_y=440;

	if(boolExplode==true)
	{

		if(setDustClouds==1)
		for(xc=0;xc<MAXDUSTPARTICLES;xc++)		// dust particles
		if(dp_x[xc]>0 && dp_v[xc]>50)
		{
			ix=(dp_x[xc]-mapbegin_x*20)/(20/BLOCKSIZE);
			iy=max_y*BLOCKSIZE-(dp_y[xc]-mapbegin_y*20)/(20/BLOCKSIZE);

			yc=100+dp_v[xc]/5;
			if(yc>150) yc=150;
			if(yc<100) yc=100;

			if(ix>0 && ix<maxpixelx-1 && iy>0 && iy<maxpixely-1)
			DrawFillRect(ix,iy,ix+2,iy+2,yc,yc,255);
		}


		for(xc=0;xc<MAXEXPLOSIONPARTICLES;xc++)
		if(ep_x[xc]>0 && ep_v[xc]>0)
		{
			ix=(ep_x[xc]-mapbegin_x*20)/(20/BLOCKSIZE);
			iy=max_y*BLOCKSIZE-(ep_y[xc]-mapbegin_y*20)/(20/BLOCKSIZE);

			yc=ep_v[xc]/5;
			if(yc>255) yc=255;
			if(yc<150) yc=150;

			if(ix>0 && ix<maxpixelx-1 && iy>0 && iy<maxpixely-1)
			DrawFillRect(ix,iy,ix+2,iy+2,yc,yc,255);
		}

	}

	for(xc=0;xc<max_x;xc++)
	for(yc=0;yc<max_y;yc++)
	{
	mapx=mapbegin_x+xc;
	mapy=mapbegin_y+(max_y-1)-yc;
		if(map[mapx][mapy]>0)
		{
//			cMapredraw[mapx][mapy]--;
//			bFlip=true;

			src_bx=map[mapx][mapy]*BLOCKSIZE;
			src_by=0;

			while(src_bx>=xreduce)
			{
				src_bx-=xreduce;
				src_by+=BLOCKSIZE;
			}

		damadd=0;

		if(mapend[mapx][mapy]<block_end[map[mapx][mapy]]) damadd=BLOCKSIZE*4;
		if(mapend[mapx][mapy]<block_end[map[mapx][mapy]]/2) damadd=BLOCKSIZE*8;

		ShowBMP1(src_bx,src_by+add_y+damadd,BLOCKSIZE,BLOCKSIZE,xc*BLOCKSIZE,yc*BLOCKSIZE);	// draw the block

		if(damadd>0)	// draw damaged sides to blocks, if next to air
		{
			if(mapx>0 && map[mapx-1][mapy]==0) ShowBMP1(maskx,masky,BLOCKSIZE,BLOCKSIZE,xc*BLOCKSIZE,yc*BLOCKSIZE);
			if(mapx<MAPSIZE-1 && map[mapx+1][mapy]==0) ShowBMP1(maskx+2*BLOCKSIZE,masky,BLOCKSIZE,BLOCKSIZE,xc*BLOCKSIZE,yc*BLOCKSIZE);
			if(mapy>0 && map[mapx][mapy-1]==0) ShowBMP1(maskx+3*BLOCKSIZE,masky,BLOCKSIZE,BLOCKSIZE,xc*BLOCKSIZE,yc*BLOCKSIZE);
			if(mapy<MAPSIZE-1 && map[mapx][mapy+1]==0) ShowBMP1(maskx+BLOCKSIZE,masky,BLOCKSIZE,BLOCKSIZE,xc*BLOCKSIZE,yc*BLOCKSIZE);
		}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	sprintf(msg,"%d",mapv[mapx][mapy]);
//		sprintf(msg,"%d",mapcounted[mapx][mapy]);
/*		sprintf(msg,"-");
		if(mapsupport[mapx][mapy]>0)
			sprintf(msg,"+");
*/
//		sprintf(msg,"%d",mapsupport[mapx][mapy]);
//		sprintf(msg,"%d",map[mapbegin_x+xc][mapbegin_y+yc]);
//		sprintf(msg,"%d",mapy);
//	DrawNormalText(msg,xc*BLOCKSIZE+2,yc*BLOCKSIZE+2,1);

/*		if(mapv[mapx][mapy]>0)
		{
		DrawFillRect(xc*BLOCKSIZE+7,yc*BLOCKSIZE+7,xc*BLOCKSIZE+7+3,yc*BLOCKSIZE+7+3,255,0,0);
		}
*/
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		}
	}

	chargesize=BLOCKSIZE/2;
	tempx=2;
	if(BLOCKSIZE==10) tempx=1;

	for(xc=0;xc<MAXCHARGES;xc++)
	if(ec_x[xc]>0)
	{
		r=255;
		g=0;
		b=0;

	if(ec_selected==xc) {g=255;b=255;}

	if(ec_primed[xc]==false)
//	if(ec_x[xc]>mapbegin_x && ec_x[xc]<mapbegin+max_x)
//	if(ec_y[xc]>mapbegin_y && ec_y[xc]<mapbegin_y+max_y)
	{
//		mapx=mapbegin_x+m_x/BLOCKSIZE;
//		mapy=mapbegin_y+(max_y-1)-m_y/BLOCKSIZE;

		chargex=(ec_x[xc]-mapbegin_x*20)/(20/BLOCKSIZE)-chargesize/2;
		chargey=max_y*BLOCKSIZE-(ec_y[xc]-mapbegin_y*20)/(20/BLOCKSIZE)-chargesize/2;

//		printf("EXPL %d %d\n",chargex,chargey);

		if(chargex>0 && chargex<maxpixelx-chargesize && chargey>0 && chargey<maxpixely-chargesize)
		{
			DrawFillRect(chargex,chargey,chargex+chargesize,chargey+chargesize,cBaseColours[0]+50,cBaseColours[1]+50,cBaseColours[2]+50);
			DrawFillRect(chargex+tempx,chargey+tempx,chargex+chargesize-tempx,chargey+chargesize-tempx,r,g,b);
//			DrawLine(chargex,chargey,chargex+chargesize,chargey+chargesize,cBaseColours[0]+50,cBaseColours[1]+50,cBaseColours[2]+50);
//			DrawLine(chargex+chargesize,chargey,chargex,chargey+chargesize,cBaseColours[0]+50,cBaseColours[1]+50,cBaseColours[2]+50);

			if(bAddDamper==true)
			{
			// circle for determining explosive strength
			temprad=((exp_velocity[ec_type[xc]]/50)+(ec_amount[xc]*exp_velocity[ec_type[xc]]/250)+plr_careful)/(20/BLOCKSIZE);
//			(exp_velocity[ec_type[charge]]/150)+(exp_velocity[ec_type[charge]]*ec_amount[charge]/100);
			DrawCircle(chargex+chargesize/2,chargey+chargesize/2,temprad,r,g,b);
			}

			if(ec_dir[xc]>=0)
			{
			tempdir=ec_dir[xc]-90;
			if(tempdir<0) tempdir+=360;
			tempdir*=rad;

			ix=short(chargex+chargesize/2+24*cos(tempdir));
			iy=short(chargey+chargesize/2-24*sin(tempdir));

			if(ix>0 && ix<maxpixelx && iy>0 && iy<maxpixely)
				DrawLine(chargex+chargesize/2,chargey+chargesize/2,ix,iy,255,0,0);
			}
		}

	}

	}

	if(mapdestx!=mapbegin_x || mapdesty!=mapbegin_y)
	{
	mapx=maxpixelx/2+(mapdestx-mapbegin_x)*BLOCKSIZE;
	mapy=maxpixely/2+(mapbegin_y-mapdesty)*BLOCKSIZE;
//	DrawFillRect(mapx-2,mapy-2,mapx+2,mapy+2,255,255,255);
	DrawLine(mapx-5,mapy-5,mapx+5,mapy+5,255,255,255);
	DrawLine(mapx-5,mapy+5,mapx+5,mapy-5,255,255,255);


/*	ix=maxpixelx/2;
	iy=maxpixely/2;
	DrawFillRect(ix-2,iy-2,ix+2,iy+2,255,255,255);
*/
//	DrawLine(mapx,mapy,ix,iy,255,255,255);
	}


if(boolExplode==true)
{
	switch(BLOCKSIZE)
	{
	case 10:dstx=50;change=500;tempx=800;break;
	case 20:dstx=100;change=0;tempx=700;break;
	}



	if(setDustClouds==1)
	{
		for(xc=0;xc<MAXDUST;xc++)		// dust
		if(dustx[xc]>0)
		{
			ix=(dustx[xc]-mapbegin_x*20)/(20/BLOCKSIZE)-3*BLOCKSIZE;
	//		iy=(dusty[xc]-mapbegin_y*20)/(20/BLOCKSIZE)-3*BLOCKSIZE;
			iy=max_y*BLOCKSIZE-(dusty[xc]-mapbegin_y*20)/(20/BLOCKSIZE)-3*BLOCKSIZE;

			if(ix>0 && ix<maxpixelx-dstx && iy>0 && iy<maxpixely-dstx)
			{
				iframe=dustframe[xc];

				while(iframe>4) iframe-=5;
//				switch(BLOCKSIZE)
//				{
//				case 10:
					ShowBMP2(change+iframe*dstx,tempx,dstx,dstx,ix,iy);//break;
//				case 20:ShowBMP2(iframe*dstx,700,dstx,dstx,ix,iy);break;
//				}
			}
		}
	}


	if(setExplosions==1)
	for(xc=0;xc<MAXCHARGES;xc++)
	if(ec_primed[xc]==true)
	if(ec_timer[xc]<lTimer && ec_timer[xc]+50>lTimer)		// explosion shows only for "500 mseconds"
		{
		iframe=0;

		switch(BLOCKSIZE)
		{
		case 10:
			if(ec_amount[xc]<20)
				iframe=2;
			else
				iframe=1;
			break;
		case 20:
			if(ec_amount[xc]<20) iframe=1;
			break;
		}

		switch(iframe)
		{
		case 0:
			iframeybegin=0;
			iframesize=200;
			iframeperrow=5;
			break;
		case 1:
			iframeybegin=600;
			iframesize=100;
			iframeperrow=10;
			break;
		case 2:
			iframeybegin=800;
			iframesize=50;
			iframeperrow=15;
			break;
		}

		chargex=(ec_x[xc]-mapbegin_x*20)/(20/BLOCKSIZE)-iframesize/2;
//		chargey=(ec_y[xc]-mapbegin_y*20)/(20/BLOCKSIZE)-iframesize/2;
		chargey=max_y*BLOCKSIZE-(ec_y[xc]-mapbegin_y*20)/(20/BLOCKSIZE)-iframesize/2;

		if(chargex>0 && chargex<SIZE_X-260-iframesize && chargey>0 && chargey<SIZE_Y-iframesize)
		{
			iframe=(lTimer-ec_timer[xc])/2;
			yc=0;
			while(iframe>=15) iframe-=5;

			while(iframe>iframeperrow-1)
			{
				if(iframesize>50)
				{
				iframe-=iframeperrow;
				yc++;
				if(iframesize==100 && yc>=1)
				{
					yc=1;
				}
				if(iframesize==200 && yc>2) yc=2;
				}
			}

			ShowBMP2(iframe*iframesize,iframeybegin+yc*iframesize,iframesize,iframesize,chargex,chargey);
			}
		}
//	if(boolExplode==true)
}


	if(GameOn==1 && bAddCharge==true && bAddDamper==false && m_event==1 && m_button==1 && m_down_x<SIZE_X-260 && boolMapgenerated==true)
	{
		DrawFillRect(m_down_x-chargesize/2,m_down_y-chargesize/2,m_down_x+chargesize/2,m_down_y+chargesize/2,255,0,0);
		DrawLine(m_down_x,m_down_y,m_x,m_y,255,0,0);
	}

	if(iDampermode==1)
	{
		if(iDamperbeginx>=mapbegin_x && iDamperbeginx<mapbegin_x+max_x)
		if(iDamperbeginy>=mapbegin_y && iDamperbeginy<mapbegin_y+max_y)
		{
			ix=(iDamperbeginx-mapbegin_x)*BLOCKSIZE;
			iy=(max_y-(iDamperbeginy-mapbegin_y))*BLOCKSIZE-BLOCKSIZE;

			ixend=(m_x/BLOCKSIZE)*BLOCKSIZE;
			iyend=(m_y/BLOCKSIZE)*BLOCKSIZE;

			if(ix>ixend) {change=ixend;ixend=ix;ix=change;}
			if(iy>iyend) {change=iyend;iyend=iy;iy=change;}
			ixend+=BLOCKSIZE;
			iyend+=BLOCKSIZE;

			if(ix<0) ix=0;
			if(iy<0) iy=0;
			if(ixend>SIZE_X-260) ixend=SIZE_X-260;
			if(iyend>SIZE_Y) iyend=SIZE_Y;

			if(ix>SIZE_X-260) ix=SIZE_X-260;
			if(iy>SIZE_Y) iy=SIZE_Y;
			if(ixend<0) ixend=0;
			if(iyend<0) iyend=0;

			DrawRect(ix,iy,ixend,iyend,200,200,200);
		}
	}


//	sprintf(msg,"%s %u %u %u",b_localename[iTargetbuilding],lBlocksmoving, tmpmovex,tmpmovey);//, city_name[plr_currentcity]);
//	sprintf(msg,"%s %u %u",b_localename[iTargetbuilding],p_buildbegin,p_buildwidth);
	sprintf(msg,"%s",b_localename[iTargetbuilding]);

//	if(ed_building[0]!=0) sprintf(msg,"%s %u",ed_building,lBlocksmoving);
	if(ed_building[0]!=0) sprintf(msg,"%s",ed_building);
	DrawNormalText(msg,10,10,0);

	if(bTutorial==true && cTutorial==2) DrawTutorialCharges();
}


void DrawBlockData()
{
	short mapx,mapy,icolor,endtemp,max_y;
	bool blockok;
	char msg[50];

	max_y=SIZE_Y/BLOCKSIZE;

	DrawFillRect(SIZE_X-120,0,SIZE_X,110,cBaseColours[0],cBaseColours[1],cBaseColours[2]);

	if(m_x<SIZE_X-260)// && boolMapgenerated==true
	{

		mapx=mapbegin_x+m_x/BLOCKSIZE;
		mapy=mapbegin_y+(max_y-1)-m_y/BLOCKSIZE;

		icolor=iTextColor;
		blockok=false;

		if(map[mapx][mapy]>=1 && map[mapx][mapy]<=55) blockok=true;
		if(map[mapx][mapy]>=111 && map[mapx][mapy]<=129) blockok=true;

		if(blockok==true)
		{
			if(maptarget[mapx][mapy]==true)
				icolor=3;
			else
				icolor=2;
		}


/*		sprintf(msg,"%4u",mapend[mapx][mapy]);	// for testing only
		DrawNormalText(msg,SIZE_X-120,40,iTextColor);*/

		sprintf(msg,"%.15s",block_name[map[mapx][mapy]]);
		if(mapy>=0) DrawNormalText(msg,SIZE_X-120,10,icolor);

		//sprintf(msg,"%4u (%d)",mapend[mapx][mapy],mapsupport[mapx][mapy]);
		if(mapend[mapx][mapy]>0) // && map[mapx][mapy])
		{
		if(block_end[map[mapx][mapy]]>0)
			endtemp=mapend[mapx][mapy]*4/block_end[map[mapx][mapy]];
		else
			endtemp=0;

		sprintf(msg,"%4u/%4u",mapend[mapx][mapy],block_end[map[mapx][mapy]]);
		DrawNormalText(msg,SIZE_X-120,30,iTextColor);

		if(mapend[mapx][mapy]==block_end[map[mapx][mapy]])
			DrawNormalText(gametxt[62],SIZE_X-120,50,iTextColor);	// undamaged
			else
			DrawNormalText(gametxt[58+endtemp],SIZE_X-120,50,iTextColor);	// degree of damage
		}

		sprintf(msg,"X%3d Y%3d",mapx,mapy);
		if(mapy>=0) DrawNormalText(msg,SIZE_X-120,70,iTextColor);

//		if(boolExplode==false)
		{
			DrawFillRect(SIZE_X-120,100,SIZE_X,120,cBaseColours[0],cBaseColours[1],cBaseColours[2]);
			sprintf(msg,"$%u",block_cost[map[mapx][mapy]]);
			DrawNormalText(msg,SIZE_X-120,90,icolor);

			// for testing
//			sprintf(msg,"%u %d",mapcounted[mapx][mapy],mapsupport[mapx][mapy]);
//			DrawNormalText(msg,SIZE_X-120,100,icolor);
		}
	}

}

void DrawCounter()
{
	char msg[10];
	long lTempTimer,lSec;

	lTempTimer=lTimer*10;
	lSec=0;
	while(lTempTimer>=100)
	{
		lTempTimer-=100;
		lSec++;
	}
	if(boolExplode==true)
	{
	sprintf(msg,"%3u:%02u",lSec,lTempTimer);
	DrawDigits(msg,SIZE_X-120,110);
	}
}

void DrawSidebar()
{
	char msg[50];
	short ix,iy;
	int iCounter;
	double tempdir;
/*	char endtxt[4][20]={
		"Heavy damage",
		"Medium damage",
		"Slight damage",
		"Superficial damage"
	};
*/
//	DrawFillRect(1020,135,1280,1024,150,150,150);
	DrawFillRect(SIZE_X-260,135,SIZE_X,SIZE_Y,cBaseColours[0],cBaseColours[1],cBaseColours[2]);
	DrawFillRect(SIZE_X-120,0,SIZE_X,135,cBaseColours[0],cBaseColours[1],cBaseColours[2]);
	DrawBlockData();

	if(boolMapgenerated==true)
		{
		for(iCounter=0;iCounter<MAXEXPLOSIVETYPES;iCounter++)
		{
			sprintf(msg,"%-15s($%2u) %4u",gametxt[132+iCounter],exp_cost[iCounter],exp_needed[iCounter]);	// expl name
			if(exp_selected==iCounter)
				DrawTextWBack(msg,SIZE_X-250,139+iCounter*16,1,cBaseColours[0]+50,cBaseColours[1]+50,cBaseColours[2]+50);
				else
	//			DrawTextWBack(msg,SIZE_X-250,290+iCounter*10,1,150,150,150);
				DrawNormalText(msg,SIZE_X-250,140+iCounter*16,iTextColor);

				DrawTextRect("?",SIZE_X-30,140+iCounter*16,SIZE_X-5,152+iCounter*16);	// question mark next to explosive
		}

		// unit buttons
		sprintf(msg,gametxt[63]);	// add
		if(bAddCharge==false) sprintf(msg,gametxt[64]);	// select
		if(ec_x[ec_selected]>0 && lTimer==0) sprintf(msg,gametxt[163]); // move
		DrawBMPButton(msg,SIZE_X-250,235,2);

		DrawBMPButton(gametxt[65],SIZE_X-170,235,2);	// remove

	//	sprintf(msg,"Det. %d",ec_selected);
		DrawBMPButton(gametxt[66],SIZE_X-90,235,2);		// det.sel

/*		for(iCounter=0;iCounter<MAXCHARGES;iCounter++)
		{
			sprintf(msg,"%u %u ",iCounter,ec_x[iCounter]);
			DrawNormalText(msg,100,iCounter*15,1);
		}
*/
		// unit size
		sprintf(msg,"%s %u",gametxt[67],ec_amount[ec_selected]);	// unit size
		DrawNormalText(msg,SIZE_X-250,265,iTextColor);
		DrawRect(SIZE_X-110,265,SIZE_X-10,270,cBaseColours[0]+50,cBaseColours[1]+50,cBaseColours[2]+50);
//		DrawButton(SIZE_X-110+ec_amount[ec_selected],260,SIZE_X-105+ec_amount[ec_selected],275);
		DrawBMPButton("",SIZE_X-118+ec_amount[ec_selected]*4,260,9);

		// unit delay
		tempdir=ec_timer[ec_selected];
		tempdir/=10;
		sprintf(msg,"%s %4.1f s",gametxt[68],tempdir);	// delay

		DrawNormalText(msg,SIZE_X-250,285,iTextColor);
		DrawRect(SIZE_X-110,285,SIZE_X-10,290,cBaseColours[0]+50,cBaseColours[1]+50,cBaseColours[2]+50);
		DrawLine(SIZE_X-60,280,SIZE_X-60,295,cBaseColours[0]+50,cBaseColours[1]+50,cBaseColours[2]+50);
//		DrawButton(SIZE_X-110+(ec_timer[ec_selected]/2),280,SIZE_X-105+(ec_timer[ec_selected]/2),295);


		ix=ec_timer[ec_selected]*2;
		if(ec_timer[ec_selected]>25)
			{
				ix=ec_timer[ec_selected]/5+50;
//				ec_timer[ec_selected]=25+ix; //*ix/4;
			}
		DrawBMPButton("",SIZE_X-118+ix,280,9);

		// direction

//		ShowBMP(1,600,50,50,50,SIZE_X-130,300);
		DrawBMPButton("",SIZE_X-130,300,8);

		if(ec_dir[ec_selected]>=0)
		{

		tempdir=ec_dir[ec_selected]-90;
		if(tempdir<0) tempdir+=360;
		tempdir*=rad;

		ix=short(SIZE_X-104+24*cos(tempdir));
		iy=short(325-24*sin(tempdir));

		DrawLine(SIZE_X-104,325,ix,iy,cBaseColours[0]+iColOffset,cBaseColours[1]+iColOffset,cBaseColours[2]+iColOffset);
		DrawBMPButton(gametxt[71],SIZE_X-250,320,2);//Reset
		}
		else
			sprintf(msg,"%s",gametxt[70]);	// not directed

		DrawNormalText(gametxt[69],SIZE_X-250,305,iTextColor);	// direction of explosion


		// accessories
		for(iCounter=0;iCounter<MAXACCESSORIES;iCounter++)
		{
			sprintf(msg,"%-15s($%2u) %4u",gametxt[138+iCounter],acc_cost[iCounter],acc_needed[iCounter]);	// accessories
			DrawNormalText(msg,SIZE_X-250,355+iCounter*12,iTextColor);
			DrawTextRect("?",SIZE_X-30,358+iCounter*12,SIZE_X-5,369+iCounter*12);	// question mark next to explosive
		}

		sprintf(msg,"%-17s %u h",gametxt[72],lPlacehours);	// readying takes
		DrawNormalText(msg,SIZE_X-250,410,iTextColor);

		sprintf(msg,"%-17s $%u",gametxt[73],lTotalcost);	// total cost

		if(lTotalcost<plr_money)
			DrawNormalText(msg,SIZE_X-250,422,iTextColor);
		else
			DrawNormalText(msg,SIZE_X-250,422,2);


		// buttons
		if(lTimer==0)
		DrawBMPButton(gametxt[74],SIZE_X-250,450,3);		// detonate
/*			else
		DrawTextRect(gametxt[74],SIZE_X-250,450,SIZE_X-131,479);
*/
		if(bAddDamper==true)
			DrawBMPButton(gametxt[75],SIZE_X-130,450,3);	//Damper mode
		else
			DrawBMPButton(gametxt[76],SIZE_X-130,450,3);	//Expl. mode
	}		//		boolMapgenerated=true;

	sprintf(msg,"%-17s $%u",gametxt[37],plr_money);			// money
	if(plr_money==0)
		DrawNormalText(msg,SIZE_X-250,434,2);
	else
		DrawNormalText(msg,SIZE_X-250,434,iTextColor);

	if(GameOn!=3)
//		DrawTextRect(gametxt[77],SIZE_X-250,480,SIZE_X-10,509);
//	else
		DrawBMPButton(gametxt[77],SIZE_X-250,480,4);		// financial decisions

	if(BLOCKSIZE==10) sprintf(msg,gametxt[78]);		// zoom in
	if(BLOCKSIZE==20) sprintf(msg,gametxt[79]);		// zoom out

	DrawBMPButton(msg,SIZE_X-250,510,3);		// zoom button

	if(setSoundEffects==0)
		//sprintf(msg,"Sounds off");
		DrawBMPButton("",SIZE_X-60,510,7);
	else
//		sprintf(msg,"Sounds on");
		DrawBMPButton("",SIZE_X-60,510,6);

//	DrawBMPButton(msg,SIZE_X-130,510,3);
	DrawNormalText(gametxt[80],SIZE_X-120,520,iTextColor);//Sounds

	if(boolJobDone==true && lTimer==0)
	{
	if(plr_gametype==0)	// career
		{
//		printf("%d %d \n",boolJobSuccess,boolMapgenerated);

		if(GameOn!=5)	// change city
//			DrawTextRect(gametxt[81],SIZE_X-250,540,SIZE_X-130,570);
//		else
			DrawBMPButton(gametxt[81],SIZE_X-250,540,3);


		if(GameOn!=4)	//Customers
//			DrawTextRect(gametxt[26],SIZE_X-130,540,SIZE_X-10,570);
//		else
			DrawBMPButton(gametxt[26],SIZE_X-130,540,3);
		}
	else
		{

		DrawBMPButton(gametxt[82],SIZE_X-250,330,4);		// random map, version 1.1
		DrawBMPButton(gametxt[28],SIZE_X-250,360,4);			//Map nr/code
		DrawBMPButton(gametxt[533],SIZE_X-250,390,4);			//from file

//		DrawBMPButton(gametxt[82],SIZE_X-250,540,3);		// random map
//		DrawBMPButton(gametxt[28],SIZE_X-130,540,3);			//Map nr/code
		}

	}

	if(lTimer==0)	// finish/cancel buttons only visible when not exploding
	{
		if(boolJobSuccess==true)
			DrawBMPButton(gametxt[83],SIZE_X-250,540,4);	//Finish this site
		else
		{
		if(boolJobDone==false && boolMapgenerated==true)
			DrawBMPButton(gametxt[10],SIZE_X-250,540,4);		//Cancel job
		}
	}


	// find target building (TYPE)
//	DrawFillRect(SIZE_X-8,540,SIZE_X-2,570,cBaseColours[0]-50,cBaseColours[1]-50,cBaseColours[2]-50);

	DrawFillRect(SIZE_X-250,575,SIZE_X-10,595+messagelines*16,cBaseColours[0]-25,cBaseColours[1]-25,cBaseColours[2]-25);

	if(bShowHelp==false)
	{
		ix=0;
		iy=1;
		if(cBaseColours[0]+cBaseColours[1]+cBaseColours[2]<200) iy=0;
		DrawFillRect(SIZE_X-130,575,SIZE_X-10,595,cBaseColours[0]-50,cBaseColours[1]-50,cBaseColours[2]-50);

//		DrawRect(SIZE_X-130,575,SIZE_X-10,590,cBaseColours[0]+50,cBaseColours[1]+50,cBaseColours[2]+50);
	}
	else
	{
		ix=1;
		iy=0;
		if(cBaseColours[0]+cBaseColours[1]+cBaseColours[2]<200) ix=0;
		DrawFillRect(SIZE_X-250,575,SIZE_X-130,595,cBaseColours[0]-50,cBaseColours[1]-50,cBaseColours[2]-50);
	}

	DrawBMPButton("",SIZE_X-150,575,10);	// up
	DrawBMPButton("",SIZE_X-130,575,11);	// down

	DrawNormalText(gametxt[200],SIZE_X-240,575,ix);	// messages
	DrawNormalText(gametxt[201],SIZE_X-100,575,iy);		// help


	// message/help window
	if(bShowHelp==false)
	{
		ix=0;
		for(iCounter=0;iCounter<messagelines;iCounter++)
//		if(messagewindow[messagebegin+iCounter][0]!=0)
		{
			sprintf(msg,"%-30s",messagewindow[messagebegin+iCounter]);
			DrawNormalText(msg,SIZE_X-250,595+ix*16,messagecolor[messagebegin+iCounter]);
			ix++;
		}
	}
	else
	{

		ix=0;
		for(iCounter=0;iCounter<messagelines;iCounter++)
//		if(helpwindow[iCounter][0]!=0)
		{
			sprintf(msg,"%-30s",helpwindow[helpbegin+iCounter]);
			DrawNormalText(msg,SIZE_X-250,595+ix*16,helpcolor[helpbegin+iCounter]);
			ix++;
		}
	}

	DrawBMPButton(gametxt[84],SIZE_X-250,SIZE_Y-70,4);		// Save & Load
	DrawBMPButton(gametxt[85],SIZE_X-250,SIZE_Y-40,4);	// Exit to main menu"

	DrawCounter();
}

void DrawSmallMap()
{
//    SDL_Rect src;
//    SDL_Rect dest;

	int ix,iy,src_bx,src_by,max_x,max_y;

	DrawFillRect(SIZE_X-260,0,SIZE_X-120,135,cBaseColours[0],cBaseColours[1],cBaseColours[2]);
	DrawFillRect(SIZE_X-255,5,SIZE_X-255+MAPSIZE/2,5+MAPSIZE/2,100,100,250);	// sky

	for(ix=0;ix<MAPSIZE/2;ix++)
	for(iy=0;iy<MAPSIZE/2;iy++)
	if(map[ix*2][iy*2]>0)
	{

		src_bx=map[ix*2][iy*2];
		src_by=0;

		while(src_bx>40)
		{
			src_bx-=40;
			src_by++;
		}

/*	src.x=src_bx*20+10;
	src.y=src_by*20+70;
	src.w=1;
	src.h=1;

    dest.x = 1025+ix;
    dest.y = 5+iy;
    dest.w = 1;
    dest.h = 1;
*/
	ShowBMP1(src_bx,src_by+560,1,1,SIZE_X-255+ix,130-iy);
	}

	max_x=(SIZE_X-260)/BLOCKSIZE/2;
	max_y=SIZE_Y/BLOCKSIZE/2;
	DrawRect(SIZE_X-255+mapbegin_x/2,130-mapbegin_y/2-max_y,SIZE_X-255+max_x+mapbegin_x/2,130-mapbegin_y/2,cBaseColours[0]+50,cBaseColours[1]+50,cBaseColours[2]+50);
}


void DrawAll()
{
//	char msg[50];
//	bFlip=true;

if(cDraw>0)
{
	cDraw--;

//	if(setAccelerationMode==1)
//	SDL_ShowCursor(SDL_DISABLE);

	switch(GameOn)
	{
	case 0:
		DrawMenus();
	break;

	case 1:

		DrawMap();
		DrawSidebar();

		if(iDrawSmallMap>0)
		{
			DrawSmallMap();
			iDrawSmallMap--;
		}
		CheckDust();
		break;
	case 2:
		DrawMenus();
		break;
	case 3:
		DrawMap();
		DrawSidebar();
		DrawFinancialMenu();
		DrawSmallMap();
		cDraw=2;

		break;
	case 4:
		DrawMap();
		DrawSidebar();
		DrawCityMenu();
		DrawSmallMap();
		break;
	case 5:
		DrawMap();
		DrawSidebar();
		DrawCustomerMenu();
		DrawSmallMap();
		break;
	case 6:
/*		DrawMap();
		sprintf(msg,"Searching... (%u)",randseed);
		DrawTextRect(msg,SIZE_X/2-120,SIZE_Y/2-20,SIZE_X/2+120,SIZE_Y/2+20);
		*/
		Statistics();
		break;
	default:break;
	}

}
if(GameOn==1 && boolExplode==false)
{
	DrawBlockData();
}

if(bTutorial==true)
if(GameOn==1 || GameOn==3 || GameOn==5) DrawTutorial();


//	if(bTutorial==true) DrawTutorial();


/*	if(GameOn==6)	// search map
	{
		DrawMap();
		sprintf(msg,"Searching... (%u)",randseed);
		DrawTextRect(msg,SIZE_X/2-120,SIZE_Y/2-20,SIZE_X/2+120,SIZE_Y/2+20);
	}

/*	switch(setAccelerationMode)
	{
	case 1:
		SDL_ShowCursor(SDL_ENABLE);
//		if(bFlip==true)
		DoFlip();
		break;
	case 2:Update();break;
	}*/
//	SDL_Delay(10);


//	SDL_ShowCursor(SDL_ENABLE);

}

void ShowInitProgress()
{
	double dProg;

	iProgress++;

	dProg=iProgress;
	dProg/=11;	// amount of showinitprogress calls
	dProg*=200;

	DrawFillRect(xmiddle-150,ymiddle-100,xmiddle+150,ymiddle+100,cBaseColours[0],cBaseColours[1],cBaseColours[2]);
//	DrawFillRect(xmiddle-100,ymiddle+20,xmiddle+100,ymiddle+40,cBaseColours[0]-50,cBaseColours[1]-50,cBaseColours[2]-50);
//	DrawFillRect(xmiddle-100,ymiddle+20,xmiddle-100+(short) dProg,ymiddle+40,cBaseColours[0]+50,cBaseColours[1]+50,cBaseColours[2]+50);
	ShowBMP3(100,100,200,20,xmiddle-100,ymiddle+20);			// progress, not ready
	ShowBMP3(100,120,(short) dProg,20,xmiddle-100,ymiddle+20);			// progress, ready

	DrawNormalText("OPERATION CLEANER 2",xmiddle-140,ymiddle-90,iTextColor);
	DrawNormalText(gametxt[148],xmiddle-140,ymiddle-70,iTextColor);
	Update();
	SDL_Delay(10);
}

void ShowQuitProgress()
{
	DrawButton(xmiddle-100,ymiddle-25,xmiddle+100,ymiddle+25);
	DrawNormalText("OPERATION CLEANER 2",xmiddle-90,ymiddle-20,iTextColor);
	DrawNormalText(gametxt[97],xmiddle-90,ymiddle,iTextColor);
	Update();
	SDL_Delay(10);
}

void ShowParseProgress(char * msg)
{
	double dProg;

	iPprogress++;
	dProg=iPprogress;
	dProg/=6;
	dProg*=200;
	if(dProg>200) dProg=200;

	if(lMapnr==0)
	{
	DrawButton(xmiddle-150,ymiddle-100,xmiddle+150,ymiddle+100);
//	DrawFillRect(xmiddle-100,ymiddle+20,xmiddle+100,ymiddle+40,cBaseColours[0]-50,cBaseColours[1]-50,cBaseColours[2]-50);
//	DrawFillRect(xmiddle-100,ymiddle+20,xmiddle-100+(short) dProg,ymiddle+40,cBaseColours[0]+50,cBaseColours[1]+50,cBaseColours[2]+50);

	ShowBMP3(100,100,200,20,xmiddle-100,ymiddle+20);			// progress, not ready
	ShowBMP3(100,120,(short) dProg,20,xmiddle-100,ymiddle+20);			// progress, ready

	DrawNormalText(gametxt[149],xmiddle-140,ymiddle-70,iTextColor);
//	DrawNormalText(msg,xmiddle-140,ymiddle-50,iTextColor);

	Update();
	}
}
