//#include "SDL.h"   /* All SDL App's need this */
// #include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "OC2b.h"

//	*********************************************************
//	* PARSE													*
//	*********************************************************
//	* DESCRIPTION:	parses code into map					*
//	* VERSION:		0.3										*
//	*********************************************************

/*
CHANGES:
	0.3 no need for GROUNDLEVEL Steepness any more
*/
//static							FILE *stream;

extern unsigned char			map[MAPSIZE][MAPSIZE];

//extern short					mapend[MAPSIZE][MAPSIZE];

char  							mapcode[MAXMAPCODE][1000];
short							mapcodelines;

short							parsedcode[MAXMAPCODE][20];		// for 'parsed' code that has only numbers
short							parseline;
//short							beginloop; //,repeats;

short p_buildbegin;		//p for parse
short p_buildend;
short p_buildwidth;
short p_doorinterval;
short p_elevinterval;
short p_extras;
short p_floorheight;
short p_groundfix;
short p_material;
short p_outside;
short p_printcode;
short p_v[100]; //={110,111,112,113,114,115,116,117,118,119};
short p_x;
short p_y;
short p_doortype;
short p_block;

const short MAXVARIABLES=20;
short variablelen[MAXVARIABLES];
char variable[MAXVARIABLES][15]={
"BUILDBEGIN",
"BUILDEND",
"BUILDWIDTH",
"DOORINTERVAL",
"ELEVINTERVAL",
"EXTRAS",				//5
"FLOORHEIGHT",
"GROUNDFIX",
"MATERIAL",
"OUTSIDE",
"V",					//10
"X",
"Y",
"DOORTYPE",
"BLOCK"
};

const short MAXCOMMANDS=50;
short commandlen[MAXCOMMANDS];
char command[MAXCOMMANDS][15]={
"ADDV",			//0
"BDOWN(",
"BDOWNLEFT(",
"BDOWNRIGHT(",
"BEGIN",
"BLEFT(",		//5
"BRIGHT(",
"BUILDBEGIN=",
"BUILDELEV(",
"BUILDFLOOR(",
"BUILDROOF(",	//10
"BUILDWIDTH=",
"BUP(",
"BUPLEFT(",
"BUPRIGHT(",
"DIVV",			//15
"DOORINTERVAL=",
"DOWN(",
"DOWNLEFT(",
"DOWNRIGHT(",
"ELEVINTERVAL=",	//20
"EXTRAS=",
"",
"FLOORHEIGHT=",
"GOTOLOW",
"GROUNDFIX=",	//25
"GROUNDLEVEL(",
"GROUNDVALLEY(",
"IFV",
"LEFT(",
"MATERIAL=",		//30
"MULV",
"OUTSIDE=",
"PRINT(",
"PRINTCODE=",
"RAND(",			//35
"REPEAT",
"RIGHT(",
"SETV",
"SETX=",
"SETY=",		//40
"SUBV",
"UP(",
"UPLEFT(",
"UPRIGHT(",
"SETFREQUENCY=",	//45
"ENDIF",
"IFBLOCK(",
"PRINTVAR(",
"DOORTYPE="
};

void WriteInfo(char *txt)	// precaution for crash and stdout.txt doesn't get written
{
   stream = fopen("out.txt","a");
   fprintf(stream,"%s\n",txt);
   fclose(stream);
}


short Compare(char * find, char * str, int begin, int len)
{
	short i,match=0,retval=0;

	if(strlen(str)<(unsigned int) begin+len)
	{
		retval=1;
	}
	if(strlen(find)<(unsigned int) len)
	{
		retval=1;
	}

	for(i=0;i<len;i++)
	{
		if(find[i]==str[i+begin]) match++;
	}

	if(match==len)
	{
		retval=0;
//		printf("COMPARE: [%s] found in [%s]\n",find,str);
//		return 0;
	}
	if(match<len)
	{
		retval=1;
	}

	return retval;
}

short ParseLine(short line)		// changes commands & variables into numbers
{
	short i,linelen,parsedx,realnr,inr,multiply,j,tmpnr1,tmpnr2;
	char nr[5];
//	char * stop;

linelen=strlen(mapcode[line]);
parsedx=0;
i=0;

while(i<linelen)
{
	if(Compare("//",mapcode[line],i,2)==0)
	{
		i=linelen;			// rem, ignore rest of the line
		return 0;
	}

	while(mapcode[line][i]==32 && i<linelen) i++;

	for(inr=0;inr<4;inr++) nr[inr]=0;

	inr=0;
	while(mapcode[line][i]>=48 && mapcode[line][i]<=57 && inr<4 && i<linelen)	// number
	{
		nr[inr]=mapcode[line][i]-48;
		inr++;
		i++;
	}

	multiply=1;
	realnr=0;

	while(inr>0)
	{
		inr--;
		realnr+=multiply*nr[inr];
		multiply*=10;
	}

	if(realnr>=0 && multiply>1)									// found a number
	{
	parsedcode[line][parsedx]=realnr;
//	printf("{%d %d} ",parsedx,realnr);
	parsedx++;
	}

	if(i<linelen)
	for(j=0;j<MAXCOMMANDS;j++)
	if(commandlen[j]>0)		// only valid commands
	{
	if(Compare(command[j],mapcode[line],i,commandlen[j])==0)		// found a command
		{
		parsedcode[line][parsedx]=30000+j;						// command codes begin from 30000
//		printf("{%d %s} ",parsedx,command[j]);
		parsedx++;
		i+=commandlen[j]-1;

		if(command[j][commandlen[j]-1]=='V')		// variable, nr stored in next parsedx ( and value in next parsedx )
		{
			tmpnr1=mapcode[line][i+1]-48;
			tmpnr2=mapcode[line][i+2]-48;
			if(tmpnr1>=0 && tmpnr1<=9)
			{
			if(tmpnr2>=0 && tmpnr2<=9)
			{
				parsedcode[line][parsedx]=tmpnr1*10+tmpnr2;
				i++;
			}
			else
				parsedcode[line][parsedx]=tmpnr1;
			}
//			parsedcode[line][parsedx]=mapcode[line][i+1]-48;
			parsedx++;
			i++;
		}

		if(j==33)		// print command
		{
			parsedcode[line][parsedx]=i+1;	// location where text begins
			parsedx++;
			i++;
		}
		}
	}

	if(i<linelen)
	for(j=0;j<MAXVARIABLES;j++)
	if(variablelen[j]>0)		// only valid variables
	{
	if(Compare(variable[j],mapcode[line],i,variablelen[j])==0)		// found a variable
		{
		realnr=31000+j;			// variables begin from 31000

		if(j==10)	// V = variable
		{
			tmpnr1=mapcode[line][i+1]-48;
			tmpnr2=mapcode[line][i+2]-48;
			if(tmpnr1>=0 && tmpnr1<=9)
			{
			if(tmpnr2>=0 && tmpnr2<=9)
			{
				realnr=31100+(tmpnr1*10+tmpnr2);
				i++;
			}
			else
				realnr=31100+tmpnr1;
			}

//			realnr=31100+tmpnr1;	// V variables begin from 31100
		}

		parsedcode[line][parsedx]=realnr;
//		printf("{%d %d} ",parsedx,realnr);
		parsedx++;
		i+=variablelen[j];
		}
	}

	i++;
}
parsedcode[line][parsedx]=-1;

return 0;
}

void Errormsg(short line,char * msg)
{
//	printf("PARSE ERROR ON LINE %d: %s\n",(line+2)/2,msg);
}

void ValueErrormsg(short line,char * msg)
{
//	printf("INVALID VALUE ON LINE %d: %s\n",(line+2)/2,msg);
}

short GetVariable(short code)
{
	short realnr=0;

	switch(code)
	{
	case 31000:realnr=p_buildbegin;break;	//BUILDBEGIN
	case 31001:realnr=p_buildend;break;		//BUILDEND
	case 31002:realnr=p_buildwidth;break;	//BUILDWIDTH
	case 31003:realnr=p_doorinterval;break;	//DOORINTERVAL
	case 31004:realnr=p_elevinterval;break;	//ELEVINTERVAL
	case 31005:realnr=p_extras;break;		//EXTRAS
	case 31006:realnr=p_floorheight;break;	//FLOORHEIGHT
	case 31007:realnr=p_groundfix;break;	//GROUNDFIX
	case 31008:realnr=p_material;break;		//MATERIAL
	case 31009:realnr=p_outside;break;		//OUTSIDE
	case 31011:realnr=p_x;break;			//X
	case 31012:realnr=p_y;break;			//Y
	case 31013:realnr=p_doortype;break;	//DOORTYPE
	case 31014:realnr=p_block;break;	//BLOCK

	default:break;
	}

	if(code>=31100 && code<31200)			//V = variable
	{
		realnr=p_v[code-31100];
	}

	if(code<30000) realnr=code;		// otherwise it's a normal number

	if(realnr>=0)
		return realnr;
	else
		return 0;
}

char CheckValue(int value, int min, int max)
{
	if(value>=31000)
	{
		value=GetVariable(value);
	}

	if(value>=min && value<=max)
		return 0;
	else
		return -1;
}

short IsGround(short mapcode)
{
	if(mapcode>=57 && mapcode<=63) return 1;
	if(mapcode>=65 && mapcode<=91) return 1;
	return 0;
}

void ParseIntoMap()		// puts blocks to the map
{
	short i,ix,iy,ok,iwindow,miny,ibegin,iwidth,iblock,iwater,ideepness,itmp,idoorbegin,ielevbegin,ielevx,ielevy,ielevwidth,ielevheight,ielevdoor;
	double d1,d2,d3;
	short r1,r2,r,ipurgeok;
	//	char msg[30];

	i=0;
	while(parsedcode[parseline][i]>=0)
	{
		if(parsedcode[parseline][i]>=30000)
		switch(parsedcode[parseline][i])	// main parse routine
		{
			case 30000:		//ADDV
				if(CheckValue(parsedcode[parseline][i+1],0,32000)==0 && CheckValue(parsedcode[parseline][i+2],0,32000)==0)
					p_v[parsedcode[parseline][i+1]]+=GetVariable(parsedcode[parseline][i+2]);
				else
					ValueErrormsg(parseline,"ADDV");
				break;

			case 30001:		//BDOWN
				if(CheckValue(parsedcode[parseline][i+1],0,MAPSIZE)==0)
				{
					for(ix=0;ix<GetVariable(parsedcode[parseline][i+1]);ix++) PutMap(p_x,p_y-ix,p_material);
				}
				else
					ValueErrormsg(parseline,"BDOWN");
				break;
			case 30002:		//BDOWNLEFT
				if(CheckValue(parsedcode[parseline][i+1],0,MAPSIZE)==0)
				{
					for(ix=0;ix<GetVariable(parsedcode[parseline][i+1]);ix++) PutMap(p_x-ix,p_y-ix,p_material);
				}
				else
					ValueErrormsg(parseline,"BDOWNLEFT");
				break;
			case 30003:		//BDOWNRIGHT
				if(CheckValue(parsedcode[parseline][i+1],0,MAPSIZE)==0)
				{
					for(ix=0;ix<GetVariable(parsedcode[parseline][i+1]);ix++) PutMap(p_x+ix,p_y-ix,p_material);
				}
				else
					ValueErrormsg(parseline,"BDOWNRIGHT");
				break;
			case 30004:		//BEGIN

				ix=GetVariable(parsedcode[parseline][18]);	// repeat line nr
				parsedcode[ix][18]=1;

//				beginloop=parseline;
//				repeats=0;
				break;
			case 30005:		//BLEFT
				if(CheckValue(parsedcode[parseline][i+1],0,MAPSIZE)==0)
				{
					for(ix=0;ix<GetVariable(parsedcode[parseline][i+1]);ix++) PutMap(p_x-ix,p_y,p_material);
				}
				else
					ValueErrormsg(parseline,"BLEFT");
				break;
			case 30006:		//BRIGHT
				if(CheckValue(parsedcode[parseline][i+1],0,MAPSIZE)==0)
				{
					for(ix=0;ix<GetVariable(parsedcode[parseline][i+1]);ix++) PutMap(p_x+ix,p_y,p_material);
				}
				else
					ValueErrormsg(parseline,"BRIGHT");
				break;
			case 30007:		//BUILDBEGIN
				if(CheckValue(parsedcode[parseline][i+1],0,MAPSIZE)==0)
					p_buildbegin=GetVariable(parsedcode[parseline][i+1]);
				else
					ValueErrormsg(parseline,"BUILDBEGIN");
				break;
			case 30008:		//BUILDELEV
				if(CheckValue(parsedcode[parseline][i+1],0,MAPSIZE)==0 && CheckValue(parsedcode[parseline][i+2],0,MAPSIZE)==0
				&& CheckValue(parsedcode[parseline][i+3],0,MAPSIZE)==0 && CheckValue(parsedcode[parseline][i+4],0,MAPSIZE)==0
				&& CheckValue(parsedcode[parseline][i+5],0,2)==0)
				{
					ielevx=GetVariable(parsedcode[parseline][i+1]);
					ielevy=GetVariable(parsedcode[parseline][i+2]);
					ielevwidth=GetVariable(parsedcode[parseline][i+3])-1;
					ielevheight=GetVariable(parsedcode[parseline][i+4]);
					ielevdoor=GetVariable(parsedcode[parseline][i+5]);

					ok=0;
					if(MapBlock(ielevx,ielevy)==2 || MapBlock(ielevx,ielevy)==5 || MapBlock(ielevx,ielevy)==8
						|| MapBlock(ielevx,ielevy)==11 || MapBlock(ielevx,ielevy)==14 || MapBlock(ielevx,ielevy)==17) ok=1;

					if(ok==1)		// let's build an elevator!
					{
						iblock=MapBlock(ielevx,ielevy);
						for(iy=0;iy<ielevheight;iy++)
						{
							// change material if needed
						if(MapBlock(ielevx,ielevy+iy)==2 || MapBlock(ielevx,ielevy+iy)==5 || MapBlock(ielevx,ielevy+iy)==8
							|| MapBlock(ielevx,ielevy+iy)==11 || MapBlock(ielevx,ielevy+iy)==14 || MapBlock(ielevx,ielevy+iy)==17) iblock=MapBlock(ielevx,ielevy+iy);

							if(MapBlock(ielevx,ielevy+iy)==iblock)
							if(MapBlock(ielevx+1,ielevy+iy+1)==iblock+1 || MapBlock(ielevx,ielevy+iy+1)==iblock+1)
							if(ielevdoor<=1)		// left wall
							{
								for(i=1;i<p_floorheight-1;i++)
									PutMap(ielevx,ielevy+iy+i,20);
								//PutMap(ielevx,ielevy+iy+2,20);
//								i=3;
								while(MapBlock(ielevx,ielevy+iy+i)==iblock+1)
								{
									PutMap(ielevx,ielevy+iy+i,iblock);
									i++;
								}
							}

							if(iy>0 && iy<ielevheight)	// background
							{
								if(MapBlock(ielevx,ielevy+iy)!=iblock || MapBlock(ielevx+1,ielevy+iy)!=iblock)
								{
								PutMap(ielevx-1,ielevy+iy,iblock+1);
								PutMap(ielevx+ielevwidth+1,ielevy+iy,iblock+1);
								}

								for(i=1;i<ielevwidth;i++)
								{
									PutMap(ielevx+i,ielevy+iy,iblock+1);
								}
							}

							if(MapBlock(ielevx+ielevwidth,ielevy+iy)==iblock)
							if(MapBlock(ielevx+ielevwidth-1,ielevy+iy+1)==iblock+1 || MapBlock(ielevx+ielevwidth,ielevy+iy+1)==iblock+1)
							if(ielevdoor==0 || ielevdoor==2)		// right wall
							{
								for(i=1;i<p_floorheight-1;i++)
									PutMap(ielevx+ielevwidth,ielevy+iy+i,20);
//								PutMap(ielevx+ielevwidth,ielevy+iy+2,20);
//								i=3;
								while(MapBlock(ielevx+ielevwidth,ielevy+iy+i)==iblock+1)
								{
									PutMap(ielevx+ielevwidth,ielevy+iy+i,iblock);
									i++;
								}
							}

						}
					}
					else
					{
						Errormsg(parseline,"BUILDELEV BEGIN COORDINATES NOT VALID");
//						printf("Block in %d,%d is %d\n",ielevx,ielevy,MapBlock(ielevx,ielevy));
					}
				}
				else
					Errormsg(parseline,"BUILDELEV");
				break;
			case 30009:		//BUILDFLOOR
				if(CheckValue(parsedcode[parseline][i+1],0,MAPSIZE)==0 && CheckValue(parsedcode[parseline][i+2],0,MAPSIZE)==0 && CheckValue(parsedcode[parseline][i+3],0,MAPSIZE)==0)
				{
				ibegin=GetVariable(parsedcode[parseline][i+1])+1;
				iwidth=GetVariable(parsedcode[parseline][i+2])-1;
				miny=GetVariable(parsedcode[parseline][i+3]);

				ielevbegin=0;
				idoorbegin=0;

//				if(p_outside>=1 && p_outside<=2)
				{
					ibegin++;
					iwidth-=2;
				}
//				printf("1: %d 2: %d 3: %d\n",GetVariable(parsedcode[parseline][i+1]),GetVariable(parsedcode[parseline][i+2]),GetVariable(parsedcode[parseline][i+3]));
				if(p_outside==2)
				{
					PutMap(ibegin-1,miny,p_material+1);// left balcony
					PutMap(ibegin-1,miny+1,22);

					PutMap(ibegin+iwidth-1,miny,p_material+1);// right balcony
					PutMap(ibegin+iwidth-1,miny+1,22);
				}

				for(iy=1;iy<p_floorheight;iy++)
				{
					iblock=p_material;				// left wall

						if(p_outside==1 || p_outside==2)
						if(iy<p_floorheight-1)
						{
							if(p_doortype==1)				// window door
							{
								if(iy==1) iblock=19;
								if(iy>1) iblock=18;
							}

							if(p_doortype==2)				// normal door
								iblock=19;
							if(p_doortype==3)				// steel door
								iblock=21;
						}

					if(p_outside==4 && iy>1) iblock=18;
					if(p_outside==5) iblock=18;

					PutMap(ibegin,miny+iy,iblock);

					iblock=p_material+1;		// right wall

						if(p_outside==1 || p_outside==2)
						if(iy<p_floorheight-1)
						{
							if(p_doortype==1)				// window door
							{
								if(iy==1) iblock=19;
								if(iy>1) iblock=18;
							}

							if(p_doortype==2)				// normal door
								iblock=19;
							if(p_doortype==3)				// steel door
								iblock=21;

						}
					if(p_outside==4 && iy>1) iblock=18;
					if(p_outside==5) iblock=18;

					PutMap(ibegin+iwidth-2,miny+iy,iblock);


						for(ix=1;ix<iwidth-2;ix++)
						{
							iblock=p_material+2;

							if(p_doorinterval>0 && ix<iwidth-5 && ix>4)
							if(ix%p_doorinterval==0)
							{
								ok=1;

								if(ix-ielevbegin>0 && ix-ielevbegin<5) ok=0;

								if(ok==1)
								{
									idoorbegin=ix;
									iblock=p_material+1;
									if(iy<p_floorheight-1)
									{
										if(p_doortype==1)				// window door
										{
											if(iy==1) iblock=19;
											if(iy>1) iblock=18;
										}

										if(p_doortype==2)				// normal door
											iblock=19;
										if(p_doortype==3)				// steel door
											iblock=21;
									}
								}	// ok ends

							}

							if(p_elevinterval>0 && ix>5 && ix<iwidth-8)	// elevator shaft
							if(abs(idoorbegin-ix)>3)
							{
								if(ix%p_elevinterval==0)
								{
									ielevbegin=ix;
								}
							}

							if(p_elevinterval>0 && ielevbegin>0)
								if(ix-ielevbegin==0 || ix-ielevbegin==3)	// begin + end
							{
								if(iy==p_floorheight-1)
									iblock=p_material+1;
								else
									iblock=20;	// metal door
							}

							PutMap(ibegin+ix,miny+iy,iblock);	// background
						}
					}

				ielevbegin=0;
				idoorbegin=0;

				for(ix=0;ix<iwidth-1;ix++)
					{
						iblock=p_material+1;
						if(p_outside==5)
							if(ix==0 || ix==iwidth-2) iblock=18;	// glass wall

						if(p_elevinterval>0 && ix>5 && ix<iwidth-8)
						{
							if(ix%p_elevinterval==0)
							{
								if(MapBlock(ibegin+ix,miny+1)==20) ielevbegin=ix;
							}
						}
						if(p_elevinterval>0 && ielevbegin>0)
							if(ix-ielevbegin==1 || ix-ielevbegin==2) iblock=p_material+2;

						PutMap(ibegin+ix,miny,iblock);	// floor
						PutMap(ibegin+ix,miny+p_floorheight,iblock);	// floor
					}
				}
				else
					ValueErrormsg(parseline,"BUILDFLOOR");
				break;
			case 30010:		//BUILDROOF
				if(CheckValue(parsedcode[parseline][i+1],0,MAPSIZE)==0 && CheckValue(parsedcode[parseline][i+2],0,MAPSIZE)==0 && CheckValue(parsedcode[parseline][i+3],0,MAPSIZE)==0)
				{
					iwindow=0;
					if(p_material==18) iwindow=7;	// if material is window
					if(p_material>=33 && p_material<=39) iwindow=7;	// or glass roof -> glass roof

					PutMap(GetVariable(parsedcode[parseline][i+1]),GetVariable(parsedcode[parseline][i+3]),28+iwindow);
					for(ix=1;ix<GetVariable(parsedcode[parseline][i+2])-1;ix++) PutMap(GetVariable(parsedcode[parseline][i+1])+ix,GetVariable(parsedcode[parseline][i+3]),29+iwindow);
					PutMap(GetVariable(parsedcode[parseline][i+1])+GetVariable(parsedcode[parseline][i+2])-1,GetVariable(parsedcode[parseline][i+3]),30+iwindow);
				}
				else
					ValueErrormsg(parseline,"BUILDROOF");
				break;
			case 30011:		//BUILDWIDTH
				if(CheckValue(parsedcode[parseline][i+1],1,MAPSIZE)==0)
					p_buildwidth=GetVariable(parsedcode[parseline][i+1]);
				else
					ValueErrormsg(parseline,"BUILDWIDTH");

				break;
			case 30012:		//BUP
				if(CheckValue(parsedcode[parseline][i+1],0,MAPSIZE)==0)
				{
					for(ix=0;ix<GetVariable(parsedcode[parseline][i+1]);ix++) PutMap(p_x,p_y+ix,p_material);
				}
				else
					ValueErrormsg(parseline,"BUP");
				break;
			case 30013:		//BUPLEFT
				if(CheckValue(parsedcode[parseline][i+1],0,MAPSIZE)==0)
				{
					for(ix=0;ix<GetVariable(parsedcode[parseline][i+1]);ix++) PutMap(p_x-ix,p_y+ix,p_material);
				}
				else
					ValueErrormsg(parseline,"BUPLEFT");
				break;
			case 30014:		//BUPRIGHT
				if(CheckValue(parsedcode[parseline][i+1],0,MAPSIZE)==0)
				{
					for(ix=0;ix<GetVariable(parsedcode[parseline][i+1]);ix++) PutMap(p_x+ix,p_y+ix,p_material);
				}
				else
					ValueErrormsg(parseline,"BUPRIGHT");
				break;
			case 30015:		//DIVV
				if(CheckValue(parsedcode[parseline][i+1],0,100)==0 && CheckValue(parsedcode[parseline][i+2],1,32000)==0)
				{
					d1=p_v[parsedcode[parseline][i+1]];
					d2=GetVariable(parsedcode[parseline][i+2]);
					d3=d1/d2;

					p_v[parsedcode[parseline][i+1]]=short(d3);
				}
				else
					ValueErrormsg(parseline,"DIVV");
				break;
			case 30016:		//DOORINTERVAL
				if(CheckValue(parsedcode[parseline][i+1],0,MAPSIZE)==0)
				{
					p_doorinterval=GetVariable(parsedcode[parseline][i+1]);
				}
				else
					ValueErrormsg(parseline,"DOORINTERVAL");
				break;
			case 30017:		//DOWN
				if(CheckValue(parsedcode[parseline][i+1],0,MAPSIZE)==0)
					p_y-=GetVariable(parsedcode[parseline][i+1]);
				else
					ValueErrormsg(parseline,"DOWN");
				break;
			case 30018:		//DOWNLEFT
				if(CheckValue(parsedcode[parseline][i+1],0,MAPSIZE)==0)
				{
					p_y-=GetVariable(parsedcode[parseline][i+1]);
					p_x-=GetVariable(parsedcode[parseline][i+1]);
				}
				else
					ValueErrormsg(parseline,"DOWNLEFT");
				break;
			case 30019:		//DOWNRIGHT
				if(CheckValue(parsedcode[parseline][i+1],0,MAPSIZE)==0)
				{
					p_y-=GetVariable(parsedcode[parseline][i+1]);
					p_x+=GetVariable(parsedcode[parseline][i+1]);
				}
				else
					ValueErrormsg(parseline,"DOWNRIGHT");
				break;
			case 30020:		//ELEVINTERVAL
				if(CheckValue(parsedcode[parseline][i+1],0,MAPSIZE)==0)
					p_elevinterval=GetVariable(parsedcode[parseline][i+1]);
				else
					ValueErrormsg(parseline,"ELEVINTERVAL");
				break;
			case 30021:		//EXTRAS
				if(CheckValue(parsedcode[parseline][i+1],0,MAPSIZE)==0)
						p_extras=GetVariable(parsedcode[parseline][i+1]);
				else
					ValueErrormsg(parseline,"EXTRAS");
				break;
//			case 30022:		//FINDPLACE	: no need -> removed
//				break;
			case 30023:		//FLOORHEIGHT
				if(CheckValue(parsedcode[parseline][i+1],0,MAPSIZE)==0)
					p_floorheight=GetVariable(parsedcode[parseline][i+1]);
				else
					ValueErrormsg(parseline,"FLOORHEIGHT");
				break;
			case 30024:		//GOTOLOW
				if(CheckValue(parsedcode[parseline][i+1],0,MAPSIZE)==0 && CheckValue(parsedcode[parseline][i+2],0,MAPSIZE)==0)
				{
				ibegin=GetVariable(parsedcode[parseline][i+1]);
				iwidth=GetVariable(parsedcode[parseline][i+2]);
				miny=MAPSIZE;
//				sprintf(msg,"Gotolow %d-%d",ibegin,ibegin+iwidth );
//				AddMessage(msg,1);

				for(ix=0;ix<iwidth;ix++)
				if(ix+ibegin<MAPSIZE)
					{
					for(iy=0;iy<MAPSIZE;iy++)
						if(MapBlock(ix+ibegin,iy)==0 && iy<miny) miny=iy;
					}
				p_y=miny-1;
				}
				else
					ValueErrormsg(parseline,"GOTOLOW");
				break;
			case 30025:		//GROUNDFIX
				if(CheckValue(parsedcode[parseline][i+1],0,1)==0)
					p_groundfix=GetVariable(parsedcode[parseline][i+1]);
				else
					ValueErrormsg(parseline,"GROUNDFIX");
				break;
			case 30026:		//GROUNDLEVEL
				if(CheckValue(parsedcode[parseline][i+1],2,MAPSIZE-3)==0 && CheckValue(parsedcode[parseline][i+2],1,MAPSIZE)==0) // && CheckValue(parsedcode[parseline][i+3],0,2)==0)
				{
					ibegin=GetVariable(parsedcode[parseline][i+1]);
					iwidth=GetVariable(parsedcode[parseline][i+2]);
//					isteepness=GetVariable(parsedcode[parseline][i+3]);

					miny=MAPSIZE;

					iy=0;
					while(iy<MAPSIZE && MapBlock(ibegin,iy)>40)
					{
						iy++;
					}
					miny=iy;

					for(ix=0;ix<iwidth;ix++)
					if(ix+ibegin<MAPSIZE && ix+ibegin>0)
					{
						iy=miny;
						ok=0;
						while(ok==0)
						{
							if(MapBlock(ix+ibegin,iy)==71) ok=1;
							if(MapBlock(ix+ibegin,iy)==81) ok=1;
							if(MapBlock(ix+ibegin,iy)>=64 && MapBlock(ix+ibegin,iy)<=67) ok=1;

							PutMap(ibegin+ix,iy,71);
							iy--;
						}

						ipurgeok=0;

						for(iy=miny;iy<MAPSIZE;iy++) if(MapBlock(ix+ibegin,iy)>0 && MapBlock(ix+ibegin,iy)<40) ipurgeok=1;

						if(ipurgeok==0) for(iy=miny;iy<MAPSIZE;iy++) PutMap(ix+ibegin,iy,0);
					}

					// new purge part
					itmp=0;
					r2=0;
					ix=ibegin-1;
					iy=miny;
					while(MapBlock(ix-1,iy+1)>0 && ix>0 && iy<MAPSIZE && itmp<1000)	// left, up
					{
						itmp++;
						ok=1;
						for(r1=iy;r1<MAPSIZE;r1++) if(MapBlock(ix,r1)>0 && MapBlock(ix,r1)<40) ok=0;

						if(ok==1)
						{
							PutMap(ix,iy,72);
							r1=iy+1;
							while(MapBlock(ix,r1)>0 && r1<MAPSIZE) {PutMap(ix,r1,0);r1++;}
							r2=1;
						}
						ix--;
						iy++;
					}

					if(r2==0)
					{
					ix=ibegin-1;
					iy=miny-1;
					itmp=0;
					while(MapBlock(ix-1,iy)==0 && ix>0 && iy>0 && itmp<1000)	// left, down
					{
						itmp++;
						ok=1;
						for(r1=0;r1<MAPSIZE;r1++) if(MapBlock(ix,r1)>0 && MapBlock(ix,r1)<40) ok=0;

						if(ok==1)
						{
						PutMap(ix,iy,70);
						r1=iy-1;
						r2=0;
						while(r2==0 && r1>0) {r2=1;if(MapBlock(ix,r1)==0 || MapBlock(ix,r1)>40) r2=0;PutMap(ix,r1,71);r1--;}
						}
						ix--;
						iy--;
					}
					}

					r2=0;
					ix=ibegin+iwidth;
					iy=miny;
					itmp=0;
					while(MapBlock(ix+1,iy+1)>0 && ix<MAPSIZE && iy<MAPSIZE && itmp<1000)	// right, up
					{
						itmp++;
						ok=1;
						for(r1=iy;r1<MAPSIZE;r1++) if(MapBlock(ix,r1)>0 && MapBlock(ix,r1)<40) ok=0;

						if(ok==1)
						{
						PutMap(ix,iy,70);
						r1=iy+1;
						while(MapBlock(ix,r1)>0 && r1<MAPSIZE) {PutMap(ix,r1,0);r1++;}
						r2=1;
						}
						ix++;
						iy++;
					}

					if(r2==0)
					{
					ix=ibegin+iwidth;
					iy=miny-1;
					itmp=0;
					while(MapBlock(ix+1,iy)==0 && ix<MAPSIZE && iy>0 && itmp<1000)	// right, down
					{
						itmp++;
						ok=1;
						for(r1=0;r1<MAPSIZE;r1++) if(MapBlock(ix,r1)>0 && MapBlock(ix,r1)<40) ok=0;

						if(ok==1)
						{
						PutMap(ix,iy,72);
						r1=iy-1;
						r2=0;

						while(r2==0 && r1>0) {r2=1;if(MapBlock(ix,r1)==0 || MapBlock(ix,r1)>40) r2=0;PutMap(ix,r1,71);r1--;}

						}
						ix++;
						iy--;
					}
					}
					// new purge part ends


/*
//					printf("GROUNDLEVEL VALUES %d %d\n",ibegin,iwidth);
					iy=0;
					while(iy<MAPSIZE && MapBlock(ibegin,iy)>40)
					{
						iy++;
					}
					miny=iy;

					for(ix=0;ix<iwidth;ix++)
					if(ix+ibegin<MAPSIZE && ix+ibegin>0)
					{
						iy=miny;
						ok=0;
						while(ok==0)
						{
							if(MapBlock(ix+ibegin,iy)==71) ok=1;
							if(MapBlock(ix+ibegin,iy)==81) ok=1;
							if(MapBlock(ix+ibegin,iy)>=64 && MapBlock(ix+ibegin,iy)<=67) ok=1;

							PutMap(ibegin+ix,iy,71);
							iy--;
						}

						ipurgeok=0;

						for(iy=miny;iy<MAPSIZE;iy++) if(MapBlock(ix+ibegin,iy)>0 && MapBlock(ix+ibegin,iy)<40) ipurgeok=1;

						if(ipurgeok==0) for(iy=miny;iy<MAPSIZE;iy++) PutMap(ix+ibegin,iy,0);
					}

					ix=ibegin-1;
					iy=miny;
					itmp=miny;

					while(IsGround(MapBlock(ix,iy))==1 && ix>0)		// up
					{
						ipurgeok=0;

						for(i2=miny;i2<MAPSIZE;i2++)
							if(MapBlock(ix-isteepness-1,i2)>0 && MapBlock(ix-isteepness-1,i2)<40) ipurgeok=1;

						if(ipurgeok==0)
						{
						if(isteepness==1)
							PutMap(ix,iy,72);
						else
						{
							PutMap(ix,iy,74);
							PutMap(ix-1,iy,73);
						}


						while(iy<MAPSIZE-2)
						{
							iy++;
							PutMap(ix,iy,0);
							if(isteepness==2) PutMap(ix-1,iy,0);
						}

						itmp++;
						iy=itmp;
						}

						ix-=isteepness;
					}


					ix=ibegin-1;
					iy=miny;
					itmp=miny;

					while(MapBlock(ix,iy-1)==0 && ix>0 && itmp>1)	// down
					{

					ipurgeok=0;

					for(i2=miny-2;i2>1;i2--) if(MapBlock(ix-isteepness-1,i2)>0 && MapBlock(ix-isteepness-1,i2)<40) ipurgeok=1;

					if(ipurgeok==0)
					{
						if(isteepness==1)
							PutMap(ix,iy-1,70);
						else
						{
							PutMap(ix,iy-1,69);
							PutMap(ix-1,iy-1,68);
						}

						itmp--;
						iy=itmp;

						ok=0;
						while(ok==0)
						{
							if(MapBlock(ix,iy-1)>=64 && MapBlock(ix,iy-1)<=67) ok=1;
							if(MapBlock(ix,iy-1)==71 || MapBlock(ix,iy-1)==81) ok=1;

							iy--;
							PutMap(ix,iy,71);
							if(isteepness==2) PutMap(ix-1,iy,71);
						}
					}

						ix-=isteepness;
					}


					ix=ibegin+iwidth;
					iy=miny;
					itmp=miny;

					while(IsGround(MapBlock(ix,iy))==1 && ix<MAPSIZE-2)		//up
					{
						ipurgeok=0;

						for(i2=miny;i2<MAPSIZE;i2++)
							if(MapBlock(ix+isteepness+1,i2)>0 && MapBlock(ix+isteepness+1,i2)<40) ipurgeok=1;

						if(ipurgeok==0)
						{
						if(isteepness==1)
							PutMap(ix,iy,70);
						else
						{
							PutMap(ix,iy,68);
							PutMap(ix+1,iy,69);
						}

						while(iy<MAPSIZE-2)
						{
							iy++;
							PutMap(ix,iy,0);
							if(isteepness==2) PutMap(ix+1,iy,0);
						}

						itmp++;
						iy=itmp;
						}
						ix+=isteepness;
					}

					ix=ibegin+iwidth;
					iy=miny-1;
					itmp=miny-1;

					while(MapBlock(ix,iy)==0 && ix<MAPSIZE-2 && itmp>1)		// down
					{

					ipurgeok=0;
					for(i2=miny-2;i2>1;i2--) if(MapBlock(ix+isteepness+1,i2)>0 && MapBlock(ix+isteepness+1,i2)<40) ipurgeok=1;

						if(ipurgeok==0)
						{
							if(isteepness==1)
								PutMap(ix,iy,72);
							else
							{
								PutMap(ix,iy,73);
								PutMap(ix+1,iy,74);
							}

							ok=0;

							while(ok==0)
							{
								if(MapBlock(ix,iy-1)>=64 && MapBlock(ix,iy-1)<=67) ok=1;
								if(MapBlock(ix,iy-1)==71 || MapBlock(ix,iy-1)==81) ok=1;

								iy--;
								PutMap(ix,iy,71);
								if(isteepness==2) PutMap(ix+1,iy,71);
							}

							itmp--;
							iy=itmp;
						}
						ix+=isteepness;
					}
					*/
				}
				else
					ValueErrormsg(parseline,"GROUNDLEVEL");

				break;
			case 30027:		//GROUNDVALLEY
				if(CheckValue(parsedcode[parseline][i+1],2,MAPSIZE-20)==0 && CheckValue(parsedcode[parseline][i+2],0,MAPSIZE)==0 && CheckValue(parsedcode[parseline][i+3],0,MAPSIZE)==0 && CheckValue(parsedcode[parseline][i+4],0,MAPSIZE)==0)
				{
					ibegin=GetVariable(parsedcode[parseline][i+1]);
					iwidth=GetVariable(parsedcode[parseline][i+2]);
					ideepness=GetVariable(parsedcode[parseline][i+3]);
					iwater=GetVariable(parsedcode[parseline][i+4]);

					if(ibegin+iwidth<MAPSIZE)
					{
//						isteepness=2;
//						if(ideepness*2>iwidth) isteepness=1;

						miny=MAPSIZE;
						for(ix=0;ix<iwidth;ix++)
						{
							iy=0;

							while(MapBlock(ix+ibegin,iy)>0) iy++;
							if(iy<miny) miny=iy;
						}

						miny--;

						if(miny+ideepness>MAPSIZE-4) ideepness=MAPSIZE-miny-5;

//						for(ix=0;ix<=iwidth;ix++)
						ix=0;
						while(ix<=iwidth)
						{
							iy=miny-ix;

							iblock=72;	// ground

							if(ix>=iwidth-ideepness)
							{
								iy=miny-iwidth+ix;
								iblock-=2;
							}

//							if(iwater>0 && miny-iy>ideepness-iwater) iblock+=52;		// water: removed

							if(ix>ideepness && ix<iwidth-ideepness)
							{
								iy=miny-ideepness;
/*								if(iwater>0)			// water: removed
									iblock--;
								else*/
									iblock=0;
							}

							PutMap(ix+ibegin,iy,iblock);
							iy++;
							itmp=iy;

							while(iy<MAPSIZE-1)		// purge
							{
								iblock=0;
//								if(iwater>0 && miny-iy>ideepness-iwater) iblock=123;	// water:removed
								PutMap(ix+ibegin,iy,iblock);
								iy++;
							}

							iy=itmp-2;

							while(iy>0)
							{
								ok=1;
								if(MapBlock(ix+ibegin,iy)>=64 && MapBlock(ix+ibegin,iy)<=67) ok=0;
								if(MapBlock(ix+ibegin,iy)==71 || MapBlock(ix+ibegin,iy)==81) ok=0;
								if(ok==1) PutMap(ix+ibegin,iy,71);
								iy--;
							}

							ix++;
						}


						PutMap(ibegin-1,miny,60);		// left bank
						PutMap(ibegin-2,miny,60);
						for(itmp=miny+1;itmp<MAPSIZE-1;itmp++)
						{
							PutMap(ibegin-1,itmp,0);
							PutMap(ibegin-2,itmp,0);
						}

						for(ix=1;ix<3;ix++)
						{
						itmp=miny-1;
						while(itmp>0)
						{
							ok=0;
							if(MapBlock(ibegin-ix,itmp)>=64 && MapBlock(ibegin-ix,itmp)<=67) ok=1;
							if(MapBlock(ibegin-ix,itmp)==71 || MapBlock(ibegin-ix,itmp)==81) ok=1;

							if(ok==0) PutMap(ibegin-ix,itmp,71);
							itmp--;
						}
						}

						PutMap(ibegin+iwidth+1,miny,60);		// right bank
						PutMap(ibegin+iwidth+2,miny,60);
						for(itmp=miny+1;itmp<MAPSIZE-1;itmp++)
						{
							PutMap(ibegin+iwidth+1,itmp,0);
							PutMap(ibegin+iwidth+2,itmp,0);
						}

						for(ix=1;ix<3;ix++)
						{
						itmp=miny-1;
						while(itmp>0)
						{
							ok=0;
							if(MapBlock(ibegin+iwidth+ix,itmp)>=64 && MapBlock(ibegin+iwidth+ix,itmp)<=67) ok=1;
							if(MapBlock(ibegin+iwidth+ix,itmp)==71 || MapBlock(ibegin+iwidth+ix,itmp)==81) ok=1;

//							printf("%d %d %d %d\n",MapBlock(ibegin-ix][itmp],ibegin+iwidth+ix,itmp,ok);
							if(ok==0) PutMap(ibegin+iwidth+ix,itmp,71);
							itmp--;
						}
						}

					}
					else
						ValueErrormsg(parseline,"GROUNDVALLEY, BEGIN, WIDTH");
				}
				else
					ValueErrormsg(parseline,"GROUNDVALLEY");

				break;
			case 30028:		//IFV
				if(CheckValue(parsedcode[parseline][i+1],0,100)==0 && CheckValue(parsedcode[parseline][i+2],0,32000)==0 && CheckValue(parsedcode[parseline][i+3],0,32000)==0 && CheckValue(parsedcode[parseline][19],0,MAXMAPCODE)==0)
				{
					if(p_v[parsedcode[parseline][i+1]]<GetVariable(parsedcode[parseline][i+2]) || p_v[parsedcode[parseline][i+1]]>GetVariable(parsedcode[parseline][i+3]))
					{
						ok=0;
//						parseline++;
						i=0;
						parseline=GetVariable(parsedcode[parseline][19]);	// jump to the endif

/*						while(ok==0)
						{
							if(parsedcode[parseline][i]==30046)				// move until endif is found
								ok=1;
							else
								i++;

							if(parsedcode[parseline][i]<0)
							{
								parseline++;
								i=0;
							}
						}
						*/
					}
				}
				else
				{
					ValueErrormsg(parseline,"IFV");
//					printf("ValueError %d %d %d %d\n",GetVariable(parsedcode[parseline][i+1]),GetVariable(parsedcode[parseline][i+2]),GetVariable(parsedcode[parseline][i+3]),GetVariable(parsedcode[parseline][19]));
				}
				break;
			case 30029:		//LEFT
				if(CheckValue(parsedcode[parseline][i+1],0,MAPSIZE)==0)
					p_x-=GetVariable(parsedcode[parseline][i+1]);
				else
					ValueErrormsg(parseline,"LEFT");
				break;
			case 30030:		//MATERIAL
				if(CheckValue(parsedcode[parseline][i+1],0,MAXBLOCKS)==0)
				{
					if(parsedcode[parseline][i+1]>=0 && parsedcode[parseline][i+1]<32000)
							p_material=GetVariable(parsedcode[parseline][i+1]);
					else
						ValueErrormsg(parseline,"MATERIAL");
				}
				else
					ValueErrormsg(parseline,"MATERIAL");
				break;
			case 30031:		//MULV
				if(CheckValue(parsedcode[parseline][i+1],0,100)==0 && CheckValue(parsedcode[parseline][i+2],0,32000)==0)
					p_v[parsedcode[parseline][i+1]]*=GetVariable(parsedcode[parseline][i+2]);
				else
					ValueErrormsg(parseline,"MULV");
				break;
			case 30032:		//OUTSIDE
				if(CheckValue(parsedcode[parseline][i+1],0,5)==0)
						p_outside=parsedcode[parseline][i+1];
				else
					ValueErrormsg(parseline,"OUTSIDE");
				break;
			case 30033:		//PRINT, NO VARIABLES
				if(CheckValue(parsedcode[parseline][i+1],0,strlen(mapcode[parseline]))==0)
				{
					printf ("PRINT: [");
					for(ix=parsedcode[parseline][i+1];ix<short (strlen(mapcode[parseline])-1);ix++) printf ("%c",mapcode[parseline][ix]);
					printf ("]\n");
				}
				else
					ValueErrormsg(parseline,"PRINT");
				break;
			case 30034:		//PRINTCODE
				if(CheckValue(parsedcode[parseline][i+1],0,1)==0)
					p_printcode=parsedcode[parseline][i+1];
				else
					ValueErrormsg(parseline,"PRINTCODE");
				break;
			case 30035:		//RAND, do nothing

			if(CheckValue(parsedcode[parseline][i+1],0,30000)==0 && CheckValue(parsedcode[parseline][i+2],0,30000)==0)
			{
				if(GetVariable(parsedcode[parseline][i+1])<GetVariable(parsedcode[parseline][i+2]))
				{
					r1=GetVariable(parsedcode[parseline][i+1]);
					r2=GetVariable(parsedcode[parseline][i+2]);
				}
				else
				{
					r2=GetVariable(parsedcode[parseline][i+1]);
					r1=GetVariable(parsedcode[parseline][i+2]);
				}
				r=rand()%(r2-r1+1)+r1;

//				parsedcode[parseline][i]=r;
//				parsedcode[parseline][i+1]=0;
//				parsedcode[parseline][i+2]=0;
				if(i>0)
				{
					parsedcode[parseline][i-1]=r;
					i=15;
//					i=19;
				}
			}
			else
			{
				Errormsg(parseline,"RANDOM values");
			}

				break;
			case 30036:		//REPEAT [i+1]: repeat_times, [18]: counter, [19]: return line
				if(CheckValue(parsedcode[parseline][i+1],0,32000)==0 && CheckValue(parsedcode[parseline][18],0,32000)==0 && CheckValue(parsedcode[parseline][19],0,MAXMAPCODE)==0)
				{
//					printf("REPEAT %d %d\n",GetVariable(parsedcode[parseline][i+1]),GetVariable(parsedcode[parseline][19]));
					if(GetVariable(parsedcode[parseline][i+1])>GetVariable(parsedcode[parseline][18])) //repeats+1)
					{
						ix=GetVariable(parsedcode[parseline][18])+1;
						parsedcode[parseline][18]=ix;
						parseline=GetVariable(parsedcode[parseline][19]);
						i=0;
//						printf("LOOP TIMES: %d TO LINE: %d\n",ix,parseline);
//						printf("REPEAT TO LINE %d\n",beginloop);
//						repeats++;
//						parseline=beginloop;		// return to line next to BEGIN line
					}
				}
				else
					ValueErrormsg(parseline,"REPEAT");
				break;
			case 30037:		//RIGHT
				if(CheckValue(parsedcode[parseline][i+1],0,MAPSIZE)==0)
						p_x+=GetVariable(parsedcode[parseline][i+1]);
				else
					ValueErrormsg(parseline,"RIGHT");
				break;
			case 30038:		//SETV
				if(CheckValue(parsedcode[parseline][i+1],0,100)==0 && CheckValue(parsedcode[parseline][i+2],0,32000)==0)
				{
					p_v[parsedcode[parseline][i+1]]=GetVariable(parsedcode[parseline][i+2]);
				}
				else
					ValueErrormsg(parseline,"SETV");
				break;
			case 30039:		//SETX
				if(CheckValue(parsedcode[parseline][i+1],0,MAPSIZE)==0)
					p_x=GetVariable(parsedcode[parseline][i+1]);
				else
					ValueErrormsg(parseline,"SETX");
				break;
			case 30040:		//SETY
				if(CheckValue(parsedcode[parseline][i+1],0,MAPSIZE)==0)
					p_y=GetVariable(parsedcode[parseline][i+1]);
				else
					ValueErrormsg(parseline,"SETY");
				break;
			case 30041:		//SUBV
				if(CheckValue(parsedcode[parseline][i+1],0,100)==0 && CheckValue(parsedcode[parseline][i+2],0,32000)==0)
						p_v[parsedcode[parseline][i+1]]-=GetVariable(parsedcode[parseline][i+2]);
				else
					ValueErrormsg(parseline,"SUBV");
				break;
			case 30042:		//UP
				if(CheckValue(parsedcode[parseline][i+1],0,MAPSIZE)==0)
					p_y+=GetVariable(parsedcode[parseline][i+1]);
				else
					ValueErrormsg(parseline,"UP");
				break;
			case 30043:		//UPLEFT
				if(CheckValue(parsedcode[parseline][i+1],0,MAPSIZE)==0)
				{
					p_x-=GetVariable(parsedcode[parseline][i+1]);
					p_y+=GetVariable(parsedcode[parseline][i+1]);
				}
				else
					ValueErrormsg(parseline,"UPLEFT");
				break;
			case 30044:		//UPRIGHT
				if(CheckValue(parsedcode[parseline][i+1],0,MAPSIZE)==0)
				{
					p_x+=GetVariable(parsedcode[parseline][i+1]);
					p_y+=GetVariable(parsedcode[parseline][i+1]);
				}
				else
					ValueErrormsg(parseline,"UPRIGHT");
				break;
			case 30045:		//SETFREQUENCY, do nothing
				if(CheckValue(parsedcode[parseline][i+1],0,10000)!=0)
					ValueErrormsg(parseline,"SETFREQUENCY");
				break;
			case 30046:		//ENDIF, do nothing
				break;
			case 30047:		//IFBLOCK
				if(CheckValue(parsedcode[parseline][i+1],0,MAXBLOCKS)==0 && CheckValue(parsedcode[parseline][i+2],0,MAXBLOCKS)==0 && CheckValue(parsedcode[parseline][19],0,MAXMAPCODE)==0)
				{
					if(MapBlock(p_x,p_y)<GetVariable(parsedcode[parseline][i+1]) || MapBlock(p_x,p_y)>GetVariable(parsedcode[parseline][i+2]))
					{
						ok=0;
//						parseline++;
						i=0;
						parseline=GetVariable(parsedcode[parseline][19]);
/*						while(ok==0)
						{
							if(parsedcode[parseline][i]==30046)				// move until endif is found
								ok=1;
							else
								i++;

							if(parsedcode[parseline][i]<0)
							{
								parseline++;
								i=0;
							}
						}
						*/
					}
				}
				else
					ValueErrormsg(parseline,"IFBLOCK");
				break;
			case 30048:		//PRINTVAR
				if(CheckValue(parsedcode[parseline][i+1],0,32000)==0)
				{
					printf ("%d\n",GetVariable(parsedcode[parseline][i+1]));
				}
				else
					ValueErrormsg(parseline,"PRINTVAR");
				break;
			case 30049:
				if(CheckValue(parsedcode[parseline][i+1],0,MAPSIZE)==0)
				{
					p_doortype=GetVariable(parsedcode[parseline][i+1]);
				}
				else
					ValueErrormsg(parseline,"DOORTYPE");
				break;

			default:break;

		}

		if(p_x<0) p_x=0;
		if(p_y<0) p_y=0;
		if(p_x>MAPSIZE-1) p_x=MAPSIZE-1;
		if(p_y>MAPSIZE-1) p_y=MAPSIZE-1;

		i++;
	}

}

void ParsePhase2(short line)
// checks that random numbers are ok, IFV and ENDIFV are matched, BEGIN and REPEAT are matched
{
	short i,tmpline,ok,loop,original_i,r1,r2,r;

	i=0;
	while(parsedcode[line][i]>=0)
	{
		if(parsedcode[line][i]==30035)	// random
		{
			if(CheckValue(parsedcode[line][i+1],0,30000)==0 && CheckValue(parsedcode[line][i+2],0,30000)==0)
			{
				if(parsedcode[line][i+1]<parsedcode[line][i+2])
				{
					r1=parsedcode[line][i+1];
					r2=parsedcode[line][i+2];
				}
				else
				{
					r2=parsedcode[line][i+1];
					r1=parsedcode[line][i+2];
				}
				r=rand()%(r2-r1+1)+r1;

				parsedcode[line][i]=r;


				parsedcode[line][i+1]=30035;
				parsedcode[line][i+2]=r1;
				parsedcode[line][i+3]=r2;

				i=18;

//				parsedcode[line][19]=r;
			}
			else
			{
				Errormsg(line,"RANDOM values");
			}
		}	// random ends

		if(parsedcode[line][i]==30028 || parsedcode[line][i]==30047)	// IFV/IFBLOCK begins
		{
//			if(p_v[parsedcode[line][i+1]]<parsedcode[line][i+2] || p_v[parsedcode[line][i+1]]>parsedcode[line][i+3])	// what is this???????
//			{
				tmpline=line;
				original_i=i;
				line++;
				i=0;
				ok=0;
				loop=0;

				while(ok==0)
				{
//					printf("Searching for ENDIFV %d %d/%d CMD=%d\n",i,line,mapcodelines,parsedcode[line][i]);
					if(parsedcode[line][i]==30046)
					{

						if(loop==0)
						{
							parsedcode[tmpline][19]=line+1;
//							printf("ENDIF found -  line %d - tmpline %d\n",line,tmpline);
							ok=2;		// endif found
						}
						else
						{
							loop--;
						}
					}

					if(parsedcode[line][i]==30028 || parsedcode[line][i]==30047)	// another IFV/IFBLOCK begins
						loop++;

					if(line>mapcodelines) ok=1;
					if(parsedcode[line][i]<0)
					{
						line++;
						i=0;
					}
					else
						i++;
				}

				if(ok==1) Errormsg(tmpline,"ENDIF not found");
//				if(ok==2) printf("ENDIFV found on row %d\n",line-1);
				line=tmpline+1;
				i=0;
//			}

		}			// IFV ends

		if(parsedcode[line][i]==30004)	// BEGIN begins
		{
			tmpline=line;
			line++;
			original_i=i;
			i=0;
			ok=0;
			loop=0;

			while(ok==0)
			{
//				printf("Searching for REPEAT %d %d/%d CMD=%d\n",i,line,mapcodelines,parsedcode[line][i]);
				if(parsedcode[line][i]==30036)
				{
					if(loop==0)
					{
						ok=2;		// repeat found
						parsedcode[tmpline][18]=line;
						parsedcode[line][19]=tmpline;
//						printf("REPEAT found on row %d\n",line);
					}
					else
						loop--;
				}

				if(parsedcode[line][i]==30004) loop++;

				if(line>mapcodelines) ok=1;
				if(parsedcode[line][i]<0)
				{
					line++;
					i=0;
				}
				else
					i++;
			}

			if(ok==1) Errormsg(tmpline,"REPEAT not found");
			line=tmpline+1;
			i=0;

		}	// BEGIN ends


		i++;
	}
}

short FindPlace(short istart, short iend)
{
	short itries,ok,ibegin,ix,iy,iw;
//	char msg[100];

	itries=0;

	while(itries<20)	// 20 tries to find a place
	{

//	sprintf(msg,"Findplace %d, %d-%d",itries,istart,iend);
//	WriteInfo(msg);

	ok=0;
	while(ok==0)
	{
		if(istart==0 && iend==0)
		{
			iw=MAPSIZE-p_buildwidth-10;
			if(iw<=0) iw=MAPSIZE-10;
			ibegin=5+rand()%iw;
		}
		else
		{
			iw=iend-istart;
			if(iw<=0) iw=10;
			ibegin=rand()%iw+istart;
		}

		for(ix=0;ix<p_buildwidth+6;ix++)
		{
		if(ix+ibegin>MAPSIZE)
			ok=1;	// failure
		else
			for(iy=0;iy<MAPSIZE;iy++)
				{
				if(MapBlock(ibegin+ix-3,iy)>0 && MapBlock(ibegin+ix-3,iy)<40) ok=1;
				}
		}

		if(ok==0) ok=2;
	}

	if(ok==2)		// succesfull
	{
//		sprintf(msg,"X %d-%d %d",ibegin,ibegin+p_buildwidth,p_buildwidth);
//		AddMessage(msg,1);
		if(ibegin<4) ibegin=4;
		p_buildbegin=ibegin;
		itries=100;
		return 1;
	}

	itries++;
	}

return 0;	// otherwise unsuccesfull
}

void GetLens()
{
short i,j;

for(i=0;i<MAXCOMMANDS;i++) commandlen[i]=strlen(command[i]);
for(i=0;i<MAXVARIABLES;i++) variablelen[i]=strlen(variable[i]);

for(i=0;i<MAXMAPCODE;i++)
	for(j=0;j<20;j++) parsedcode[i][j]=-1;

for(i=0;i<100;i++) p_v[i]=0;

p_buildbegin=0;		//p for parse
p_buildend=0;
p_buildwidth=0;
p_doorinterval=0;
p_elevinterval=0;
p_extras=0;
p_floorheight=0;
p_groundfix=0;
p_material=0;
p_outside=0;
p_printcode=0;
p_x=0;
p_y=0;
p_doortype=0;
p_block=0;

}

short FindBuildWidth()
{
short line,i;

for(line=0;line<mapcodelines;line++)
{
	i=0;
	while(parsedcode[line][i]>=0)
	{
		if(parsedcode[line][i]==30011)	// BUILDWIDTH
		{
			p_buildwidth=GetVariable(parsedcode[line][i+1]);
//			printf("p_buildwidth found: %d\n",p_buildwidth);
			return 1;
		}
		i++;
	}
}

return 0;
}

short ParseMapCode(bool bfirst)
{
/*	PROBLEM MAPS
7640	- office building cut by purge
13168	- bunker and pipes


*/

short line,ret;
GetLens();
//char msg[200];
short i;

//printf("[%d] ",p_buildbegin);

for(line=0;line<mapcodelines;line++)
{
	ParseLine(line);
}
if(FindBuildWidth()==0) Errormsg(0,"BUILDWIDTH not given");

for(line=0;line<mapcodelines;line++)
{
	ParsePhase2(line);

	if(p_printcode==1)	// print out to "console"
	{
	for(i=0;i<20;i++)
	if(parsedcode[line][i]>=0)
		printf (" %d",parsedcode[line][i]);
	printf ("\n\n");
	}
}
if(FindBuildWidth()==0) Errormsg(0,"BUILDWIDTH not given");

parseline=0;
//beginloop=-1;

//WriteInfo("Findplace beginning...");

if(bfirst==true)
	ret=FindPlace(MAPSIZE/2-p_buildwidth/2,MAPSIZE/2+p_buildwidth/2);
else
	ret=FindPlace(10,MAPSIZE-10);
//printf("p_buildbegin %d p_buildwidth %d\n",p_buildbegin,p_buildwidth);

if(ret>0)
do
{
/*	sprintf(msg,"Line %d, buildwidth: %d, v10:%d:",parseline,p_buildwidth,p_v[9]);
	WriteInfo(msg);
	WriteInfo(mapcode[parseline]);
	*/
	ParseIntoMap();
/*
//	printf("* %d %s\n*",parseline,mapcode[parseline]);
	for(i=0;i<20;i++)
	if(parsedcode[parseline][i]>=0)
//		printf(" %d",parsedcode[parseline][i]);
//	printf("\n\n");
*/
	parseline++;
} while(parseline<mapcodelines);

//todo: groundfix, needed or not??? Perhaps later some fix
/*for(line=0;line<mapcodelines;line++)
{
//	printf("* LINE %d * %s\n*",line+1, mapcode[line]);
	for(i=0;i<20;i++)
	if(parsedcode[line][i]>=0)
//		printf(" %d",parsedcode[line][i]);
//	printf("\n\n");
}
*/
//*/
return ret;
}
