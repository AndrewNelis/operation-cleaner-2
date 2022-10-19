#include <math.h>
#include <time.h>
//#include <windows.h>
#include "SDL/SDL.h"   /* All SDL App's need this */
#include "oc2.h"

extern short			SIZE_X,SIZE_Y;
extern unsigned char	map[MAPSIZE][MAPSIZE];
extern short			mapend[MAPSIZE][MAPSIZE];

extern unsigned char	mapv[MAPSIZE][MAPSIZE];
extern int				mapsupport[MAPSIZE][MAPSIZE];
extern bool				maptarget[MAPSIZE][MAPSIZE];

extern bool				blocksmoving,boolJobSuccess;

extern short			block_penres[MAXBLOCKS];
extern short			block_end[MAXBLOCKS];

bool					boolExplode;

short					ec_x[MAXCHARGES];
short					ec_y[MAXCHARGES];
short					ec_dir[MAXCHARGES];
unsigned char			ec_type[MAXCHARGES];
short					ec_amount[MAXCHARGES]={1};
long					ec_timer[MAXCHARGES];					// in 1/100 seconds
bool					ec_primed[MAXCHARGES];

unsigned char			ec_selected;

short					ec_counter=0;

short					ep_x[MAXEXPLOSIONPARTICLES];
short					ep_y[MAXEXPLOSIONPARTICLES];
double					ep_dir[MAXEXPLOSIONPARTICLES];
short					ep_v[MAXEXPLOSIONPARTICLES];			// velocity
long					ep_e[MAXEXPLOSIONPARTICLES];			// explosive power, depending on amount & type
extern char				exp_selected;
extern int				exp_needed[MAXEXPLOSIVETYPES];

extern short			dp_x[MAXDUSTPARTICLES];	// dust particles
extern short			dp_y[MAXDUSTPARTICLES];
extern short			dp_v[MAXDUSTPARTICLES];
extern double			dp_dir[MAXDUSTPARTICLES];
short					dp_dirlc[MAXDUSTPARTICLES];		// last change

extern short			dustx[MAXDUST];
extern short			dusty[MAXDUST];
extern char				dustframe[MAXDUST];

long					lTimer,lLastDetonation,lExpParticles;
extern long				lLastSound;

extern int				mapbegin_x,mapbegin_y;
extern int				m_x,m_y;
extern int				m_down_x,m_down_y,m_up_x,m_up_y;

extern short			BLOCKSIZE;
extern char				iDrawSmallMap;

short					lastAddedx,lastAddedy,lastCharge;

extern long				lTargetValue;

extern char				cDraw;
extern short			setDustClouds,setSoundEffects;

extern bool				bTutorial;

extern char				gametxt[MAXGAMETXT][52];

extern double			dFps;

extern double			dp_cleanup;

void AddExplosion(short charge)
{
	short iparticle,iadd,iamount,iveldiv,iamdiv,iveldiv2;
	double diradd;

	iparticle=0;

/*	iamount=ec_amount[charge]*10;
	if(ec_amount[charge]>20) iamount=ec_amount[charge]*(10+ec_amount[charge]/10);
	if(iamount<100) iamount=100;
*/

	iamount=200+ec_amount[charge]*50;
//	if(iamount>=imaxexpparticles) iamount=imaxexpparticles;
//	if(ec_amount[charge]>20) iamount=ec_amount[charge]*(10+ec_amount[charge]/10);
//	if(iamount<300) iamount=300;

	while(ep_x[iparticle]>0 && iparticle<(MAXEXPLOSIONPARTICLES-(iamount*2)))
	{
		iparticle++;
	}

	iveldiv=150;	// in 1.0.3
	iamdiv=100;		// in 1.0.3
	iveldiv2=4;		// in 1.0.3

	if(ec_amount[charge]<6) {iveldiv=250;iamdiv=150;iveldiv2=6;}		// in 1.0.3
	if(ec_amount[charge]<3) {iveldiv=300;iamdiv=200;iveldiv2=7;}		// in 1.0.3

//	printf("P %d\n", iparticle);
	diradd=360.0;
	diradd/=double(iamount);

	if(ec_dir[charge]>=0) diradd=90.0/double(iamount);

	for(iadd=0;iadd<iamount;iadd++)
	if(iparticle<MAXEXPLOSIONPARTICLES-1)
	{
	ep_x[iparticle]=ec_x[charge];
	ep_y[iparticle]=ec_y[charge];
	ep_v[iparticle]=exp_velocity[ec_type[charge]]/iveldiv2+rand()%(exp_velocity[ec_type[charge]]/40);	// 2

//	ep_e[iparticle]=(exp_velocity[ec_type[charge]]/5)+(exp_velocity[ec_type[charge]]*ec_amount[charge]/20);		// original

//	ep_e[iparticle]=(exp_velocity[ec_type[charge]]/150)+(exp_velocity[ec_type[charge]]*ec_amount[charge]/100);	// working
	ep_e[iparticle]=(exp_velocity[ec_type[charge]]/iveldiv)+(exp_velocity[ec_type[charge]]*ec_amount[charge]/iamdiv);	// testing

	if(ec_amount[charge]>20) //ep_e[iparticle]=(exp_velocity[ec_type[charge]]/5)+(exp_velocity[ec_type[charge]]*(20+(ec_amount[charge]/5))/100);
	{
//		ep_e[iparticle]=(exp_velocity[ec_type[charge]]/25)+(exp_velocity[ec_type[charge]]*(20+(ec_amount[charge]/5))/500);	// original
//	ep_e[iparticle]=(exp_velocity[ec_type[charge]]/150)+(exp_velocity[ec_type[charge]]*ec_amount[charge]/100);	// test, same as in small explosives
	}

	if(ec_dir[charge]>=0)
		ep_dir[iparticle]=ec_dir[charge]+(diradd*iadd)-135;
	else
		ep_dir[iparticle]=(diradd*iadd)-90;				// X * iadd where X=360 / amount of particles

	if(ep_dir[iparticle]<0) ep_dir[iparticle]+=360;
	ep_dir[iparticle]*=rad;

	iparticle++;
	}

	ec_amount[charge]=0;
}

long MoveDustParticles(long iparticleamount)
{
	short iparticle,iadd,ix,iy,mapx,mapy;
	double addx,addy,ddir;
	long iamount=0;
//	char msg[30];

	for(iparticle=0;iparticle<MAXDUSTPARTICLES;iparticle++)
	{
	if(dp_v[iparticle]<=0)
	{
			dp_v[iparticle]=0;
			dp_x[iparticle]=0;
			dp_y[iparticle]=0;
			dp_dir[iparticle]=0;
	}
	else
	{
		iamount++;
		addx=cos(dp_dir[iparticle]);
		addy=sin(dp_dir[iparticle]);

		ix=dp_x[iparticle];
		iy=dp_y[iparticle];

//		for(iadd=0;iadd<dp_v[iparticle]/50;iadd++)
		{
			iadd=dp_v[iparticle]/50;
			dp_x[iparticle]=ix+short(iadd*addx);
			dp_y[iparticle]=iy+short(iadd*addy);

			mapx=dp_x[iparticle]/20;
			mapy=dp_y[iparticle]/20;

			if(mapx<1 || mapx>MAPSIZE-2) dp_v[iparticle]=0;
			if(mapy<1 || mapy>MAPSIZE-2) dp_v[iparticle]=0;

			if(map[mapx][mapy]>0)
			{
				dp_v[iparticle]=0;
			}
			else
			{
				if(dp_v[iparticle]>100)
				{
					if(dp_v[iparticle]>300)
						dp_v[iparticle]-=rand()%5;

					dp_v[iparticle]-=rand()%5;

					if(iparticleamount<1000)	// increase this value to get dust vanishing faster
						dp_v[iparticle]-=3;

					ddir=(dp_dir[iparticle]/rad)+90;
					if(ddir>=360) ddir-=360;

					if(dp_dirlc[iparticle]>2)
					{
					dp_dirlc[iparticle]=0;

					if(ddir<180)
						ddir-=(rand()%15+5);
					else
						ddir+=(rand()%15+5);
					}
					else
						dp_dirlc[iparticle]++;

					ddir-=90;
					if(ddir<0) ddir+=360;
					dp_dir[iparticle]=ddir*rad;

//				sprintf(msg,"%d dust v%d d%.2f",iparticle,dp_v[iparticle],ddir);
//				AddMessage(msg,0);
				}
				else
					dp_v[iparticle]=0;
			}

		}


	}
	}

//	sprintf(msg,"%d dust %d",lTimer,iamount);
//	AddMessage(msg,1);

return iamount;
}

void AddDustParticle(short px, short py, double pdir, short pvel)
{
	short ix=0;
	double tempvel;

	while(ix<MAXDUSTPARTICLES-1000 && dp_x[ix]>0) ix++;

	dp_x[ix]=px;
	dp_y[ix]=py;
	tempvel=pvel;
	tempvel*=0.75;
	if(tempvel>500) tempvel=500;
	if(tempvel<100) tempvel=100;
	dp_v[ix]=short(tempvel);

	pdir-=90;
	if(pdir>=360) pdir-=360;

	dp_dir[ix]=pdir*rad;
}

/*Move explosion particles*/
long MoveParticles()
{
	short iparticle,iadd,ix,iy,mapx,mapy,idst;
	double addx,addy,dtemp,ddirtemp;
	long iamount=0;

	for(iparticle=0;iparticle<MAXEXPLOSIONPARTICLES;iparticle++)
	if(ep_v[iparticle]>0)
	{
		ddirtemp=(ep_dir[iparticle]/rad)+90;
		iamount++;
		iadd=ep_v[iparticle]/20;
		addx=cos(ep_dir[iparticle]);
		addy=sin(ep_dir[iparticle]);

		ix=ep_x[iparticle];
		iy=ep_y[iparticle];

		for(iadd=0;iadd<ep_v[iparticle]/50;iadd++)
		if(ep_v[iparticle]>0)
		{
//			if(iparticle==20)
//			if(addx!=0 || addy!=0) printf("PARTICLE X %d (%1.1f)  Y %d (%1.1f)\n",ep_x[iparticle],addx,ep_y[iparticle],addy);

			ep_x[iparticle]=ix+short(iadd*addx);
			ep_y[iparticle]=iy+short(iadd*addy);

			mapx=ep_x[iparticle]/20;
			mapy=ep_y[iparticle]/20;

			if(mapx<1 || mapx>MAPSIZE-2) ep_v[iparticle]=0;
			if(mapy<1 || mapy>MAPSIZE-2) ep_v[iparticle]=0;

			if(ep_v[iparticle]>=0)
			{
				if(block_penres[map[mapx][mapy]]<ep_v[iparticle])	//ep_e[iparticle])
				{
					dtemp=block_penres[map[mapx][mapy]];
					dtemp/=10;

					if(mapend[mapx][mapy]>short(ep_e[iparticle]))
					{
						mapend[mapx][mapy]-=short(ep_e[iparticle]);
					}
					else
						mapend[mapx][mapy]=0;

					ep_e[iparticle]-=short(dtemp); //block_penres[map[mapx][mapy]];
					dtemp/=10;
					if(dtemp<1) dtemp=1;

/*					if(ep_dir[iparticle]<=90) ep_dir[iparticle]=abs(short(ep_dir[iparticle])-90)+90;
					if(ep_dir[iparticle]>90 && ep_dir[iparticle]<=180) ep_dir[iparticle]=abs(short(ep_dir[iparticle])-180)+180;
					if(ep_dir[iparticle]>180 && ep_dir[iparticle]<=270) ep_dir[iparticle]=abs(short(ep_dir[iparticle])-270)+180;
					if(ep_dir[iparticle]>270) ep_dir[iparticle]=abs(short(ep_dir[iparticle])-360)+180;
*/
					ep_v[iparticle]-=short(dtemp);
				}
				else
				{
					if(mapend[mapx][mapy]>short(ep_e[iparticle]/5))
					{
						mapend[mapx][mapy]-=short(ep_e[iparticle]/5);
					}
					else
						mapend[mapx][mapy]=0;

					ep_e[iparticle]=0;
					ep_v[iparticle]=0;
				}

			if(map[mapx][mapy]>0 && mapend[mapx][mapy]<=0)
				{
					if(block_end[map[mapx][mapy]]>200)
					{
//						ddirtemp+=rand()%40-20;
						if(ddirtemp<0) ddirtemp+=360;
						if(ddirtemp>=360) ddirtemp-=360;

						for(idst=0;idst<20;idst++) AddDustParticle(ep_x[iparticle]+rand()%20-10,ep_y[iparticle]+rand()%20-10,ddirtemp,ep_v[iparticle]+rand()%50-25);
					}
					map[mapx][mapy]=0;
					mapend[mapx][mapy]=block_end[0];
					mapv[mapx][mapy]=0;
					maptarget[mapx][mapy]=false;
				}

//			if(mapend[mapx][mapy]<block_end[map[mapx][mapy]])
//			printf("END %d %d %d\n",mapx,mapy,mapend[mapx][mapy]);
			}

		if(ep_e[iparticle]<=0 || ep_v[iparticle]<=300)
		{

			if(ep_e[iparticle]>0)
			{
				ddirtemp=(ep_dir[iparticle]/rad)+90;
				if(ddirtemp>360) ddirtemp-=360;
				AddDustParticle(ep_x[iparticle],ep_y[iparticle],ddirtemp,ep_v[iparticle]);
			}
				ep_v[iparticle]=0;
				ep_x[iparticle]=0;
				ep_y[iparticle]=0;
				ep_e[iparticle]=0;
		}


		}

//		ep_v[iparticle]-=500;
	}

//	printf("MAX %d\n",iparticlemax);
return iamount;
}


void AddExplosive()
{
//	char msg[30];
	short max_y,x,y,prevamount;
//	unsigned char cx;

	max_y=SIZE_Y/BLOCKSIZE;
/*
	if(ec_selected>=MAXCHARGES-2)	// find empty place for charge
	{
		y=0;
		for(cx=0;cx<MAXCHARGES;cx++)
		{
			if(ec_x[cx]==0 && y==0) {ec_selected=cx;y=1;}
		}

		if(y==0) ec_selected=1;
	}
*/
	if(ec_selected<MAXCHARGES-1 && (lExpParticles+200+ec_amount[ec_selected]*50)<MAXEXPLOSIONPARTICLES)
	{
		ec_counter=ec_selected;
	x=mapbegin_x*20+(m_down_x*20/BLOCKSIZE);
	y=(mapbegin_y+max_y)*20-(m_down_y*20/BLOCKSIZE);

//	printf("EXPL %d %d X %d Y %d\n",mapbegin_x,mapbegin_y,x,y);

	if(map[x/20][y/20]!=0)
	{

		if(abs(m_down_x-m_up_x)>10 || abs(m_down_y-m_up_y)>10)
			ec_dir[ec_selected]=CountAngle(m_up_x,m_down_y,m_down_x,m_up_y);
		else
			ec_dir[ec_selected]=-1;
/*
		sprintf(msg,"%u %u %u %u",m_up_x,m_down_y,m_down_x,m_up_y);
		AddMessage(msg,1);

		sprintf(msg,"%d ",ec_dir[ec_selected]);
		AddMessage(msg,1);
*/
		if(ec_x[ec_selected]>0)
		{
			ec_x[ec_selected]=x; //mapbegin_x*20+(m_down_x*20/BLOCKSIZE);
			ec_y[ec_selected]=y; //(mapbegin_y-max_y)*20+(m_down_y*20/BLOCKSIZE);
			lastAddedx=m_down_x;
			lastAddedy=m_down_y;
			prevamount=ec_amount[ec_selected];
			ec_counter++;
//			if(ec_counter<MAXCHARGES)
				ec_selected=int(ec_counter);
			PlayClick();

			if(ec_amount[ec_selected]==1) ec_amount[ec_selected]=prevamount;
		}
		else
		{
			if(abs(lastAddedx-x)>BLOCKSIZE || abs(lastAddedy-y)>BLOCKSIZE)
			{
				ec_x[ec_counter]=x; //mapbegin_x*20+(m_down_x*20/BLOCKSIZE);
				ec_y[ec_counter]=y; //(mapbegin_y-max_y)*20+(m_down_y*20/BLOCKSIZE);
				ec_type[ec_counter]=exp_selected;

				ec_counter++;
	//			if(ec_counter<MAXCHARGES)
					ec_selected=int(ec_counter);
				ec_amount[ec_counter]=ec_amount[ec_counter-1];
				ec_timer[ec_counter]=ec_timer[ec_counter-1];

				lastAddedx=x;
				lastAddedy=y;

				PlayClick();
	//			printf("Added %d\n",ec_counter);
			}
		}

	}

	}
	else
	{	//  can't add anymore
		PlayWav("media\\beep.wav");
		AddMessage(gametxt[159],2);
		AddMessage(gametxt[160],2);
	}
//	sprintf(msg,"X%d Y%d a%d",ec_x[ec_counter-1],ec_y[ec_counter-1],ec_amount[ec_counter-1]);
//	AddMessage(msg,2);
CountNeeded(1);

}

void ExplosionSound(short itype, long lMax)
{
	char mediafile[100];

	if(setSoundEffects==1)
	{
	if(lMax<20)
		sprintf(mediafile,"media/exp1.wav", itype);
	else
		sprintf(mediafile,"media/exp2.wav", itype);

	PlayWav(mediafile);
	}
}

void CountUnattached()
{
	short ix, iy;
	char msg[30];

	for(ix=1;ix<MAPSIZE-1;ix++)
	for(iy=1;iy<MAPSIZE-1;iy++)
		if(map[ix][iy]>0 && mapv[ix][iy]==0)
		{
			if(map[ix+1][iy]==0 && map[ix-1][iy]==0 && map[ix][iy+1]==0 && map[ix][iy-1]==0)
			{
				sprintf(msg,"ALERT %d %d",ix,iy);
				AddMessage("*******************",2);
				AddMessage(msg,2);
				AddMessage("*******************",2);

				blocksmoving=true;
				mapv[ix][iy]=1;
			}
		}
}



void HandleExplosives()
{
	short i_charge,i_particle,iExptocome,imaxtype=-1, idelay;
	long iparticleamount,lTempTargetValue,idustp,lMaxamount=0;
	double dTemp2;

	iExptocome=0;

	for(i_charge=0;i_charge<MAXCHARGES;i_charge++)
	if(ec_primed[i_charge]==true)
	{
		if(ec_x[i_charge]>0 && ec_timer[i_charge]==lTimer)
		{
			if(ec_type[i_charge]>imaxtype) imaxtype=ec_type[i_charge];
			lMaxamount+=ec_amount[i_charge];
			AddExplosion(i_charge);
//			printf("charge x%d y%d %d\n",ec_x[i_charge],ec_y[i_charge],ec_amount[i_charge]);
			ec_type[i_charge]=0;
			ec_amount[i_charge]=0;
		}

		if(ec_amount[i_charge]>0) iExptocome=1;
	}

	if(lLastDetonation<lTimer && imaxtype>=0) ExplosionSound(imaxtype+1,lMaxamount);
	lLastDetonation=lTimer;
//	lLastSound=lTimer;

	iparticleamount=MoveParticles();
	if(setDustClouds==1)
		idustp=MoveDustParticles(iparticleamount);
	else
		idustp=0;

//if(idustp<40 && idustp>10) if(lLastSound>0 && lTimer>0) PlayWav("media\\crash7.wav");	//if(lLastSound>0)

//	if(idustp>0 && cDraw==0) cDraw=1;

/*	char msg[30];

	sprintf(msg,"p %d dp %d",iparticleamount,idustp);// temp
	AddMessage(msg,1);
*/

	iparticleamount+=idustp;

	MapCount();

	HandleBlocks();

	idelay=2;
	if(dFps<25) idelay=10;

	if(lTimer%idelay==0)
	{
		iDrawSmallMap=1;
	}

	lTimer++;


	if(blocksmoving==false && iExptocome==0 && iparticleamount==0)	//lTimer>lLastDetonation+50 && //
	{
		CountUnattached();
		lTimer=0;
//		lLastDetonation=-1;
		boolExplode=false;
		lLastSound=0;

		for(i_charge=0;i_charge<MAXCHARGES;i_charge++)
		if(ec_type[i_charge]==0 && ec_amount[i_charge]==0)
		{
			ec_x[i_charge]=0;
			ec_y[i_charge]=0;
			ec_amount[i_charge]=1;
			ec_primed[i_charge]=false;
			ec_timer[i_charge]=0;
			ec_type[i_charge]=0;
			ec_dir[i_charge]=-1;
		}

		for(i_particle=0;i_particle<MAXEXPLOSIONPARTICLES;i_particle++)
		{
			ep_x[i_particle]=0;
			ep_y[i_particle]=0;
			ep_v[i_particle]=0;
			ep_e[i_particle]=0;
			ep_dir[i_particle]=0;
		}
	ec_counter=0;
	ec_selected=0;

	dTemp2=lTargetValue;	// version 1.1.2
	dTemp2*=(100-dp_cleanup/2);
	dTemp2/=100;

	lTempTargetValue=TargetValue();

	if(lTempTargetValue<long (dTemp2)) // lTargetValue)
	{
		boolJobSuccess=true;
	}

	ZeroExplosives();
	CountNeeded(0);
	}
}

short CountExplosives()
{
	short ix,iamount=0;

	lLastDetonation=-1;

	for(ix=0;ix<MAXCHARGES;ix++)
		if(ec_x[ix]>0 && ec_amount[ix]>0) iamount++;

	return iamount;
}

void ZeroExplosives()
{
	short ix;


	for(ix=0;ix<MAXCHARGES;ix++)
	{
		ec_x[ix]=0;
		ec_y[ix]=0;
		ec_dir[ix]=-1;
		ec_timer[ix]=0;
		ec_type[ix]=0;
		ec_amount[ix]=1;
		ec_primed[ix]=false;
	}

	for(ix=0;ix<MAXEXPLOSIONPARTICLES;ix++)
	{
		ep_x[ix]=0;
		ep_y[ix]=0;
		ep_v[ix]=0;
		ep_dir[ix]=0;
	}

	for(ix=0;ix<MAXDUSTPARTICLES;ix++)
	{
		dp_x[ix]=0;
		dp_y[ix]=0;
		dp_dir[ix]=0;
		dp_v[ix]=0;
	}

	for(ix=0;ix<MAXDUST;ix++)
	{
		dustx[ix]=0;
		dusty[ix]=0;
		dustframe[ix]=0;
	}

	lastAddedx=0;
	lastAddedy=0;

	lExpParticles=0;

/*	ec_x[0]=2615;
	ec_y[0]=872;
	ec_type[0]=1;
	ec_amount[0]=15;

	ec_x[1]=2650;
	ec_y[1]=1024;
	ec_type[1]=1;
	ec_amount[1]=15;

	ec_x[2]=2646;
	ec_y[2]=1163;
	ec_type[2]=1;
	ec_amount[2]=15;
*/
//charge x2615 y872 0
//charge x2650 y1024 0
//charge x2646 y1163 0

}
