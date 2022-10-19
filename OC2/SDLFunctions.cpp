#include "SDL/SDL.h"   /* All SDL App's need this */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "oc2.h"

extern long		lMapnr;

short			SIZE_X,SIZE_Y,BLOCKSIZE,xmiddle,ymiddle;
short			sResolutionX[100],sResolutionY[100];
short			sResolutionSelected;
unsigned char	cBaseColours[3];
extern short	sResolutionBegin;
extern short	setBits;
//extern short	setAccelerationMode;
char			iTextColor;
char			iColOffset;

void CheckModes()
{

SDL_Rect **modes;
int i,i2;

sResolutionBegin=0;
/* Get available fullscreen/hardware modes */
modes=SDL_ListModes(NULL, SDL_FULLSCREEN|SDL_HWSURFACE);

/* Check is there are any modes available */
if(modes == (SDL_Rect **)0){
  printf ("No display modes available!\n");
  exit(-1);
}

/* Print valid modes */
i2=0;
//printf("Available Modes\n");
for(i=0;modes[i];++i)
{
	if(modes[i]->w>=1024)
	{
		sResolutionX[i2]=modes[i]->w;
		sResolutionY[i2]=modes[i]->h;

		if(sResolutionX[i2]==SIZE_X && sResolutionY[i2]==SIZE_Y)
		{
//			if(i2>2)
			sResolutionBegin=i2;
			sResolutionSelected=i2;
		}

		i2++;
		if(i2>=99) i2=99;
//		printf("  %d x %d\n", modes[i]->w, modes[i]->h);
	}

}

while(sResolutionX[sResolutionBegin+4]==0 && sResolutionBegin>0) sResolutionBegin--;	// scroll the list

}

int InitSDL()
{
/*	char cAcceleration[3][20]={"",
		"Hardware",
		"Software"};
*/

	if(setBits==0) setBits=32;
//    printf("Initializing SDL.\n");

    /* Initialize defaults, Video and Audio */
    if((SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO)==-1))
	{
        printf ("Could not initialize SDL: %s.\n", SDL_GetError());
        return -1;
    }

	SDL_EnableUNICODE(1);
	SDL_WM_SetCaption("Operation Cleaner 2", "Operation Cleaner 2");
	SDL_WM_SetIcon(SDL_LoadBMP("media/cleaner2.bmp"),NULL);

	BLOCKSIZE=20;

//	if(setAccelerationMode==2)
	if(lMapnr==0)
		screen = SDL_SetVideoMode(SIZE_X, SIZE_Y, setBits, SDL_SWSURFACE|SDL_ASYNCBLIT|SDL_FULLSCREEN); //|SDL_FULLSCREEN|SDL_DOUBLEBUF
	else
		screen = SDL_SetVideoMode(320, 200, 0, SDL_SWSURFACE); //|SDL_FULLSCREEN|SDL_DOUBLEBUF
//	else	// precaution, all other cases use hardware acceleration
//		screen = SDL_SetVideoMode(SIZE_X, SIZE_Y, 32, SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_ASYNCBLIT|SDL_FULLSCREEN); //|SDL_FULLSCREEN|SDL_DOUBLEBUF

    if ( screen == NULL )
	{
        fprintf(stderr, "Couldn't set %dx%dx%d (software acceleration) video mode: %s\n",SIZE_X,SIZE_Y,setBits,SDL_GetError());
		return -1;
    }
//	else
//		printf("SDL initialized\n");

	xmiddle=SIZE_X/2;
	ymiddle=SIZE_Y/2;

    if(sResolutionX[0]==0) CheckModes();
	return 0;

}

int QuitSDL()
{
	SDL_CloseAudio();
	SDL_Quit();
//	printf("Quit requested, quitting.\n");
	return 0;
}

void DoFlip()
{
	SDL_Flip(screen);
}

void LoadBMP(char const* file, unsigned char imagenr)
{

	Uint32 col,col2,col3;
   /* Load the BMP file into a surface */

	if(imagenr==1)
	{
    image = SDL_LoadBMP(file);
    if ( image == NULL ) {
        fprintf(stderr, "Couldn't load %s: %s\n", file, SDL_GetError());
        return;
    }
	col=SDL_MapRGB(image->format, 255, 255, 255);
	SDL_SetColorKey(image, SDL_SRCCOLORKEY, col);
	}

	if(imagenr==2)
	{
    explosionimage = SDL_LoadBMP(file);
    if ( explosionimage == NULL ) {
        fprintf(stderr, "Couldn't load %s: %s\n", file, SDL_GetError());
        return;
    }
	col2=SDL_MapRGB(explosionimage->format, 255, 255, 255);
	SDL_SetColorKey(explosionimage, SDL_SRCCOLORKEY, col2);
	}

	if(imagenr==3)
	{
    buttonimage = SDL_LoadBMP(file);
    if ( buttonimage == NULL ) {
        fprintf(stderr, "Couldn't load %s: %s\n", file, SDL_GetError());
        return;
    }
	col3=SDL_MapRGB(buttonimage->format, 255, 255, 255);
	SDL_SetColorKey(buttonimage, SDL_SRCCOLORKEY, col3);
	}


}

void DrawDigits(char *text,int x,int y)
{
	unsigned int iCounter;
	int iChar,iY,iCnr;

	char * p;

	iCounter=0;

	for(p=text;p<text+strlen(text);p++)
	{
		iY=0;
		iCnr=(unsigned char) *p;
		if(iCnr<0) iCnr+=255;
//		iCnr=int(text[iCounter]-32);
		iChar=(iCnr-48)*16;

		ShowBMP1(iChar,575,16,21,x+iCounter*16,y);
		iCounter++;
	}

}

void DrawNormalText(char const* text,int x,int y,int color)	// 0 white, 1 black, 2 red, 3 green
{
	unsigned int iCounter;
	int iChar,iY,iCnr;

	char * p;

	iCounter=0;

	for(p=(char *)text;p<text+strlen(text);p++)
	{
		iY=0;
		iCnr=(unsigned char) *p;
		if(iCnr<0) iCnr+=255;
//		iCnr=int(text[iCounter]-32);
		iChar=(iCnr-32)*8;

		while(iChar>=760)
		{
			iY+=15;
			iChar-=760;
		}
//		printf("%c %u %u %u\n",*p,iCnr,iChar,iY);
		ShowBMP1(iChar,color*50+iY,8,15,x+iCounter*8,y);
		iCounter++;
	}

}

void DrawTextWBack(char *text,int x,int y,int color,int r,int g,int b)
{
	int iLgt=strlen(text)*8;

	DrawFillRect(x,y,x+iLgt,y+16,r,g,b);
	DrawNormalText(text,x,y+1,color);
}

void ShowBMP1(short src_x, short src_y, short src_w, short src_h,short dest_x,short dest_y)
{
    SDL_Rect src;
    SDL_Rect dest;

	src.x=src_x;
	src.y=src_y;
	src.w=src_w;
	src.h=src_h;

    dest.x = dest_x;
    dest.y = dest_y;
    dest.w = src_w; //image->w;
    dest.h = src_h; //image->h;

	SDL_BlitSurface(image, &src, screen, &dest);
}

void ShowBMP2(short src_x, short src_y, short src_w, short src_h,short dest_x,short dest_y)
{
    SDL_Rect src;
    SDL_Rect dest;

	src.x=src_x;
	src.y=src_y;
	src.w=src_w;
	src.h=src_h;

    dest.x = dest_x;
    dest.y = dest_y;
    dest.w = src_w; //image->w;
    dest.h = src_h; //image->h;

	SDL_BlitSurface(explosionimage, &src, screen, &dest);
}

void ShowBMP3(short src_x, short src_y, short src_w, short src_h,short dest_x,short dest_y)
{
    SDL_Rect src;
    SDL_Rect dest;

	src.x=src_x;
	src.y=src_y;
	src.w=src_w;
	src.h=src_h;

    dest.x = dest_x;
    dest.y = dest_y;
    dest.w = src_w; //image->w;
    dest.h = src_h; //image->h;

	SDL_BlitSurface(buttonimage, &src, screen, &dest);
}


void DrawLine(int left,int top,int right,int bottom,short r,short g,short b)
{
int temp,dx,dy,x,y,x_sign,y_sign,flag;

SDL_LockSurface(screen);
dx=abs(right-left);
dy=abs(bottom-top);
if((dx>=dy && left>right) || (dy>dx && top>bottom))
        {
        temp=left;
        left=right;
        right=temp;
        temp=top;
        top=bottom;
        bottom=temp;
        }
if((bottom-top)<0) y_sign=-1;
else y_sign=1;
if((right-left)<0) x_sign=-1;
else x_sign=1;
if(dx>=dy)
        {
        for(x=left,y=top,flag=0;x<=right;x++,flag+=dy)
                {
                if(flag>=dx)
                        {
                        flag-=dx;
                        y+=y_sign;
                        }
//                if(x>0 && x<X && y>0 && y<Y)
					Putpixel(x,y,r,g,b);
                }
        }
        else
        {
        for(x=left,y=top,flag=0;y<=bottom;y++,flag+=dx)
                {
                if(flag>=dy)
                        {
                        flag-=dy;
                        x+=x_sign;
                        }
//                if(x>0 && x<X && y>0 && y<Y)
					Putpixel(x,y,r,g,b);
                }
        }
SDL_UnlockSurface(screen);
}

void DrawRect(int left_top_x, int left_top_y, int right_bottom_x, int right_bottom_y, int r, int g, int b)
{
	int ix,iy;
	SDL_LockSurface(screen);

	for(ix=left_top_x;ix<=right_bottom_x;ix++)
	{
		Putpixel(ix,left_top_y,r,g,b);
		Putpixel(ix,right_bottom_y,r,g,b);
	}
	for(iy=left_top_y;iy<=right_bottom_y;iy++)
	{
		Putpixel(left_top_x,iy,r,g,b);
		Putpixel(right_bottom_x,iy,r,g,b);
	}

	SDL_UnlockSurface(screen);
}

void DrawFillRect(int left_top_x, int left_top_y, int right_bottom_x, int right_bottom_y, int r, int g, int b)
{

	SDL_Rect rect;

	rect.x=left_top_x;
	rect.y=left_top_y;
	rect.w=right_bottom_x-left_top_x;
	rect.h=right_bottom_y-left_top_y;

	Uint32 col=SDL_MapRGB(screen->format, r, g, b);

	SDL_FillRect(screen,&rect,col);

//    SDL_UpdateRects(screen, 1, &rect);
}

void DrawTextRect(char * text,short left,short top,short right,short bottom)
{
	SDL_Rect rect;

	rect.x=left;
	rect.y=top;
	rect.w=right-left;
	rect.h=bottom-top;

	Uint32 col=SDL_MapRGB(screen->format, cBaseColours[0], cBaseColours[1], cBaseColours[2]);

	SDL_FillRect(screen,&rect,col);

	DrawRect(left,top,right,bottom,cBaseColours[0]+50,cBaseColours[1]+50,cBaseColours[2]+50);
	DrawNormalText(text,left+(right-left)/2-strlen(text)*4,top+(bottom-top)/2-7,iTextColor);

}


void DrawButton(int left_top_x, int left_top_y, int right_bottom_x, int right_bottom_y)
{

	SDL_Rect rect;
	int x,y,t;

	rect.x=left_top_x;
	rect.y=left_top_y;
	rect.w=right_bottom_x-left_top_x;
	rect.h=right_bottom_y-left_top_y;

	Uint32 col=SDL_MapRGB(screen->format, cBaseColours[0], cBaseColours[1], cBaseColours[2]);

	SDL_FillRect(screen,&rect,col);

	SDL_LockSurface(screen);

	for(t=0;t<5;t++)
	{
		for(x=left_top_x+t;x<right_bottom_x-t;x++)
		{
			Putpixel(x,left_top_y+t,cBaseColours[0]+50,cBaseColours[1]+50,cBaseColours[2]+50);
			Putpixel(x,right_bottom_y-t,cBaseColours[0]-50,cBaseColours[1]-50,cBaseColours[2]-50);
		}

		for(y=left_top_y+t;y<right_bottom_y-t+1;y++)
		{
			Putpixel(left_top_x+t,y,cBaseColours[0]+50,cBaseColours[1]+50,cBaseColours[2]+50);
			Putpixel(right_bottom_x-t,y,cBaseColours[0]-50,cBaseColours[1]-50,cBaseColours[2]-50);
		}
	}

	SDL_UnlockSurface(screen);

//    SDL_UpdateRects(screen, 1, &rect);

}

void DrawCircle(short x, short y, double radius, unsigned char r, unsigned char g, unsigned char b)
{
short i;
double dx,dy,rc;

rc=radius*10;
if(radius>300) rc=radius*25;

SDL_LockSurface(screen);

for(i=0;i<rc;i++)
{
	dx=x;
	dx+=radius*cos(i);
	dy=y;
	dy+=radius*sin(i);

	if(dx>0 && dx<SIZE_X-260 && dy>0 && dy<SIZE_Y)
	{

		Putpixel(short(dx),short(dy),r,g,b);
	}
}

SDL_UnlockSurface(screen);
}

void Update()
{
    SDL_UpdateRect(screen, 0,0,0,0);
}

void DrawBMPButton(char const* text, int left_top_x,int left_top_y,int iType)
{
	int w,h,tx,ty;

	switch(iType)
	{			// begin x, begin y, width, height
	case 1:ShowBMP3(0,0,40,20,left_top_x,left_top_y);w=40;h=20;break;		// + or - sign
	case 2:ShowBMP3(40,0,80,26,left_top_x,left_top_y);w=80;h=25;break;		// add, remove explosive etc
	case 3:ShowBMP3(120,0,120,30,left_top_x,left_top_y);w=120;h=30;break;	// Ok, cancel etc
	case 4:ShowBMP3(0,30,239,30,left_top_x,left_top_y);w=239;h=30;break;	// Financial decisions etc
	case 5:ShowBMP3(0,60,300,40,left_top_x,left_top_y);w=300;h=40;break;	// main menu
	case 6:ShowBMP3(240,30,30,30,left_top_x,left_top_y);break;				// Yes (Ok bug)
	case 7:ShowBMP3(270,30,30,30,left_top_x,left_top_y);break;				// No (X)
	case 8:ShowBMP3(0,100,50,50,left_top_x,left_top_y);break;				// direction
	case 9:ShowBMP3(50,100,16,16,left_top_x,left_top_y);break;				// slider
	case 10:ShowBMP3(240,0,20,20,left_top_x,left_top_y);break;				// up
	case 11:ShowBMP3(260,0,20,20,left_top_x,left_top_y);break;				// down
	case 12:ShowBMP3(66,100,30,30,left_top_x,left_top_y);break;				// tutorial
	default:break;
	}

if(iType<=5)
{
	tx=w/2-strlen(text)*4;
	ty=h/2-7;
	DrawNormalText(text,left_top_x+tx,left_top_y+ty,iTextColor);
}
}

void DrawTextButton(char * text,int left_top_x, int left_top_y, int right_bottom_x, int right_bottom_y)
{
	int textlen=strlen(text);
	int w=(right_bottom_x-left_top_x)/2-textlen*4;
	int h=(right_bottom_y-left_top_y)/2-6;
	DrawButton(left_top_x,left_top_y,right_bottom_x,right_bottom_y);

	DrawNormalText(text,left_top_x+w,left_top_y+h,iTextColor);

}


/*
 * Set the pixel at (x, y) to the given value
 * NOTE: The surface MUST BE LOCKED before calling this!
*/
void Putpixel(int x, int y, int r,int g,int b)
{
    int bpp = screen->format->BytesPerPixel;
    // Here p is the address to the pixel we want to set
    Uint8 *p = (Uint8 *)screen->pixels + y * screen->pitch + x * bpp;

	*(Uint32 *)p = SDL_MapRGB(screen->format, r, g, b);
}

//*/
