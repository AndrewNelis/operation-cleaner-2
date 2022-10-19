#include <stdlib.h>
//#include "SDL.h"   /* All SDL App's need this */
#include "OC2b.h"

extern char				GameOn;

//extern bool				boolMapgenerated;

char					b_name[BUILDINGS][255];
short					b_freq[BUILDINGS];		// frequency
unsigned long			lb_freq;

/*char					b_name[BUILDINGS][50];
unsigned char			b_element[BUILDINGS];
unsigned char			b_size_min[BUILDINGS];
unsigned char			b_size_max[BUILDINGS];
unsigned char			b_floors_min[BUILDINGS];
unsigned char			b_floors_max[BUILDINGS];
unsigned char			b_extras[BUILDINGS];
unsigned char			b_possibility[BUILDINGS];
unsigned char			b_basetype[BUILDINGS];
*/
int						ipossibility;

extern unsigned char	map[MAPSIZE][MAPSIZE];
extern bool				maptarget[MAPSIZE][MAPSIZE];
//extern int				mapbegin_x,mapbegin_y;

//int						b_size_x,b_size_y,b_floors,b_elevators,b_balconies,floorheight;
//int						iBuildingType,iBuilt;
//int						iTargetBuildingType;
//int						i_Groundmaxy,i_Groundminy;

//int						iTargetx,begin_x,iTarget,iTargetendx;
extern unsigned int		randseed;
//extern char				iDrawSmallMap,cDraw;

void AddExtras()
{
short ix,iy,iasphalt,iroof,iextras,i=0, itries;
short asphaltx[MAPSIZE];
short asphalty[MAPSIZE];
short roofx[MAPSIZE];
short roofy[MAPSIZE];

for(ix=0;ix<MAPSIZE;ix++)
{
	asphaltx[ix]=0;
	asphalty[ix]=0;
	roofx[ix]=0;
	roofy[ix]=0;
}

iasphalt=0;
iroof=0;
ix=1;
itries=0;


do
{
	iy=MAPSIZE-1;

	while(MapBlock(ix,iy)==0) iy--;

	if(MapBlock(ix,iy)==29)
	{
		roofx[iroof]=ix;
		roofy[iroof]=iy+1;
		iroof++;
	}

	if(MapBlock(ix,iy)==60 && MapBlock(ix+1,iy)==60 && MapBlock(ix+2,iy)==60)
	if(MapBlock(ix,iy+1)==0 && MapBlock(ix+1,iy+1)==0 && MapBlock(ix+2,iy+1)==0)
	{
		asphaltx[ix]=ix;
		asphalty[ix]=iy+1;
/*
		asphaltx[ix+1]=ix+1;
		asphalty[ix+1]=iy+1;

		asphaltx[ix+2]=ix+2;
		asphalty[ix+2]=iy+1;
*/
		iasphalt+=3;
		ix+=2;
	}

	ix++;
} while(ix<MAPSIZE-3);

if(iasphalt>0)
{
	iextras=(rand()%iasphalt)/2+1;

	do
	{
		ix=rand()%MAPSIZE; // iasphalt;	// x
		iy=rand()%14;		// type
		i=0;

		if(asphaltx[ix]>0)
		{
		switch(iy)
		{
		case 0:PutMap(asphaltx[ix],asphalty[ix],51);PutMap(asphaltx[ix],asphalty[ix]-1,40);break;	// water post + pipe
		case 1:PutMap(asphaltx[ix],asphalty[ix],52);PutMap(asphaltx[ix],asphalty[ix]+1,52);PutMap(asphaltx[ix],asphalty[ix]+2,52);PutMap(asphaltx[ix],asphalty[ix]+3,53);break;	// telephone pole
		case 2:PutMap(asphaltx[ix],asphalty[ix],127);PutMap(asphaltx[ix]+1,asphalty[ix],128);asphaltx[ix+1]=0;break;	// car
		case 3:PutMap(asphaltx[ix],asphalty[ix],129);PutMap(asphaltx[ix]+1,asphalty[ix],130);asphaltx[ix+1]=0;break;	// car
		case 4:PutMap(asphaltx[ix],asphalty[ix],131);PutMap(asphaltx[ix]+1,asphalty[ix],132);asphaltx[ix+1]=0;break;	// car
		case 5:PutMap(asphaltx[ix],asphalty[ix],139);PutMap(asphaltx[ix],asphalty[ix]+1,139);PutMap(asphaltx[ix],asphalty[ix]+2,140);break;	// stop
		case 6:PutMap(asphaltx[ix],asphalty[ix],139);PutMap(asphaltx[ix],asphalty[ix]+1,139);PutMap(asphaltx[ix],asphalty[ix]+2,141);break;	// parking
		case 7:PutMap(asphaltx[ix],asphalty[ix],139);PutMap(asphaltx[ix],asphalty[ix]+1,139);PutMap(asphaltx[ix],asphalty[ix]+2,142);break;	// no parking
		case 8:PutMap(asphaltx[ix],asphalty[ix],139);PutMap(asphaltx[ix],asphalty[ix]+1,139);PutMap(asphaltx[ix],asphalty[ix]+2,143);break;	// no stopping
		case 9:PutMap(asphaltx[ix],asphalty[ix],139);PutMap(asphaltx[ix],asphalty[ix]+1,139);PutMap(asphaltx[ix],asphalty[ix]+2,144);break;	// lamp
		case 10:PutMap(asphaltx[ix],asphalty[ix],139);PutMap(asphaltx[ix],asphalty[ix]+1,139);PutMap(asphaltx[ix],asphalty[ix]+2,139);PutMap(asphaltx[ix],asphalty[ix]+3,145);break;	// lamp
		case 11:PutMap(asphaltx[ix],asphalty[ix],139);PutMap(asphaltx[ix],asphalty[ix]+1,139);PutMap(asphaltx[ix],asphalty[ix]+2,139);PutMap(asphaltx[ix],asphalty[ix]+3,146);break;	// lamp
		case 12:	// fence
			i=asphaltx[ix];
			while(i>0 && MapBlock(i,asphalty[ix]-1)==60 && MapBlock(i,asphalty[ix])==0 && i<MAPSIZE-2)
			{
			PutMap(i,asphalty[ix],23);
			PutMap(i,asphalty[ix]+1,24);
			asphaltx[i]=0;
			i++;
			}

			break;
		case 13:	// chain fence
			i=asphaltx[ix];
			while(i>0 && MapBlock(i,asphalty[ix]-1)==60 && MapBlock(i,asphalty[ix])==0 && i<MAPSIZE-2)
			{
			PutMap(i,asphalty[ix],25);
			PutMap(i,asphalty[ix]+1,25);
			asphaltx[i]=0;
			i++;
			}
			break;
		default:break;
		}

		asphaltx[ix]=0;
		iextras--;
		itries=0;
		}

		itries++;
		if(itries>100) iextras=0;

	} while(iextras>0);

}	// iasphalt

if(iroof>0)
{
	iextras=(rand()%iroof)/10;

	do
	{
		ix=rand()%iroof;
		iy=rand()%100;

		if(iy%2==0)
			PutMap(roofx[ix],roofy[ix],54);		// satellite dish
		else
			PutMap(roofx[ix],roofy[ix],55);		// satellite dish

		if(maptarget[roofx[ix]][roofy[ix]-1]==true) maptarget[roofx[ix]][roofy[ix]]=true;

		iextras--;
	} while(iextras>0);
}

}

void BuildPipes()
{
	short ipx,ipy,iPipebeginy,i,iOk,j1,j2,j,i2;

	iPipebeginy=0;

	ipx=0;
	for(ipy=MAPSIZE;ipy>0;ipy--)
	{
		if(MapBlock(0,ipy)>0) ipx++;
		if(ipx==5) iPipebeginy=ipy;
	}

	ipx=0;

	ipy=iPipebeginy;

	while(ipx<MAPSIZE)
	{
		PutMap(ipx,ipy,41);	// horizontal pipe

		iOk=0;
		i2=0;
		j=0;

		for(i=ipy+1;i<MAPSIZE;i++)	// make sure there is no building in the way
		{
			if(MapBlock(ipx,i)==40)
				{
				iOk=1;
				j=i;
				}
			if(iOk==0 && MapBlock(ipx,i)>0 && MapBlock(ipx,i)<40) i2=i;
		}

		if(i2>0 && iOk==1)
		{
			PutMap(ipx,j,71);
			PutMap(ipx,j+1,0);
			iOk=0;
		}

		if(iOk==1)
		{

			PutMap(ipx,ipy,46);		// section upwards
			i=ipy+1;

			while(MapBlock(ipx,i)!=40 && i<MAPSIZE)
			{
				PutMap(ipx,i,40);
				i++;
			}

			if(maptarget[ipx][i]==true)	// if targetbuilding -> pipes to next junction belong to targetarea
				while(MapBlock(ipx,i)==40)
				{
					maptarget[ipx][i]=true;
					i--;
				}
		}

		j=0;

		if(iOk==0 && ipx<MAPSIZE-3)
		for(i2=0;i2<3;i2++)
		{
		if(MapBlock(ipx+3,ipy+i2)<40) j=1;
		if(MapBlock(ipx+3,ipy+i2)>=114 && MapBlock(ipx+3,ipy+i2)<=126) j=1;
		}

		if(j==1)
		{

//			i=0;

			PutMap(ipx,ipy,43);	// down

			do
			{
				i=0;
				for(j1=0;j1<5;j1++)
				for(j2=0;j2<3;j2++)
				{
					if(MapBlock(ipx+j1,ipy+j2)<40) i=1;
					if(MapBlock(ipx+j1,ipy+j2)>=114 && MapBlock(ipx+j1,ipy+j2)<=126) i=1;
				}

				if(ipy<=3) i=0;

				ipy--;
				PutMap(ipx,ipy,40);	// vertical
			} while(i>0);


/*			for(i=0;i<2;i++)
			{
				ipy--;
				PutMap(ipx,ipy,40);	// vertical
			}
*/
			PutMap(ipx,ipy,44);	// bend to right
//			ipx++;
			iOk=20;
			i=0;
		}

		if(iOk==0)
		{
			iOk=0;
			for(i=0;i<6;i++)
			{
				if(MapBlock(ipx+i-3,ipy+5)>=64 && MapBlock(ipx+i-3,ipy+5)<=71) iOk++;
//				if(MapBlock(ipx+i-5,ipy+5)==71) iOk++;
			}
		}
		else
			iOk--;

		if(iOk==6 && i>0)
		{
			PutMap(ipx,ipy,45);		// bend upwards

			do
			{
			iOk=0;
			for(i=0;i<6;i++)
			{
			if(MapBlock(ipx+i-3,ipy+5)>=64 && MapBlock(ipx+i-3,ipy+5)<=71) iOk++;
//			if(MapBlock(ipx+i-4,ipy+5)==71) iOk++;
			}

//			if(iOk==4)
			{
				ipy++;
				PutMap(ipx,ipy,40);		// vertical
			}

			} while(iOk==6);

			PutMap(ipx,ipy,42);		// bend
		}

		ipx++;
	}
}
