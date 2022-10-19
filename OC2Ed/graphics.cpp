#include "SDL.h"   /* All SDL App's need this */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "OC2.h"

// handles almost all graphics in the game

extern char				GameOn;
extern bool				boolMapgenerated;

extern char				block_name[MAXBLOCKS][30];
extern short			block_end[MAXBLOCKS];
extern unsigned short	block_cost[MAXBLOCKS];
extern short			block_penres[MAXBLOCKS];

extern int				mapbegin_x,mapbegin_y;
extern short			mapdestx,mapdesty;
extern unsigned char	map[MAPSIZE][MAPSIZE];
extern short			mapend[MAPSIZE][MAPSIZE];
extern unsigned char	mapv[MAPSIZE][MAPSIZE];
//extern int				mapsupport[MAPSIZE][MAPSIZE];
extern bool				maptarget[MAPSIZE][MAPSIZE];
extern short			mapcounted[MAPSIZE][MAPSIZE];
//extern short			mapvertv[MAPSIZE][MAPSIZE];
extern short			iTargetbuilding;

extern short			SIZE_X,SIZE_Y,BLOCKSIZE,xmiddle,ymiddle;

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

extern int				exp_needed[MAXEXPLOSIVETYPES]={0,0,0,0,0,0};
extern int				acc_needed[MAXEXPLOSIVETYPES]={0,1,50,0};
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

// OC2ED begins
extern char				ed_building[52];
extern short			ed_block;
extern bool				ed_bsingle,ed_bfill,ed_bassist,ed_bbuilding,ed_brectangle,ed_bfrectangle,ed_btarget,ed_insert,ed_bcopy, ed_bsaveok,ed_bshowdropping;
extern short			ed_scut;
extern short			ed_beginx,ed_beginy,ed_endx,ed_endy;
extern char				ed_message[10][52];

extern short			ed_currentcity;
extern char				ed_customer[52];

extern short			ed_buildsel;

// OC2ED ends

void DrawMap()
{
	short xc,yc,bx,by;
	short src_bx,src_by; //,iframe,iframesize,iframeybegin,iframeperrow,chargex,chargey;
	short max_x,max_y,maxpixelx,maxpixely,xreduce,add_y,mapx,mapy,maskx,masky,tempx,tempy,damadd;
	char msg[100];
/*	
	short max_x,max_y,,mapx,mapy,ix,iy,ixend,iyend,,chargesize,,dstx;
	double tempdir,temprad;
	unsigned char r,g,b;
*/
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

	for(xc=0;xc<max_x;xc++)
	for(yc=0;yc<max_y;yc++)
	{
	mapx=mapbegin_x+xc;
	mapy=mapbegin_y+(max_y-1)-yc;
//		if(cMapredraw[mapx][mapy]>0)
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

		if(ed_btarget==true && maptarget[mapx][mapy]==true)
			DrawFillRect(xc*BLOCKSIZE+7,yc*BLOCKSIZE+7,xc*BLOCKSIZE+7+3,yc*BLOCKSIZE+7+3,0,200,0);

		if(ed_bshowdropping==true && mapv[mapx][mapy]>0)
			DrawRect(xc*BLOCKSIZE+3,yc*BLOCKSIZE+3,xc*BLOCKSIZE+BLOCKSIZE-3,yc*BLOCKSIZE+BLOCKSIZE-3,200,0,0);

/*		if(ed_insert==true && mapx==ed_beginx && mapy==ed_beginy)
		{
			DrawRect(xc*BLOCKSIZE+3,yc*BLOCKSIZE+3,xc*BLOCKSIZE+17,yc*BLOCKSIZE+17,255,0,0);
		}
		*/
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//		sprintf(msg,"%d",mapv[mapx][mapy]);
//		sprintf(msg,"%d",mapcounted[mapx][mapy]);
//		if(mapsupport[mapx][mapy]>0)
//		sprintf(msg,"%d",mapsupport[mapx][mapy]);
//		sprintf(msg,"%d",map[mapbegin_x+xc][mapbegin_y+yc]);
//		sprintf(msg,"%d",mapy);		
//		DrawNormalText(msg,xc*BLOCKSIZE+2,yc*BLOCKSIZE+2,1);
//	//	DrawFillRect(xc*BLOCKSIZE+7,yc*BLOCKSIZE+7,xc*BLOCKSIZE+7+3,yc*BLOCKSIZE+7+3,255,255,255);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

	if(ed_insert==true)
	{
/*
			ix=(iDamperbeginx-mapbegin_x)*BLOCKSIZE;
			iy=(max_y-(iDamperbeginy-mapbegin_y))*BLOCKSIZE;

			ixend=m_x;
			iyend=m_y;

*/		
		bx=(ed_beginx-mapbegin_x)*BLOCKSIZE;
		by=(max_y-(ed_beginy-mapbegin_y)-1)*BLOCKSIZE;

		if(ed_scut==2)
		{
			xc=(ed_endx-mapbegin_x)*BLOCKSIZE;
			yc=(max_y-(ed_endy-mapbegin_y))*BLOCKSIZE;
			yc-=BLOCKSIZE;
		}
		else
		{
			xc=(m_x/BLOCKSIZE)*BLOCKSIZE;
			yc=(m_y/BLOCKSIZE)*BLOCKSIZE;
		}

		if(bx>xc) {tempx=xc;xc=bx;bx=tempx;}
		if(by>yc) {tempx=yc;yc=by;by=tempx;}

		xc+=BLOCKSIZE;
		yc+=BLOCKSIZE;

//		sprintf(msg,"%u %u %u %u",bx,by,xc,yc);
//		DrawTextWBack(msg,m_x,m_y-20,1,255,255,255);

		if(bx<0) bx=0;
		if(by<0) by=0;
		if(xc>SIZE_X-260) xc=SIZE_X-260;
		if(yc>SIZE_Y) yc=SIZE_Y;

		if(bx>SIZE_X-260) bx=SIZE_X-260;
		if(by>SIZE_Y) by=SIZE_Y;
		if(xc<0) xc=0;
		if(yc<0) yc=0;

		{
			DrawRect(bx,by,xc,yc,255,255,255);
		}
	}

	if(ed_scut==2)
	{
		bx=m_x/BLOCKSIZE*BLOCKSIZE;
		by=m_y/BLOCKSIZE*BLOCKSIZE;

		xc=bx+(ed_endx-ed_beginx+1)*BLOCKSIZE;
		yc=by+(ed_endy-ed_beginy+1)*BLOCKSIZE;

		if(bx<0) bx=0;
		if(by<0) by=0;

		if(xc>SIZE_X-260) xc=(SIZE_X-260);
		if(yc>SIZE_Y) yc=(SIZE_Y);

		DrawRect(bx,by,xc,yc,255,0,0);

	}
//		sprintf(msg,"%u",ed_scut);
//		DrawNormalText(msg,m_x-20,m_y,1);



//	sprintf(msg,"Rand %u TargetX %d  %s Time %u",randseed,iTargetx,b_name[iTarget],lTimer);
//	sprintf(msg,"Rand %u %s (%d) Time %u ",randseed,b_localename[iTargetbuilding],iTargetbuilding, lTimer);
	sprintf(msg,"%s",b_localename[iTargetbuilding]) ;//, city_name[plr_currentcity]);
//	if(ed_building[0]!=0) 
		sprintf(msg,"%s",ed_building);
	DrawNormalText(msg,10,10,0);
}


void DrawBlockData()
{
	short mapx,mapy,icolor,endtemp,max_y;
	bool blockok;
	char msg[50];

	max_y=SIZE_Y/BLOCKSIZE;

	DrawFillRect(SIZE_X-120,0,SIZE_X,115,cBaseColours[0],cBaseColours[1],cBaseColours[2]);

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

			sprintf(msg,"%u",mapcounted[mapx][mapy]);
			DrawNormalText(msg,SIZE_X-120,100,icolor);
		}
	}

}

void DrawSidebar()
{
	char msg[50];
	short ix,iy,ibx,iby,itx,ity;
	int iCounter;
//	double tempdir;
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



	// block selector begins
	DrawFillRect(SIZE_X-255,135,SIZE_X-5,405,cBaseColours[0]-50,cBaseColours[1]-50,cBaseColours[2]-50);
	ibx=0;
	iby=200;

	itx=0;
	ity=0;

	for(ix=0;ix<MAXBLOCKS;ix++)
	{
		if(block_penres[ix]>0)
		{
			ShowBMP1(ibx,iby,20,20,SIZE_X-250+itx,140+ity);
			if(ed_block==ix) 
			{
				/*
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

  */
				DrawRect(SIZE_X-248+itx,142+ity,SIZE_X-234+itx,157+ity,255,0,0);

				if(m_x>SIZE_X-260)
				{
				ShowBMP1(ibx,iby,20,20,SIZE_X-120,5);
				sprintf(msg,"%s",block_name[ix]);
				DrawNormalText(msg,SIZE_X-120,30,iTextColor);

				sprintf(msg,"%s %u",gametxt[516],block_end[ix]);		// end
				DrawNormalText(msg,SIZE_X-120,50,iTextColor);

				sprintf(msg,"%s $%u",gametxt[517],block_cost[ix]);		// cost
				DrawNormalText(msg,SIZE_X-120,70,iTextColor);
				}

//				DrawFillRect(0,0,SIZE_X,100,255,0,0);
			}
		}

		ibx+=20;
		if(ibx>=800) {ibx=0;iby+=20;}

		itx+=20;
		if(itx>230) {itx=0;ity+=20;}
	}
	// block selector ends

	sprintf(msg,"");
	if(ed_bsingle==true)
	{
		DrawRect(SIZE_X-255,405,SIZE_X-226,434,cBaseColours[0]+50,cBaseColours[1]+50,cBaseColours[2]+50);
		sprintf(msg,gametxt[520]);		// single block
	}
	else
	{
		DrawButton(SIZE_X-255,405,SIZE_X-225,435);
	}
	ShowBMP1(550,550,20,20,SIZE_X-250,410);


	if(ed_bfill==true)
	{
		DrawRect(SIZE_X-225,405,SIZE_X-196,434,cBaseColours[0]+50,cBaseColours[1]+50,cBaseColours[2]+50);
		sprintf(msg,gametxt[521]);	
	}
	else
	{
		DrawButton(SIZE_X-225,405,SIZE_X-195,435);
	}
	ShowBMP1(570,550,20,20,SIZE_X-220,410);


	if(ed_brectangle==true)
	{
		DrawRect(SIZE_X-195,405,SIZE_X-166,434,cBaseColours[0]+50,cBaseColours[1]+50,cBaseColours[2]+50);
		sprintf(msg,gametxt[522]);
	}
	else
	{
		DrawButton(SIZE_X-195,405,SIZE_X-165,435);
	}
	ShowBMP1(590,550,20,20,SIZE_X-190,410);



	if(ed_bfrectangle==true)
	{
		DrawRect(SIZE_X-165,405,SIZE_X-136,434,cBaseColours[0]+50,cBaseColours[1]+50,cBaseColours[2]+50);
		sprintf(msg,gametxt[523]);
	}
	else
	{
		DrawButton(SIZE_X-165,405,SIZE_X-135,435);
	}
	ShowBMP1(610,550,20,20,SIZE_X-160,410);


	if(ed_bbuilding==true)
	{
		DrawRect(SIZE_X-135,405,SIZE_X-106,434,cBaseColours[0]+50,cBaseColours[1]+50,cBaseColours[2]+50);
		sprintf(msg,gametxt[524]);

		DrawBMPButton(gametxt[529],SIZE_X-250,460,4);		// select building
		DrawNormalText(b_localename[ed_buildsel],SIZE_X-250,490,iTextColor);
	}
	else
	{
		DrawButton(SIZE_X-135,405,SIZE_X-105,435);
	}
	ShowBMP1(630,550,20,20,SIZE_X-130,410);



	if(ed_btarget==true)
	{
		DrawRect(SIZE_X-105,405,SIZE_X-76,434,cBaseColours[0]+50,cBaseColours[1]+50,cBaseColours[2]+50);
		sprintf(msg,gametxt[525]);
	}
	else
	{
		DrawButton(SIZE_X-105,405,SIZE_X-75,435);
	}
	ShowBMP1(650,550,20,20,SIZE_X-100,410);


	if(ed_scut>0)
	{
		DrawRect(SIZE_X-75,405,SIZE_X-46,434,cBaseColours[0]+50,cBaseColours[1]+50,cBaseColours[2]+50);
		sprintf(msg,gametxt[526]);

		if(ed_bcopy==true)
		{
			DrawTextRect(gametxt[538],SIZE_X-250,460,SIZE_X-132,489);
			DrawBMPButton(gametxt[537],SIZE_X-130,460,3);		// cut
		}
		else
		{
			DrawTextRect(gametxt[537],SIZE_X-130,460,SIZE_X-12,489);
			DrawBMPButton(gametxt[538],SIZE_X-250,460,3);		// copy
		}

/*		for(ix=0;ix<4;ix++)
		{
			DrawRect(SIZE_X-255+ix*25,460,SIZE_X-230+ix*25,485,cBaseColours[0]+50,cBaseColours[1]+50,cBaseColours[2]+50);
			ShowBMP1(710+ix*20,550,20,20,SIZE_X-252+ix*25,462);
		}*/

	}
	else
	{
		DrawButton(SIZE_X-75,405,SIZE_X-45,435);
	}
	ShowBMP1(670,550,20,20,SIZE_X-70,410);


	if(ed_bassist==true)
	{
		DrawRect(SIZE_X-45,405,SIZE_X-16,434,cBaseColours[0]+50,cBaseColours[1]+50,cBaseColours[2]+50);
	}
	else
	{
		DrawButton(SIZE_X-45,405,SIZE_X-15,435);
	}
	ShowBMP1(690,550,20,20,SIZE_X-40,410);



	DrawNormalText(msg,SIZE_X-255,440,1);	// tool name

	if(ed_bbuilding==false && ed_scut==0)
	{
		DrawNormalText(gametxt[507],SIZE_X-255,460,iTextColor);//Show dropping blocks
		if(ed_bshowdropping==true)
			DrawBMPButton("",SIZE_X-60,460,6);
		else
			DrawBMPButton("",SIZE_X-60,460,7);
	}


//	ShowBMP1(550,550,160,20,SIZE_X-250,410);


//	DrawBMPButton(gametxt[77],SIZE_X-250,480,4);		// financial decisions

	if(BLOCKSIZE==10) sprintf(msg,gametxt[78]);		// zoom in
	if(BLOCKSIZE==20) sprintf(msg,gametxt[79]);		// zoom out

	DrawBMPButton(msg,SIZE_X-250,510,3);		// zoom button
	DrawBMPButton(gametxt[528],SIZE_X-130,510,3);		// map settings


	DrawBMPButton(gametxt[536],SIZE_X-250,540,3);	// remake pipes 
	if(ed_bsaveok==false) DrawBMPButton(gametxt[527],SIZE_X-130,540,3);	// test map 

/*	if(lTimer==0)	// finish/cancel buttons only visible when not exploding
	{
		if(boolJobSuccess==true)
			DrawBMPButton(gametxt[83],SIZE_X-250,540,4);	//Finish this site
		else
		{
		if(boolJobDone==false && boolMapgenerated==true)
			DrawBMPButton(gametxt[10],SIZE_X-250,540,4);		//Cancel job
		}
	}
*/

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
}

void DrawMapSettings()
{
	short i;
	char msg[10];

	DrawButton(xmiddle-250,ymiddle-250,xmiddle+250,ymiddle+250);

	DrawNormalText(gametxt[29],xmiddle-240,ymiddle-240,iTextColor);	// building
	DrawFillRect(xmiddle-100,ymiddle-240,xmiddle+240,ymiddle-223,cBaseColours[0]-50,cBaseColours[1]-50,cBaseColours[2]-50);
	DrawNormalText(ed_building,xmiddle-95,ymiddle-238,iTextColor);

	DrawNormalText(gametxt[27],xmiddle-240,ymiddle-220,iTextColor);	// customer
	DrawFillRect(xmiddle-100,ymiddle-220,xmiddle+240,ymiddle-203,cBaseColours[0]-50,cBaseColours[1]-50,cBaseColours[2]-50);
	DrawNormalText(ed_customer,xmiddle-95,ymiddle-218,iTextColor);

	DrawNormalText(gametxt[36],xmiddle-240,ymiddle-200,iTextColor);	// city
	DrawNormalText(city_name[ed_currentcity],xmiddle-55,ymiddle-200,iTextColor);
	DrawBMPButton("",xmiddle-100,ymiddle-200,10);	//up
	DrawBMPButton("",xmiddle-80,ymiddle-200,11);	//down

	DrawNormalText(gametxt[200],xmiddle-240,ymiddle-180,iTextColor);	// messages

	DrawFillRect(xmiddle-205,ymiddle-163,xmiddle+205,ymiddle-10,cBaseColours[0]-50,cBaseColours[1]-50,cBaseColours[2]-50);
	for(i=0;i<10;i++)
	{
		if(i%2==0)
			DrawFillRect(xmiddle-205,ymiddle-160+i*15,xmiddle+205,ymiddle-145+i*15,cBaseColours[0]-50,cBaseColours[1]-50,cBaseColours[2]-50);
		else
			DrawFillRect(xmiddle-205,ymiddle-160+i*15,xmiddle+205,ymiddle-145+i*15,cBaseColours[0]-35,cBaseColours[1]-35,cBaseColours[2]-35);

		sprintf(msg,"%2u",i+1);
		DrawNormalText(msg,xmiddle-230,ymiddle-160+i*15,iTextColor);
		DrawNormalText(ed_message[i],xmiddle-200,ymiddle-160+i*15,iTextColor);
	}

	DrawBMPButton(gametxt[539],xmiddle-150,ymiddle+100,5);	// clear messages 
	DrawBMPButton(gametxt[530],xmiddle-150,ymiddle+140,5);	// clear all 

	DrawBMPButton(gametxt[3],xmiddle-150,ymiddle+180,5);	// ok
}

void DrawBuildSel()
{
	short ix;
//	char msg[300];

	DrawButton(xmiddle-250,ymiddle-370,xmiddle+250,ymiddle+370);
	DrawFillRect(xmiddle-240,ymiddle-360,xmiddle+240,ymiddle+320,cBaseColours[0]-50,cBaseColours[1]-50,cBaseColours[2]-50);

	for(ix=0;ix<BUILDINGS;ix++)
	if(b_localename[ix][0]!=0)
	{
		if(ed_buildsel==ix)
			DrawTextWBack(b_localename[ix],xmiddle-230,ymiddle-360+ix*15,iTextColor,cBaseColours[0]+20,cBaseColours[1]+20,cBaseColours[2]+20);
		else
			DrawNormalText(b_localename[ix],xmiddle-230,ymiddle-360+ix*15,iTextColor);
	}
	
//	sprintf(msg,"%s %s %d",b_name[ed_buildsel],b_localename[ed_buildsel],ed_buildsel);
//	DrawNormalText(msg,xmiddle,ymiddle-400,iTextColor);

	DrawBMPButton(gametxt[3],xmiddle-120,ymiddle+330,4);	// ok
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
	char msg[50];
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
		break;
	case 2:
		DrawMenus();
		break;
	case 3:
		DrawMap();
//		DrawSidebar();
		DrawMapSettings();
		DrawSmallMap();
		break;
	case 4:
		DrawMap();
		DrawBuildSel();
		DrawSmallMap();
		
		break;
	default:break;
	}

}

/*
if(GameOn==1) 
{
	DrawBlockData();		// not needed???!
}
*/

//	if(bTutorial==true) DrawTutorial();

	Actions();		// draws also inputboxes etc.

	if(GameOn==6)	// search map
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

	DrawButton(xmiddle-150,ymiddle-100,xmiddle+150,ymiddle+100);
//	DrawFillRect(xmiddle-100,ymiddle+20,xmiddle+100,ymiddle+40,cBaseColours[0]-50,cBaseColours[1]-50,cBaseColours[2]-50);
//	DrawFillRect(xmiddle-100,ymiddle+20,xmiddle-100+(short) dProg,ymiddle+40,cBaseColours[0]+50,cBaseColours[1]+50,cBaseColours[2]+50);

	ShowBMP3(100,100,200,20,xmiddle-100,ymiddle+20);			// progress, not ready
	ShowBMP3(100,120,(short) dProg,20,xmiddle-100,ymiddle+20);			// progress, ready

	DrawNormalText(gametxt[149],xmiddle-140,ymiddle-70,iTextColor);
//	DrawNormalText(msg,xmiddle-140,ymiddle-50,iTextColor);

	Update();
}
