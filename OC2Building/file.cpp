//#include "SDL/SDL.h"   /* All SDL App's need this */
#include "OC2b.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <glob.h>
#include <ctype.h>
/*
extern char						b_name[BUILDINGS][50];
extern unsigned char			b_element[BUILDINGS];
extern unsigned char			b_size_min[BUILDINGS];
extern unsigned char			b_size_max[BUILDINGS];
extern unsigned char			b_floors_min[BUILDINGS];
extern unsigned char			b_floors_max[BUILDINGS];
extern unsigned char			b_extras[BUILDINGS];
extern unsigned char			b_possibility[BUILDINGS];
extern unsigned char			b_basetype[BUILDINGS];
*/
extern char						b_name[BUILDINGS][255];
char							b_localename[BUILDINGS][52];
extern short					b_freq[BUILDINGS];		// frequency
extern unsigned long			lb_freq;

extern char						block_name[MAXBLOCKS][30];
extern short					block_penres[MAXBLOCKS];
extern short					block_end[MAXBLOCKS];
extern unsigned short			block_cost[MAXBLOCKS];

//extern unsigned long			map_nr[MAXCAREERMAPS];
extern char						map_city[MAXCAREERMAPS];
extern char						map_nrc[MAXCAREERMAPS][52];

extern char						city_name[MAXCITIES][20];
extern char						city_description[MAXCITIES][5][52];

char							customer_name[MAXCUSTOMERS][12];
extern short					iTargetbuilding;
extern short					customer_nr;

extern unsigned char			city_price[MAXCITIES];
extern unsigned char			city_insurance[MAXCITIES];
extern unsigned char			city_payment[MAXCITIES];
extern short					city_x[MAXCITIES];
extern short					city_y[MAXCITIES];
/*
extern long						plr_money,plr_taxpaid;
extern short					plr_crew[4];
extern short					plr_reputation,plr_gametype,plr_currentcustomer;
extern long						plr_careful,plr_currentcity;
extern unsigned long			plr_jobsdone,plr_daysinbusiness,plr_lastspecial;
extern char						plr_name[40];
extern char						plr_usedmaps[200];			// career game maps
extern char						plr_advdecisions[4];
extern char						plr_slot,cSaveLoadFile;

extern unsigned char			cBaseColours[3];
extern char						iTextColor,iColOffset;
*/

/*char							cSaveGameName[MAXSAVEFILES][60];
char							cButtonFile[50];
unsigned char					cButtonFileList[100][50];

char							cButtonText[5][52];
char							cButtonAuthor[52];
extern short					sButtonBegin,sButtonSelected;
extern char						cOKBoxText[10][50];

extern char						cLanglist[100][50];
extern short					sLangBegin,sLangSelected;
*/
//extern char 					gametxt[MAXGAMETXT][52];
char							cLangfile[50];

//extern short					setSoundEffects,setDustClouds,setExplosions,setBits, setScroll, tmpScroll, setMaxFramerate;
extern short					SIZE_X,SIZE_Y,BLOCKSIZE;

extern char 					mapcode[MAXMAPCODE][1000];
extern short					mapcodelines;

extern short					hs_reputation[MAXHISCORE];
extern char						hs_player[MAXHISCORE][30];
extern char						hs_date[MAXHISCORE][12];
extern unsigned long			hs_money[MAXHISCORE];
extern unsigned long			hs_daysinbusiness[MAXHISCORE];
extern short					hs_gametype[MAXHISCORE];

// OC2ED begins
extern short					ed_currentcity;
extern char						ed_filename[52];
extern long						ed_mapfilenr;
extern char						ed_customer[52];
extern char						ed_building[52];

extern char						ed_message[10][52];

//extern short					ed_buildsel;

//extern char						ed_mapfile[MAXMAPFILES][100];

extern unsigned char			map[MAPSIZE][MAPSIZE];
extern short					mapend[MAPSIZE][MAPSIZE];

extern short					p_buildbegin, p_buildwidth;
short							tmpbuildbegin,tmpbuildwidth;

extern long						lTargetValue,lOtherValue;

//extern char						temp_ed_message[10][52];
//extern char						temp_ed_building[52];
// OC2ED ends

//static							FILE *stream;

int OpenFile(char const* s, char const* mode, short iErrors)
{
//char msg[500];
if( (stream  = fopen( s, mode )) == NULL )
	{
//	sprintf(msg,"%s %s (%s)",gametxt[202],s,mode);
//	if(iErrors==1) OKBox(msg,40);
	printf("Error opening file %s\n",s);
	//	sprintf(msg,"Error opening file %s",s);
	//	dialog(msg);
	return 1;
	//	errormsg("File open error %s.",txt);
	}
	else
	{
	return 0;
//	printf( "The file '%s' was opened\n",s );
	}

}

void CloseFile()
{
    if( fclose( stream ) )
	{
	printf( "The file was not closed\n");
	}
}

void GetBuildings(char iCode)	// 0=load all, 1=load only locale names
{
short i=0,l,iCounter;
unsigned char c;
char msg[20];
char fn[255];

if(iCode==0)
{
	if (b_name[0][0] == 0)
	{
		lb_freq = 0;

		glob_t globbuf;

		if (glob("buildings/*.map", 0, NULL, &globbuf) == 0) {

			for (int j=0; j<globbuf.gl_pathc; j++) {
				l = strlen(globbuf.gl_pathv[j]);
				// Path excluding ".map" extension
				strncpy(b_name[j], globbuf.gl_pathv[j], l - 4);
			}


		};

		globfree(&globbuf);
	}

	for(l=0;l<i;l++)
	{
			sprintf(fn,"buildings/%s.map",b_name[l]);
			if(OpenFile(fn,"rb",1)==0)
			{

				fgets(msg,14,stream);

				if(strncmp(msg,"SETFREQUENCY=",13)==0)
					fscanf(stream,"%d ",&b_freq[l]);
				else
					b_freq[l]=10;

				lb_freq+=b_freq[l];
				CloseFile();

//				printf("Building: %u %s %d\n",l,b_name[l],b_freq[l]);
			}
			else
				b_freq[l]=-1;
	}

}

// load language names for buildings

	iCounter=0;

	sprintf(fn,"languages/%s/buildings.txt",cLangfile);
	OpenFile(fn,"rb",1);

	while(!feof(stream))
	{
	fgets(b_localename[iCounter],52,stream);
	b_localename[iCounter][strlen(b_localename[iCounter])-1]=0;
	iCounter++;
	}

	CloseFile();
}

void LoadSettings()
{
	char cBFTemp[100];
	short i;

	if(OpenFile("settings.txt","r",1)==0)
	{
	fscanf(stream,"%u %u %u %u %u %u %u %u ",&i,&i,&i,&i,&i,&i,&i,&i);

//	setSounds=soundtemp;
	fgets(cBFTemp,50,stream);
	fgets(cLangfile,50,stream);
//	cLangfile[strlen(cLangfile)-1]=0;
	CloseFile();

//	LoadTheme(cButtonFile);

/*	sprintf(cBFTemp,"themes/%s.txt",cButtonFile);

	if(OpenFile(cBFTemp,"r")==0)
		{
		fscanf(stream,"%u %u %u ",&cBaseColours[0],&cBaseColours[1],&cBaseColours[2]);

		CloseFile();
		}
		else
		{
//			printf("%s not found. Using default colours and theme 'default'.\n",cBFTemp);
			sprintf(cButtonFile,"default");
		}
*/
	}
//	printf("X%u Y%u SND%u EXP%u DST%u SAV%u R%u G%u B%u BTN%s LNG%s\n",SIZE_X,SIZE_Y,setSoundEffects,setExplosions,setDustClouds,setSaveAllDoneMaps,cBaseColours[0],cBaseColours[1],cBaseColours[2],cButtonFile,cLangfile);
}


void GetBlockData(char iCode)	// 0=load all, 1=load only locale block names
{
	int iCounter;
	char cBfile[60];

	for(iCounter=0;iCounter<MAXBLOCKS;iCounter++)
	{
	block_name[iCounter][0]='\0';
	if(iCode==0)
		{
		block_cost[iCounter]=0;
		block_end[iCounter]=0;
		block_penres[iCounter]=0;
		}
	}

	// load block names from language file
	iCounter=0;

//	sprintf(cBfile,"languages/%s/blocks.txt",cLangfile);
//	OpenFile(cBfile,"rb",1);

//	while(block_penres[iCounter]==0)
/*	while(!feof(stream))
	{
	fgets(block_name[iCounter],30,stream);
	block_name[iCounter][strlen(block_name[iCounter])-1]=0;
	iCounter++;
	}

	CloseFile();
*/
	if(iCode==0)
	{
		// load block data
		iCounter=0;
		sprintf(cBfile,"data/blockdata.txt");
		OpenFile(cBfile,"rb",1);

		while(block_penres[iCounter]==0)
		{
	//	fgets(block_name[iCounter],20,stream);

		fscanf(stream,"%d %d %d ",&block_penres[iCounter],&block_end[iCounter],&block_cost[iCounter]);
		iCounter++;
		}

		CloseFile();
	}

//	for(iCounter=0;iCounter<MAXBLOCKS;iCounter++)
//		printf("%d %s\n",iCounter,block_name[iCounter]);

}


void LoadMapCode(char * file)		// loads 'script' of building to memory for execution
{
	short i,iToolong;
//	char msg[300];

	for(i=0;i<MAXMAPCODE;i++) sprintf(mapcode[i],"");

	i=0;
	iToolong=0;

	if(OpenFile(file,"r",1)==0)
	{
		while(!feof(stream))
		{
			if(i>MAXMAPCODE-2)
			{
//				sprintf(msg,"%s %s",gametxt[202],file);
//				OKBox(msg,40);

				printf("Too long map file!\n");
				iToolong=1;
				i=0;
			}

			fgets(mapcode[i],1000,stream);
			// _strupr(mapcode[i]);		// capitalize
			int l = strlen(mapcode[i]);
			for (int j=0; j<l; j++) {
				mapcode[i][j] = toupper(mapcode[i][j]);
			}
			// /capitalise replacement
			mapcode[i][strlen(mapcode[i])-1]=0;
			i+=2;

		}
	CloseFile();
	}
//	else
//		printf("Error opening file %s.\n",file);

	mapcodelines=i-1;

	if(iToolong==1)
	{
//		sprintf(msg,"%s %s",gametxt[202],file);
//		OKBox(msg,40);
		printf("Too long map file, not used!\n");
		for(i=0;i<MAXMAPCODE;i++) sprintf(mapcode[i],"");
		mapcodelines=0;
	}
//	printf("%d rows in mapcode.\n",mapcodelines);
}


void GetCustomers()
{
	int iCounter,ix;
	char cBfile[20];

	for(iCounter=0;iCounter<MAXCUSTOMERS;iCounter++)
	{
		customer_name[iCounter][0]=0;
	}

	iCounter=0;

	sprintf(cBfile,"data/customers.txt");
	OpenFile(cBfile,"rb",1);

	for(iCounter=0;iCounter<MAXCUSTOMERS;iCounter++)
	{
	fgets(customer_name[iCounter],12,stream);
	ix=strlen(customer_name[iCounter]);
	customer_name[iCounter][ix-1]=0;
	}

	CloseFile();
}

/* save & compress map file */
short SaveMap(char const* s)		// char *s NOT USED
{// illegal chars: \/:*?"<>|
	short ix,iy;
	// char *s2="";
/*
	for(ix=0;ix<(short) strlen(s);ix++)		// illegal chars -> _
		if(s[ix]==34 || s[ix]==42 || s[ix]==47 || s[ix]==58 || s[ix]==60 || s[ix]==62 || s[ix]==63 || s[ix]==92 || s[ix]==124) s[ix]=95;

	if(strlen(s)<1) return 1;		// no zero-lengths
*/
	if(OpenFile("maptemp.tmp","wb",0)==0)
	{
		for(ix=0;ix<MAPSIZE;ix++)
		for(iy=0;iy<MAPSIZE;iy++)
		{
		fprintf(stream,"%c",map[ix][iy]);
		}

		fprintf(stream," %u %u %u ", tmpbuildbegin, tmpbuildwidth,ed_currentcity);
		fprintf(stream,"%s\n%s\n", customer_name[customer_nr], ed_building);
		for(ix=0;ix<10;ix++)
		{
			ed_message[ix][strlen(ed_message[ix])-1]=0;
			fprintf(stream,"%s\n", ed_message[ix]);
		}

		CloseFile();

	return 0;
	}
	else
		return 1;

}


int LoadNeededFiles()
{
//	char cTemp[70];

	LoadSettings();
	GetBuildings(0);
	GetBlockData(0);
	GetCustomers();
	return 0;

}
