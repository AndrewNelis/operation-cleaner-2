#include "SDL/SDL.h"   /* All SDL App's need this */
#include "oc2.h"
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <glob.h>

short save_version=2;		// save file version number

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

extern char						customer_name[MAXCUSTOMERS][12];

extern unsigned char			city_price[MAXCITIES];
extern unsigned char			city_insurance[MAXCITIES];
extern unsigned char			city_payment[MAXCITIES];
extern short					city_x[MAXCITIES];
extern short					city_y[MAXCITIES];

extern long						plr_money,plr_taxpaid;
extern short					plr_crew[4];
extern short					plr_gametype,plr_currentcustomer;
extern long						plr_reputation,plr_careful,plr_currentcity;
extern unsigned long			plr_jobsdone,plr_daysinbusiness,plr_lastspecial;
extern char						plr_name[40];
extern char						plr_usedmaps[200];			// career game maps
extern char						plr_advdecisions[4];
extern char						plr_slot,cSaveLoadFile;

extern unsigned long			plrs_money[MAXPLAYERS][MAXSTATS];
extern unsigned long			plr_month;
extern unsigned char			plrs_name[MAXPLAYERS];
extern short					plr_monthcounter,plr_difficulty;

extern unsigned char			cBaseColours[3];
extern char						iTextColor,iColOffset;

char							cSaveGameName[MAXSAVEFILES][60];
char							cButtonFile[50];
unsigned char					cButtonFileList[100][50];

char							cButtonText[5][52];
char							cButtonAuthor[52];
extern short					sButtonBegin,sButtonSelected;
extern char						cOKBoxText[10][50];

extern char						cLangfile[50];
extern char						cLanglist[100][50];
extern short					sLangBegin,sLangSelected;

extern char 					gametxt[MAXGAMETXT][52];

extern short					setSoundEffects,setDustClouds,setExplosions,setBits, setScroll, tmpScroll, setMaxFramerate, setDifficulty;
extern short					SIZE_X,SIZE_Y,BLOCKSIZE;

extern char 					mapcode[MAXMAPCODE][1000];
extern short					mapcodelines;

extern short					hs_reputation[MAXHISCORE];
extern char						hs_player[MAXHISCORE][30];
extern char						hs_date[MAXHISCORE][12];
extern unsigned long			hs_money[MAXHISCORE];
extern unsigned long			hs_daysinbusiness[MAXHISCORE];
extern short					hs_gametype[MAXHISCORE];
extern short					hs_diff[MAXHISCORE];

// OC2ED begins
extern short					ed_currentcity;
extern char						ed_filename[52];
extern long						ed_mapfilenr;
extern char						ed_customer[52];
extern char						ed_building[52];
extern char						ed_message[10][52];

extern short					ed_buildsel;

extern char						ed_mapfile[MAXMAPFILES][100];

extern unsigned char			map[MAPSIZE][MAPSIZE];
extern short					mapend[MAPSIZE][MAPSIZE];

extern short					p_buildbegin, p_buildwidth;
short							tmpbuildbegin,tmpbuildwidth;

extern long						lTargetValue,lOtherValue;

extern char						temp_ed_message[10][52];
extern char						temp_ed_building[52];
// OC2ED ends


int OpenFile(char *s,char *mode, short iErrors)
{
char msg[500];
if( (stream  = fopen( s, mode )) == NULL )
	{
	sprintf(msg,"%s %s (%s)",gametxt[202],s,mode);
	if(iErrors==1) OKBox(msg,40);
//	printf("Error opening file %s\n",s);
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

void DeleteCurrentSaveFile()
{
	char sfile[20];
	char msg[100];

	if(cSaveLoadFile>=0)
	{
	sprintf(sfile,"save\\plr%02d.txt",cSaveLoadFile);

	if(remove(sfile) != 0)
	{
//		printf("Error deleting file %s!\n",sfile);
		sprintf(msg,"%s %s",gametxt[513],sfile);	//Can't delete file:
		OKBox(msg,40);
	}

	cSaveGameName[0][0]=0;
	LoadSaveGameHeaders();		// update the savegame header data
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
	if(b_name[0][0]==0)
	{

		lb_freq=0;

		glob_t globbuf;
		int glob_return = glob("buildings/*.map", 0, NULL, &globbuf);
		if (glob_return != 0) {
			printf("Glob buildings/*.map failed with code: %d\n", glob_return);
		}

		for (int i=0; i<globbuf.gl_pathc; i++) {
			l = strlen(globbuf.gl_pathv[i]);
			strncpy(b_name[i], globbuf.gl_pathv[i], l - 4);
		}
	}

	for(l=0;l<i;l++)
	{
			sprintf(fn,"buildings\\%s.map",b_name[l]);
			if(OpenFile(fn,"rb",1)==0)
			{

				fgets(msg,14,stream);

				if(strncmp(msg,"SETFREQUENCY=",13)==0)
					fscanf(stream,"%d ",&b_freq[l]);
				else
					b_freq[l]=10;

				lb_freq+=b_freq[l];
				CloseFile();
			}
			else {
				b_freq[l]=-1;
			}
	}

}

// load language names for buildings

	iCounter=0;

	sprintf(fn,"languages\\%s\\buildings.txt",cLangfile);
	OpenFile(fn,"rb",1);

	while(!feof(stream))
	{
	fgets(b_localename[iCounter],52,stream);
	b_localename[iCounter][strlen(b_localename[iCounter])-1]=0;
	iCounter++;
	}

	CloseFile();
}


void GetBlockData(char iCode)	// 0=load all, 1=load only locale block names
{
	int iCounter;
	char * cBfile="";

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

	sprintf(cBfile,"languages\\%s\\blocks.txt",cLangfile);
	OpenFile(cBfile,"rb",1);

//	while(block_penres[iCounter]==0)
	while(!feof(stream))
	{
	fgets(block_name[iCounter],30,stream);
	block_name[iCounter][strlen(block_name[iCounter])-1]=0;
	iCounter++;
	}

	CloseFile();

	if(iCode==0)
	{
		// load block data
		iCounter=0;
		sprintf(cBfile,"data\\blockdata.txt",cLangfile);
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

void GetMapData()
{
	short iCounter;
	short ix;

	for(iCounter=0;iCounter<MAXCAREERMAPS;iCounter++)
	{
		sprintf(map_nrc[iCounter],"%c",0);
//		map_nr[iCounter]=0;
		map_city[iCounter]=-1;
	}

	OpenFile("data\\maps.txt","rb",1);

	iCounter=1;

//	while(map_city[iCounter]!=-10)
	while(!feof(stream))
	{
//		fscanf(stream,"%u %d ",&map_nr[iCounter],&map_city[iCounter]);
		fscanf(stream,"%u ",&map_city[iCounter]);
		fgets(map_nrc[iCounter],50,stream);
//		map_nrc[iCounter][strlen(map_nrc[iCounter])]=0;
		for(ix=strlen(map_nrc[iCounter])-1;ix<52;ix++) map_nrc[iCounter][ix]=0;
		iCounter++;
	}

	CloseFile();

//	for(ix=1;ix<MAXCAREERMAPS;ix++)
//		printf("%u %u %s\n",ix,map_city[ix],map_nrc[ix]);
}

void GetCityData()
{
	short iCounter,iRow,i;
	char * cCfile="";

	for(iCounter=0;iCounter<MAXCITIES;iCounter++)
	{
		sprintf(city_name[iCounter],"");
		city_price[iCounter]=0;
		city_insurance[iCounter]=0;
		city_payment[iCounter]=0;
		city_x[iCounter]=0;
		city_y[iCounter]=0;
	}

//	sprintf(cCfile,"languages\\%s\\cities.txt",cLangfile);
	sprintf(cCfile,"data/cities.txt");
	OpenFile(cCfile,"rb",1);

	iCounter=0;
	do
	{
		fscanf(stream,"%d %d %d %d %d ",&city_price[iCounter],&city_insurance[iCounter],&city_payment[iCounter],&city_x[iCounter],&city_y[iCounter]);
//		fgets(city_name[iCounter],20,stream);
		iCounter++;
	} while(city_price[iCounter-1]!=0);

	CloseFile();
//	printf("city int ok\n");

	sprintf(cCfile,"languages/%s/cities.txt",cLangfile);
	OpenFile(cCfile,"rb",1);
	iCounter=0;

	while(!feof(stream))
	{
		fgets(city_name[iCounter],20,stream);
		for(iRow=0;iRow<5;iRow++)
		{
			fgets(city_description[iCounter][iRow],52,stream);
			i=strlen(city_description[iCounter][iRow]);
			city_description[iCounter][iRow][i-1]=0;
		}
		iCounter++;
	} //while(city_name[iCounter-1][0]!='#');

	CloseFile();

}

void LoadSaveGameHeaders()
{
	short save_tmp;
	char ix;
	char sfile[20];
	char tempname[35];
	char tempdate[22];

	if(cSaveGameName[0][0]==0)
	{
		for(ix=0;ix<MAXSAVEFILES;ix++)
		{
			sprintf(sfile,"save\\plr%02d.txt",ix);
			if(OpenFile(sfile,"rb",0)==0)
			{
				fscanf(stream,"%u ",&save_tmp);
				fgets(tempname,35,stream);
				fgets(tempdate,20,stream);
				sprintf(cSaveGameName[ix],"%2d %-31s %.14s",ix+1,tempname,tempdate);
				CloseFile();
			}
			else
				sprintf(cSaveGameName[ix],"%2d (Empty slot)",ix+1);
		}
	}

}

void SaveGame()
{
	char sfile[20];
	short ix,i;
	long lcheck,lcheck2;
	char cdate[20];
	struct tm *newtime;
	time_t aclock;

	time( &aclock );
	newtime = localtime( &aclock );
	strftime(cdate, 20, "%y-%m-%d %H:%M", newtime);

	sprintf(sfile,"save\\plr%02d.txt",plr_slot);

	OpenFile(sfile,"wb",1);

	fprintf(stream,"%u %.30s\r\n%s\r\n%u %u %u %u %u %u %d %u %u %u %u %u %u %u %u %u %u %u %u ",save_version,plr_name,cdate,plr_money,plr_taxpaid,
		plr_crew[0],plr_crew[1],plr_crew[2],plr_crew[3],plr_reputation,plr_careful,plr_gametype,plr_currentcity,
		plr_currentcustomer,plr_jobsdone,plr_daysinbusiness,plr_lastspecial,plr_advdecisions[0],plr_advdecisions[1],
		plr_advdecisions[2],plr_advdecisions[3],plr_difficulty);

	lcheck=0;
	ix=0;

	do
	{
//		printf("%c %u ",plr_name[ix],plr_name[ix]);

		if(plr_name[ix]==13) plr_name[ix]=0;

		if(ix%3==0)
		lcheck-=plr_name[ix];
			else
		lcheck+=(plr_name[ix]+ix*ix);
		ix++;
	} while(plr_name[ix]!=0 && plr_name[ix]!=13 && ix<30);


	lcheck2=plr_money+plr_taxpaid+plr_reputation+plr_careful+plr_currentcity+plr_jobsdone+plr_daysinbusiness+plr_lastspecial;

	if(plr_gametype==0)
	{
		for(ix=0;ix<200;ix++)
		{
			fprintf(stream,"%u ",plr_usedmaps[ix]);
			lcheck+=plr_usedmaps[ix]+1;
		}
	}

	for(i=0;i<MAXPLAYERS;i++)	// version 1.1, not compatible with previous version!
	{
	fprintf(stream,"%u ",plrs_name[i]);

		for(ix=0;ix<MAXSTATS;ix++)
		{
			fprintf(stream,"%u ",plrs_money[i][ix]);
		}

	}
	fprintf(stream,"%u %u ",plr_month,plr_monthcounter);

//	printf("SAVE lcheck %u lcheck2 %u\n",lcheck,lcheck2);
	lcheck*=lcheck2;

	fprintf(stream,"%u",lcheck);
//	printf("lcheck SAVE %u\n",lcheck);

	CloseFile();

	cSaveGameName[0][0]=0;
}

bool LoadGame()
{
	short save_tmp;
	char sfile[20];
	short ix,i;
	long lcheck,lcheck2,lcheckfromfile,ltemp;
	// char cdate[10];
	char tempname[40];
	char tempdate[15];
	char tmp[100];

	// _strdate(cdate);

	sprintf(sfile,"save\\plr%02d.txt",plr_slot);

	if(OpenFile(sfile,"rb",1)==0)
	{

		fscanf(stream,"%u ",&save_tmp);

		if(save_tmp==save_version)
		{
			lcheck=0;
			lcheckfromfile=-1;

			fgets(tempname,40,stream);
			fgets(tempdate,15,stream);

			strcpy(plr_name,tempname);
			plr_name[strlen(plr_name)-1]=0;

			fscanf(stream,"%u %u %u %u %u %u %d %u %u %u %u %u %u %u %u %u %u %u %u ",&plr_money,&ltemp,
				&plr_crew[0],&plr_crew[1],&plr_crew[2],&plr_crew[3],&plr_reputation,&plr_careful,&plr_gametype,&plr_currentcity,
				&plr_currentcustomer,&plr_jobsdone,&plr_daysinbusiness,&plr_lastspecial,&plr_advdecisions[0],&plr_advdecisions[1],
				&plr_advdecisions[2],&plr_advdecisions[3],&plr_difficulty);
			plr_taxpaid=ltemp;

		/*	printf("%u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u ",plr_money,plr_taxpaid,
				plr_crew[0],plr_crew[1],plr_crew[2],plr_crew[3],plr_reputation,plr_careful,plr_gametype,plr_currentcity,
				plr_currentcustomer,plr_jobsdone,plr_daysinbusiness,plr_advdecisions[0],plr_advdecisions[1],plr_advdecisions[2],
				plr_advdecisions[3]);
		*/
			lcheck=0;

			ix=0;


			do
			{
		//		printf("%c %u ",plr_name[ix],plr_name[ix]);
				if(ix%3==0)
				lcheck-=plr_name[ix];
					else
				lcheck+=(plr_name[ix]+ix*ix);
				ix++;
			} while(plr_name[ix]!=13 && ix<30);

			for(ix=0;ix<40;ix++) if(plr_name[ix]==13) plr_name[ix]=0;

			lcheck2=plr_money+plr_taxpaid+plr_reputation+plr_careful+plr_currentcity+plr_jobsdone+plr_daysinbusiness+plr_lastspecial;

			if(plr_gametype==0)
			{
				for(ix=0;ix<200;ix++)
				{
					fscanf(stream,"%u ",&plr_usedmaps[ix]);
					lcheck+=plr_usedmaps[ix]+1;
				}
			}

			for(i=0;i<MAXPLAYERS;i++)	// version 1.1, not compatible with previous version!
			{
				fscanf(stream,"%u ",&plrs_name[i]);
				for(ix=0;ix<MAXSTATS;ix++)
				{
					fscanf(stream,"%u ",&plrs_money[i][ix]);
				}
			}
			fscanf(stream,"%u %u ",&plr_month,&plr_monthcounter);

		//printf("LOAD lcheck %u lcheck2 %u\n",lcheck,lcheck2);

			lcheck*=lcheck2;

			fscanf(stream,"%u",&lcheckfromfile);
		}

	CloseFile();

	if(save_tmp!=save_version)	// wrong savefile version, give error message
	{
		sprintf(tmp,"%s plr%02d.txt",gametxt[204],plr_slot);	// Incompatible file version.
		OKBox(tmp,40);
		return false;
	}

	// cheat detection
	if(lcheck!=lcheckfromfile)
	{
		sprintf(tmp,"%s plr%02d.txt",gametxt[202],plr_slot);
//		sprintf(cOKBoxText[1],"%u file:%u", lcheck,lcheckfromfile);
		OKBox(tmp,40);
		return false;
	}


//	printf("Checksum %u from file %u\n",lcheck,lcheckfromfile);
	return true;
	}
	else
	{	// error
		return false;
	}
}

void LoadButtonThemes()
{
	int i;
	short l;
	unsigned char c;
	char *msg = "";

	if (cButtonFileList[0][0] == 0)
	{

		glob_t globbuf;
		int glob_return = glob("themes/*.bmp", 0, NULL, &globbuf);

		if (glob_return != 0)
		{
			printf("Themes listing: Glob error: %d\n", glob_return);
		}
		else
		{

			for (i = 0; i < globbuf.gl_pathc; i++)
			{
				l = strlen(globbuf.gl_pathv[i]);
				strncpy((char *)cButtonFileList[i], globbuf.gl_pathv[i], l - 4);
				sprintf(msg, "%s", cButtonFileList[i]);
				if (strcmp(msg, cButtonFile) == 0)
				{
					sButtonSelected = i;
					sButtonBegin = i;
				}
			}
			globfree(&globbuf);
		}
	}

	if (sButtonBegin > 0 && i < 6)
		sButtonBegin = 0;
	while (cButtonFileList[sButtonBegin + 4][0] == 0 && sButtonBegin > 0)
		sButtonBegin--; // scroll the list
}


void LoadLanguages()
{
short i=0, j, l;
unsigned char c;

if(cLanglist[0][0]==0)
{

	glob_t globbuf;

  	int glob_result = glob("languages/*.", 0, NULL, &globbuf);

	if (glob_result != 0) {
		printf("Failed to load language: glob result: %d\n", glob_result);
	}

	for (i=0, j=0; i<globbuf.gl_pathc; i++) {
		l = strlen(globbuf.gl_pathv[i]);

		if(globbuf.gl_pathv[i][0] != '.')
		{
				strncpy(cLanglist[j], globbuf.gl_pathv[i], l);

				if (strcmp(cLanglist[j], cLangfile)==0)
				{
					sLangSelected=j;
					sLangBegin=j;
				}
			j++;
		}

		if(sLangBegin>0 && j<4) sLangBegin=0;

		while(cLanglist[sLangBegin+3][0]==0 && sLangBegin>0) sLangBegin--;	// scroll the list
	}
	}
}


void LoadTheme(char *file)
{
	char cBFTemp[300];
	int i;

	sprintf(cBFTemp,"themes\\%s.txt",file);

//	printf("Attempting to open %s\n",cBFTemp);

	if(OpenFile(cBFTemp,"r",1)==0)
		{
		fscanf(stream,"%u %u %u ",&cBaseColours[0],&cBaseColours[1],&cBaseColours[2]);
		for(i=0;i<5;i++) fgets(cButtonText[i],52,stream);
		fgets(cButtonAuthor,52,stream);

		CloseFile();
		}
		else
		{
//			printf("%s not found. Using default colours and theme 'default'.\n",cBFTemp);
			sprintf(cButtonFile,"default");
		}

	i=cBaseColours[0]+cBaseColours[1]+cBaseColours[2];

	iTextColor=0;			// normally white text
	iColOffset=50;

	if(i>500)
		{
		iTextColor=1;	// if bright colors -> black text
		iColOffset=-50;
		}

	if(cBaseColours[0]<50)	cBaseColours[0]=50;
	if(cBaseColours[0]>200) cBaseColours[0]=200;
	if(cBaseColours[1]<50)	cBaseColours[1]=50;
	if(cBaseColours[1]>200) cBaseColours[1]=200;
	if(cBaseColours[2]<50)	cBaseColours[2]=50;
	if(cBaseColours[2]>200) cBaseColours[2]=200;
}

void LoadSettings()
{
//	char cBFTemp[100];

	if(OpenFile("settings.txt","r",1)==0)
	{
	fscanf(stream,"%u %u %u %u %u %u %u %u %u ",&SIZE_X,&SIZE_Y,&setBits,&setSoundEffects,&setExplosions,&setDustClouds, &setScroll, &setMaxFramerate, &setDifficulty);

//	setSounds=soundtemp;
	fgets(cButtonFile,50,stream);
	cButtonFile[strlen(cButtonFile)-1]=0;
	fgets(cLangfile,50,stream);
//	cLangfile[strlen(cLangfile)-1]=0;
	CloseFile();

	LoadTheme(cButtonFile);

/*	sprintf(cBFTemp,"themes\\%s.txt",cButtonFile);

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
	else
	{
//		printf("Settings.txt not found. Using default settings.\n");
		SIZE_X=1024;
		SIZE_Y=768;
		setSoundEffects=1;
		setExplosions=1;
		setDustClouds=1;
		setBits=32;
		setScroll=1;
		setMaxFramerate=40;
		setDifficulty=5;
//		printf("Using default colours and theme 'default'.\n",cBFTemp);
//		sprintf(cButtonFile,"default");
	}

//	printf("X%u Y%u SND%u EXP%u DST%u SAV%u R%u G%u B%u BTN%s LNG%s\n",SIZE_X,SIZE_Y,setSoundEffects,setExplosions,setDustClouds,setSaveAllDoneMaps,cBaseColours[0],cBaseColours[1],cBaseColours[2],cButtonFile,cLangfile);
	tmpScroll=setScroll;	// gum
}


void SaveSettings()
{
	if(OpenFile("settings.txt","wb",1)==0)
	{
	fprintf(stream,"%u %u %u %u %u %u %u %u %u %s\n%s",SIZE_X,SIZE_Y,setBits,setSoundEffects,setExplosions,setDustClouds,setScroll,setMaxFramerate,setDifficulty, cButtonFile,cLangfile);
	CloseFile();
	}
}

void toupper(char* text) {
	while (*text = toupper(*text)) {
		text++;
	}
}

void LoadMapCode(char * file)		// loads 'script' of building to memory for execution
{
	short i,iToolong;
	char msg[300];

	for(i=0;i<MAXMAPCODE;i++) sprintf(mapcode[i],"");

	i=0;
	iToolong=0;

	if(OpenFile(file,"r",1)==0)
	{
		while(!feof(stream))
		{
			if(i>MAXMAPCODE-2)
			{
				sprintf(msg,"%s %s",gametxt[202],file);
				OKBox(msg,40);

//				printf("Too long map file!\n");
				iToolong=1;
				i=0;
			}

			fgets(mapcode[i],1000,stream);
			toupper(mapcode[i]);		// capitalize
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
		sprintf(msg,"%s %s",gametxt[202],file);
		OKBox(msg,40);
//		printf("Too long map file, not used!\n");
		for(i=0;i<MAXMAPCODE;i++) sprintf(mapcode[i],"");
		mapcodelines=0;
	}
//	printf("%d rows in mapcode.\n",mapcodelines);
}


void LoadGameTexts()
{
	short i=1;
	char *msg="";
	const SDL_version * v = SDL_Linked_Version();

	sprintf(msg,"languages\\%s\\gametext.txt",cLangfile);

	if(OpenFile(msg,"r",1)==0)
	{
		while(!feof(stream))
		{
//			fscanf(stream,"%u",gametxtline[i]);
			fgets(gametxt[i],51,stream);
			gametxt[i][strlen(gametxt[i])-1]=0;

//			printf("row %d %s\n",i,gametxt[i]);
			i++;
		} //while (gametxt[i-1][0]!='#');

	CloseFile();
	}
	else
	if(OpenFile("languages\\english.txt","r",1)==0)
	{
//		printf("Can't find %s.txt, using English.",cLangfile);
		sprintf(cLangfile,"english");
		while(!feof(stream))
		{
//			fscanf(stream,"%u",gametxtline[i]);
			fgets(gametxt[i],50,stream);

//			printf("row %d %s\n",i,gametxt[i]);
			i++;
		} //while (gametxt[i-1][0]!='#');

	CloseFile();
	}

//	LoadMapCode(msg);
	sprintf(gametxt[373],"%s %u.%u.%u",gametxt[373], v->major, v->minor, v->patch);

}

void GetHiscores()
{
	short iCounter,il;
	char tmp[100];
	unsigned long lCheck=0,lCheck2=100;

	for(iCounter=0;iCounter<MAXHISCORE;iCounter++)
	{
		hs_date[iCounter][0]=0;
		hs_player[iCounter][0]=0;
	}

	if(OpenFile("data\\hiscore.txt","rb",1)==0)
	{

	iCounter=0;

	while(!feof(stream) && iCounter<MAXHISCORE-1)
	{
		fgets(tmp,35,stream);
		strcpy(hs_player[iCounter],tmp);
		il=strlen(hs_player[iCounter])-1;
		while(hs_player[iCounter][il]<32)
		{
			hs_player[iCounter][il]=0;
			il--;
		}
//		hs_player[iCounter][strlen(hs_player[iCounter])-2]=0;

		fgets(hs_date[iCounter],12,stream);
		fscanf(stream,"%u %u %d %d %d ",&hs_money[iCounter],&hs_gametype[iCounter],&hs_reputation[iCounter],&hs_daysinbusiness[iCounter],&hs_diff[iCounter]);

		lCheck2+=hs_player[iCounter][0];
		lCheck2+=hs_money[iCounter]*2+hs_reputation[iCounter]*3+hs_daysinbusiness[iCounter]*7;
		iCounter++;
	}

	fscanf(stream,"%u ",&lCheck);
	CloseFile();
	}

//	for(iCounter=0;iCounter<12;iCounter++) printf("hs_player[%d]=%s %s %u %u %u \n",iCounter, hs_player[iCounter],hs_date[iCounter],hs_money[iCounter],hs_reputation[iCounter],hs_daysinbusiness[iCounter]);

	if(lCheck!=lCheck2)	// hiscore list cheat prevention
	{
//		printf("Hiscore check failed %u %u\n",lCheck,lCheck2);
		for(il=0;il<MAXHISCORE-1;il++)
		{
			sprintf(hs_player[il],"Player %d",il+1);
			sprintf(hs_date[il],"2005-01-01");
			hs_money[il]=(MAXHISCORE-il+1)*50000;
			hs_reputation[il]=(MAXHISCORE-il+1)*40;
			hs_daysinbusiness[il]=(MAXHISCORE-il+1)*60;
			hs_gametype[il]=0;
			hs_diff[il]=10;
		}
		sprintf(tmp,"%s hiscore.txt",gametxt[202]);
		OKBox(tmp,40);
	}

//	SaveHiscores();
}

void SaveHiscores()
{
	short iCounter;
	unsigned long lCheck=100;

	if(OpenFile("data\\hiscore.txt","wb",1)==0)
	{
	for(iCounter=0;iCounter<MAXHISCORE-1;iCounter++)
	{
		lCheck+=hs_player[iCounter][0];
		lCheck+=hs_money[iCounter]*2+hs_reputation[iCounter]*3+hs_daysinbusiness[iCounter]*7;

		fprintf(stream,"%s\r\n%s\r\n%u %u %d %u %u\r\n",hs_player[iCounter],hs_date[iCounter],hs_money[iCounter],hs_gametype[iCounter],hs_reputation[iCounter],hs_daysinbusiness[iCounter],hs_diff[iCounter]);
	}

	fprintf(stream,"%u ",lCheck);
	CloseFile();
	}

}

void GetCustomers()
{
	int iCounter;
	char * cBfile="";

	for(iCounter=0;iCounter<MAXCUSTOMERS;iCounter++)
	{
		customer_name[iCounter][0]=0;
	}

	iCounter=0;

	sprintf(cBfile,"data\\customers.txt",cLangfile);
	OpenFile(cBfile,"rb",1);

	for(iCounter=0;iCounter<MAXCUSTOMERS;iCounter++)
	{
	fgets(customer_name[iCounter],12,stream);
	}

	CloseFile();
}

/* save & compress map file */
short SaveMap(char* s)
{// illegal chars: \/:*?"<>|
	int ix,iy;
	char *s2="";

	for(ix=0; ix < strlen(s); ix++)		// illegal chars -> _
		if(s[ix]==34 || s[ix]==42 || s[ix]==47 || s[ix]==58 || s[ix]==60 || s[ix]==62 || s[ix]==63 || s[ix]==92 || s[ix]==124) {
			s[ix] = 95;
		}

	if(strlen(s)<1) return 1;		// no zero-lengths

	if(OpenFile("maptemp.tmp","wb",0)==0)
	{
		for(ix=0;ix<MAPSIZE;ix++)
		for(iy=0;iy<MAPSIZE;iy++)
		{
		fprintf(stream,"%c",map[ix][iy]);
		}

		fprintf(stream," %u %u %u ", tmpbuildbegin, tmpbuildwidth,ed_currentcity);
		fprintf(stream,"%s\n%s\n", ed_customer,ed_building);
		for(ix=0;ix<10;ix++)
		{
//			ed_message[ix][strlen(ed_message[ix])-1]=0;
			fprintf(stream,"%s\n", ed_message[ix]);
		}

		CloseFile();

		sprintf(s2,"maps/%s.c2m",s);
		zpack("maptemp.tmp",s2);
//		printf("target %u %u\n",p_buildbegin,p_buildwidth);
//		printf("save pbb, pbw %u %u\n", targetareabegin, targetareaend);
//		printf("File save: %s: %u %u %u\n",s2, p_buildbegin, p_buildwidth, ed_currentcity);

/*		if(DeleteFile("maptemp.tmp")==0)
		{
			printf("Error deleting maptemp.tmp!\n");
		}
*/
//		printf("File %s.c2m saved\n",s);
	return 0;
	}
	else
		return 1;

}



/* uncompress and load map file */
short LoadMap(char *s, short iCode)
{
	short ix,iy;
	char *s2="";

	if(iCode!=0)
	{
	sprintf(s2,"maps/%s.c2m",s);
	zunpack(s2,"maptemp.tmp");
	}

	if(OpenFile("maptemp.tmp","rb",0)==0)
	{
		for(ix=0;ix<MAPSIZE;ix++)
		for(iy=0;iy<MAPSIZE;iy++)
		{
		fscanf(stream,"%c",&map[ix][iy]);
		mapend[ix][iy]=block_end[map[ix][iy]];
		}

		fscanf(stream," %u %u %u ", &p_buildbegin, &p_buildwidth, &ed_currentcity);
//		p_buildwidth=ix-p_buildbegin;

		fgets(ed_customer,50,stream);
		ed_customer[strlen(ed_customer)-1]=0;
		fgets(ed_building,50,stream);
		ed_building[strlen(ed_building)-1]=0;
		for(ix=0;ix<10;ix++)
		{
			fgets(ed_message[ix],50,stream);
			ed_message[ix][strlen(ed_message[ix])-1]=0;
		}

		CloseFile();

//		printf("target %u %u\n",p_buildbegin,p_buildwidth);
		DefineTargetArea();

		lTargetValue=TargetValue();
		lOtherValue=OtherValue();

		if(remove("maptemp.tmp") != 0)
		{
//			printf("Error deleting maptemp.tmp!\n");	///
			sprintf(cOKBoxText[0],"maptemp.tmp");
			OKBox(gametxt[513],60);	/// Can't delete file:
		}

		return 0;
	}
	else
		return 1;
}

/* uncompress and load map file */
short LoadMapTemp(char *s)
{
	short ix,iy,itmp;
	char *s2="";
	char tmpc[52];

	sprintf(temp_ed_building,"%c",0);
	for(ix=0;ix<10;ix++) sprintf(temp_ed_message[ix],"%c",0);

	sprintf(s2,"maps/%s.c2m",s);
	zunpack(s2,"maptemp.tmp");

	if(OpenFile("maptemp.tmp","rb",0)==0)
	{
		for(ix=0;ix<MAPSIZE;ix++)
		for(iy=0;iy<MAPSIZE;iy++)
		{
		fscanf(stream,"%c",&itmp);
		}

		fscanf(stream," %u %u %u ", &itmp, &itmp, &itmp);
//		p_buildwidth=ix-p_buildbegin;

		fgets(tmpc,50,stream);
		fgets(temp_ed_building,50,stream);
		temp_ed_building[strlen(temp_ed_building)-1]=0;
		for(ix=0;ix<10;ix++)
		{
			fgets(temp_ed_message[ix],50,stream);
			ed_message[ix][strlen(temp_ed_message[ix])-1]=0;
		}

		CloseFile();

		if(remove("maptemp.tmp") != 0)
		{
			sprintf(tmpc,"%s maptemp.tmp",gametxt[513]);	//Can't delete file:
			OKBox(tmpc,40);
//			printf("Error deleting maptemp.tmp!\n");
		}

		return 0;
	}
	else
		return 1;
}

void LoadMaplist(short iCode)
{
int l;
int i=0;
unsigned char c;

if(ed_mapfile[0][0]==0 || iCode==1)
{

	glob_t maps;

	int glob_return = glob("maps/*.c2m", 0, NULL, &maps);

  	if (glob_return != 0) {
    	printf ("Failed to find maps: Glob errorcode: %d\n", glob_return);
  	} else {
	  ed_mapfilenr=0;

		int j;
		for (i=0; i<maps.gl_pathc; i++) {
			if(maps.gl_pathv[i][0] != '.' && i < MAXMAPFILES)
			{
				l = strlen(maps.gl_pathv[i]) - 4;
				strncpy(ed_mapfile[j], maps.gl_pathv[i], l);
				j++;
			}

		}

	}

	}

}
