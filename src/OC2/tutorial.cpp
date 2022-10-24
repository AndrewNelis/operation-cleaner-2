#include <string.h>
#include "SDL/SDL.h"   /* All SDL App's need this */
#include "oc2.h"

extern char				GameOn;

bool					bTutorial;
char					cTutorialsdone[MAXTUTORIALS];
char					cTutorial;

extern short			SIZE_X,SIZE_Y;
extern int				m_x;

extern char				gametxt[MAXGAMETXT][52];
extern unsigned char	cBaseColours[3];
extern bool				bShowHelp;
extern char				cOKBoxText[10][50];
extern bool				bAddDamper;
extern short			iDampermode;
extern long				plr_money;
extern long				lTimer;

extern char				iTextColor;
long					l_money;

void DrawTutorial()
{

	short ihlpx[MAXTUTORIALS]={SIZE_X-240,SIZE_X-240,SIZE_X-100,SIZE_X-240,SIZE_X-30,SIZE_X-240,SIZE_X-240};
	short ihlpy[MAXTUTORIALS]={550,490,25,240,460,460,550};
	short i;
	bool bOk;

	DrawButton(10,30,250,200);
	DrawNormalText(gametxt[400],20,40,iTextColor);	//Tutorial
	DrawNormalText(gametxt[401],20,60,iTextColor);	//Tasks to do:

	i=0;

	while(strlen(gametxt[402+i])>0)
	{
		if(cTutorial==i)
			DrawTextWBack(gametxt[402+i],20,80+i*15,cTutorialsdone[i],cBaseColours[0]+50,cBaseColours[1]+50,cBaseColours[2]+50);
		else
			DrawNormalText(gametxt[402+i],20,80+i*15,cTutorialsdone[i]);

		i++;
	}

	if(cTutorial<6)
	{
		bOk=true;


		if(GameOn==3 || GameOn==5) bOk=false;
		if(cTutorial==2 && m_x>SIZE_X-260) bOk=false;

		if(bOk==true) DrawBMPButton("",ihlpx[cTutorial],ihlpy[cTutorial],12);
	}
	if(cTutorial==6 && lTimer==0) DrawBMPButton("",ihlpx[cTutorial],ihlpy[cTutorial],12);
}


void MoveToNextTutorial(short iDone)
{
	short i;
	short ihlptxtbegin[MAXTUTORIALS+1]={410,419,426,443,459,473,476,489};
	double dTemp;

	if(bTutorial==true)
	{

	if(iDone>=0)
	{
		cTutorialsdone[iDone]=3;
	}
	cTutorial=iDone+1;

	bShowHelp=true;
	EmptyHelp();

	for(i=ihlptxtbegin[cTutorial];i<ihlptxtbegin[cTutorial+1];i++)
	{
		AddHelp(gametxt[i],iTextColor);
//		printf("%s %d\n",gametxt[i],iTextColor);
	}

	if(cTutorial==7)
	{
		bAddDamper=false;
		iDampermode=0;
		dTemp=plr_money*100;
		dTemp/=l_money;

		if(dTemp<100) strcpy(cOKBoxText[0],gametxt[490]);						//You'll need some more practise.
		if(plr_money>=100 && plr_money<=130) strcpy(cOKBoxText[0],gametxt[491]);	//That was a pretty decent detonation.
		if(plr_money>=130) strcpy(cOKBoxText[0],gametxt[492]);					//Excellent job!

		sprintf(cOKBoxText[1],"");
		strcpy(cOKBoxText[2],gametxt[493]);		// good luck
		OKBox(gametxt[489],150);				// This is end of tutorial.

		bTutorial=false;
		cTutorial=0;
		bShowHelp=false;

/*		char msg[30];
		sprintf(msg,"diff %i ratio %.f",plr_money-l_money,dTemp);
		AddMessage(msg,1);
		*/
	}
	}

	l_money=plr_money;

}
