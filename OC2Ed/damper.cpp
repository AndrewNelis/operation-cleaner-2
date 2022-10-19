#include "SDL.h"   /* All SDL App's need this */
#include "OC2.h"

extern int				m_up_x,m_up_y;

extern short			BLOCKSIZE,SIZE_Y;

extern unsigned char	map[MAPSIZE][MAPSIZE];
extern short			block_end[MAXBLOCKS];
extern short			mapend[MAPSIZE][MAPSIZE];

extern int				mapbegin_x,mapbegin_y;

short					iDampermode;
short					iDamperbeginx,iDamperbeginy;

extern char				gametxt[MAXGAMETXT][52];
extern char				iTextColor;

extern bool				bTutorial;

void AddDampers()
{
	short change,failure=0,ix,iy;
	short iDamperendx,iDamperendy;
	bool bEmpty,bRemoveDampers;
	short max_y=SIZE_Y/BLOCKSIZE;
	char msg[30];


	iDamperendx=mapbegin_x+(m_up_x/BLOCKSIZE);
//	iDamperendy=mapbegin_y+(m_up_y/BLOCKSIZE);
	iDamperendy=(mapbegin_y+max_y-1)-(m_up_y/BLOCKSIZE); 

	if(iDamperendx<iDamperbeginx)
	{
		change=iDamperendx;
		iDamperendx=iDamperbeginx;
		iDamperbeginx=change;
	}
	if(iDamperendy>iDamperbeginy)
	{
		change=iDamperendy;
		iDamperendy=iDamperbeginy;
		iDamperbeginy=change;
	}

//	sprintf(msg,"%d %d - %d %d\n",iDamperbeginx,iDamperbeginy,iDamperendx,iDamperendy);
//	AddMessage(msg,1);

	bEmpty=false;
	bRemoveDampers=false;

	for(ix=iDamperbeginx;ix<=iDamperendx;ix++)
	{
		if(map[ix][iDamperendy-1]==0) bEmpty=true;
	}

	if(bEmpty==false)
	{
		for(ix=iDamperbeginx;ix<=iDamperendx;ix++)
		for(iy=iDamperbeginy;iy>=iDamperendy;iy--)
		{
			switch(map[ix][iy])
			{
			case 0:
				map[ix][iy]=56;
				mapend[ix][iy]=block_end[56];
				break;
			default:
				failure++;
				break;
			}
		}
		if(bTutorial==true) MoveToNextTutorial(4);
	}
	else
	{
		AddMessage(gametxt[34],2);	// cant place on air
	}

	if(failure>0) 
	{
		sprintf(msg,"%s %u",gametxt[35],failure);	// nr of dampers not placed
		AddMessage(msg,iTextColor);
	}

}

bool CheckConnection(short ix, short iy)
{
bool bOk=false;

	if(map[ix][iy]==56 && mapend[ix][iy]==block_end[56])
	{
		if(ix<MAPSIZE-1 && map[ix+1][iy]==56 && mapend[ix+1][iy]==0) bOk=true;
		if(ix>0 && map[ix-1][iy]==56 && mapend[ix-1][iy]==0) bOk=true;
		if(iy<MAPSIZE-1 && map[ix][iy+1]==56 && mapend[ix][iy+1]==0) bOk=true;
		if(iy>0 && map[ix][iy-1]==56 && mapend[ix][iy-1]==0) bOk=true;
	}

	if(bOk==true)
	{
		mapend[ix][iy]=0;
		return true;
	}
	else
		return false;

}

bool CheckDampers()
{
	bool bdone;
	bool bremoved;
	short ix,iy,mapx,mapy,irange=1;

	if(map[iDamperbeginx][iDamperbeginy]==56 && mapend[iDamperbeginx][iDamperbeginy]==block_end[56]) mapend[iDamperbeginx][iDamperbeginy]=0;

	do
	{
		bdone=true;

		for(ix=0;ix<=irange*2;ix++)
		for(iy=0;iy<=irange*2;iy++)
		{
			mapx=iDamperbeginx-irange+ix;
			mapy=iDamperbeginy-irange+iy;

			if(mapend[mapx][mapy]>0)
			if(CheckConnection(mapx,mapy)==true)
			{
				bdone=false;
			}
		}

		irange++;

	} while(bdone==false);

	bremoved=false;

	for(ix=0;ix<MAPSIZE;ix++)
	for(iy=0;iy<MAPSIZE;iy++)
		if(map[ix][iy]==56 && mapend[ix][iy]==0) 
		{
			map[ix][iy]=0;
			mapend[ix][iy]=block_end[0];
			bremoved=true;
		}

	return bremoved;

}